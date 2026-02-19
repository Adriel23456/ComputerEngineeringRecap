// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/execution/IntMUL_Arbiter.h
// ============================================================================

#pragma once

/**
 * @file IntMUL_Arbiter.h
 * @brief Component 30: Integer MUL Arbiter. Single RS pass-through.
 *
 * There is only one integer MUL reservation station (RS_IntMUL0), so no
 * multi-way arbitration is needed. Forwards the request directly to
 * IntMUL when the unit is not busy.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class IntMUL_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "IntMUL_Arbiter"; }
    /** @brief Grants RS_IntMUL0 and dispatches to IntMUL when the unit is free. */
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< Pure combinational — no state.
    void reset() override;
};