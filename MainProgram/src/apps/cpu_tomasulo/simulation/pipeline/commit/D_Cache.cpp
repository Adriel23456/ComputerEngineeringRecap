// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/commit/D_Cache.cpp
// ============================================================================

/**
 * @file D_Cache.cpp
 * @brief Component 40 implementation -- Data Cache.
 *
 * 8-set, 4-way set-associative write-back cache with LRU replacement.
 * Cache geometry:
 *   Sets:          8   (3-bit index)
 *   Ways:          4   (LRU tracked per set)
 *   Line size:    64 bytes  (WORDS_PER_LINE × 8)
 *   Address decomposition: [tag | 3-bit set | 6-bit offset]
 *
 * Miss handling (writeback-before-fill protocol):
 *   1. If the victim line is dirty, a writeback countdown begins first.
 *   2. After writeback completes (or immediately if victim is clean),
 *      a fill countdown of MISS_LATENCY cycles begins.
 *   3. Once the fill completes the original request is replayed on the
 *      now-resident line and DC_Done_o is asserted.
 *
 * All access from Memory_Arbiter arrives via DC_Req_o / DC_RW_o.
 * DC_Ready_o is de-asserted while any miss or writeback is in progress,
 * preventing Memory_Arbiter from issuing a new request.
 *
 * Performance counters:
 *   m_missTotal      — total cache misses since reset.
 *   m_missStallCycles — total cycles spent waiting on misses/writebacks.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/commit/D_Cache.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"
#include <iostream>
#include <cstring>

 // ----------------------------------------------------------------------------
 // findWay
 // Searches all ways in @p set for a valid line whose tag matches @p tag.
 // Returns the way index on hit, or -1 on miss.
 // ----------------------------------------------------------------------------
int D_Cache::findWay(int set, uint64_t tag) const {
    for (int w = 0; w < NUM_WAYS; w++) {
        if (m_lines[set][w].valid && m_lines[set][w].tag == tag)
            return w;
    }
    return -1;
}

// ----------------------------------------------------------------------------
// lruWay
// Returns the way to evict from @p set using LRU policy.
// Prefers invalid ways (cold misses) before evicting a valid line.
// ----------------------------------------------------------------------------
int D_Cache::lruWay(int set) const {
    int minLRU = 255;
    int victim = 0;
    for (int w = 0; w < NUM_WAYS; w++) {
        if (!m_lines[set][w].valid) return w;  // Prefer invalid slot
        if (m_lru[set][w] < minLRU) {
            minLRU = m_lru[set][w];
            victim = w;
        }
    }
    return victim;
}

// ----------------------------------------------------------------------------
// updateLRU
// Marks @p way as most-recently used within @p set.
// All ways with a higher LRU counter than the accessed way are decremented
// to maintain a compact, gap-free LRU ordering.
// ----------------------------------------------------------------------------
void D_Cache::updateLRU(int set, int way) {
    for (int w = 0; w < NUM_WAYS; w++) {
        if (m_lru[set][w] > m_lru[set][way])
            m_lru[set][w]--;
    }
    m_lru[set][way] = NUM_WAYS - 1;  // Most recently used
}

// ============================================================================
// Evaluate (combinational outputs)
// ============================================================================

void D_Cache::evaluate(TomasuloBus& bus) {
    // DC_Ready_o is false while any miss fill or writeback countdown is active.
    // Memory_Arbiter polls this signal before issuing a new request.
    bus.DC_Ready_o = !m_missPending && !m_writebackPending;
}

// ============================================================================
// Clock Edge (sequential -- miss/writeback state machine + request handling)
// ============================================================================

void D_Cache::clockEdge(TomasuloBus& bus) {
    bus.DC_Done_o = false;

    // ── Track stall cycles for performance analysis ──────────────────
    if (m_missPending || m_writebackPending) {
        ++m_missStallCycles;
    }

    // ── Phase A: Writeback countdown ─────────────────────────────────
    // Wait for the dirty victim line to be flushed to RAM before fetching.
    if (m_writebackPending) {
        if (m_missCounter > 0) {
            --m_missCounter;
            return;
        }
        // Writeback complete — write the dirty line to RAM word-by-word
        CacheLine& victim = m_lines[m_missSet][m_victimWay];
        for (int i = 0; i < WORDS_PER_LINE; i++) {
            uint64_t word;
            std::memcpy(&word, &victim.data[i * 8], 8);
            m_ram->writeAddress(m_writebackAddr + i * 8, word);
        }
        m_writebackPending = false;

        // Transition: writeback done — now start the cache line fill
        m_missPending = true;
        m_missCounter = MISS_LATENCY;
        return;
    }

    // ── Phase B: Miss fill countdown ─────────────────────────────────
    // Simulate MISS_LATENCY cycles of RAM access latency.
    if (m_missPending) {
        if (m_missCounter > 0) {
            --m_missCounter;
            return;
        }
        // Fill complete — load the cache line from RAM
        int set = m_missSet;
        int way = m_victimWay;
        CacheLine& line = m_lines[set][way];

        for (int i = 0; i < WORDS_PER_LINE; i++) {
            uint64_t word = m_ram->readAddress(m_missAddr + i * 8);
            std::memcpy(&line.data[i * 8], &word, 8);
        }
        line.tag = extractTag(m_missAddr);
        line.valid = true;
        line.dirty = false;
        updateLRU(set, way);
        m_missPending = false;

        // ── Replay the pending request on the newly filled line ──────
        // The original access is guaranteed to hit now.
        if (m_pendingReq) {
            int offset = extractOffset(m_pendingAddr);

            if (!m_pendingRW) {
                // Replay read
                uint64_t rdata = 0;
                if (m_pendingSize == 0x03) {
                    std::memcpy(&rdata, &line.data[offset], 8);  // 8-byte word
                }
                else {
                    rdata = line.data[offset];                    // 1-byte
                }
                bus.DC_RData_o = rdata;
                bus.DC_Done_o = true;
            }
            else {
                // Replay write
                if (m_pendingSize == 0x03) {
                    std::memcpy(&line.data[offset], &m_pendingWData, 8);
                }
                else {
                    line.data[offset] = (uint8_t)(m_pendingWData & 0xFF);
                }
                line.dirty = true;
                bus.DC_Done_o = true;
            }
            updateLRU(set, way);
            m_pendingReq = false;
        }
        return;
    }

    // ── Phase C: Handle new request from Memory_Arbiter ──────────────
    if (bus.DC_Req_o && !m_missPending && !m_writebackPending) {
        uint64_t addr = bus.DC_Addr_o;
        int      set = extractSet(addr);
        uint64_t tag = extractTag(addr);
        int      offset = extractOffset(addr);
        int      way = findWay(set, tag);

        if (way >= 0) {
            // ── HIT ──────────────────────────────────────────────────
            if (!bus.DC_RW_o) {
                // Read hit: copy data from the cache line
                uint64_t rdata = 0;
                if (bus.DC_Size_o == 0x03) {
                    std::memcpy(&rdata, &m_lines[set][way].data[offset], 8);
                }
                else {
                    rdata = m_lines[set][way].data[offset];
                }
                bus.DC_RData_o = rdata;
                bus.DC_Done_o = true;
            }
            else {
                // Write hit: update the cache line and mark dirty
                if (bus.DC_Size_o == 0x03) {
                    std::memcpy(&m_lines[set][way].data[offset], &bus.DC_WData_o, 8);
                }
                else {
                    m_lines[set][way].data[offset] = (uint8_t)(bus.DC_WData_o & 0xFF);
                }
                m_lines[set][way].dirty = true;
                bus.DC_Done_o = true;
            }
            updateLRU(set, way);
        }
        else {
            // ── MISS ─────────────────────────────────────────────────
            ++m_missTotal;

            // Record the miss address and victim for the fill phase
            m_missAddr = alignToLine(addr);
            m_missSet = set;
            m_victimWay = lruWay(set);

            // Save the original request for replay after fill
            m_pendingReq = true;
            m_pendingRW = bus.DC_RW_o;
            m_pendingAddr = addr;
            m_pendingWData = bus.DC_WData_o;
            m_pendingSize = bus.DC_Size_o;

            if (m_lines[set][m_victimWay].valid && m_lines[set][m_victimWay].dirty) {
                // Victim is dirty — writeback before fill
                uint64_t wbAddr = (m_lines[set][m_victimWay].tag << 9)
                    | ((uint64_t)set << 6);
                m_writebackPending = true;
                m_writebackAddr = wbAddr;
                m_missCounter = MISS_LATENCY;
            }
            else {
                // Victim is clean (or invalid) — fill immediately
                m_missPending = true;
                m_missCounter = MISS_LATENCY;
            }
        }
    }
}

// ============================================================================
// UI accessor helpers (read-only inspection for the debug/visualization layer)
// ============================================================================

bool D_Cache::lineValid(int set, int way) const {
    if (set < 0 || set >= NUM_SETS || way < 0 || way >= NUM_WAYS) return false;
    return m_lines[set][way].valid;
}

bool D_Cache::lineDirty(int set, int way) const {
    if (set < 0 || set >= NUM_SETS || way < 0 || way >= NUM_WAYS) return false;
    return m_lines[set][way].dirty;
}

uint64_t D_Cache::lineTag(int set, int way) const {
    if (set < 0 || set >= NUM_SETS || way < 0 || way >= NUM_WAYS) return 0;
    return m_lines[set][way].tag;
}

const uint8_t* D_Cache::lineData(int set, int way) const {
    if (set < 0 || set >= NUM_SETS || way < 0 || way >= NUM_WAYS) return nullptr;
    return m_lines[set][way].data;
}

// ============================================================================
// Reset
// ============================================================================

void D_Cache::reset() {
    for (int s = 0; s < NUM_SETS; s++) {
        for (int w = 0; w < NUM_WAYS; w++) {
            m_lines[s][w].valid = false;
            m_lines[s][w].dirty = false;
            m_lines[s][w].tag = 0;
            m_lru[s][w] = w;   // Initial LRU order: way 0 = LRU, way N-1 = MRU
            std::memset(m_lines[s][w].data, 0, LINE_BYTES);
        }
    }
    m_missPending = false;
    m_writebackPending = false;
    m_pendingReq = false;
    m_missCounter = 0;
    m_missTotal = 0;
    m_missStallCycles = 0;
    std::cout << "[D_Cache] reset()\n";
}