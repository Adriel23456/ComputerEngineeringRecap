// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/cdb/CDB_Arbiter.h
// ============================================================================

#pragma once

/**
 * @file CDB_Arbiter.h
 * @brief Component 36: Common Data Bus Arbiter.
 *
 * Selects up to 2 of 7 requestors per cycle for CDB_A and CDB_B.
 * Priority: IntALU > FPALU > IntMUL > FPMUL > LB0 > LB1 > LB2.
 *
 * The first winner is forwarded to CDB_A, the second to CDB_B.
 * All remaining requestors receive a CDBStall signal and must hold
 * their results for re-arbitration in the next cycle.
 *
 * @note Load buffers never produce flag results; FlagsValid is forced 0.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class CDB_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "CDB_Arbiter"; }
    /** @brief Selects up to 2 winners and drives CDBStall for the losers. */
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< Pure combinational — no state.
    void reset() override;
};