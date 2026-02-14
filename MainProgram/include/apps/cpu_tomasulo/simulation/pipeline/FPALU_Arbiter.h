#pragma once

/**
 * @file FPALU_Arbiter.h
 * @brief Component 28: FP ALU Arbiter. Single RS pass-through.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class FPALU_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "FPALU_Arbiter"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override;
};