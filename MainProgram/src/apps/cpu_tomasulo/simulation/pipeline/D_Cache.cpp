/**
 * @file D_Cache.cpp
 * @brief Component 40 implementation -- Data Cache.
 *
 * Write-back, write-allocate, 4-way set-associative.
 * Hit: DC_Done_o=1 in same clockEdge. Miss: fetch from RAM.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/D_Cache.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
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
        if (!m_lines[set][w].valid) return w;  // Use invalid line first
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

void D_Cache::readLineFromBus(int set, int way, const uint64_t ramData[8]) {
    for (int i = 0; i < WORDS_PER_LINE; i++) {
        std::memcpy(&m_lines[set][way].data[i * 8], &ramData[i], 8);
    }
}

void D_Cache::writeLineToBus(int set, int way, uint64_t ramData[8]) const {
    for (int i = 0; i < WORDS_PER_LINE; i++) {
        std::memcpy(&ramData[i], &m_lines[set][way].data[i * 8], 8);
    }
}

// ============================================================================
// Evaluate (combinational -- report readiness)
// ============================================================================

void D_Cache::evaluate(TomasuloBus& bus) {
    bus.DC_Ready_o = !m_missPending && !m_writebackPending;
}

// ============================================================================
// Clock Edge (sequential -- handle requests, misses, RAM responses)
// ============================================================================

void D_Cache::clockEdge(TomasuloBus& bus) {
    // ── Count stall cycles while miss/writeback is pending ──────
    if (m_missPending || m_writebackPending) {
        ++m_missStallCycles;
    }

    bus.DC_Done_o = false;
    bus.DCtoRAM_RdReq_i = false;
    bus.DCtoRAM_WrReq_i = false;

    // -- Handle writeback completion, then initiate line fetch --
    if (m_writebackPending && bus.RAMtoDC_Ready_o) {
        m_writebackPending = false;
        // Now fetch the missing line
        bus.DCtoRAM_RdReq_i = true;
        bus.DCtoRAM_Addr_i = m_missAddr;
        m_missPending = true;
        std::cout << "[D_Cache] Writeback done, fetching line 0x"
            << std::hex << m_missAddr << std::dec << "\n";
        return;
    }

    // -- Handle miss fill completion --
    if (m_missPending && bus.RAMtoDC_Ready_o) {
        int set = m_missSet;
        int way = m_victimWay;
        m_lines[set][way].tag = extractTag(m_missAddr);
        readLineFromBus(set, way, bus.RAMtoDC_Data_o);
        m_lines[set][way].valid = true;
        m_lines[set][way].dirty = false;
        updateLRU(set, way);
        m_missPending = false;

        std::cout << "[D_Cache] Miss fill complete: set=" << set
            << " way=" << way << " addr=0x" << std::hex
            << m_missAddr << std::dec << "\n";

        // Replay the pending request (now guaranteed hit)
        if (m_pendingReq) {
            uint64_t addr = m_pendingAddr;
            int offset = extractOffset(addr);

            if (!m_pendingRW) {
                // Read
                uint64_t rdata = 0;
                if (m_pendingSize == 0x03) {
                    std::memcpy(&rdata, &m_lines[set][way].data[offset], 8);
                }
                else {
                    rdata = m_lines[set][way].data[offset];
                }
                bus.DC_RData_o = rdata;
                bus.DC_Done_o = true;
                std::cout << "[D_Cache] Replay read hit: 0x" << std::hex
                    << rdata << std::dec << "\n";
            }
            else {
                // Write
                if (m_pendingSize == 0x03) {
                    std::memcpy(&m_lines[set][way].data[offset], &m_pendingWData, 8);
                }
                else {
                    m_lines[set][way].data[offset] = (uint8_t)(m_pendingWData & 0xFF);
                }
                m_lines[set][way].dirty = true;
                bus.DC_Done_o = true;
                std::cout << "[D_Cache] Replay write hit.\n";
            }
            updateLRU(set, way);
            m_pendingReq = false;
        }
        return;
    }

    // -- Handle new request from Memory_Arbiter --
    if (bus.DC_Req_o && !m_missPending && !m_writebackPending) {
        uint64_t addr = bus.DC_Addr_o;
        int set = extractSet(addr);
        uint64_t tag = extractTag(addr);
        int offset = extractOffset(addr);

        int way = findWay(set, tag);

        if (way >= 0) {
            // === HIT ===
            if (!bus.DC_RW_o) {
                // Read
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
                // Write
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

            // Save pending request for replay
            m_pendingReq = true;
            m_pendingRW = bus.DC_RW_o;
            m_pendingAddr = addr;
            m_pendingWData = bus.DC_WData_o;
            m_pendingSize = bus.DC_Size_o;

            // Check if victim needs writeback
            if (m_lines[set][m_victimWay].valid && m_lines[set][m_victimWay].dirty) {
                // Writeback dirty victim first
                uint64_t wbAddr = (m_lines[set][m_victimWay].tag << 9)
                    | ((uint64_t)set << 6);
                bus.DCtoRAM_WrReq_i = true;
                bus.DCtoRAM_Addr_i = wbAddr;
                writeLineToBus(set, m_victimWay, bus.DCtoRAM_WrData_i);
                m_writebackPending = true;
                m_writebackAddr = wbAddr;
                std::cout << "[D_Cache] Writeback victim: addr=0x" << std::hex
                    << wbAddr << std::dec << "\n";
            }
            else {
                // No writeback needed, fetch directly
                bus.DCtoRAM_RdReq_i = true;
                bus.DCtoRAM_Addr_i = m_missAddr;
                m_missPending = true;
                std::cout << "[D_Cache] Fetching line: addr=0x" << std::hex
                    << m_missAddr << std::dec << "\n";
            }
        }
    }
}

// UI Implementations:
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
    m_missTotal = 0;
    m_missStallCycles = 0;
    std::cout << "[D_Cache] reset()\n";
}