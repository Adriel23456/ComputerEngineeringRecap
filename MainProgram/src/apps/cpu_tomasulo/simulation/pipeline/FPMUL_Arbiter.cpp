#include "apps/cpu_tomasulo/simulation/pipeline/FPMUL_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void FPMUL_Arbiter::evaluate(TomasuloBus& bus) {
    bus.RSFPMUL0_Grant_o = false;
    bus.FPMUL_Start_o = false;

    if (bus.FPMUL_Free_o && bus.RSFPMUL0_Req_o) {
        bus.RSFPMUL0_Grant_o = true;
        bus.FPMUL_Start_o = true;
        bus.FPMUL_Op_o = bus.RSFPMUL0_Op_o;
        bus.FPMUL_A_o = bus.RSFPMUL0_Vj_o;
        bus.FPMUL_B_o = bus.RSFPMUL0_Vk_o;
        bus.FPMUL_DestROBTag_o = bus.RSFPMUL0_DestROBTag_o;
        bus.FPMUL_ModFlags_o = bus.RSFPMUL0_ModFlags_o;
        bus.FPMUL_NeedsFlags_o = bus.RSFPMUL0_NeedsFlags_o;
        bus.FPMUL_FlagsIn_o = bus.RSFPMUL0_FlagsValue_o;
        std::cout << "[FPMUL_Arbiter] Grant RS_FPMUL0, ROB#"
            << (int)bus.RSFPMUL0_DestROBTag_o << "\n";
    }
}

void FPMUL_Arbiter::reset() {
    std::cout << "[FPMUL_Arbiter] reset()\n";
}