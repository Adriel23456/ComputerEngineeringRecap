#pragma once

/**
 * @file PC_MUX.h
 * @brief Component 2: PC multiplexer (sequential vs branch redirect).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class PC_MUX : public ITomasuloComponent {
public:
    const char* name() const override { return "PC_MUX"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;
};