#pragma once

/**
 * @file FPMUL.h
 * @brief Component 33: FP MUL/DIV/SQRT. NOT pipelined.
 * FMUL: 5 cycles, FDIV: 20 cycles, FSQRT: 25 cycles.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class FPMUL : public ITomasuloComponent {
public:
    const char* name() const override { return "FPMUL"; }
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
    uint8_t  m_exception = 0;
    bool     m_done = false;

    static double toDouble(uint64_t bits);
    static uint64_t fromDouble(double val);
};