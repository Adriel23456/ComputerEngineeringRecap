#pragma once

/**
 * @file Branch_Arbiter.h
 * @brief Component 34: Branch Arbiter. Single RS pass-through.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class Branch_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "Branch_Arbiter"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override;
};