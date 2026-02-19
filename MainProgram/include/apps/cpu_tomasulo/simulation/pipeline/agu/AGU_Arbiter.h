// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_Arbiter.h
// ============================================================================

#pragma once

/**
 * @file AGU_Arbiter.h
 * @brief AGU Arbiter (Component 23).
 *
 * Collects pending address-calculation requests from all 5 memory
 * buffers (SB0, SB1, LB0, LB1, LB2) and dispatches up to 2 per cycle
 * to the two AGU units (AGU_0, AGU_1).
 *
 * Priority order (highest first):
 *   SB0 > SB1 > LB0 > LB1 > LB2
 *
 * Stores are prioritised over loads to minimise commit stalls — a store
 * cannot commit until its address and data are both known.
 *
 * Result routing:
 *   The AGU units themselves write back to the per-requestor done signals
 *   (SBx_AGUDone_o, LBx_AGUDone_o) using the SourceID they received from
 *   the arbiter. This component only sets the AGUx_Start_o / AGUx_Base_o /
 *   AGUx_Offset_o / AGUx_SourceID_o bus signals.
 *
 * Purely combinational — clockEdge() is a no-op.
 *
 * @note
 *   - SRP: Only handles AGU dispatch arbitration.
 *   - OCP: Priority policy is data-driven; no code changes needed to add
 *          requestors (bus fields and the pending[] array extend naturally).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class AGU_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "AGU_Arbiter"; }

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< Purely combinational — no state.
    void reset() override;
};