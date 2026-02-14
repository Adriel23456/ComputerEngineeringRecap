#pragma once

/**
 * @file FPMUL_Arbiter.h
 * @brief Component 32: FP MUL Arbiter. Single RS pass-through.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class FPMUL_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "FPMUL_Arbiter"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override;
};