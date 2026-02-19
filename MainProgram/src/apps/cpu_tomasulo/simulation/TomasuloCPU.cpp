// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/TomasuloCPU.cpp
// ============================================================================

/**
 * @file TomasuloCPU.cpp
 * @brief Implementation of TomasuloCPU.
 *
 * Responsibilities:
 *   - Constructor: initialise components that take constructor arguments
 *     (iCache, dCache, regFile) then call buildPipeline().
 *   - buildPipeline(): register all pipeline components with the coordinator
 *     in strict topological evaluation order.
 *   - step(): drive one cycle, update PipelineTracker, accumulate Stats.
 *   - compile(): assemble, load RAM, reset pipeline.
 *   - reset() / resetRAMOnly(): delegated cleanup paths.
 */

#include "apps/cpu_tomasulo/simulation/TomasuloCPU.h"
#include <iostream>

 // ============================================================================
 // Construction
 // ============================================================================

TomasuloCPU::TomasuloCPU()
    : m_iCache(&m_ram),
    m_dCache(&m_ram),
    m_regFile(&m_registers)
{
    buildPipeline();
    std::cout << "[TomasuloCPU] Constructed. Pipeline components registered.\n";
}

void TomasuloCPU::buildPipeline() {
    // ── Phase 1: Fetch ────────────────────────────────────────────
    // PC_C must be first so PCCurrent_o is valid before PC_Adder reads it.
    // PC_MUX must follow PC_Adder so it can select from PCPlus8_o and
    // BranchTarget_o (set by Commit_Unit later in the same cycle via bus
    // signals cleared at the top of executeCycle).
    m_coordinator.addComponent(&m_pc);
    m_coordinator.addComponent(&m_pcAdder);
    m_coordinator.addComponent(&m_pcMux);

    // ── Phase 2: I-Cache + Decode ─────────────────────────────────
    m_coordinator.addComponent(&m_iCache);
    m_coordinator.addComponent(&m_decoder);
    m_coordinator.addComponent(&m_extends);

    // ── Phase 3: Operand Read (ROB / RegFile / Flags) ─────────────
    // Flags_Unit before regFile / ROB so flag forwarding is visible.
    m_coordinator.addComponent(&m_flagsUnit);
    m_coordinator.addComponent(&m_regFile);
    m_coordinator.addComponent(&m_rob);

    // ── Phase 4: RS / Buffers (expose busy flags before Control_Unit) ─
    m_coordinator.addComponent(&m_sb0);
    m_coordinator.addComponent(&m_sb1);
    m_coordinator.addComponent(&m_lb0);
    m_coordinator.addComponent(&m_lb1);
    m_coordinator.addComponent(&m_lb2);
    m_coordinator.addComponent(&m_rsIntALU0);
    m_coordinator.addComponent(&m_rsIntALU1);
    m_coordinator.addComponent(&m_rsFPALU0);
    m_coordinator.addComponent(&m_rsIntMUL0);
    m_coordinator.addComponent(&m_rsFPMUL0);
    m_coordinator.addComponent(&m_rsBranch0);

    // ── Phase 5: Issue Decision ────────────────────────────────────
    // Control_Unit reads all busy flags to decide ROBAlloc, AllocStation, etc.
    m_coordinator.addComponent(&m_controlUnit);

    // ── Phase 6: AGU ──────────────────────────────────────────────
    m_coordinator.addComponent(&m_aguArbiter);
    m_coordinator.addComponent(&m_agu0);
    m_coordinator.addComponent(&m_agu1);

    // ── Phase 7: Execution Units ───────────────────────────────────
    m_coordinator.addComponent(&m_intALUArbiter);
    m_coordinator.addComponent(&m_intALU);
    m_coordinator.addComponent(&m_fpaluArbiter);
    m_coordinator.addComponent(&m_fpalu);
    m_coordinator.addComponent(&m_intMULArbiter);
    m_coordinator.addComponent(&m_intMUL);
    m_coordinator.addComponent(&m_fpmulArbiter);
    m_coordinator.addComponent(&m_fpmul);
    m_coordinator.addComponent(&m_branchArbiter);
    m_coordinator.addComponent(&m_branchExecutor);

    // ── Phase 8: CDB ──────────────────────────────────────────────
    m_coordinator.addComponent(&m_cdbArbiter);
    m_coordinator.addComponent(&m_cdbA);
    m_coordinator.addComponent(&m_cdbB);

    // ── Phase 9: Memory ───────────────────────────────────────────
    m_coordinator.addComponent(&m_memArbiter);
    m_coordinator.addComponent(&m_dCache);

    // ── Phase 10: Commit ──────────────────────────────────────────
    // Must be last so BranchRedirect / Flush are set before clockEdge().
    m_coordinator.addComponent(&m_commitUnit);
}

// ============================================================================
// Step
// ============================================================================

uint64_t TomasuloCPU::step() {
    if (m_halted) {
        std::cout << "[TomasuloCPU] CPU halted (SWI). Ignoring step.\n";
        return m_coordinator.cycleCount();
    }

    uint64_t c = m_coordinator.executeCycle(m_bus);

    // ── Update diagram slot labels ────────────────────────────────
    m_tracker.update(m_bus);

    // ── SWI halt check ────────────────────────────────────────────
    if (m_bus.Halt_o) {
        m_halted = true;
        std::cout << "[TomasuloCPU] SWI executed. CPU halted.\n";
    }

    // ── Accumulate statistics ─────────────────────────────────────
    if (m_bus.CommitPop_i) {
        ++m_stats.committedInstructions;
        uint8_t type = m_bus.ROBHeadType_o;
        // Type 1 = LOAD, Type 2 = STORE (memory instructions)
        if (type == 1 || type == 2)
            ++m_stats.committedMemoryInstructions;
    }
    if (m_bus.Flush_o) {
        ++m_stats.branchMispredictions;
    }
    if (m_bus.ROBAlloc_o) {
        uint8_t ss = m_bus.AllocSourceStation_o;
        if (ss < 11) ++m_stats.stationUses[ss];
    }

    return c;
}

// ============================================================================
// Compile
// ============================================================================

AssemblyResult TomasuloCPU::compile(const std::string& source) {
    AssemblyResult result = m_assembler.assemble(source);

    if (!result.success) {
        std::cerr << "[TomasuloCPU] Compile failed:\n" << result.summary << std::endl;
        return result;
    }

    m_ram.clear();
    size_t loaded = m_ram.loadBlock(result.instructions);

    // UPPER register tracks the byte address just past the loaded program,
    // which the RAM view and binary loader use as the data segment base.
    if (loaded > 0) {
        uint64_t upperValue = static_cast<uint64_t>(loaded) * TomasuloRAM::kStep;
        m_registers.set(TomasuloRegisterFile::UPPER, upperValue);
    }
    else {
        m_registers.set(TomasuloRegisterFile::UPPER, 0);
    }

    m_coordinator.resetAll(m_bus);

    std::cout << "[TomasuloCPU] Compiled OK: " << loaded << " instructions, UPPER=0x"
        << std::hex << m_registers.get(TomasuloRegisterFile::UPPER)
        << std::dec << ". Pipeline reset.\n";

    return result;
}

// ============================================================================
// Reset
// ============================================================================

void TomasuloCPU::reset() {
    m_ram.clear();
    m_registers.reset();
    m_coordinator.resetAll(m_bus);
    m_stats = Stats{};
    m_tracker.reset();
    m_halted = false;
    std::cout << "[TomasuloCPU] Full reset (RAM + Registers + Pipeline).\n";
}

void TomasuloCPU::resetRAMOnly() {
    m_ram.clear();
    std::cout << "[TomasuloCPU] RAM reset only.\n";
}