#include "apps/cpu_tomasulo/simulation/pipeline/IntMUL_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void IntMUL_Arbiter::evaluate(TomasuloBus& bus) {
    bus.RSIMUL0_Grant_o = false;
    bus.IntMUL_Start_o = false;

    if (bus.IntMUL_Free_o && bus.RSIMUL0_Req_o) {
        bus.RSIMUL0_Grant_o = true;
        bus.IntMUL_Start_o = true;
        bus.IntMUL_Op_o = bus.RSIMUL0_Op_o;
        bus.IntMUL_A_o = bus.RSIMUL0_Vj_o;
        bus.IntMUL_B_o = bus.RSIMUL0_Vk_o;
        bus.IntMUL_DestROBTag_o = bus.RSIMUL0_DestROBTag_o;
        bus.IntMUL_ModFlags_o = bus.RSIMUL0_ModFlags_o;
        bus.IntMUL_NeedsFlags_o = bus.RSIMUL0_NeedsFlags_o;
        bus.IntMUL_FlagsIn_o = bus.RSIMUL0_FlagsValue_o;
    }
}

void IntMUL_Arbiter::reset() {
    std::cout << "[IntMUL_Arbiter] reset()\n";
}