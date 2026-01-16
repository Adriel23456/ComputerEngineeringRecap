// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/bus/InterconnectBus.cpp
// ============================================================================

/**
 * @file InterconnectBus.cpp
 * @brief Implementation of Interconnect bus.
 */

#include "apps/cpu_tlp_shared_cache/simulation/bus/InterconnectBus.h"
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Snoop.h"
#include "apps/cpu_tlp_shared_cache/simulation/processor/SharedData.h"
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h"

#include <iostream>
#include <mutex>

namespace {

    constexpr bool ENABLE_BUS_DEBUG = false;
    std::mutex g_debugMutex;

#define BUSLOG(MSG) do { \
        if (ENABLE_BUS_DEBUG) { \
            std::lock_guard<std::mutex> lk(g_debugMutex); \
            std::cout << "[Bus] " << MSG << std::endl; \
        } \
    } while(0)

    inline uint64_t alignToLine(uint64_t addr) {
        constexpr uint64_t MASK = ~((1ULL << OFFSET_BITS) - 1ULL);
        return addr & MASK;
    }

} // anonymous namespace

// ============================================================================
// Construction
// ============================================================================

Interconnect::Interconnect(int numL1)
    : m_m2b(numL1)
    , m_b2m(numL1)
    , m_rrPointer(0)
    , m_reqEdgeBlock(numL1, false)
{
    m_snoopCallbacks.resize(numL1);
}

// ============================================================================
// Callback Registration
// ============================================================================

void Interconnect::attachSnoopCallback(int id, std::function<SnoopResp(const SnoopReq&)> cb) {
    std::lock_guard<std::mutex> lk(m_cbMutex);
    m_snoopCallbacks[id] = std::move(cb);
}

// ============================================================================
// Utility Methods
// ============================================================================

uint16_t Interconnect::idx64(uint64_t addrLine) {
    return static_cast<uint16_t>(addrLine & 0x0FFFu);
}

void Interconnect::clearOutputs() {
    const bool canAcceptWB = !(m_tx.busy && m_tx.mem != ActiveTx::MemPhase::None);
    const int owner = (m_tx.busy ? m_tx.owner : -1);

    for (size_t i = 0; i < m_b2m.size(); ++i) {
        if (static_cast<int>(i) == owner) {
            m_b2m[i].B_GRANT.store(true, std::memory_order_relaxed);
        }
        else {
            m_b2m[i].B_GRANT.store(false, std::memory_order_relaxed);
        }

        m_b2m[i].B_WREADY.store(canAcceptWB, std::memory_order_relaxed);

        if (static_cast<int>(i) != owner) {
            m_b2m[i].B_SHARED_SEEN.store(false, std::memory_order_relaxed);
            m_b2m[i].B_HITM_SEEN.store(false, std::memory_order_relaxed);
        }
    }

    if (owner >= 0) {
        m_b2m[owner].B_SHARED_SEEN.store(m_tx.seenShared, std::memory_order_relaxed);
        m_b2m[owner].B_HITM_SEEN.store(m_tx.seenHitm, std::memory_order_relaxed);
    }
}

int Interconnect::pickOwnerRR() {
    const int N = static_cast<int>(m_m2b.size());
    for (int i = 0; i < N; ++i) {
        int k = (m_rrPointer + i) % N;
        if (!m_reqEdgeBlock[k] && m_m2b[k].B_REQ.load(std::memory_order_acquire)) {
            m_rrPointer = (k + 1) % N;
            return k;
        }
    }
    return -1;
}

// ============================================================================
// DRAM Access State Machine
// ============================================================================

void Interconnect::startMemReadLine() {
    m_tx.mem = ActiveTx::MemPhase::ReadReq;
    m_tx.segment = 0;
}

bool Interconnect::stepMemReadLine() {
    if (!m_ram) {
        if (m_tx.segment == 0) {
            m_tx.rdata.fill(0);
            m_tx.segment = 4;
        }
        m_tx.mem = ActiveTx::MemPhase::None;
        return true;
    }

    if (m_tx.mem == ActiveTx::MemPhase::ReadReq) {
        auto& R = *m_ram;
        if (!R.request_active.load(std::memory_order_acquire)) {
            R.write_enable.store(false, std::memory_order_release);
            R.request_address.store(
                static_cast<uint16_t>(idx64(m_tx.addrLine) + m_tx.segment * 8),
                std::memory_order_release);
            R.request_active.store(true, std::memory_order_release);
            m_tx.mem = ActiveTx::MemPhase::ReadWait;
        }
        return false;
    }

    if (m_tx.mem == ActiveTx::MemPhase::ReadWait) {
        auto& R = *m_ram;
        if (R.response_ready.load(std::memory_order_acquire)) {
            uint64_t word = R.read_data.load(std::memory_order_acquire);

            const int offset = m_tx.segment * 8;
            for (int i = 0; i < 8; ++i) {
                m_tx.rdata[offset + i] = static_cast<uint8_t>((word >> (i * 8)) & 0xFF);
            }

            R.response_ready.store(false, std::memory_order_release);
            R.request_active.store(false, std::memory_order_release);

            m_tx.segment++;
            m_tx.mem = (m_tx.segment < 4) ? ActiveTx::MemPhase::ReadReq
                : ActiveTx::MemPhase::None;

            return (m_tx.segment >= 4);
        }
        return false;
    }

    return (m_tx.segment >= 4);
}

void Interconnect::startMemWriteLine() {
    m_tx.mem = ActiveTx::MemPhase::WriteReq;
    m_tx.segment = 0;
}

bool Interconnect::stepMemWriteLine() {
    if (!m_ram) {
        m_tx.segment = 4;
        m_tx.mem = ActiveTx::MemPhase::None;
        return true;
    }

    if (m_tx.mem == ActiveTx::MemPhase::WriteReq) {
        auto& R = *m_ram;
        if (!R.request_active.load(std::memory_order_acquire)) {
            uint64_t word = 0;
            const int offset = m_tx.segment * 8;
            for (int i = 0; i < 8; ++i) {
                word |= (static_cast<uint64_t>(m_tx.wbLine[offset + i]) << (i * 8));
            }

            R.write_data.store(word, std::memory_order_release);
            R.write_enable.store(true, std::memory_order_release);
            R.request_address.store(
                static_cast<uint16_t>(idx64(m_tx.addrLine) + m_tx.segment * 8),
                std::memory_order_release);
            R.request_active.store(true, std::memory_order_release);

            m_tx.mem = ActiveTx::MemPhase::WriteWait;
        }
        return false;
    }

    if (m_tx.mem == ActiveTx::MemPhase::WriteWait) {
        auto& R = *m_ram;
        if (R.response_ready.load(std::memory_order_acquire)) {
            R.response_ready.store(false, std::memory_order_release);
            R.request_active.store(false, std::memory_order_release);

            m_tx.segment++;
            m_tx.mem = (m_tx.segment < 4) ? ActiveTx::MemPhase::WriteReq
                : ActiveTx::MemPhase::None;

            return (m_tx.segment >= 4);
        }
        return false;
    }

    return (m_tx.segment >= 4);
}

// ============================================================================
// Main Bus Cycle
// ============================================================================

void Interconnect::tick() {
    clearOutputs();

    if (m_tx.busy && m_tx.owner >= 0) {
        m_b2m[m_tx.owner].B_GRANT.store(true, std::memory_order_relaxed);
    }

    // Unblock masters that lowered B_REQ (edge detection)
    for (size_t i = 0; i < m_m2b.size(); ++i) {
        if (m_reqEdgeBlock[i] && !m_m2b[i].B_REQ.load(std::memory_order_acquire)) {
            m_reqEdgeBlock[i] = false;
        }
    }

    // Step 0: Advance DRAM FSM if in progress
    if (m_tx.busy && m_tx.mem != ActiveTx::MemPhase::None) {
        bool memDone = false;

        switch (m_tx.mem) {
        case ActiveTx::MemPhase::ReadReq:
        case ActiveTx::MemPhase::ReadWait: {
            memDone = stepMemReadLine();
            if (memDone) {
                BUSLOG("DRAM READ done owner=" << m_tx.owner
                    << " addr=0x" << std::hex << m_tx.addrLine << std::dec);

                if (!m_tx.signaledRvalid) {
                    m_b2m[m_tx.owner].B_RDATA = m_tx.rdata;
                    m_b2m[m_tx.owner].B_RVALID.store(true, std::memory_order_release);
                    m_tx.signaledRvalid = true;
                }
                if (!m_tx.signaledDone) {
                    m_b2m[m_tx.owner].B_DONE.store(true, std::memory_order_release);
                    m_tx.signaledDone = true;
                }
                m_tx.mem = ActiveTx::MemPhase::None;
            }
            break;
        }

        case ActiveTx::MemPhase::WriteReq:
        case ActiveTx::MemPhase::WriteWait: {
            memDone = stepMemWriteLine();
            if (memDone) {
                BUSLOG("DRAM WRITE done owner=" << m_tx.owner
                    << " addr=0x" << std::hex << m_tx.addrLine << std::dec);

                if (!m_tx.signaledDone) {
                    m_b2m[m_tx.owner].B_DONE.store(true, std::memory_order_release);
                    m_tx.signaledDone = true;
                }
                m_tx.mem = ActiveTx::MemPhase::None;
            }
            break;
        }

        default:
            break;
        }

        if (m_tx.busy && m_tx.owner >= 0) {
            m_b2m[m_tx.owner].B_GRANT.store(true, std::memory_order_relaxed);
        }
        return;
    }

    // Step 0.5: Wait for master to consume response before releasing
    if (m_tx.busy && m_tx.owner >= 0 && m_tx.mem == ActiveTx::MemPhase::None) {
        bool needHold = false;

        switch (m_tx.cmd) {
        case BusCmd::BusRd:
        case BusCmd::BusRdX: {
            bool rv = m_b2m[m_tx.owner].B_RVALID.load(std::memory_order_acquire);
            bool dn = m_b2m[m_tx.owner].B_DONE.load(std::memory_order_acquire);

            if (rv || dn) {
                needHold = true;
                break;
            }

            // Wait for B_REQ to go low (cache completed FILL)
            bool req = m_m2b[m_tx.owner].B_REQ.load(std::memory_order_acquire);
            if (req) {
                needHold = true;
            }
            break;
        }

        case BusCmd::WriteBack:
        case BusCmd::BusUpgr: {
            bool dn = m_b2m[m_tx.owner].B_DONE.load(std::memory_order_acquire);
            if (dn) {
                needHold = true;
                break;
            }

            bool req = m_m2b[m_tx.owner].B_REQ.load(std::memory_order_acquire);
            if (req) {
                needHold = true;
            }
            break;
        }

        default:
            break;
        }

        if (needHold) {
            m_b2m[m_tx.owner].B_GRANT.store(true, std::memory_order_relaxed);
            return;
        }
        else {
            if (m_tx.cmd != BusCmd::WriteBack) {
                m_reqEdgeBlock[m_tx.owner] = true;
            }
            m_tx.busy = false;
        }
    }

    // Step 1: If no active transaction, find new requester
    if (!m_tx.busy) {
        int owner = pickOwnerRR();
        if (owner < 0) return;

        m_tx.busy = true;
        m_tx.owner = owner;
        m_tx.signaledRvalid = false;
        m_tx.signaledDone = false;

        m_tx.cmd = m_m2b[owner].B_CMD;
        m_tx.addrLine = alignToLine(m_m2b[owner].B_ADDR);
        m_tx.seenShared = false;
        m_tx.seenHitm = false;
        m_tx.mOwner = -1;
        m_tx.haveRdata = false;
        m_tx.invAcksNeeded = 0;
        m_tx.invAcksGot = 0;
        m_tx.mem = ActiveTx::MemPhase::None;
        m_tx.segment = 0;
        m_tx.rdata.fill(0);

        BUSLOG("NEW owner=" << owner << " cmd=" << static_cast<int>(m_tx.cmd)
            << " addr=0x" << std::hex << m_tx.addrLine << std::dec);

        m_b2m[owner].B_GRANT.store(true, std::memory_order_release);

        // Metrics: track cache miss and MESI transition
        if (m_shared) {
            if (m_tx.cmd == BusCmd::BusRd || m_tx.cmd == BusCmd::BusRdX) {
                m_shared->analysis.cache_misses.fetch_add(1, std::memory_order_relaxed);
                m_shared->analysis.transactions_mesi.fetch_add(1, std::memory_order_relaxed);
            }
            else if (m_tx.cmd == BusCmd::BusUpgr) {
                m_shared->analysis.transactions_mesi.fetch_add(1, std::memory_order_relaxed);
            }
        }

        // Broadcast snoop to all other caches
        for (int id = 0; id < static_cast<int>(m_m2b.size()); ++id) {
            if (id == owner) continue;

            std::function<SnoopResp(const SnoopReq&)> cb;
            {
                std::lock_guard<std::mutex> lk(m_cbMutex);
                cb = (id >= 0 && id < static_cast<int>(m_snoopCallbacks.size()))
                    ? m_snoopCallbacks[id] : nullptr;
            }
            if (!cb) continue;

            SnoopReq snoopReq;
            snoopReq.cmd = m_tx.cmd;
            snoopReq.addr_line = m_tx.addrLine;
            snoopReq.grant_data = false;
            snoopReq.from_self = false;

            BUSLOG("SNP -> L1" << id);
            SnoopResp snoopResp = cb(snoopReq);
            BUSLOG("SNP <- L1" << id << " shared=" << snoopResp.has_shared
                << " mod=" << snoopResp.has_mod << " inv_ack=" << snoopResp.inv_ack);

            m_tx.seenShared |= snoopResp.has_shared;
            if (snoopResp.has_mod) {
                m_tx.seenHitm = true;
                if (m_tx.mOwner < 0) {
                    m_tx.mOwner = id;
                }
            }

            // Metrics: track invalidations from BusRdX
            if (m_tx.cmd == BusCmd::BusRdX && snoopResp.inv_ack && m_shared) {
                m_shared->analysis.invalidations.fetch_add(1, std::memory_order_relaxed);
                m_shared->analysis.transactions_mesi.fetch_add(1, std::memory_order_relaxed);
            }

            if (m_tx.cmd == BusCmd::BusUpgr) {
                if (snoopResp.has_shared && !snoopResp.has_mod) {
                    m_tx.invAcksNeeded++;
                }
                if (snoopResp.inv_ack) {
                    m_tx.invAcksGot++;
                }
            }
        }

        BUSLOG("SNP summary: shared=" << m_tx.seenShared << " hitm=" << m_tx.seenHitm
            << " m_owner=" << m_tx.mOwner << " inv_needed=" << m_tx.invAcksNeeded
            << " inv_got=" << m_tx.invAcksGot);

        m_b2m[owner].B_SHARED_SEEN.store(m_tx.seenShared, std::memory_order_relaxed);
        m_b2m[owner].B_HITM_SEEN.store(m_tx.seenHitm, std::memory_order_relaxed);

        // Metrics: track invalidations from BusUpgr
        if (m_shared && m_tx.cmd == BusCmd::BusUpgr && m_tx.invAcksGot > 0) {
            m_shared->analysis.invalidations.fetch_add(
                m_tx.invAcksGot, std::memory_order_relaxed);
            m_shared->analysis.transactions_mesi.fetch_add(
                m_tx.invAcksGot, std::memory_order_relaxed);
        }

        // Execute command-specific logic
        switch (m_tx.cmd) {
        case BusCmd::BusRd:
        case BusCmd::BusRdX: {
            // Check for cache-to-cache transfer
            if (m_tx.seenHitm && m_tx.mOwner >= 0) {
                std::function<SnoopResp(const SnoopReq&)> cb2;
                {
                    std::lock_guard<std::mutex> lk(m_cbMutex);
                    cb2 = (m_tx.mOwner >= 0 &&
                        m_tx.mOwner < static_cast<int>(m_snoopCallbacks.size()))
                        ? m_snoopCallbacks[m_tx.mOwner] : nullptr;
                }

                if (cb2) {
                    SnoopReq s2;
                    s2.cmd = m_tx.cmd;
                    s2.addr_line = m_tx.addrLine;
                    s2.grant_data = true;
                    s2.from_self = false;

                    BUSLOG("C2C grant_data -> L1" << m_tx.mOwner);
                    SnoopResp r2 = cb2(s2);
                    BUSLOG("C2C grant_data <- L1" << m_tx.mOwner << " rvalid=" << r2.rvalid);

                    if (r2.rvalid) {
                        m_tx.rdata = r2.rdata;
                        m_tx.haveRdata = true;

                        if (!m_tx.signaledRvalid) {
                            m_b2m[owner].B_RDATA = m_tx.rdata;
                            m_b2m[owner].B_RVALID.store(true, std::memory_order_release);
                            m_tx.signaledRvalid = true;
                        }
                        if (!m_tx.signaledDone) {
                            m_b2m[owner].B_DONE.store(true, std::memory_order_release);
                            m_tx.signaledDone = true;
                        }

                        // Metrics: track M owner's downgrade
                        if (m_shared) {
                            m_shared->analysis.transactions_mesi.fetch_add(
                                1, std::memory_order_relaxed);
                        }
                    }
                }
            }

            // Fall back to DRAM if no C2C
            if (!m_tx.haveRdata) {
                BUSLOG("DRAM READ start addr=0x" << std::hex << m_tx.addrLine << std::dec);
                startMemReadLine();
            }
            break;
        }

        case BusCmd::WriteBack: {
            if (!m_m2b[owner].B_WVALID.load(std::memory_order_acquire)) {
                if (!m_tx.signaledDone) {
                    m_b2m[owner].B_DONE.store(true, std::memory_order_release);
                    m_tx.signaledDone = true;
                }
                break;
            }

            m_tx.wbLine = m_m2b[owner].B_WDATA;
            BUSLOG("DRAM WRITE start addr=0x" << std::hex << m_tx.addrLine << std::dec);
            startMemWriteLine();
            break;
        }

        case BusCmd::BusUpgr: {
            if (m_tx.invAcksNeeded > 0 && m_tx.invAcksGot < m_tx.invAcksNeeded) {
                BUSLOG("WARN: BusUpgr incomplete invalidations");
            }
            if (!m_tx.signaledDone) {
                m_b2m[owner].B_DONE.store(true, std::memory_order_release);
                m_tx.signaledDone = true;
            }
            break;
        }

        default:
            break;
        }
    }
}