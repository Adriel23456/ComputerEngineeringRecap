// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/branch/RS_Branch0.h
// ============================================================================

#pragma once

/**
 * @file RS_Branch0.h
 * @brief Branch Reservation Station (Component 22).
 *
 * Holds a branch instruction from issue until its flag dependency is
 * resolved, then presents a dispatch request to Branch_Arbiter.
 *
 * Key differences from integer/FP reservation stations:
 *   - No register operands (Rn/Rm). Branch target = PC + ImmExt, computed
 *     at allocation time and stored internally.
 *   - Unconditional B (0x45) is immediately ready on allocation.
 *   - Conditional branches depend only on the architectural flags register.
 *   - Always predict not-taken (m_predictedTaken = false).
 *
 * 3-tier flags resolution (evaluated at clockEdge allocation):
 *   Tier 1 — FlagsQiValid_o == false  → no in-flight writer; use FlagsArch_o.
 *   Tier 2 — ROBReadFlagsReady_o == true → producer finished; forward value.
 *   Tier 3 — producer still executing   → save FlagsQi, wait for CDB broadcast.
 *   Fallback — FlagsQiValid_o drops to false mid-wait → use updated FlagsArch_o.
 *
 * @note
 *   - SRP: Only tracks one pending branch and its flag dependency.
 *   - DRY: Flag resolution mirrors the pattern used in integer RS classes.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class RS_Branch0 : public ITomasuloComponent {
public:
    RS_Branch0() = default;
    const char* name() const override { return "RS_Branch0"; }

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    bool     m_busy = false;
    uint8_t  m_op = 0;
    uint64_t m_targetPC = 0;   ///< PC + ImmExt, latched at allocation.
    bool     m_predictedTaken = false;
    uint8_t  m_robTag = 0;

    uint8_t  m_flagsQi = 0;
    bool     m_flagsQiValid = false;
    uint8_t  m_flagsValue = 0;   ///< Resolved NZCV value (4 bits).
};