// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/commit/Memory_Arbiter.h
// ============================================================================

#pragma once

/**
 * @file Memory_Arbiter.h
 * @brief Component 39: Memory Arbiter.
 *
 * Coordinates access to the single D_Cache port from committed stores
 * (highest priority) and load buffer memory requests (LB0 > LB1 > LB2).
 * Tracks which requestor is active so the D_Cache response is routed back.
 *
 * Load-ordering safety:
 *   A load is suppressed (storeConflict returns true) when an older store
 *   in the ROB has already resolved its address to the same word. The load
 *   must wait until that store commits and the address is no longer exposed.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class Memory_Arbiter : public ITomasuloComponent {
public:
    const char* name() const override { return "Memory_Arbiter"; }
    /** @brief Combinational stub — all work is done in clockEdge. */
    void evaluate(TomasuloBus& bus) override;
    /** @brief Issues new D_Cache requests and routes completed responses. */
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    /** @brief Identifies which requestor currently owns the D_Cache port. */
    enum class Source : uint8_t { NONE = 0, STORE_COMMIT = 1, LB0 = 2, LB1 = 3, LB2 = 4 };

    Source m_currentSource = Source::NONE; ///< Requestor that issued the in-flight D_Cache access.
    bool   m_waitingForDCache = false;         ///< True while a D_Cache request has not yet completed.

    /**
     * @brief Maps a full opcode to the DC_Size_o encoding.
     * LDR/STR -> 0x03 (8 bytes), LDRB/STRB -> 0x00 (1 byte).
     */
    static uint8_t decodeSize(uint8_t op);

    /**
     * @brief Returns true if @p storeTag is older than @p loadTag in the ROB
     *        (i.e., closer to the head in circular ROB order).
     */
    static bool isOlderInROB(uint8_t storeTag, uint8_t loadTag, uint8_t head, uint8_t robSize);

    /**
     * @brief Returns true when an older, address-resolved store targets the
     *        same aligned word as the given load, blocking speculative loads.
     */
    static bool storeConflict(const TomasuloBus& bus, uint64_t loadAddr, uint8_t loadROBTag);
};