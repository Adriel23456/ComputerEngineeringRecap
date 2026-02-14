#pragma once

/**
 * @file Instruction_Decoder.h
 * @brief Component 6: Instruction Decoder (combinational bit-slicer).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class Instruction_Decoder : public ITomasuloComponent {
public:
    const char* name() const override { return "Instruction_Decoder"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;
};