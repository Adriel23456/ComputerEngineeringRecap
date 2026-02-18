#pragma once

/**
 * @file TomasuloCPU.h
 * @brief Central simulation coordinator for Tomasulo CPU.
 *
 * Owns ALL simulation components: assembler, RAM, registers,
 * the wire bus, the cycle coordinator, and every pipeline component.
 */

#include "apps/cpu_tomasulo/simulation/assembler/TomasuloAssembler.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"
#include "apps/cpu_tomasulo/simulation/processor/TomasuloRegisterFile.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloCycleCoordinator.h"
#include "apps/cpu_tomasulo/simulation/pipeline/PC_C.h"
#include "apps/cpu_tomasulo/simulation/pipeline/PC_MUX.h"
#include "apps/cpu_tomasulo/simulation/pipeline/PC_Adder.h"
#include "apps/cpu_tomasulo/simulation/pipeline/I_Cache.h"
#include "apps/cpu_tomasulo/simulation/pipeline/Instruction_Decoder.h"
#include "apps/cpu_tomasulo/simulation/pipeline/Extends.h"
#include "apps/cpu_tomasulo/simulation/pipeline/Control_Unit.h"
#include "apps/cpu_tomasulo/simulation/pipeline/RegisterFilePipeline.h"
#include "apps/cpu_tomasulo/simulation/pipeline/ROB.h"
#include "apps/cpu_tomasulo/simulation/pipeline/Flags_Unit.h"
#include "apps/cpu_tomasulo/simulation/pipeline/SB0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/SB1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/LB0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/LB1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/LB2.h"
#include "apps/cpu_tomasulo/simulation/pipeline/RS_IntALU0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/RS_IntALU1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/RS_FPALU0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/RS_IntMUL0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/RS_FPMUL0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/RS_Branch0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/AGU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/AGU_0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/AGU_1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/IntALU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/IntALU.h"
#include "apps/cpu_tomasulo/simulation/pipeline/FPALU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/FPALU.h"
#include "apps/cpu_tomasulo/simulation/pipeline/IntMUL_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/IntMUL.h"
#include "apps/cpu_tomasulo/simulation/pipeline/FPMUL_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/FPMUL.h"
#include "apps/cpu_tomasulo/simulation/pipeline/Branch_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/Branch_Executor.h"
#include "apps/cpu_tomasulo/simulation/pipeline/CDB_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/CDB.h"
#include "apps/cpu_tomasulo/simulation/pipeline/Memory_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/D_Cache.h"
#include "apps/cpu_tomasulo/simulation/pipeline/Commit_Unit.h"
#include "apps/cpu_tomasulo/simulation/PipelineTracker.h"
#include <string>
#include <memory>

class TomasuloCPU {
public:
    TomasuloCPU();

    // ── High-level operations ───────────────────────────────────

    AssemblyResult compile(const std::string& source);
    void reset();
    void resetRAMOnly();

    /**
     * @brief Execute exactly one pipeline cycle.
     * @return Cycle number after execution.
     */
    uint64_t step();

    /** @brief Current cycle count. */
    uint64_t cycleCount() const { return m_coordinator.cycleCount(); }

    // ── Component access (read-only for UI sync) ────────────────

    const TomasuloRAM& ram()              const { return m_ram; }
    const TomasuloRegisterFile& registers() const { return m_registers; }
    TomasuloRAM& ram() { return m_ram; }
    TomasuloRegisterFile& registers() { return m_registers; }

    const TomasuloBus& bus()              const { return m_bus; }

    const I_Cache& iCache()  const { return m_iCache; }
    const D_Cache& dCache()  const { return m_dCache; }
    const ROB& rob()     const { return m_rob; }

    // ── Runtime statistics ──────────────────────────────────────
    struct Stats {
        uint64_t committedInstructions = 0;
        uint64_t committedMemoryInstructions = 0;
        uint64_t branchMispredictions = 0;
        uint64_t stationUses[11] = {}; // SB0,SB1,LB0,LB1,LB2,IntALU0,IntALU1,FpALU0,IntMUL0,FpMUL0,Branch0
    };

    const Stats& stats() const { return m_stats; }

    bool  m_halted = false;

    void resetStats() { m_stats = Stats{}; }

    const PipelineTracker& tracker() const { return m_tracker; }

private:
    Stats m_stats;

    PipelineTracker m_tracker;

    void buildPipeline();

    // ── Existing simulation data ────────────────────────────────
    TomasuloAssembler    m_assembler;
    TomasuloRAM          m_ram;
    TomasuloRegisterFile m_registers;

    // ── Pipeline infrastructure ─────────────────────────────────
    TomasuloBus                m_bus;
    TomasuloCycleCoordinator   m_coordinator;

    // ── Pipeline components ─────────────────────────────────────
    // Phase 1: Fetch
    PC_C                 m_pc;
    PC_Adder             m_pcAdder;
    // Phase 2: Fetch/Decode
    I_Cache              m_iCache;
    Instruction_Decoder  m_decoder;
    // Phase 3: Issue (needs decode output + Extends output)
    Extends              m_extends;
    Control_Unit         m_controlUnit;
    // Phase 4: Register Read + ROB + Flags
    RegisterFilePipeline m_regFile;
    ROB                  m_rob;
    Flags_Unit           m_flagsUnit;
    // Add these members after m_flagsUnit in the class body
    SB0              m_sb0;
    SB1              m_sb1;
    // After m_sb1 member
    LB0              m_lb0;
    LB1              m_lb1;
    LB2              m_lb2;
    // After m_lb2 member
    RS_IntALU0       m_rsIntALU0;
    RS_IntALU1       m_rsIntALU1;
    RS_FPALU0        m_rsFPALU0;
    RS_IntMUL0       m_rsIntMUL0;
    RS_FPMUL0        m_rsFPMUL0;
    RS_Branch0       m_rsBranch0;
    // Members after m_rsBranch0
    AGU_Arbiter      m_aguArbiter;
    AGU_0            m_agu0;
    AGU_1            m_agu1;
    IntALU_Arbiter   m_intALUArbiter;
    IntALU           m_intALU;
    FPALU_Arbiter    m_fpaluArbiter;
    FPALU            m_fpalu;
    IntMUL_Arbiter   m_intMULArbiter;
    IntMUL           m_intMUL;
    FPMUL_Arbiter    m_fpmulArbiter;
    FPMUL            m_fpmul;
    Branch_Arbiter   m_branchArbiter;
    Branch_Executor  m_branchExecutor;
    CDB_Arbiter      m_cdbArbiter;
    CDB_A            m_cdbA;
    CDB_B            m_cdbB;
    Memory_Arbiter   m_memArbiter;
    D_Cache          m_dCache;
    Commit_Unit      m_commitUnit;
    // Phase N: PC_MUX (after Commit sets BranchRedirect)
    PC_MUX               m_pcMux;
};