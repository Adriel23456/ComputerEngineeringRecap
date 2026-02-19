// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/execution/IntALU_Arbiter.cpp
// ============================================================================

/**
 * @file IntALU_Arbiter.cpp
 * @brief Component 26 implementation -- Integer ALU Arbiter.
 *
 * Selects between RS_IntALU0 and RS_IntALU1 and dispatches to the single
 * IntALU execution unit. Priority: RS_IntALU0 > RS_IntALU1.
 * Only issues when IntALU_Free_o is asserted (i.e., no pending CDB stall).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/execution/IntALU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

void IntALU_Arbiter::evaluate(TomasuloBus& bus) {
    bus.RSIAU0_Grant_o = false;
    bus.RSIAU1_Grant_o = false;
    bus.IntALU_Start_o = false;

    if (!bus.IntALU_Free_o) return;

    if (bus.RSIAU0_Req_o) {
        // RS_IntALU0 wins priority
        bus.RSIAU0_Grant_o = true;
        bus.IntALU_Start_o = true;
        bus.IntALU_Op_o = bus.RSIAU0_Op_o;
        bus.IntALU_A_o = bus.RSIAU0_Vj_o;
        bus.IntALU_B_o = bus.RSIAU0_Vk_o;
        bus.IntALU_DestROBTag_o = bus.RSIAU0_DestROBTag_o;
        bus.IntALU_ModFlags_o = bus.RSIAU0_ModFlags_o;
        bus.IntALU_NeedsFlags_o = bus.RSIAU0_NeedsFlags_o;
        bus.IntALU_FlagsIn_o = bus.RSIAU0_FlagsValue_o;
    }
    else if (bus.RSIAU1_Req_o) {
        // RS_IntALU1 fallback
        bus.RSIAU1_Grant_o = true;
        bus.IntALU_Start_o = true;
        bus.IntALU_Op_o = bus.RSIAU1_Op_o;
        bus.IntALU_A_o = bus.RSIAU1_Vj_o;
        bus.IntALU_B_o = bus.RSIAU1_Vk_o;
        bus.IntALU_DestROBTag_o = bus.RSIAU1_DestROBTag_o;
        bus.IntALU_ModFlags_o = bus.RSIAU1_ModFlags_o;
        bus.IntALU_NeedsFlags_o = bus.RSIAU1_NeedsFlags_o;
        bus.IntALU_FlagsIn_o = bus.RSIAU1_FlagsValue_o;
    }
}

void IntALU_Arbiter::reset() {
    std::cout << "[IntALU_Arbiter] reset()\n";
}