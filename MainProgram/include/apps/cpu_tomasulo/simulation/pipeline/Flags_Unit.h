#pragma once

/**
 * @file Flags_Unit.h
 * @brief Component 11: Architectural Flags register + speculative Qi tag.
 *
 * 3-Tier resolution for flag consumers (ADC, SBC, conditional branches):
 *   Tier 1: FlagsQiValid=false -> use FlagsArch (no pending producer)
 *   Tier 2: FlagsQiValid=true, ROB[FlagsQi].ready -> forward from ROB
 *   Tier 3: FlagsQiValid=true, ROB not ready -> wait for CDB snoop
 *   Fallback: If FlagsQiValid drops to false while waiting -> use updated FlagsArch
 *
 * @note SRP: Only manages the 4-bit NZCV flags and their speculative tag.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class Flags_Unit : public ITomasuloComponent {
public:
    const char* name() const override { return "Flags_Unit"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    uint8_t m_archFlags = 0;     // 4 bits: [N, Z, C, V]
    uint8_t m_qi = 0;     // 5 bits: ROB index of pending producer
    bool    m_qiValid = false;
};