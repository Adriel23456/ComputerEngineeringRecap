// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_Arbiter.cpp
// ============================================================================

/**
 * @file AGU_Arbiter.cpp
 * @brief Implementation of AGU_Arbiter (Component 23).
 *
 * evaluate():
 *   1. Clears start signals and all per-requestor AGUDone lines (AGU units
 *      set these fresh if they fire this cycle).
 *   2. Collects up to 5 pending requests in priority order into a local
 *      array (SB0=0, SB1=1, LB0=2, LB1=3, LB2=4).
 *   3. Dispatches the highest-priority request to AGU_0 if it is free.
 *   4. Dispatches the next request to AGU_1 if it is free and a second
 *      request is pending.
 *
 * Note: AGUDone signals are cleared here and re-set by the AGU units
 * in the same evaluate pass. Both this arbiter and the AGUs are purely
 * combinational, so no ordering conflict arises within one cycle.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/agu/AGU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

namespace {
    struct AGURequest {
        uint8_t  sourceID;
        uint64_t base;
        uint64_t offset;
    };
} // namespace

// ============================================================================
// Evaluate
// ============================================================================

void AGU_Arbiter::evaluate(TomasuloBus& bus) {
    bus.AGU0_Start_o = false;
    bus.AGU1_Start_o = false;

    // Clear per-requestor done signals (re-set by AGU units if they fire)
    bus.SB0_AGUDone_o = false;
    bus.SB1_AGUDone_o = false;
    bus.LB0_AGUDone_o = false;
    bus.LB1_AGUDone_o = false;
    bus.LB2_AGUDone_o = false;

    // ── Collect pending requests (priority: SB0 > SB1 > LB0 > LB1 > LB2) ──
    AGURequest pending[5];
    int count = 0;

    if (bus.SB0_AGUReq_o) pending[count++] = { 0, bus.SB0_AGUBase_o, bus.SB0_AGUOffset_o };
    if (bus.SB1_AGUReq_o) pending[count++] = { 1, bus.SB1_AGUBase_o, bus.SB1_AGUOffset_o };
    if (bus.LB0_AGUReq_o) pending[count++] = { 2, bus.LB0_AGUBase_o, bus.LB0_AGUOffset_o };
    if (bus.LB1_AGUReq_o) pending[count++] = { 3, bus.LB1_AGUBase_o, bus.LB1_AGUOffset_o };
    if (bus.LB2_AGUReq_o) pending[count++] = { 4, bus.LB2_AGUBase_o, bus.LB2_AGUOffset_o };

    int idx = 0;

    // ── Dispatch to AGU_0 ─────────────────────────────────────────
    if (idx < count && bus.AGU0_Free_o) {
        bus.AGU0_Start_o = true;
        bus.AGU0_Base_o = pending[idx].base;
        bus.AGU0_Offset_o = pending[idx].offset;
        bus.AGU0_SourceID_o = pending[idx].sourceID;
        ++idx;
    }

    // ── Dispatch to AGU_1 ─────────────────────────────────────────
    if (idx < count && bus.AGU1_Free_o) {
        bus.AGU1_Start_o = true;
        bus.AGU1_Base_o = pending[idx].base;
        bus.AGU1_Offset_o = pending[idx].offset;
        bus.AGU1_SourceID_o = pending[idx].sourceID;
    }
}

// ============================================================================
// Reset
// ============================================================================

void AGU_Arbiter::reset() {
    std::cout << "[AGU_Arbiter] reset()\n";
}