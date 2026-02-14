#pragma once

/**
 * @file PC_C.h
 * @brief Component 1: Program Counter register.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class PC_C : public ITomasuloComponent {
public:
    const char* name() const override { return "PC_C"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

    uint64_t currentPC() const { return m_register; }

private:
    uint64_t m_register = 0;
};