#pragma once

/**
 * @file TomasuloCPU.h
 * @brief Central simulation coordinator for Tomasulo CPU.
 *
 * Owns all simulation-side components (RAM, Registers, Assembler)
 * and provides high-level operations like compile().
 *
 * @note
 *   - SRP: Coordinates simulation components, delegates work
 *   - OCP: New simulation stages (caches, ROB, etc.) plug in here
 *   - DIP: UI layer depends on this, not the reverse
 */

#include "apps/cpu_tomasulo/simulation/assembler/TomasuloAssembler.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"
#include "apps/cpu_tomasulo/simulation/processor/TomasuloRegisterFile.h"
#include <string>

 /**
  * @class TomasuloCPU
  * @brief Top-level simulation object for the Tomasulo CPU.
  *
  * Compile workflow:
  *   1. Assembler converts source text → vector<uint64_t>
  *   2. RAM is cleared, then instructions are loaded starting at address 0x0
  *   3. UPPER register is set to  (last_instruction_address + 8)
  *
  * If assembly fails, RAM and registers are left unchanged.
  */
class TomasuloCPU {
public:
    TomasuloCPU() = default;

    // ── High-level operations ───────────────────────────────────

    /**
     * @brief Assembles source code and loads it into RAM.
     *
     * On success:
     *   - RAM is cleared and loaded with instructions from address 0x0.
     *   - UPPER register = address of last instruction + 8.
     *
     * On failure:
     *   - RAM and registers are NOT modified.
     *
     * @param source Assembly source text.
     * @return AssemblyResult (success flag, errors, summary).
     */
    AssemblyResult compile(const std::string& source);

    /**
     * @brief Resets the entire CPU state (RAM + registers).
     */
    void reset();

    /**
    * @brief Resets only RAM (keeps registers/state intact).
    */
    void resetRAMOnly();

    // ── Component access (read-only for UI sync) ────────────────

    const TomasuloRAM& ram()       const { return m_ram; }
    const TomasuloRegisterFile& registers() const { return m_registers; }

    TomasuloRAM& ram() { return m_ram; }
    TomasuloRegisterFile& registers() { return m_registers; }

private:
    TomasuloAssembler    m_assembler;
    TomasuloRAM          m_ram;
    TomasuloRegisterFile m_registers;
};