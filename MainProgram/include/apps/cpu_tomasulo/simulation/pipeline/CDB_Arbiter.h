#pragma once

/**
 * @file CDB_Arbiter.h
 * @brief Component 36: Common Data Bus Arbiter.
 * Selects up to 2 of 7 requestors per cycle for CDB_A and CDB_B.
 * Priority: IntALU > FPALU > IntMUL > FPMUL > LB0 > LB1 > LB2.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class CDB_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "CDB_Arbiter"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override;
};