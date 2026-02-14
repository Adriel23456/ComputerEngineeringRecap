/**
 * @file TomasuloCPU.cpp
 * @brief Implementation of TomasuloCPU.
 */

#include "apps/cpu_tomasulo/simulation/TomasuloCPU.h"
#include <iostream>

 // ============================================================================
 // Construction
 // ============================================================================

TomasuloCPU::TomasuloCPU()
    : m_iCache(&m_ram),
    m_regFile(&m_registers)   // Pipeline wrapper over architectural registers
{
    buildPipeline();
    std::cout << "[TomasuloCPU] Constructed. Pipeline components registered.\n";
}

void TomasuloCPU::buildPipeline() {
    // ── Phase 1: Fetch ──────────────────────────────────────────
    m_coordinator.addComponent(&m_pcAdder);
    m_coordinator.addComponent(&m_pcMux);
    m_coordinator.addComponent(&m_pc);

    // ── Phase 2: I-Cache + Decode ───────────────────────────────
    m_coordinator.addComponent(&m_iCache);
    m_coordinator.addComponent(&m_decoder);

    // ── Phase 3: Control + Extends ──────────────────────────────
    m_coordinator.addComponent(&m_extends);
    m_coordinator.addComponent(&m_controlUnit);

    // ── Phase 4: Register File + ROB + Flags ────────────────────
    // Order: Flags_Unit first (outputs FlagsQi/FlagsArch for RS capture),
    //        then RegisterFile (outputs Qi tags and values),
    //        then ROB (reads Qi tags to provide forwarding values).
    m_coordinator.addComponent(&m_flagsUnit);     // 11. Flags -> FlagsArch, FlagsQi
    m_coordinator.addComponent(&m_regFile);        // 9.  Rn,Rm -> values + Qi tags
    m_coordinator.addComponent(&m_rob);            // 10. Qi -> ROBReadReady, ROBReadValue

    // -- Phase 5: Store Buffers (alloc + CDB snoop + AGU req) --
    m_coordinator.addComponent(&m_sb0);
    m_coordinator.addComponent(&m_sb1);

    // -- Phase 6: Load Buffers (alloc + CDB snoop + AGU req + mem req + CDB pub) --
    m_coordinator.addComponent(&m_lb0);
    m_coordinator.addComponent(&m_lb1);
    m_coordinator.addComponent(&m_lb2);

    // -- Phase 7: Reservation Stations (alloc + CDB snoop + dispatch request) --
    m_coordinator.addComponent(&m_rsIntALU0);
    m_coordinator.addComponent(&m_rsIntALU1);
    m_coordinator.addComponent(&m_rsFPALU0);
    m_coordinator.addComponent(&m_rsIntMUL0);
    m_coordinator.addComponent(&m_rsFPMUL0);
    m_coordinator.addComponent(&m_rsBranch0);

    // -- Phase 8: AGU pipeline --
    m_coordinator.addComponent(&m_aguArbiter);
    m_coordinator.addComponent(&m_agu0);
    m_coordinator.addComponent(&m_agu1);

    // -- Phase 9: Execution arbiters -> execution units --
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

    // -- Phase 10: CDB arbitration and broadcast --
    m_coordinator.addComponent(&m_cdbArbiter);
    m_coordinator.addComponent(&m_cdbA);
    m_coordinator.addComponent(&m_cdbB);

    // -- Phase 11: Memory subsystem --
    m_coordinator.addComponent(&m_memArbiter);
    m_coordinator.addComponent(&m_dCache);

    // -- Phase 12: Commit (must run AFTER CDB broadcast so ROB is up-to-date) --
    m_coordinator.addComponent(&m_commitUnit);
}

// ============================================================================
// Step
// ============================================================================

uint64_t TomasuloCPU::step() {
    return m_coordinator.executeCycle(m_bus);
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

    if (loaded > 0) {
        uint64_t upperValue = static_cast<uint64_t>(loaded) * TomasuloRAM::kStep;
        m_registers.set(TomasuloRegisterFile::UPPER, upperValue);
    }
    else {
        m_registers.set(TomasuloRegisterFile::UPPER, 0);
    }

    // Reset pipeline after compile (new program loaded)
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
    std::cout << "[TomasuloCPU] Full reset (RAM + Registers + Pipeline).\n";
}

void TomasuloCPU::resetRAMOnly() {
    m_ram.clear();
    std::cout << "[TomasuloCPU] RAM reset only.\n";
}