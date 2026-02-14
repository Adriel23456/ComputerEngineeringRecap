#include "apps/cpu_tomasulo/simulation/pipeline/IntALU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void IntALU_Arbiter::evaluate(TomasuloBus& bus) {
    bus.RSIAU0_Grant_o = false;
    bus.RSIAU1_Grant_o = false;
    bus.IntALU_Start_o = false;

    if (!bus.IntALU_Free_o) return;

    if (bus.RSIAU0_Req_o) {
        bus.RSIAU0_Grant_o = true;
        bus.IntALU_Start_o = true;
        bus.IntALU_Op_o = bus.RSIAU0_Op_o;
        bus.IntALU_A_o = bus.RSIAU0_Vj_o;
        bus.IntALU_B_o = bus.RSIAU0_Vk_o;
        bus.IntALU_DestROBTag_o = bus.RSIAU0_DestROBTag_o;
        bus.IntALU_ModFlags_o = bus.RSIAU0_ModFlags_o;
        bus.IntALU_NeedsFlags_o = bus.RSIAU0_NeedsFlags_o;
        bus.IntALU_FlagsIn_o = bus.RSIAU0_FlagsValue_o;
        std::cout << "[IntALU_Arbiter] Grant RS_IntALU0, ROB#"
            << (int)bus.RSIAU0_DestROBTag_o << "\n";
    }
    else if (bus.RSIAU1_Req_o) {
        bus.RSIAU1_Grant_o = true;
        bus.IntALU_Start_o = true;
        bus.IntALU_Op_o = bus.RSIAU1_Op_o;
        bus.IntALU_A_o = bus.RSIAU1_Vj_o;
        bus.IntALU_B_o = bus.RSIAU1_Vk_o;
        bus.IntALU_DestROBTag_o = bus.RSIAU1_DestROBTag_o;
        bus.IntALU_ModFlags_o = bus.RSIAU1_ModFlags_o;
        bus.IntALU_NeedsFlags_o = bus.RSIAU1_NeedsFlags_o;
        bus.IntALU_FlagsIn_o = bus.RSIAU1_FlagsValue_o;
        std::cout << "[IntALU_Arbiter] Grant RS_IntALU1, ROB#"
            << (int)bus.RSIAU1_DestROBTag_o << "\n";
    }
}

void IntALU_Arbiter::reset() {
    std::cout << "[IntALU_Arbiter] reset()\n";
}