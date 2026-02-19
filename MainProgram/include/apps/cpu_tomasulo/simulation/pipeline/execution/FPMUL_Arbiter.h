// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/execution/FPMUL_Arbiter.h
// ============================================================================

#pragma once

/**
 * @file FPMUL_Arbiter.h
 * @brief Component 32: FP MUL Arbiter. Single RS pass-through.
 *
 * There is only one FP MUL reservation station (RS_FPMUL0), so no
 * multi-way arbitration is needed. Forwards the request directly to
 * FPMUL when the unit is not busy.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class FPMUL_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "FPMUL_Arbiter"; }
    /** @brief Grants RS_FPMUL0 and dispatches to FPMUL when the unit is free. */
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< Pure combinational — no state.
    void reset() override;
};