#include "apps/cpu_tomasulo/simulation/pipeline/FPALU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void FPALU_Arbiter::evaluate(TomasuloBus& bus) {
    bus.RSFPAU0_Grant_o = false;
    bus.FPALU_Start_o = false;

    if (bus.FPALU_Free_o && bus.RSFPAU0_Req_o) {
        bus.RSFPAU0_Grant_o = true;
        bus.FPALU_Start_o = true;
        bus.FPALU_Op_o = bus.RSFPAU0_Op_o;
        bus.FPALU_A_o = bus.RSFPAU0_Vj_o;
        bus.FPALU_B_o = bus.RSFPAU0_Vk_o;
        bus.FPALU_DestROBTag_o = bus.RSFPAU0_DestROBTag_o;
        bus.FPALU_ModFlags_o = bus.RSFPAU0_ModFlags_o;
        bus.FPALU_NeedsFlags_o = bus.RSFPAU0_NeedsFlags_o;
        bus.FPALU_FlagsIn_o = bus.RSFPAU0_FlagsValue_o;
    }
}

void FPALU_Arbiter::reset() {
    std::cout << "[FPALU_Arbiter] reset()\n";
}