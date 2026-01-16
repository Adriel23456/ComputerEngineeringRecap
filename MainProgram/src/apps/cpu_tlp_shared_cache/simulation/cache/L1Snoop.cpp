// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/cache/L1Snoop.cpp
// ============================================================================

/**
 * @file L1Snoop.cpp
 * @brief L1 Cache snoop handling for MESI coherency.
 *
 * Implements responses to bus snoop requests from other caches,
 * maintaining cache coherency across multiple processors.
 */

#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h"
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Utils.h"
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Snoop.h"
#include <iostream>

#define SNOOPLOG(MSG) do { std::cout << "[L1Snoop] " << MSG << std::endl; } while(0)

SnoopResp L1Cache::onSnoop(const SnoopReq& s) {
    std::lock_guard<std::mutex> lk(mtx_);

    SnoopResp r{};
    const auto a = splitAddress(s.addr_line);

    // ========================================================================
    // Check pending transactions FIRST
    // ========================================================================
    // If we have an active transaction for this line, respond based on
    // the state the line WILL have when complete, not its current state.
    // This prevents multiple caches from acquiring the same line in M state.

    if (pend_.is_active) {
        uint64_t pend_tag = pend_.req_addr_line >> (OFFSET_BITS + INDEX_BITS);

        if (pend_.set == a.set && pend_tag == a.tag) {
            // This transaction is for the same line being snooped

            Mesi future_state = pend_.target_state;

            const bool will_be_S = (future_state == Mesi::S);
            const bool will_be_E = (future_state == Mesi::E);
            const bool will_be_M = (future_state == Mesi::M);

            r.has_shared = (will_be_S || will_be_E || will_be_M);
            r.has_mod = will_be_M;

            // Handle snoop based on command type
            if (s.cmd == BusCmd::BusRdX) {
                // Another cache wants exclusive access - invalidate ourselves
                if (will_be_S || will_be_E) {
                    pend_.is_active = false;
                    pend_.target_state = Mesi::I;
                    r.inv_ack = true;
                }
                else if (will_be_M) {
                    pend_.target_state = Mesi::I;
                    pend_.is_active = false;
                    r.inv_ack = true;
                }
            }
            else if (s.cmd == BusCmd::BusRd) {
                // Another cache wants shared access - downgrade if needed
                if (will_be_E) {
                    pend_.target_state = Mesi::S;
                }
                else if (will_be_M) {
                    pend_.target_state = Mesi::S;
                }
            }
            else if (s.cmd == BusCmd::BusUpgr) {
                // Another cache upgrading S->M - invalidate ourselves
                if (will_be_S || will_be_E) {
                    pend_.is_active = false;
                    pend_.target_state = Mesi::I;
                    r.inv_ack = true;
                }
            }

            return r;
        }
    }

    // ========================================================================
    // Check existing cache lines
    // ========================================================================

    auto& set = sets_[a.set];
    int way = findWay(a.set, a.tag);

    if (way < 0) {
        return r;  // Line not present
    }

    auto& line = set.ways[way];
    if (!line.valid || line.tag != a.tag) {
        return r;
    }

    const bool isS = (line.state == Mesi::S);
    const bool isE = (line.state == Mesi::E);
    const bool isM = (line.state == Mesi::M);

    r.has_shared = (isS || isE || isM);
    r.has_mod = isM;

    switch (s.cmd) {
    case BusCmd::BusRd: {
        // Another cache wants to read
        if (isM) {
            // Must provide data and downgrade to Shared
            if (s.grant_data) {
                r.rdata = line.data;
                r.rvalid = true;
                line.state = Mesi::S;
            }
        }
        else if (isE) {
            // Downgrade E -> S (another cache now has a copy)
            line.state = Mesi::S;
        }
    } break;

    case BusCmd::BusRdX: {
        // Another cache wants exclusive access
        if (isM) {
            // Must provide data and invalidate
            if (s.grant_data) {
                r.rdata = line.data;
                r.rvalid = true;
                line.state = Mesi::I;
                line.valid = false;
                r.inv_ack = true;
            }
            // First pass: don't invalidate yet (wait for grant_data)
        }
        else {
            // S/E invalidate immediately
            if (line.state != Mesi::I) {
                line.state = Mesi::I;
                line.valid = false;
                r.inv_ack = true;
            }
        }
    } break;

    case BusCmd::BusUpgr: {
        // Another cache upgrading S -> M
        if (isS || isE) {
            line.state = Mesi::I;
            line.valid = false;
            r.inv_ack = true;
        }
    } break;

    default:
        break;
    }

    return r;
}