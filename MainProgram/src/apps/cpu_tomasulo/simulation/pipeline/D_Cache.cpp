#include "apps/cpu_tomasulo/simulation/pipeline/D_Cache.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"
#include <iostream>
#include <cstring>

int D_Cache::findWay(int set, uint64_t tag) const {
    for (int w = 0; w < NUM_WAYS; w++) {
        if (m_lines[set][w].valid && m_lines[set][w].tag == tag)
            return w;
    }
    return -1;
}

int D_Cache::lruWay(int set) const {
    int minLRU = 255;
    int victim = 0;
    for (int w = 0; w < NUM_WAYS; w++) {
        if (!m_lines[set][w].valid) return w;
        if (m_lru[set][w] < minLRU) {
            minLRU = m_lru[set][w];
            victim = w;
        }
    }
    return victim;
}

void D_Cache::updateLRU(int set, int way) {
    for (int w = 0; w < NUM_WAYS; w++) {
        if (m_lru[set][w] > m_lru[set][way])
            m_lru[set][w]--;
    }
    m_lru[set][way] = NUM_WAYS - 1;
}

// ============================================================================
// Evaluate
// ============================================================================

void D_Cache::evaluate(TomasuloBus& bus) {
    bus.DC_Ready_o = !m_missPending && !m_writebackPending;
}

// ============================================================================
// Clock Edge
// ============================================================================

void D_Cache::clockEdge(TomasuloBus& bus) {
    bus.DC_Done_o = false;

    // ── Count stall cycles while miss/writeback is pending ──────
    if (m_missPending || m_writebackPending) {
        ++m_missStallCycles;
    }

    // ── Handle writeback countdown ──────────────────────────────
    if (m_writebackPending) {
        if (m_missCounter > 0) {
            --m_missCounter;
            return;
        }
        // Writeback complete — write dirty line to RAM
        CacheLine& victim = m_lines[m_missSet][m_victimWay];
        for (int i = 0; i < WORDS_PER_LINE; i++) {
            uint64_t word;
            std::memcpy(&word, &victim.data[i * 8], 8);
            m_ram->writeAddress(m_writebackAddr + i * 8, word);
        }
        m_writebackPending = false;
        std::cout << "[D_Cache] Writeback done: addr=0x" << std::hex
            << m_writebackAddr << std::dec << "\n";

        // Now start fetching the missing line
        m_missPending = true;
        m_missCounter = MISS_LATENCY;
        std::cout << "[D_Cache] Fetching line: addr=0x" << std::hex
            << m_missAddr << std::dec << "\n";
        return;
    }

    // ── Handle miss fill countdown ──────────────────────────────
    if (m_missPending) {
        if (m_missCounter > 0) {
            --m_missCounter;
            return;
        }
        // Miss fill complete — read line from RAM into cache
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

        std::cout << "[D_Cache] Miss fill complete: set=" << set
            << " way=" << way << " addr=0x" << std::hex
            << m_missAddr << std::dec << "\n";

        // Replay the pending request (now guaranteed hit)
        if (m_pendingReq) {
            int offset = extractOffset(m_pendingAddr);

            if (!m_pendingRW) {
                uint64_t rdata = 0;
                if (m_pendingSize == 0x03) {
                    std::memcpy(&rdata, &line.data[offset], 8);
                }
                else {
                    rdata = line.data[offset];
                }
                bus.DC_RData_o = rdata;
                bus.DC_Done_o = true;
                std::cout << "[D_Cache] Replay read hit: 0x" << std::hex
                    << rdata << std::dec << "\n";
            }
            else {
                if (m_pendingSize == 0x03) {
                    std::memcpy(&line.data[offset], &m_pendingWData, 8);
                }
                else {
                    line.data[offset] = (uint8_t)(m_pendingWData & 0xFF);
                }
                line.dirty = true;
                bus.DC_Done_o = true;
                std::cout << "[D_Cache] Replay write hit.\n";
            }
            updateLRU(set, way);
            m_pendingReq = false;
        }
        return;
    }

    // ── Handle new request from Memory_Arbiter ──────────────────
    if (bus.DC_Req_o && !m_missPending && !m_writebackPending) {
        uint64_t addr = bus.DC_Addr_o;
        int set = extractSet(addr);
        uint64_t tag = extractTag(addr);
        int offset = extractOffset(addr);
        int way = findWay(set, tag);

        if (way >= 0) {
            // === HIT ===
            if (!bus.DC_RW_o) {
                uint64_t rdata = 0;
                if (bus.DC_Size_o == 0x03) {
                    std::memcpy(&rdata, &m_lines[set][way].data[offset], 8);
                }
                else {
                    rdata = m_lines[set][way].data[offset];
                }
                bus.DC_RData_o = rdata;
                bus.DC_Done_o = true;
                std::cout << "[D_Cache] Read hit: addr=0x" << std::hex << addr
                    << " data=0x" << rdata << std::dec << "\n";
            }
            else {
                if (bus.DC_Size_o == 0x03) {
                    std::memcpy(&m_lines[set][way].data[offset], &bus.DC_WData_o, 8);
                }
                else {
                    m_lines[set][way].data[offset] = (uint8_t)(bus.DC_WData_o & 0xFF);
                }
                m_lines[set][way].dirty = true;
                bus.DC_Done_o = true;
                std::cout << "[D_Cache] Write hit: addr=0x" << std::hex << addr
                    << std::dec << "\n";
            }
            updateLRU(set, way);
        }
        else {
            // === MISS ===
            std::cout << "[D_Cache] Miss: addr=0x" << std::hex << addr
                << std::dec << "\n";

            ++m_missTotal;
            m_missAddr = alignToLine(addr);
            m_missSet = set;
            m_victimWay = lruWay(set);

            m_pendingReq = true;
            m_pendingRW = bus.DC_RW_o;
            m_pendingAddr = addr;
            m_pendingWData = bus.DC_WData_o;
            m_pendingSize = bus.DC_Size_o;

            if (m_lines[set][m_victimWay].valid && m_lines[set][m_victimWay].dirty) {
                uint64_t wbAddr = (m_lines[set][m_victimWay].tag << 9)
                    | ((uint64_t)set << 6);
                m_writebackPending = true;
                m_writebackAddr = wbAddr;
                m_missCounter = MISS_LATENCY;
                std::cout << "[D_Cache] Writeback victim: addr=0x" << std::hex
                    << wbAddr << std::dec << "\n";
            }
            else {
                m_missPending = true;
                m_missCounter = MISS_LATENCY;
                std::cout << "[D_Cache] Fetching line: addr=0x" << std::hex
                    << m_missAddr << std::dec << "\n";
            }
        }
    }
}

// ============================================================================
// UI + Reset (unchanged)
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
void D_Cache::reset() {
    for (int s = 0; s < NUM_SETS; s++) {
        for (int w = 0; w < NUM_WAYS; w++) {
            m_lines[s][w].valid = false;
            m_lines[s][w].dirty = false;
            m_lines[s][w].tag = 0;
            m_lru[s][w] = w;
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