// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_Adder.h
// ============================================================================

#pragma once

/**
 * @file PC_Adder.h
 * @brief Component 3: PC + 8 adder (combinational).
 *
 * Computes the sequential next-PC by adding 8 (one 64-bit instruction word)
 * to the current PC value.  Result is placed on PCPlus8_o each cycle.
 * No internal state — purely combinational.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class PC_Adder : public ITomasuloComponent {
public:
    const char* name() const override { return "PC_Adder"; }

    /** @brief Combinational: PCPlus8_o = PCCurrent_o + 8. */
    void evaluate(TomasuloBus& bus) override;

    /** @brief No sequential state — nothing to latch. */
    void clockEdge(TomasuloBus& bus) override;

    /** @brief No internal state to clear. */
    void reset() override;
};