#pragma once

/**
 * @file AGU_Arbiter.h
 * @brief Component 23: Address Generation Unit Arbiter.
 *
 * Dispatches up to 2 address calculations per cycle from 5 requestors
 * (SB0, SB1, LB0, LB1, LB2) to 2 AGU units (AGU_0, AGU_1).
 * Priority: SB0 > SB1 > LB0 > LB1 > LB2 (stores first to minimize commit stalls).
 *
 * Result routing is handled by the AGU units themselves (they write directly
 * to per-requestor done signals based on SourceID).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class AGU_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "AGU_Arbiter"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override;
};