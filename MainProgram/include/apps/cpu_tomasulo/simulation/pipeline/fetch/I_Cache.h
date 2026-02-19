// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/fetch/I_Cache.h
// ============================================================================

#pragma once

/**
 * @file I_Cache.h
 * @brief Component 4: Instruction Cache.
 *
 * 4-way set-associative, 8 sets, 64-byte lines (8 × 64-bit instructions per line).
 * Hit  = result available same cycle (combinational).
 * Miss = stall until RAM fills the line (MISS_LATENCY cycles).
 *
 * Prefetch policy:
 *   - On every demand hit  : schedule next 2 lines in background prefetch slots.
 *   - On demand miss fill  : immediately queue the next 2 lines.
 *   - On prefetch complete : chain-start the line after it.
 *   - Promoted prefetch    : if the PC reaches a line that is already being
 *                            prefetched, the remaining counter is reused as the
 *                            demand-miss timer (no extra latency).
 *
 * LRU replacement: per-set order array, MRU at index 0, LRU at index NUM_WAYS-1.
 *
 * @note SRP: Only handles instruction fetch caching.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>
#include <array>

class TomasuloRAM;

class I_Cache : public ITomasuloComponent {
public:
    static constexpr int NUM_SETS = 8;   ///< Number of cache sets.
    static constexpr int NUM_WAYS = 4;   ///< Associativity.
    static constexpr int LINE_SIZE = 64;  ///< Bytes per cache line.
    static constexpr int WORDS_PER_LINE = 8;   ///< 64-bit words per cache line.

    explicit I_Cache(const TomasuloRAM* ram);

    const char* name() const override { return "I_Cache"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

    // ── UI / debug read accessors ────────────────────────────────
    bool            lineValid(int set, int way) const;
    uint64_t        lineTag(int set, int way) const;
    const uint64_t* lineData(int set, int way) const;
    int  numSets()       const { return NUM_SETS; }
    int  numWays()       const { return NUM_WAYS; }
    int  wordsPerLine()  const { return WORDS_PER_LINE; }

    /** @brief Total number of demand misses since last reset. */
    uint64_t missCount()       const { return m_missTotal; }
    /** @brief Total cycles the pipeline was stalled due to demand misses. */
    uint64_t missStallCycles() const { return m_missStallCycles; }

private:
    uint64_t m_missTotal = 0; ///< Demand miss event counter (for analysis view).
    uint64_t m_missStallCycles = 0; ///< Stall cycles accumulated while waiting for RAM.

    // ── Cache data structure ─────────────────────────────────────
    struct CacheLine {
        uint64_t tag = 0;
        uint64_t data[WORDS_PER_LINE] = {};
        bool     valid = false;
    };

    struct CacheSet {
        CacheLine ways[NUM_WAYS] = {};
        uint8_t   lruOrder[NUM_WAYS] = { 0, 1, 2, 3 }; ///< Index 0 = MRU, index NUM_WAYS-1 = LRU.
    };

    std::array<CacheSet, NUM_SETS> m_sets{};

    // ── Demand miss state ────────────────────────────────────────
    bool     m_missPending = false; ///< True while stalling on a demand miss.
    uint64_t m_missAddr = 0;     ///< Line address of the outstanding demand miss.
    int      m_missCounter = 0;     ///< Cycles remaining until the demand miss resolves.

    // ── Prefetch slots (two lines ahead) ─────────────────────────
    static constexpr int NUM_PREFETCH = 2;
    struct PrefetchSlot {
        bool     pending = false;
        uint64_t addr = 0;
        int      counter = 0;
    };
    PrefetchSlot m_prefetch[NUM_PREFETCH] = {};

    const TomasuloRAM* m_ram = nullptr;

    static constexpr int MISS_LATENCY = 50; ///< Cycles to fill a line from RAM.

    // ── Private helpers ──────────────────────────────────────────
    uint64_t extractTag(uint64_t addr) const;  ///< Bits [63:9]
    uint32_t extractSet(uint64_t addr) const;  ///< Bits [8:6]
    uint32_t extractOffset(uint64_t addr) const;  ///< Bits [5:3] (word index within line)
    uint64_t alignToLine(uint64_t addr) const;  ///< Mask off the byte-offset bits.
    int      findWay(uint32_t set, uint64_t tag) const;  ///< Returns way index or -1.
    int      lruVictim(uint32_t set) const;                 ///< Returns the LRU way index.
    void     touchWay(uint32_t set, int way);              ///< Promotes way to MRU position.
    void     fillLine(uint32_t set, int way, uint64_t lineAddr); ///< Load line from RAM.
    bool     isLineCachedOrPending(uint64_t lineAddr) const;    ///< True if already cached or in-flight.
    bool     startPrefetch(uint64_t lineAddr);                  ///< Enqueue lineAddr in a free prefetch slot.
};