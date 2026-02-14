#pragma once

/**
 * @file IntALU_Arbiter.h
 * @brief Component 26: Integer ALU Arbiter.
 * Selects between RS_IntALU0 and RS_IntALU1. Priority: RS0 > RS1.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class IntALU_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "IntALU_Arbiter"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override;
};