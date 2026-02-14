#pragma once

/**
 * @file PC_Adder.h
 * @brief Component 3: PC + 8 adder.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class PC_Adder : public ITomasuloComponent {
public:
    const char* name() const override { return "PC_Adder"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;
};