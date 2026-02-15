/**
 * @file I_Cache.cpp
 * @brief Component 4 implementation — Instruction Cache.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/I_Cache.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"
#include <iostream>

I_Cache::I_Cache(const TomasuloRAM* ram) : m_ram(ram) {}

// ============================================================================
// Address decomposition (64-byte lines, 8 sets) [2KB]
//   addr[5:3]  = word offset within line (3 bits)
//   addr[8:6] = set index (3 bits)
//   addr[63:9]= tag (55 bits)
// ============================================================================

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
    return m_sets[set].lruOrder[NUM_WAYS - 1]; // least recently used
}

void I_Cache::touchWay(uint32_t set, int way) {
    auto& order = m_sets[set].lruOrder;
    // Move 'way' to front (MRU position)
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
    // Read 8 words from RAM
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
        // ── HIT ────────────────────────────────────────────────
        bus.InstrF_o = m_sets[set].ways[hitWay].data[offset];
        bus.InstReady_o = true;
        bus.ICtoRAM_Req_o = false;
        touchWay(set, hitWay);
    }
    else {
        // ── MISS (or miss in progress) ─────────────────────────
        bus.InstrF_o = 0;
        bus.InstReady_o = false;
    }
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void I_Cache::clockEdge(TomasuloBus& bus) {
    // ── Flush handling ──────────────────────────────────────────
    if (bus.Flush_o) {
        m_prefetchPending = false;
        m_prefetchCounter = 0;
        // Do NOT cancel demand miss
        std::cout << "[I_Cache] Flush: prefetch cancelled.\n";
    }

    // ── Handle ongoing miss ─────────────────────────────────────
    if (m_missPending) {
        ++m_missStallCycles;
        --m_missCounter;
        if (m_missCounter <= 0) {
            uint32_t set = extractSet(m_missAddr);
            int victim = lruVictim(set);
            fillLine(set, victim, m_missAddr);
            m_missPending = false;
            std::cout << "[I_Cache] Miss resolved: line 0x" << std::hex
                << m_missAddr << std::dec << " filled into set "
                << set << " way " << victim << "\n";
        }
        return; // Still processing miss, don't start new ones
    }

    // ── Handle ongoing prefetch ─────────────────────────────────
    if (m_prefetchPending) {
        --m_prefetchCounter;
        if (m_prefetchCounter <= 0) {
            uint64_t lineAddr = alignToLine(m_prefetchAddr);
            uint32_t set = extractSet(lineAddr);
            if (findWay(set, extractTag(lineAddr)) < 0) {
                int victim = lruVictim(set);
                fillLine(set, victim, lineAddr);
                std::cout << "[I_Cache] Prefetch filled: line 0x" << std::hex
                    << lineAddr << std::dec << "\n";
            }
            m_prefetchPending = false;
        }
    }

    // ── Check current access ────────────────────────────────────
    uint64_t addr = bus.PCCurrent_o;
    uint32_t set = extractSet(addr);
    uint64_t tag = extractTag(addr);
    int hitWay = findWay(set, tag);

    if (hitWay >= 0) {
        // Hit — initiate next-line prefetch if not already cached/pending
        uint64_t nextLine = alignToLine(addr) + LINE_SIZE;
        uint32_t nextSet = extractSet(nextLine);
        if (findWay(nextSet, extractTag(nextLine)) < 0 && !m_prefetchPending) {
            m_prefetchPending = true;
            m_prefetchAddr = nextLine;
            m_prefetchCounter = MISS_LATENCY;
            std::cout << "[I_Cache] Prefetch initiated: line 0x" << std::hex
                << nextLine << std::dec << "\n";
        }
    }
    else if (!m_missPending) {
        // Miss — start fetch
        m_missPending = true;
        m_missAddr = alignToLine(addr);
        m_missCounter = MISS_LATENCY;
        ++m_missTotal;
        std::cout << "[I_Cache] MISS at 0x" << std::hex << addr
            << ", fetching line 0x" << m_missAddr << std::dec
            << " (" << MISS_LATENCY << " cycles)\n";
    }
}

// UI implementations:
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
    m_prefetchPending = false;
    m_missCounter = 0;
    m_prefetchCounter = 0;
    m_missTotal = 0;
    m_missStallCycles = 0;
    std::cout << "[I_Cache] reset()\n";
}