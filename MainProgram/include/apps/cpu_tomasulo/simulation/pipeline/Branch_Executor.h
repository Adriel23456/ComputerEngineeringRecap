#pragma once

/**
 * @file Branch_Executor.h
 * @brief Component 35: Branch Executor. Purely combinational.
 * Evaluates branch condition, detects mispredictions. Results go to ROB, not CDB.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class Branch_Executor : public ITomasuloComponent {
public:
    const char* name() const override { return "Branch_Executor"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override;
};