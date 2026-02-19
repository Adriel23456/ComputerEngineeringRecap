// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_MUX.h
// ============================================================================

#pragma once

/**
 * @file PC_MUX.h
 * @brief Component 2: PC multiplexer — sequential vs. branch redirect.
 *
 * Selects the next PC value to be loaded into PC_C:
 *   BranchRedirect_o == true  → PCNext_o = BranchTarget_o  (taken / mispredicted)
 *   BranchRedirect_o == false → PCNext_o = PCPlus8_o       (sequential)
 *
 * Purely combinational — no internal state.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class PC_MUX : public ITomasuloComponent {
public:
    const char* name() const override { return "PC_MUX"; }

    /** @brief Combinational: selects PCNext_o from BranchTarget_o or PCPlus8_o. */
    void evaluate(TomasuloBus& bus) override;

    /** @brief No sequential state — nothing to latch. */
    void clockEdge(TomasuloBus& bus) override;

    /** @brief No internal state to clear. */
    void reset() override;
};