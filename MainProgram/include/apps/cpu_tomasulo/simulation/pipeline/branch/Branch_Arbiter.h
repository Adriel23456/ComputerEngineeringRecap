// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Arbiter.h
// ============================================================================

#pragma once

/**
 * @file Branch_Arbiter.h
 * @brief Branch Arbiter (Component 34).
 *
 * Single-requestor pass-through arbiter for the Branch Reservation Station.
 *
 * There is only one branch RS (RS_Branch0), so no actual arbitration is
 * needed; this component simply forwards its request signals to the Branch
 * Executor when RSBr0_Req_o is asserted, setting BrEx_Start_o and
 * populating the BrEx_* inputs.
 *
 * Granting RSBr0_Grant_o allows RS_Branch0 to know it has been dispatched
 * (used to suppress duplicate issues in back-to-back cycles).
 *
 * Purely combinational — clockEdge() is a no-op.
 *
 * @note
 *   - SRP: Only handles branch dispatch handshake; no execution logic.
 *   - OCP: Adding a second branch RS only requires extending this arbiter.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class Branch_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "Branch_Arbiter"; }

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< Purely combinational — no state.
    void reset() override;
};