#pragma once

/**
 * @file FPALU.h
 * @brief Component 29: Floating-Point ALU. 3-stage pipeline.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class FPALU : public ITomasuloComponent {
public:
    const char* name() const override { return "FPALU"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    struct Stage {
        bool     active = false;
        uint8_t  op = 0;
        uint64_t A = 0;
        uint64_t B = 0;
        uint64_t result = 0;
        uint8_t  robTag = 0;
        bool     modFlags = false;
        uint8_t  flags = 0;
        bool     flagsValid = false;
        uint8_t  exception = 0;
    };

    Stage m_stage1;
    Stage m_stage2;
    Stage m_stage3;
    Stage m_outputHold;

    static double toDouble(uint64_t bits);
    static uint64_t fromDouble(double val);
    static uint8_t computeFCMPFlags(uint64_t A, uint64_t B);
};