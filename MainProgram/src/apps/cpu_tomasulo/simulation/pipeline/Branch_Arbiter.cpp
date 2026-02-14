#include "apps/cpu_tomasulo/simulation/pipeline/Branch_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void Branch_Arbiter::evaluate(TomasuloBus& bus) {
    bus.RSBr0_Grant_o = false;
    bus.BrEx_Start_o = false;

    if (bus.RSBr0_Req_o) {
        bus.RSBr0_Grant_o = true;
        bus.BrEx_Start_o = true;
        bus.BrEx_Op_o = bus.RSBr0_Op_o;
        bus.BrEx_TargetPC_o = bus.RSBr0_TargetPC_o;
        bus.BrEx_FlagsValue_o = bus.RSBr0_FlagsValue_o;
        bus.BrEx_PredTaken_o = bus.RSBr0_PredTaken_o;
        bus.BrEx_ROBTag_o = bus.RSBr0_ROBTag_o;
        std::cout << "[Branch_Arbiter] Grant RS_Branch0, ROB#"
            << (int)bus.RSBr0_ROBTag_o << "\n";
    }
}

void Branch_Arbiter::reset() {
    std::cout << "[Branch_Arbiter] reset()\n";
}