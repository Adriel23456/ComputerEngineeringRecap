#pragma once

/**
 * @file IntALU.h
 * @brief Component 27: Integer ALU. Single-cycle, purely combinational.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class IntALU : public ITomasuloComponent {
public:
    const char* name() const override { return "IntALU"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override;
};