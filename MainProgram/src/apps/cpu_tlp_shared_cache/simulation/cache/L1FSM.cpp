// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/cache/L1FSM.cpp
// ============================================================================

/**
 * @file L1FSM.cpp
 * @brief L1 Cache FSM implementation (tick and beginAccess).
 *
 * Contains the main state machine logic for processing cache
 * requests, handling misses, and coordinating with the bus.
 */

#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h"
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Utils.h"
#include "apps/cpu_tlp_shared_cache/simulation/bus/InterconnectBus.h"
#include <iostream>

namespace {

    /**
     * @brief Converts FSM state to string for debugging.
     * @param s State to convert.
     * @return State name string.
     */
    const char* stateName(L1State s) {
        switch (s) {
        case L1State::IDLE:       return "IDLE";
        case L1State::LOOKUP:     return "LOOKUP";
        case L1State::WAIT_ACK:   return "WAIT_ACK";
        case L1State::REQ_BUS:    return "REQ_BUS";
        case L1State::WAIT_GRANT: return "WAIT_GRANT";
        case L1State::WAIT_DATA:  return "WAIT_DATA";
        case L1State::FILL:       return "FILL";
        default:                  return "?";
        }
    }

    /**
     * @brief Selects victim way for replacement.
     *
     * Priority: 1) Way with matching tag, 2) Invalid way, 3) LRU way.
     *
     * @param set Cache set to examine.
     * @param target_tag Tag being requested.
     * @return Selected way index (0 or 1).
     */
    int pickVictimWay(CacheSet& set, uint64_t target_tag) {
        // First check if target tag already exists
        for (int w = 0; w < WAYS; ++w) {
            if (set.ways[w].valid && set.ways[w].tag == target_tag) {
                return w;
            }
        }

        // Look for invalid or I-state way
        for (int w = 0; w < WAYS; ++w) {
            if (!set.ways[w].valid || set.ways[w].state == Mesi::I) {
                return w;
            }
        }

        // Default to way 0 (simple LRU)
        return 0;
    }

} // anonymous namespace

// ============================================================================
// Begin Access - Called by CPU to start memory operation
// ============================================================================

void L1Cache::beginAccess(const CpuReq& req) {
    std::lock_guard<std::mutex> lk(mtx_);
    in_ = req;

    // Handle ACK in WAIT_ACK state - deassert B_REQ here
    if (fsm_ == L1State::WAIT_ACK && in_.C_READY_ACK) {
        out_.C_READY = false;

        // Deassert B_REQ before transitioning to IDLE
        if (pm_out_) {
            pm_out_->B_REQ.store(false, std::memory_order_release);
        }

        fsm_ = L1State::IDLE;
    }

    // Detect rising edge of request signal
    bool rising_req = (in_.C_REQUEST_M && !prev_req_);
    prev_req_ = in_.C_REQUEST_M;

    // Start new lookup on rising edge if idle and not already ready
    if (fsm_ == L1State::IDLE && rising_req && !out_.C_READY) {
        fsm_ = L1State::LOOKUP;
    }
}

// ============================================================================
// Debug Signal Logging
// ============================================================================

void L1Cache::logSignals_() {
    // Log FSM state transitions
    if (fsm_ != dbg_prev_fsm_) {
        std::cout << "[L1" << l1_id_ << "] FSM: "
            << stateName(dbg_prev_fsm_) << " -> " << stateName(fsm_) << "\n";
        dbg_prev_fsm_ = fsm_;
    }

    // Log bus signal changes
    if (pm_out_) {
        bool now = pm_out_->B_REQ.load(std::memory_order_acquire);
        if (now != dbg_prev_B_REQ_) {
            std::cout << "[L1" << l1_id_ << "] B_REQ=" << now << "\n";
            dbg_prev_B_REQ_ = now;
        }
    }

    if (pm_in_) {
        bool grant = pm_in_->B_GRANT.load(std::memory_order_acquire);
        if (grant != dbg_prev_B_GRANT_) {
            std::cout << "[L1" << l1_id_ << "] B_GRANT=" << grant << "\n";
            dbg_prev_B_GRANT_ = grant;
        }

        bool rvalid = pm_in_->B_RVALID.load(std::memory_order_acquire);
        if (rvalid != dbg_prev_B_RVALID_) {
            std::cout << "[L1" << l1_id_ << "] B_RVALID=" << rvalid << "\n";
            dbg_prev_B_RVALID_ = rvalid;
        }

        bool done = pm_in_->B_DONE.load(std::memory_order_acquire);
        if (done != dbg_prev_B_DONE_) {
            std::cout << "[L1" << l1_id_ << "] B_DONE=" << done << "\n";
            dbg_prev_B_DONE_ = done;
        }
    }
}

// ============================================================================
// Main FSM Tick
// ============================================================================

void L1Cache::tick() {
    std::lock_guard<std::mutex> lk(mtx_);

    using std::memory_order_acquire;
    using std::memory_order_release;
    using std::memory_order_relaxed;
    using std::memory_order_acq_rel;

    logSignals_();

    switch (fsm_) {

    // ========================================================================
    // LOOKUP: Check cache for hit/miss
    // ========================================================================
    case L1State::LOOKUP: {
        const auto p = splitAddress(in_.ALUOut_M);
        auto& set = sets_[p.set];

        int hit_way = findWay(p.set, p.tag);

        if (hit_way >= 0) {
            auto& line = set.ways[hit_way];

            // Line exists but is Invalid - treat as miss
            if (line.state == Mesi::I) {
                pend_ = {};
                pend_.req_addr_line = (in_.ALUOut_M & ~((1ULL << OFFSET_BITS) - 1ULL));
                pend_.set = p.set;
                pend_.victim = hit_way;
                pend_.need_wb = false;
                pend_.byte_in_line = p.byte_in_line;
                pend_.dw_in_line = p.dw_in_line;
                pend_.cmd = (!in_.C_WE_M) ? BusCmd::BusRd : BusCmd::BusRdX;

                if (pm_out_) {
                    pm_out_->B_CMD = pend_.cmd;
                    pm_out_->B_ADDR = pend_.req_addr_line;
                    pm_out_->B_WVALID.store(false, memory_order_relaxed);
                    pm_out_->B_REQ.store(true, memory_order_release);
                }
                fsm_ = L1State::REQ_BUS;
            }
            // Read hit - return data
            else if (!in_.C_WE_M) {
                if (in_.C_ISB_M) {
                    out_.RD_C_out = static_cast<uint64_t>(read8_in_line(line, p.byte_in_line));
                }
                else if ((p.off & 0x7) == 0) {
                    out_.RD_C_out = read64_in_line(line, p.dw_in_line);
                }

                set.lru = (hit_way == 0) ? 0 : 1;
                out_.C_READY = true;
                fsm_ = L1State::WAIT_ACK;
            }
            // Write hit
            else {
                // Shared state requires upgrade before write
                if (line.state == Mesi::S) {
                    pend_ = {};
                    pend_.cmd = BusCmd::BusUpgr;
                    pend_.req_addr_line = (in_.ALUOut_M & ~((1ULL << OFFSET_BITS) - 1ULL));
                    pend_.set = p.set;
                    pend_.victim = hit_way;
                    pend_.byte_in_line = p.byte_in_line;
                    pend_.dw_in_line = p.dw_in_line;

                    if (pm_out_) {
                        pm_out_->B_CMD = BusCmd::BusUpgr;
                        pm_out_->B_ADDR = pend_.req_addr_line;
                        pm_out_->B_WVALID.store(false, memory_order_relaxed);
                        pm_out_->B_REQ.store(true, memory_order_release);
                    }
                    fsm_ = L1State::REQ_BUS;
                    break;
                }

                // E or M state - can write directly
                if (in_.C_ISB_M) {
                    write8_in_line(line, p.byte_in_line,
                        static_cast<uint8_t>(in_.RD_Rm_Special_M & 0xFF));
                }
                else {
                    if ((p.off & 0x7) != 0) break;
                    write64_in_line(line, p.dw_in_line, in_.RD_Rm_Special_M);
                }

                // E transitions to M on write
                if (line.state == Mesi::E) {
                    line.state = Mesi::M;
                }

                set.lru = (hit_way == 0) ? 0 : 1;
                out_.C_READY = true;
                fsm_ = L1State::WAIT_ACK;
            }
        }
        // Cache miss - need to fetch from memory
        else {
            pend_ = {};
            pend_.req_addr_line = (in_.ALUOut_M & ~((1ULL << OFFSET_BITS) - 1ULL));
            pend_.set = p.set;

            pend_.victim = pickVictimWay(set, p.tag);
            auto& vict = set.ways[pend_.victim];

            // Check if victim needs writeback
            pend_.need_wb = (vict.valid && vict.state == Mesi::M);
            if (pend_.need_wb) {
                pend_.victim_addr_line =
                    ((vict.tag << (INDEX_BITS + OFFSET_BITS)) |
                        (static_cast<uint64_t>(pend_.set) << OFFSET_BITS));
                pend_.wb_line = vict.data;
            }

            pend_.byte_in_line = p.byte_in_line;
            pend_.dw_in_line = p.dw_in_line;
            pend_.cmd = (!in_.C_WE_M) ? BusCmd::BusRd : BusCmd::BusRdX;

            if (pm_out_) {
                pm_out_->B_CMD = pend_.need_wb ? BusCmd::WriteBack : pend_.cmd;
                pm_out_->B_ADDR = pend_.need_wb ? pend_.victim_addr_line : pend_.req_addr_line;
                pm_out_->B_WDATA = pend_.wb_line;
                pm_out_->B_WVALID.store(pend_.need_wb, memory_order_relaxed);
                pm_out_->B_REQ.store(true, memory_order_release);
            }
            fsm_ = L1State::REQ_BUS;
        }
    } break;

    // ========================================================================
    // REQ_BUS: Assert bus request
    // ========================================================================
    case L1State::REQ_BUS: {
        if (pm_out_) {
            pm_out_->B_REQ.store(true, memory_order_release);
        }
        fsm_ = L1State::WAIT_GRANT;
    } break;

    // ========================================================================
    // WAIT_GRANT: Wait for bus arbiter to grant access
    // ========================================================================
    case L1State::WAIT_GRANT: {
        if (pm_in_ && pm_in_->B_GRANT.load(memory_order_acquire)) {
            pend_.saw_shared |= pm_in_->B_SHARED_SEEN.load(memory_order_relaxed);
            pend_.saw_hitm |= pm_in_->B_HITM_SEEN.load(memory_order_relaxed);
            fsm_ = L1State::WAIT_DATA;
        }
    } break;

    // ========================================================================
    // WAIT_DATA: Wait for data from memory or another cache
    // ========================================================================
    case L1State::WAIT_DATA: {
        if (!pm_in_) break;

        // Accumulate snoop results
        pend_.saw_shared |= pm_in_->B_SHARED_SEEN.load(memory_order_relaxed);
        pend_.saw_hitm |= pm_in_->B_HITM_SEEN.load(memory_order_relaxed);

        // Check for incoming data
        if (pm_in_->B_RVALID.exchange(false, memory_order_acq_rel)) {
            temp_fill_ = pm_in_->B_RDATA;
        }

        // Check for transaction completion
        if (pm_in_->B_DONE.exchange(false, memory_order_acq_rel)) {
            // If this was a writeback, now issue the actual request
            if (pm_out_ && pm_out_->B_CMD == BusCmd::WriteBack) {
                // Deassert B_REQ first for edge detection
                pm_out_->B_REQ.store(false, memory_order_release);

                // Setup next request
                pm_out_->B_CMD = pend_.cmd;
                pm_out_->B_ADDR = pend_.req_addr_line;
                pm_out_->B_WVALID.store(false, memory_order_relaxed);
                pend_.saw_shared = false;
                pend_.saw_hitm = false;

                // Return to REQ_BUS to reassert request
                fsm_ = L1State::REQ_BUS;
                break;
            }
            fsm_ = L1State::FILL;
        }
    } break;

    // ========================================================================
    // FILL: Install fetched line and complete access
    // ========================================================================
    case L1State::FILL: {
        const auto p_now = splitAddress(in_.ALUOut_M);
        auto& set = sets_[pend_.set];

        // Handle BusUpgr completion (S->M transition)
        if (pend_.cmd == BusCmd::BusUpgr) {
            int way = findWay(pend_.set, (pend_.req_addr_line >> (OFFSET_BITS + INDEX_BITS)));
            if (way >= 0) {
                auto& line = set.ways[way];
                line.state = Mesi::M;

                if (in_.C_ISB_M) {
                    write8_in_line(line, pend_.byte_in_line,
                        static_cast<uint8_t>(in_.RD_Rm_Special_M & 0xFF));
                }
                else {
                    write64_in_line(line, pend_.dw_in_line, in_.RD_Rm_Special_M);
                }
                set.lru = (way == 0) ? 0 : 1;
            }

            out_.C_READY = true;
            if (pm_out_) {
                pm_out_->B_WVALID.store(false, memory_order_release);
                // B_REQ is deasserted in beginAccess when ACK received
            }
            fsm_ = L1State::WAIT_ACK;
            break;
        }

        // Install new line
        auto& vict = set.ways[pend_.victim];
        vict.data = temp_fill_;
        vict.tag = (pend_.req_addr_line >> (OFFSET_BITS + INDEX_BITS));
        vict.valid = true;

        if (pend_.cmd == BusCmd::BusRd) {
            // Read: set state based on snoop results
            vict.state = (pend_.saw_shared || pend_.saw_hitm) ? Mesi::S : Mesi::E;

            if (in_.C_ISB_M) {
                out_.RD_C_out = static_cast<uint64_t>(read8_in_line(vict, pend_.byte_in_line));
            }
            else {
                out_.RD_C_out = read64_in_line(vict, pend_.dw_in_line);
            }
        }
        else {
            // Write: set to Modified and write data
            vict.state = Mesi::M;

            if (in_.C_ISB_M) {
                write8_in_line(vict, pend_.byte_in_line,
                    static_cast<uint8_t>(in_.RD_Rm_Special_M & 0xFF));
            }
            else {
                write64_in_line(vict, pend_.dw_in_line, in_.RD_Rm_Special_M);
            }
        }

        set.lru = (pend_.victim == 0) ? 0 : 1;
        out_.C_READY = true;

        if (pm_out_) {
            pm_out_->B_WVALID.store(false, memory_order_release);
            // B_REQ is deasserted in beginAccess when ACK received
        }

        fsm_ = L1State::WAIT_ACK;
    } break;

    // ========================================================================
    // WAIT_ACK: Handled in beginAccess
    // ========================================================================
    case L1State::WAIT_ACK: {
        // ACK handling moved to beginAccess for proper edge detection
    } break;

    default:
        break;
    }
}

int L1Cache::findWay(uint32_t set, uint64_t tag) const {
    const auto& cs = sets_[set];
    for (int w = 0; w < static_cast<int>(WAYS); ++w) {
        const auto& line = cs.ways[w];
        if (line.valid && line.tag == tag) {
            return w;
        }
    }
    return -1;
}