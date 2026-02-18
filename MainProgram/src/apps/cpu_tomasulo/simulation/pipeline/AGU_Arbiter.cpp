/**
 * @file AGU_Arbiter.cpp
 * @brief Component 23 implementation -- AGU Arbiter dispatch logic.
 *
 * Purely combinational. Collects AGU requests from SB0/SB1/LB0/LB1/LB2,
 * dispatches up to 2 per cycle to AGU_0 and AGU_1.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/AGU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

struct AGURequest {
    uint8_t  sourceID;
    uint64_t base;
    uint64_t offset;
};

void AGU_Arbiter::evaluate(TomasuloBus& bus) {
    bus.AGU0_Start_o = false;
    bus.AGU1_Start_o = false;

    // Clear per-requestor AGU done signals (set fresh by AGU if it fires)
    bus.SB0_AGUDone_o = false;
    bus.SB1_AGUDone_o = false;
    bus.LB0_AGUDone_o = false;
    bus.LB1_AGUDone_o = false;
    bus.LB2_AGUDone_o = false;

    // Collect pending requests in priority order: SB0 > SB1 > LB0 > LB1 > LB2
    AGURequest pending[5];
    int count = 0;

    if (bus.SB0_AGUReq_o) { pending[count++] = { 0, bus.SB0_AGUBase_o, bus.SB0_AGUOffset_o }; }
    if (bus.SB1_AGUReq_o) { pending[count++] = { 1, bus.SB1_AGUBase_o, bus.SB1_AGUOffset_o }; }
    if (bus.LB0_AGUReq_o) { pending[count++] = { 2, bus.LB0_AGUBase_o, bus.LB0_AGUOffset_o }; }
    if (bus.LB1_AGUReq_o) { pending[count++] = { 3, bus.LB1_AGUBase_o, bus.LB1_AGUOffset_o }; }
    if (bus.LB2_AGUReq_o) { pending[count++] = { 4, bus.LB2_AGUBase_o, bus.LB2_AGUOffset_o }; }

    int idx = 0;

    // Dispatch to AGU_0
    if (idx < count && bus.AGU0_Free_o) {
        bus.AGU0_Start_o = true;
        bus.AGU0_Base_o = pending[idx].base;
        bus.AGU0_Offset_o = pending[idx].offset;
        bus.AGU0_SourceID_o = pending[idx].sourceID;
        idx++;
    }

    // Dispatch to AGU_1
    if (idx < count && bus.AGU1_Free_o) {
        bus.AGU1_Start_o = true;
        bus.AGU1_Base_o = pending[idx].base;
        bus.AGU1_Offset_o = pending[idx].offset;
        bus.AGU1_SourceID_o = pending[idx].sourceID;
        idx++;
    }
}

void AGU_Arbiter::reset() {
    std::cout << "[AGU_Arbiter] reset()\n";
}