// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/TomasuloCPU.h
// ============================================================================

#pragma once

/**
 * @file TomasuloCPU.h
 * @brief Central simulation coordinator for the Tomasulo CPU.
 *
 * Owns and wires together every simulation subsystem:
 *   - Assembler         (source → 64-bit instruction words)
 *   - RAM               (50 KB word-addressable data / instruction store)
 *   - Register File     (16 × 64-bit GPRs with Tomasulo rename tags)
 *   - TomasuloBus       (shared combinational wire bundle)
 *   - CycleCoordinator  (topological evaluation + clock edge dispatch)
 *   - All pipeline components in correct evaluation order
 *   - PipelineTracker   (slot labels for the architecture diagram)
 *   - Runtime Stats     (committed instructions, mispredictions, RS uses)
 *
 * Public interface used by TomasuloSimController:
 *   - compile(source)   assemble + load into RAM + reset pipeline
 *   - step()            execute one cycle, update tracker + stats
 *   - reset()           full CPU + RAM + register + pipeline reset
 *   - resetRAMOnly()    zero RAM without touching registers or pipeline
 *
 * Read-only accessors (called under sim mutex by CpuTomasuloState sync methods):
 *   - ram(), registers(), bus(), iCache(), dCache(), rob(), tracker(), stats()
 *
 * @note
 *   - SRP: Coordinates construction and step dispatch; does not implement
 *          pipeline logic directly.
 *   - buildPipeline() encapsulates registration order so the coordinator
 *          never needs to be touched when adding new components.
 */

#include "apps/cpu_tomasulo/simulation/assembler/TomasuloAssembler.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRegisterFile.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloCycleCoordinator.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/pipeline/fetch/PC_C.h"
#include "apps/cpu_tomasulo/simulation/pipeline/fetch/PC_MUX.h"
#include "apps/cpu_tomasulo/simulation/pipeline/fetch/PC_Adder.h"
#include "apps/cpu_tomasulo/simulation/pipeline/fetch/I_Cache.h"
#include "apps/cpu_tomasulo/simulation/pipeline/decode/Instruction_Decoder.h"
#include "apps/cpu_tomasulo/simulation/pipeline/decode/Extends.h"
#include "apps/cpu_tomasulo/simulation/pipeline/decode/Control_Unit.h"
#include "apps/cpu_tomasulo/simulation/pipeline/issue/RegisterFilePipeline.h"
#include "apps/cpu_tomasulo/simulation/pipeline/issue/ROB.h"
#include "apps/cpu_tomasulo/simulation/pipeline/issue/Flags_Unit.h"
#include "apps/cpu_tomasulo/simulation/pipeline/buffers/SB0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/buffers/SB1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/buffers/LB0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/buffers/LB1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/buffers/LB2.h"
#include "apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntALU0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntALU1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/reservation/RS_FPALU0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntMUL0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/reservation/RS_FPMUL0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/branch/RS_Branch0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/agu/AGU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/agu/AGU_0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/agu/AGU_1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/execution/IntALU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/execution/IntALU.h"
#include "apps/cpu_tomasulo/simulation/pipeline/execution/FPALU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/execution/FPALU.h"
#include "apps/cpu_tomasulo/simulation/pipeline/execution/IntMUL_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/execution/IntMUL.h"
#include "apps/cpu_tomasulo/simulation/pipeline/execution/FPMUL_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/execution/FPMUL.h"
#include "apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Executor.h"
#include "apps/cpu_tomasulo/simulation/pipeline/cdb/CDB_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/cdb/CDB.h"
#include "apps/cpu_tomasulo/simulation/pipeline/commit/Memory_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/commit/D_Cache.h"
#include "apps/cpu_tomasulo/simulation/pipeline/commit/Commit_Unit.h"
#include "apps/cpu_tomasulo/simulation/PipelineTracker.h"
#include <string>
#include <memory>

class TomasuloCPU {
public:
    TomasuloCPU();

    // ── High-Level Operations ────────────────────────────────────

    /**
     * @brief Assembles source text, loads the result into RAM, and resets
     *        the pipeline so execution starts from address 0.
     * @return AssemblyResult containing success flag, encoded words, and summary.
     */
    AssemblyResult compile(const std::string& source);

    /** @brief Full reset: clears RAM, registers, pipeline, stats, and tracker. */
    void reset();

    /** @brief Zeroes RAM without touching registers or the pipeline state. */
    void resetRAMOnly();

    /**
     * @brief Executes exactly one pipeline cycle.
     *
     * After evaluation, updates the pipeline tracker and accumulates
     * per-cycle statistics from the bus signals.
     *
     * @return Cycle number after execution.
     */
    uint64_t step();

    /** @brief Returns the current cycle count (non-blocking). */
    uint64_t cycleCount() const { return m_coordinator.cycleCount(); }

    // ── Read-Only Component Access (called under sim mutex by UI) ─

    const TomasuloRAM& ram()       const { return m_ram; }
    const TomasuloRegisterFile& registers() const { return m_registers; }
    TomasuloRAM& ram() { return m_ram; }
    TomasuloRegisterFile& registers() { return m_registers; }

    const TomasuloBus& bus()    const { return m_bus; }
    const I_Cache& iCache() const { return m_iCache; }
    const D_Cache& dCache() const { return m_dCache; }
    const ROB& rob()    const { return m_rob; }

    // ── Runtime Statistics ───────────────────────────────────────

    /**
     * @struct Stats
     * @brief Accumulated per-cycle counters updated by step().
     *
     * stationUses[] maps directly to AllocSourceStation_o values:
     *   [0]=SB0, [1]=SB1, [2]=LB0, [3]=LB1, [4]=LB2,
     *   [5]=IntALU0, [6]=IntALU1, [7]=FpALU0,
     *   [8]=IntMUL0, [9]=FpMUL0, [10]=Branch0
     */
    struct Stats {
        uint64_t committedInstructions = 0;
        uint64_t committedMemoryInstructions = 0;
        uint64_t branchMispredictions = 0;
        uint64_t stationUses[11] = {};
    };

    const Stats& stats() const { return m_stats; }
    void         resetStats() { m_stats = Stats{}; }

    // ── Pipeline Tracker ─────────────────────────────────────────

    /** @brief Returns the cycle-accurate slot label array for the diagram overlay. */
    const PipelineTracker& tracker() const { return m_tracker; }

    // ── Halt Flag ────────────────────────────────────────────────

    /** @brief Set to true by step() when a SWI instruction commits. */
    bool m_halted = false;

private:
    /**
     * @brief Registers all pipeline components with the coordinator in
     *        topological evaluation order.
     *
     * Called once from the constructor. The order here is the contract:
     *   Fetch → I-Cache/Decode → Issue/ROB/Flags → RS/Buffers →
     *   Control_Unit → AGU → Execution → CDB → Memory → Commit → PC_MUX
     */
    void buildPipeline();

    // ── Stats / Tracker ──────────────────────────────────────────
    Stats           m_stats;
    PipelineTracker m_tracker;

    // ── Memory Subsystem ─────────────────────────────────────────
    TomasuloAssembler    m_assembler;
    TomasuloRAM          m_ram;
    TomasuloRegisterFile m_registers;

    // ── Pipeline Infrastructure ───────────────────────────────────
    TomasuloBus              m_bus;
    TomasuloCycleCoordinator m_coordinator;

    // ── Fetch ─────────────────────────────────────────────────────
    PC_C        m_pc;
    PC_Adder    m_pcAdder;
    PC_MUX      m_pcMux;         ///< Registered last (needs BranchRedirect from Commit).

    // ── I-Cache / Decode ──────────────────────────────────────────
    I_Cache              m_iCache;
    Instruction_Decoder  m_decoder;
    Extends              m_extends;
    Control_Unit         m_controlUnit;

    // ── Issue ─────────────────────────────────────────────────────
    RegisterFilePipeline m_regFile;
    ROB                  m_rob;
    Flags_Unit           m_flagsUnit;

    // ── Store Buffers ─────────────────────────────────────────────
    SB0 m_sb0;
    SB1 m_sb1;

    // ── Load Buffers ──────────────────────────────────────────────
    LB0 m_lb0;
    LB1 m_lb1;
    LB2 m_lb2;

    // ── Reservation Stations ──────────────────────────────────────
    RS_IntALU0 m_rsIntALU0;
    RS_IntALU1 m_rsIntALU1;
    RS_FPALU0  m_rsFPALU0;
    RS_IntMUL0 m_rsIntMUL0;
    RS_FPMUL0  m_rsFPMUL0;
    RS_Branch0 m_rsBranch0;

    // ── AGU ───────────────────────────────────────────────────────
    AGU_Arbiter m_aguArbiter;
    AGU_0       m_agu0;
    AGU_1       m_agu1;

    // ── Execution Units ───────────────────────────────────────────
    IntALU_Arbiter  m_intALUArbiter;
    IntALU          m_intALU;
    FPALU_Arbiter   m_fpaluArbiter;
    FPALU           m_fpalu;
    IntMUL_Arbiter  m_intMULArbiter;
    IntMUL          m_intMUL;
    FPMUL_Arbiter   m_fpmulArbiter;
    FPMUL           m_fpmul;
    Branch_Arbiter  m_branchArbiter;
    Branch_Executor m_branchExecutor;

    // ── CDB ───────────────────────────────────────────────────────
    CDB_Arbiter m_cdbArbiter;
    CDB_A       m_cdbA;
    CDB_B       m_cdbB;

    // ── Memory / Commit ───────────────────────────────────────────
    Memory_Arbiter m_memArbiter;
    D_Cache        m_dCache;
    Commit_Unit    m_commitUnit;
};