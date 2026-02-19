// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/execution/FPALU_Arbiter.h
// ============================================================================

#pragma once

/**
 * @file FPALU_Arbiter.h
 * @brief Component 28: FP ALU Arbiter. Single RS pass-through.
 *
 * There is only one FP ALU reservation station (RS_FPALU0), so no
 * multi-way arbitration is needed. This component simply forwards
 * RS_FPALU0's request to the FPALU unit when FPALU_Free_o is asserted.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class FPALU_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "FPALU_Arbiter"; }
    /** @brief Grants RS_FPALU0 and dispatches to FPALU when the unit is free. */
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< Pure combinational — no state.
    void reset() override;
};