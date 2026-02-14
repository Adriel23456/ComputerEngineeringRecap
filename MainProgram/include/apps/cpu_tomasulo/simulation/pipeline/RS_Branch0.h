#pragma once

/**
 * @file RS_Branch0.h
 * @brief Component 22: Branch Reservation Station.
 *
 * Unlike other RS, branches have NO register operands.
 * They only depend on flags (conditional) or are always ready (unconditional B).
 * Target PC is pre-computed at issue: PC + ImmExt.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
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
    uint64_t m_targetPC = 0;
    bool     m_predictedTaken = false;
    uint8_t  m_robTag = 0;

    uint8_t  m_flagsQi = 0;
    bool     m_flagsQiValid = false;
    uint8_t  m_flagsValue = 0;
};