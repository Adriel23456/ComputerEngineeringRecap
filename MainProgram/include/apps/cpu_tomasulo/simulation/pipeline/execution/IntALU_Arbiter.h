// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/execution/IntALU_Arbiter.h
// ============================================================================

#pragma once

/**
 * @file IntALU_Arbiter.h
 * @brief Component 26: Integer ALU Arbiter.
 *
 * Selects between RS_IntALU0 and RS_IntALU1 and dispatches to the single
 * IntALU execution unit. Priority: RS_IntALU0 > RS_IntALU1.
 *
 * A grant is only issued when IntALU_Free_o is true (i.e., the ALU is not
 * stalled waiting for CDB arbitration). Only one RS can be granted per cycle.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class IntALU_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "IntALU_Arbiter"; }
    /** @brief Selects the winning RS and drives IntALU_Start_o with its operands. */
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< Pure combinational — no state.
    void reset() override;
};