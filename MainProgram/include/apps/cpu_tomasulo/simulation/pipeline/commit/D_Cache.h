// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/commit/D_Cache.h
// ============================================================================

#pragma once

/**
 * @file D_Cache.h
 * @brief Component 40: Data Cache.
 *
 * 8-set, 4-way set-associative write-back cache with LRU replacement.
 * Handles load and store requests routed from Memory_Arbiter.
 * On a miss, a dirty victim is written back before the new line is fetched.
 *
 * Latency model:
 *   - Hit:      1 cycle (response in same clockEdge).
 *   - Miss:     MISS_LATENCY cycles (simulated RAM round-trip).
 *   - Writeback + miss: 2 × MISS_LATENCY cycles.
 *
 * Cache geometry:
 *   - LINE_BYTES = 64  (offset: bits [5:0])
 *   - NUM_SETS   = 8   (index:  bits [8:6])
 *   - Tag:             bits [63:9]
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class TomasuloRAM;  // Forward declaration

class D_Cache : public ITomasuloComponent {
public:
    D_Cache() = default;
    explicit D_Cache(TomasuloRAM* ram) : m_ram(ram) {}

    void setRAM(TomasuloRAM* ram) { m_ram = ram; }

    const char* name() const override { return "D_Cache"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

    // ── UI read access ──────────────────────────────────────────
    bool        lineValid(int set, int way) const;
    bool        lineDirty(int set, int way) const;
    uint64_t    lineTag(int set, int way) const;
    const uint8_t* lineData(int set, int way) const;
    int numSets() const { return NUM_SETS; }
    int numWays() const { return NUM_WAYS; }
    int lineSizeBytes() const { return LINE_BYTES; }
    uint64_t missCount() const { return m_missTotal; }         ///< Total miss events since reset.
    uint64_t missStallCycles() const { return m_missStallCycles; } ///< Cycles stalled waiting for RAM.

private:
    TomasuloRAM* m_ram = nullptr;

    uint64_t m_missTotal = 0;         ///< Cumulative miss counter (for analysis view).
    uint64_t m_missStallCycles = 0;   ///< Cumulative stall cycles due to misses.

    static constexpr int NUM_SETS = 8;
    static constexpr int NUM_WAYS = 4;
    static constexpr int LINE_BYTES = 64;
    static constexpr int WORDS_PER_LINE = 8;
    static constexpr int MISS_LATENCY = 50;  ///< Simulated RAM round-trip in cycles.

    struct CacheLine {
        uint64_t tag = 0;
        uint8_t  data[LINE_BYTES] = {};
        bool     valid = false;
        bool     dirty = false;
    };

    CacheLine m_lines[NUM_SETS][NUM_WAYS];
    uint8_t   m_lru[NUM_SETS][NUM_WAYS] = {};  ///< LRU counters; higher = more recently used.

    // ── Miss handling state ─────────────────────────────────────
    bool     m_missPending = false;       ///< True while fetching a new line from RAM.
    bool     m_writebackPending = false;  ///< True while writing a dirty victim to RAM.
    int      m_missCounter = 0;           ///< Countdown for RAM latency (cycles remaining).
    uint64_t m_missAddr = 0;              ///< Line-aligned address of the missing line.
    uint64_t m_writebackAddr = 0;         ///< Line-aligned address of the dirty victim.
    int      m_victimWay = 0;             ///< Way selected for eviction.
    int      m_missSet = 0;               ///< Set index of the missed line.

    // ── Pending request state (saved while handling miss) ───────
    bool     m_pendingReq = false;
    bool     m_pendingRW = false;   ///< false = read, true = write.
    uint64_t m_pendingAddr = 0;
    uint64_t m_pendingWData = 0;
    uint8_t  m_pendingSize = 0;

    // ── Address decomposition helpers ───────────────────────────
    static uint64_t extractTag(uint64_t addr) { return addr >> 9; }
    static int      extractSet(uint64_t addr) { return (addr >> 6) & 0x7; }
    static int      extractOffset(uint64_t addr) { return addr & 0x3F; }
    static uint64_t alignToLine(uint64_t addr) { return addr & ~(uint64_t)63; }

    int  findWay(int set, uint64_t tag) const;  ///< Returns way index on hit, -1 on miss.
    int  lruWay(int set) const;                 ///< Returns the LRU (eviction candidate) way.
    void updateLRU(int set, int way);           ///< Promotes @p way to MRU position.
};