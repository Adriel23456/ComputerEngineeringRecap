#pragma once

/**
 * @file Memory_Arbiter.h
 * @brief Component 39: Memory Arbiter.
 *
 * Coordinates access to the single D_Cache port from committed stores
 * (highest priority) and load buffer memory requests (LB0 > LB1 > LB2).
 * Tracks which requestor is active so the D_Cache response is routed back.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class Memory_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "Memory_Arbiter"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    enum class Source : uint8_t { NONE = 0, STORE_COMMIT = 1, LB0 = 2, LB1 = 3, LB2 = 4 };

    Source m_currentSource = Source::NONE;
    bool   m_waitingForDCache = false;

    static uint8_t decodeSize(uint8_t op);
};