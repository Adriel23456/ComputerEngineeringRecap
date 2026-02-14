#pragma once

/**
 * @file IntMUL_Arbiter.h
 * @brief Component 30: Integer MUL Arbiter. Single RS pass-through.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class IntMUL_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "IntMUL_Arbiter"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override;
};