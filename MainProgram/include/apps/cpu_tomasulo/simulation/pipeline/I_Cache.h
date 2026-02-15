#pragma once

/**
 * @file I_Cache.h
 * @brief Component 4: Instruction Cache.
 *
 * 4-way set-associative, 32 sets, 64-byte lines (8 instructions per line).
 * Hit = same cycle. Miss = stall until RAM responds.
 * Next-line prefetch on hit.
 *
 * @note SRP: Only handles instruction fetch caching.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>
#include <array>

class TomasuloRAM;

class I_Cache : public ITomasuloComponent {
public:
    static constexpr int NUM_SETS = 8;
    static constexpr int NUM_WAYS = 4;
    static constexpr int LINE_SIZE = 64;   // bytes
    static constexpr int WORDS_PER_LINE = 8;    // 64 / 8

    explicit I_Cache(const TomasuloRAM* ram);

    const char* name() const override { return "I_Cache"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

    // ── UI read access ──────────────────────────────────────────
    bool     lineValid(int set, int way) const;
    uint64_t lineTag(int set, int way) const;
    const uint64_t* lineData(int set, int way) const; // 8 x uint64_t
    int numSets() const { return NUM_SETS; }
    int numWays() const { return NUM_WAYS; }
    int wordsPerLine() const { return WORDS_PER_LINE; }
    uint64_t missStallCycles() const { return m_missStallCycles; }
    uint64_t missCount() const { return m_missTotal; }

private:
    uint64_t m_missTotal = 0;
    uint64_t m_missStallCycles = 0;

    struct CacheLine {
        uint64_t tag = 0;
        uint64_t data[WORDS_PER_LINE] = {};
        bool     valid = false;
    };

    struct CacheSet {
        CacheLine ways[NUM_WAYS] = {};
        uint8_t   lruOrder[NUM_WAYS] = { 0, 1, 2, 3 }; // lruOrder[0] = MRU
    };

    std::array<CacheSet, NUM_SETS> m_sets{};

    // Miss handling
    bool     m_missPending = false;
    uint64_t m_missAddr = 0;
    bool     m_prefetchPending = false;
    uint64_t m_prefetchAddr = 0;

    // RAM reference for direct fill (simulation simplification)
    const TomasuloRAM* m_ram = nullptr;

    // Latency simulation
    int      m_missCounter = 0;
    int      m_prefetchCounter = 0;
    static constexpr int MISS_LATENCY = 50;

    // Helpers
    uint64_t extractTag(uint64_t addr) const;
    uint32_t extractSet(uint64_t addr) const;
    uint32_t extractOffset(uint64_t addr) const;
    uint64_t alignToLine(uint64_t addr) const;
    int  findWay(uint32_t set, uint64_t tag) const;
    int  lruVictim(uint32_t set) const;
    void touchWay(uint32_t set, int way);
    void fillLine(uint32_t set, int way, uint64_t lineAddr);
};