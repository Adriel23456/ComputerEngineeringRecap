// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/issue/Flags_Unit.h
// ============================================================================

#pragma once

/**
 * @file Flags_Unit.h
 * @brief Component 11: Architectural Flags register + speculative Qi tag.
 *
 * Maintains the 4-bit NZCV architectural flags and a single speculative
 * rename tag (Qi) pointing to the ROB entry that will produce the next
 * architectural flags value.
 *
 * 3-Tier resolution for flag consumers (ADC, SBC, conditional branches):
 *   Tier 1: FlagsQiValid == false
 *              → use FlagsArch_o directly (no pending producer).
 *   Tier 2: FlagsQiValid == true  AND  ROB[FlagsQi].ready == true
 *              → forward flags from the ROB entry (in-order commit not yet).
 *   Tier 3: FlagsQiValid == true  AND  ROB[FlagsQi].ready == false
 *              → stall and wait for the CDB to broadcast the result.
 *   Fallback: If FlagsQiValid drops to false while waiting (flush/commit)
 *              → consumer switches back to FlagsArch.
 *
 * Clock-edge priority:
 *   1. Flush      → clear m_qiValid (keep architectural flags).
 *   2. CommitEn   → write m_archFlags; clear tag if it matches committed ROB idx.
 *   3. ROBAlloc+FlagsTagWrEn → rename: set m_qi = ROBTail, m_qiValid = true.
 *
 * @note SRP: Only manages the 4-bit NZCV flags and their speculative tag.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class Flags_Unit : public ITomasuloComponent {
public:
    const char* name() const override { return "Flags_Unit"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    uint8_t m_archFlags = 0;      ///< Committed NZCV flags: bits [3:0] = N,Z,C,V.
    uint8_t m_qi = 0;      ///< ROB index of the in-flight flags producer (5 bits used).
    bool    m_qiValid = false;  ///< True when a speculative flags producer is pending.
};