// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.cpp
// ============================================================================

/**
 * @file L1Cache.cpp
 * @brief L1 Cache initialization and reset implementation.
 */

#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h"
#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Utils.h"

L1Cache::L1Cache() {
    reset();
}

void L1Cache::reset() {
    std::lock_guard<std::mutex> lk(mtx_);

    // Clear all cache lines
    for (auto& s : sets_) {
        s.lru = 0;
        for (auto& w : s.ways) {
            w.tag = INVALID_TAG_SENTINEL;
            w.state = Mesi::I;
            w.valid = false;
            w.data.fill(0);
        }
    }

    // Reset output signals
    out_.C_READY = false;
    out_.RD_C_out = 0;

    // Reset FSM state
    fsm_ = L1State::IDLE;
    in_ = {};
    pend_ = {};
    prev_req_ = false;
    temp_fill_.fill(0);

    // Reset debug tracking
    dbg_prev_fsm_ = L1State::IDLE;
    dbg_prev_B_REQ_ = false;
    dbg_prev_B_GRANT_ = false;
    dbg_prev_B_RVALID_ = false;
    dbg_prev_B_DONE_ = false;
}