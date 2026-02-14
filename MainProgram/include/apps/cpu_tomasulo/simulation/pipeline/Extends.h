#pragma once

/**
 * @file Extends.h
 * @brief Component 7: Immediate Extender (32->64 bit).
 *
 * Integer: sign-extend. FP: float32 -> float64 bit conversion.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"

class Extends : public ITomasuloComponent {
public:
    const char* name() const override { return "Extends"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    static uint64_t signExtend32(uint32_t imm);
    static uint64_t float32ToDouble64Bits(uint32_t imm);
};