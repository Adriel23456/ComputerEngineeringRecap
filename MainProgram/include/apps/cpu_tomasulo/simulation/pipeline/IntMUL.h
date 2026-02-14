#pragma once

/**
 * @file IntMUL.h
 * @brief Component 31: Integer Multiplier/Divider. NOT pipelined.
 * MUL: 4 cycles, DIV: 16 cycles.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class IntMUL : public ITomasuloComponent {
public:
    const char* name() const override { return "IntMUL"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    bool     m_busy = false;
    int      m_cyclesRemaining = 0;
    uint8_t  m_op = 0;
    uint64_t m_A = 0;
    uint64_t m_B = 0;
    uint8_t  m_robTag = 0;
    uint64_t m_result = 0;
    uint8_t  m_flags = 0;
    bool     m_flagsValid = false;
    uint8_t  m_exception = 0;
    bool     m_done = false;
};