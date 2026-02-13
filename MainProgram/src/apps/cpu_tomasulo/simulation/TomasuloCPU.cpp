/**
 * @file TomasuloCPU.cpp
 * @brief Implementation of TomasuloCPU.
 */

#include "apps/cpu_tomasulo/simulation/TomasuloCPU.h"
#include <iostream>

 // ============================================================================
 // Compile
 // ============================================================================

AssemblyResult TomasuloCPU::compile(const std::string& source) {

    // Step 1 — Assemble
    AssemblyResult result = m_assembler.assemble(source);

    if (!result.success) {
        // Do NOT touch RAM or registers on failure
        std::cerr << "[TomasuloCPU] Compile failed:\n" << result.summary << std::endl;
        return result;
    }

    // Step 2 — Load instructions into RAM (clear first)
    m_ram.clear();
    size_t loaded = m_ram.loadBlock(result.instructions);

    // Step 3 — Update UPPER register
    //   UPPER = address of last instruction + 8
    //   last instruction address = (loaded - 1) * 8
    //   so UPPER = (loaded - 1) * 8 + 8 = loaded * 8
    if (loaded > 0) {
        uint64_t upperValue = static_cast<uint64_t>(loaded) * TomasuloRAM::kStep;
        m_registers.set(TomasuloRegisterFile::UPPER, upperValue);
    }
    else {
        m_registers.set(TomasuloRegisterFile::UPPER, 0);
    }

    std::cout << "[TomasuloCPU] Compiled OK: " << loaded << " instructions loaded, UPPER = 0x"
        << std::hex << m_registers.get(TomasuloRegisterFile::UPPER)
        << std::dec << std::endl;

    return result;
}

// ============================================================================
// Reset
// ============================================================================

void TomasuloCPU::reset() {
    m_ram.clear();
    m_registers.reset();
    std::cout << "[TomasuloCPU] Full reset.\n";
}

void TomasuloCPU::resetRAMOnly() {
    m_ram.clear();
    std::cout << "[TomasuloCPU] RAM reset only.\n";
}
