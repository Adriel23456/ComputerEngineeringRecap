// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_C.h
// ============================================================================

#pragma once

/**
 * @file PC_C.h
 * @brief Component 1: Program Counter register.
 *
 * Holds the address of the instruction currently being fetched.
 * Updated on the clock edge according to priority:
 *   1. RESET signal     → jump to 0x0000000000000000
 *   2. Flush_o (branch) → jump to BranchTarget_o
 *   3. ROBAlloc_o       → advance to PCNext_o (sequential or predicted)
 *
 * Combinational output: PCCurrent_o = m_register.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class PC_C : public ITomasuloComponent {
public:
    const char* name() const override { return "PC_C"; }

    /** @brief Drives PCCurrent_o with the stored register value. */
    void evaluate(TomasuloBus& bus) override;

    /** @brief Updates the register based on RESET / Flush / ROBAlloc priority. */
    void clockEdge(TomasuloBus& bus) override;

    /** @brief Clears the register to zero. */
    void reset() override;

    /** @brief Returns the current PC value (for debug / UI). */
    uint64_t currentPC() const { return m_register; }

private:
    uint64_t m_register = 0; ///< Latched PC value (updated on clock edge).
};