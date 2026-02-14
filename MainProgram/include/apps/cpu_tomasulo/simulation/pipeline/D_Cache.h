#pragma once

/**
 * @file D_Cache.h
 * @brief Component 40: Data Cache.
 *
 * 4-way set-associative, write-back, write-allocate.
 * 32 sets x 4 ways x 64 bytes/line = 8 KB.
 * Hit latency: 1 cycle. Miss penalty: RAM latency (50+ cycles).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class D_Cache : public ITomasuloComponent {
public:
    const char* name() const override { return "D_Cache"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    static constexpr int NUM_SETS = 32;
    static constexpr int NUM_WAYS = 4;
    static constexpr int LINE_BYTES = 64;
    static constexpr int WORDS_PER_LINE = 8;  // 64 / 8

    struct CacheLine {
        uint64_t tag = 0;
        uint8_t  data[LINE_BYTES] = {};
        bool     valid = false;
        bool     dirty = false;
    };

    CacheLine m_lines[NUM_SETS][NUM_WAYS];
    uint8_t   m_lru[NUM_SETS][NUM_WAYS] = {};  // Higher = more recently used

    // Miss handling state
    bool     m_missPending = false;
    bool     m_writebackPending = false;
    uint64_t m_missAddr = 0;
    uint64_t m_writebackAddr = 0;
    uint8_t  m_writebackData[LINE_BYTES] = {};
    int      m_victimWay = 0;
    int      m_missSet = 0;

    // Pending request state (saved while handling miss)
    bool     m_pendingReq = false;
    bool     m_pendingRW = false;
    uint64_t m_pendingAddr = 0;
    uint64_t m_pendingWData = 0;
    uint8_t  m_pendingSize = 0;

    static uint64_t extractTag(uint64_t addr) { return addr >> 11; }
    static int      extractSet(uint64_t addr) { return (addr >> 6) & 0x1F; }
    static int      extractOffset(uint64_t addr) { return addr & 0x3F; }
    static uint64_t alignToLine(uint64_t addr) { return addr & ~(uint64_t)63; }

    int  findWay(int set, uint64_t tag) const;
    int  lruWay(int set) const;
    void updateLRU(int set, int way);
    void readLineFromBus(int set, int way, const uint64_t ramData[8]);
    void writeLineToBus(int set, int way, uint64_t ramData[8]) const;
};