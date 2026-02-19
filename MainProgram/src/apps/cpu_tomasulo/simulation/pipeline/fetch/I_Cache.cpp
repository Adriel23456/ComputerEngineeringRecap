// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/fetch/I_Cache.cpp
// ============================================================================

/**
 * @file I_Cache.cpp
 * @brief Component 4 implementation — Instruction Cache.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/fetch/I_Cache.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"
#include <iostream>

 // ============================================================================
 // Construction
 // ============================================================================

I_Cache::I_Cache(const TomasuloRAM* ram) : m_ram(ram) {}

// ============================================================================
// Address Decomposition Helpers
// ============================================================================

/** @brief Tag = bits [63:9]  (identifies the memory block). */
uint64_t I_Cache::extractTag(uint64_t addr) const { return addr >> 9; }
/** @brief Set  = bits [8:6]  (selects one of NUM_SETS sets). */
uint32_t I_Cache::extractSet(uint64_t addr) const { return (addr >> 6) & 0x7; }
/** @brief Offset = bits [5:3] (word index within the 64-byte line). */
uint32_t I_Cache::extractOffset(uint64_t addr) const { return (addr >> 3) & 0x7; }
/** @brief Aligns addr down to the start of its 64-byte cache line. */
uint64_t I_Cache::alignToLine(uint64_t addr) const { return addr & ~uint64_t(63); }

// ============================================================================
// Set / Way Helpers
// ============================================================================

/**
 * @brief Searches a set for a valid way with a matching tag.
 * @return Way index [0, NUM_WAYS) on hit, or -1 on miss.
 */
int I_Cache::findWay(uint32_t set, uint64_t tag) const {
    for (int w = 0; w < NUM_WAYS; ++w)
        if (m_sets[set].ways[w].valid && m_sets[set].ways[w].tag == tag)
            return w;
    return -1;
}

/** @brief Returns the LRU (least-recently-used) victim way for the given set. */
int I_Cache::lruVictim(uint32_t set) const {
    return m_sets[set].lruOrder[NUM_WAYS - 1];  // LRU is always at the end of the order array.
}

/**
 * @brief Promotes @p way to the MRU position by shifting the other entries down.
 * Called on every cache hit and after every fill to maintain the LRU order.
 */
void I_Cache::touchWay(uint32_t set, int way) {
    auto& order = m_sets[set].lruOrder;
    int pos = -1;
    for (int i = 0; i < NUM_WAYS; ++i) {
        if (order[i] == static_cast<uint8_t>(way)) { pos = i; break; }
    }
    if (pos > 0) {
        uint8_t val = order[pos];
        for (int i = pos; i > 0; --i) order[i] = order[i - 1];
        order[0] = val;
    }
}

/**
 * @brief Loads a full cache line from RAM into (set, way).
 *
 * Reads WORDS_PER_LINE consecutive 64-bit words starting at @p lineAddr,
 * then calls touchWay() to mark it as MRU.
 */
void I_Cache::fillLine(uint32_t set, int way, uint64_t lineAddr) {
    auto& line = m_sets[set].ways[way];
    line.tag = extractTag(lineAddr);
    line.valid = true;
    for (int w = 0; w < WORDS_PER_LINE; ++w) {
        uint64_t wordAddr = lineAddr + static_cast<uint64_t>(w) * 8;
        size_t   row = static_cast<size_t>(wordAddr / TomasuloRAM::kStep);
        line.data[w] = (row < TomasuloRAM::kTotalRows) ? m_ram->read(row) : 0;
    }
    touchWay(set, way);
}

/**
 * @brief Returns true if @p lineAddr is already cached, or an outstanding
 *        demand miss / prefetch is already in-flight for it.
 *
 * Used to avoid duplicate prefetch requests for the same line.
 */
bool I_Cache::isLineCachedOrPending(uint64_t lineAddr) const {
    uint32_t set = extractSet(lineAddr);
    uint64_t tag = extractTag(lineAddr);
    if (findWay(set, tag) >= 0) return true;
    if (m_missPending && alignToLine(m_missAddr) == lineAddr) return true;
    for (int i = 0; i < NUM_PREFETCH; ++i)
        if (m_prefetch[i].pending && m_prefetch[i].addr == lineAddr) return true;
    return false;
}

/**
 * @brief Enqueues @p lineAddr for background prefetch in the first free slot.
 * @return true if a free slot was found; false if all slots are occupied.
 */
bool I_Cache::startPrefetch(uint64_t lineAddr) {
    if (isLineCachedOrPending(lineAddr)) return false;
    for (int i = 0; i < NUM_PREFETCH; ++i) {
        if (!m_prefetch[i].pending) {
            m_prefetch[i] = { true, lineAddr, MISS_LATENCY };
            return true;
        }
    }
    return false;  // All slots busy.
}

// ============================================================================
// Evaluate (combinational)
// ============================================================================

void I_Cache::evaluate(TomasuloBus& bus) {
    uint64_t addr = bus.PCCurrent_o;
    uint32_t set = extractSet(addr);
    uint64_t tag = extractTag(addr);
    uint32_t offset = extractOffset(addr);

    int hitWay = findWay(set, tag);

    if (hitWay >= 0 && !m_missPending) {
        // ── Hit: drive the instruction word and signal ready ──────
        bus.InstrF_o = m_sets[set].ways[hitWay].data[offset];
        bus.InstReady_o = true;
        bus.ICtoRAM_Req_o = false;
        touchWay(set, hitWay);
    }
    else {
        // ── Miss (or demand miss pending): stall the fetch stage ──
        bus.InstrF_o = 0;
        bus.InstReady_o = false;
    }
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void I_Cache::clockEdge(TomasuloBus& bus) {

    // ── Flush: cancel all pending prefetches (demand miss kept) ──
    if (bus.Flush_o) {
        for (int i = 0; i < NUM_PREFETCH; ++i)
            m_prefetch[i].pending = false;
    }

    // ── Tick prefetch slots (run in background regardless of stall) ──
    for (int i = 0; i < NUM_PREFETCH; ++i) {
        if (!m_prefetch[i].pending) continue;

        --m_prefetch[i].counter;
        if (m_prefetch[i].counter <= 0) {
            uint64_t lineAddr = m_prefetch[i].addr;
            uint32_t set = extractSet(lineAddr);
            // Only fill if the line isn't already cached (race with demand miss).
            if (findWay(set, extractTag(lineAddr)) < 0) {
                fillLine(set, lruVictim(set), lineAddr);
            }
            m_prefetch[i].pending = false;

            // Chain: when this prefetch completes, start the next line.
            startPrefetch(lineAddr + LINE_SIZE);
        }
    }

    // ── Service demand miss ───────────────────────────────────────
    if (m_missPending) {
        ++m_missStallCycles;
        --m_missCounter;
        if (m_missCounter <= 0) {
            // Fill the missing line and immediately queue the next two lines.
            uint32_t set = extractSet(m_missAddr);
            fillLine(set, lruVictim(set), m_missAddr);
            m_missPending = false;

            startPrefetch(m_missAddr + LINE_SIZE);
            startPrefetch(m_missAddr + LINE_SIZE * 2);
        }
        return;  // Still stalling — do not process new PC.
    }

    // ── Check current PC for hit/miss ─────────────────────────────
    uint64_t addr = bus.PCCurrent_o;
    uint32_t set = extractSet(addr);
    uint64_t tag = extractTag(addr);
    int      hitWay = findWay(set, tag);

    if (hitWay >= 0) {
        // Hit — ensure the next two lines are already being prefetched.
        uint64_t curLine = alignToLine(addr);
        startPrefetch(curLine + LINE_SIZE);
        startPrefetch(curLine + LINE_SIZE * 2);
    }
    else {
        // Demand miss — check whether a prefetch slot can be promoted.
        uint64_t lineAddr = alignToLine(addr);
        bool     promotedFromPrefetch = false;

        for (int i = 0; i < NUM_PREFETCH; ++i) {
            if (m_prefetch[i].pending && m_prefetch[i].addr == lineAddr) {
                // Promote: inherit remaining counter so no extra latency is added.
                m_missPending = true;
                m_missAddr = lineAddr;
                m_missCounter = m_prefetch[i].counter;
                m_prefetch[i].pending = false;
                ++m_missTotal;
                promotedFromPrefetch = true;
                break;
            }
        }

        if (!promotedFromPrefetch) {
            // Cold miss: start a fresh demand miss and prefetch the next line.
            m_missPending = true;
            m_missAddr = lineAddr;
            m_missCounter = MISS_LATENCY;
            ++m_missTotal;
            startPrefetch(lineAddr + LINE_SIZE);
        }
    }
}

// ============================================================================
// UI Accessors
// ============================================================================

bool I_Cache::lineValid(int set, int way) const {
    if (set < 0 || set >= NUM_SETS || way < 0 || way >= NUM_WAYS) return false;
    return m_sets[set].ways[way].valid;
}

uint64_t I_Cache::lineTag(int set, int way) const {
    if (set < 0 || set >= NUM_SETS || way < 0 || way >= NUM_WAYS) return 0;
    return m_sets[set].ways[way].tag;
}

const uint64_t* I_Cache::lineData(int set, int way) const {
    if (set < 0 || set >= NUM_SETS || way < 0 || way >= NUM_WAYS) return nullptr;
    return m_sets[set].ways[way].data;
}

// ============================================================================
// Reset
// ============================================================================

void I_Cache::reset() {
    for (auto& s : m_sets) {
        for (auto& w : s.ways) {
            w.valid = false;
            w.tag = 0;
            for (auto& d : w.data) d = 0;
        }
        s.lruOrder[0] = 0; s.lruOrder[1] = 1;
        s.lruOrder[2] = 2; s.lruOrder[3] = 3;
    }
    m_missPending = false;
    m_missCounter = 0;
    for (int i = 0; i < NUM_PREFETCH; ++i) m_prefetch[i] = {};
    m_missTotal = 0;
    m_missStallCycles = 0;
    std::cout << "[I_Cache] reset()\n";
}