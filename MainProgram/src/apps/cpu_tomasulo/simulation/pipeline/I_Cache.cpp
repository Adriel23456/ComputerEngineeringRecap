#include "apps/cpu_tomasulo/simulation/pipeline/I_Cache.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"
#include <iostream>

I_Cache::I_Cache(const TomasuloRAM* ram) : m_ram(ram) {}

uint64_t I_Cache::extractTag(uint64_t addr) const { return addr >> 9; }
uint32_t I_Cache::extractSet(uint64_t addr) const { return (addr >> 6) & 0x7; }
uint32_t I_Cache::extractOffset(uint64_t addr) const { return (addr >> 3) & 0x7; }
uint64_t I_Cache::alignToLine(uint64_t addr) const { return addr & ~uint64_t(63); }

int I_Cache::findWay(uint32_t set, uint64_t tag) const {
    for (int w = 0; w < NUM_WAYS; ++w)
        if (m_sets[set].ways[w].valid && m_sets[set].ways[w].tag == tag)
            return w;
    return -1;
}

int I_Cache::lruVictim(uint32_t set) const {
    return m_sets[set].lruOrder[NUM_WAYS - 1];
}

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

void I_Cache::fillLine(uint32_t set, int way, uint64_t lineAddr) {
    auto& line = m_sets[set].ways[way];
    line.tag = extractTag(lineAddr);
    line.valid = true;
    for (int w = 0; w < WORDS_PER_LINE; ++w) {
        uint64_t wordAddr = lineAddr + static_cast<uint64_t>(w) * 8;
        size_t row = static_cast<size_t>(wordAddr / TomasuloRAM::kStep);
        if (row < TomasuloRAM::kTotalRows)
            line.data[w] = m_ram->read(row);
        else
            line.data[w] = 0;
    }
    touchWay(set, way);
}

bool I_Cache::isLineCachedOrPending(uint64_t lineAddr) const {
    uint32_t set = extractSet(lineAddr);
    uint64_t tag = extractTag(lineAddr);
    if (findWay(set, tag) >= 0) return true;
    if (m_missPending && alignToLine(m_missAddr) == lineAddr) return true;
    for (int i = 0; i < NUM_PREFETCH; ++i)
        if (m_prefetch[i].pending && m_prefetch[i].addr == lineAddr) return true;
    return false;
}

bool I_Cache::startPrefetch(uint64_t lineAddr) {
    if (isLineCachedOrPending(lineAddr)) return false;
    for (int i = 0; i < NUM_PREFETCH; ++i) {
        if (!m_prefetch[i].pending) {
            m_prefetch[i].pending = true;
            m_prefetch[i].addr = lineAddr;
            m_prefetch[i].counter = MISS_LATENCY;
            return true;
        }
    }
    return false; // All slots busy
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
        bus.InstrF_o = m_sets[set].ways[hitWay].data[offset];
        bus.InstReady_o = true;
        bus.ICtoRAM_Req_o = false;
        touchWay(set, hitWay);
    }
    else {
        bus.InstrF_o = 0;
        bus.InstReady_o = false;
    }
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void I_Cache::clockEdge(TomasuloBus& bus) {
    // ── Flush: cancel prefetches, keep demand miss ──────────────
    if (bus.Flush_o) {
        for (int i = 0; i < NUM_PREFETCH; ++i)
            m_prefetch[i].pending = false;
    }

    // ── Tick all prefetch slots (run in background always) ──────
    for (int i = 0; i < NUM_PREFETCH; ++i) {
        if (!m_prefetch[i].pending) continue;
        --m_prefetch[i].counter;
        if (m_prefetch[i].counter <= 0) {
            uint64_t lineAddr = m_prefetch[i].addr;
            uint32_t set = extractSet(lineAddr);
            if (findWay(set, extractTag(lineAddr)) < 0) {
                int victim = lruVictim(set);
                fillLine(set, victim, lineAddr);
            }
            m_prefetch[i].pending = false;

            // Chain: when a prefetch completes, start next line prefetch
            uint64_t nextLine = lineAddr + LINE_SIZE;
            startPrefetch(nextLine);
        }
    }

    // ── Handle demand miss ──────────────────────────────────────
    if (m_missPending) {
        ++m_missStallCycles;
        --m_missCounter;
        if (m_missCounter <= 0) {
            uint32_t set = extractSet(m_missAddr);
            int victim = lruVictim(set);
            fillLine(set, victim, m_missAddr);
            m_missPending = false;

            // Immediately queue prefetches for next 2 lines
            uint64_t next1 = m_missAddr + LINE_SIZE;
            uint64_t next2 = m_missAddr + LINE_SIZE * 2;
            startPrefetch(next1);
            startPrefetch(next2);
        }
        return; // Still stalling on demand miss
    }

    // ── Check current PC ────────────────────────────────────────
    uint64_t addr = bus.PCCurrent_o;
    uint32_t set = extractSet(addr);
    uint64_t tag = extractTag(addr);
    int hitWay = findWay(set, tag);

    if (hitWay >= 0) {
        // Hit — ensure next 2 lines are prefetching/cached
        uint64_t curLine = alignToLine(addr);
        uint64_t next1 = curLine + LINE_SIZE;
        uint64_t next2 = curLine + LINE_SIZE * 2;
        startPrefetch(next1);
        startPrefetch(next2);
    }
    else {
        // Demand miss — check if a prefetch slot has this line
        uint64_t lineAddr = alignToLine(addr);
        bool promotedFromPrefetch = false;
        for (int i = 0; i < NUM_PREFETCH; ++i) {
            if (m_prefetch[i].pending && m_prefetch[i].addr == lineAddr) {
                // Promote prefetch to demand miss (keep remaining cycles)
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
            m_missPending = true;
            m_missAddr = lineAddr;
            m_missCounter = MISS_LATENCY;
            ++m_missTotal;

            // Immediately start prefetching next line too
            startPrefetch(lineAddr + LINE_SIZE);
        }
    }
}

// ============================================================================
// UI access
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

void I_Cache::reset() {
    for (auto& set : m_sets) {
        for (auto& way : set.ways) {
            way.valid = false;
            way.tag = 0;
            for (auto& d : way.data) d = 0;
        }
        set.lruOrder[0] = 0; set.lruOrder[1] = 1;
        set.lruOrder[2] = 2; set.lruOrder[3] = 3;
    }
    m_missPending = false;
    m_missCounter = 0;
    for (int i = 0; i < NUM_PREFETCH; ++i)
        m_prefetch[i] = {};
    m_missTotal = 0;
    m_missStallCycles = 0;
    std::cout << "[I_Cache] reset()\n";
}