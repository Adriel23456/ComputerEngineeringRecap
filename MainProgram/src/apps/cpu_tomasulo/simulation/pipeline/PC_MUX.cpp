/**
 * @file PC_MUX.cpp
 * @brief Component 2 implementation — PC MUX (combinational).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/PC_MUX.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void PC_MUX::evaluate(TomasuloBus& bus) {
    if (bus.BranchRedirect_o) {
        bus.PCNext_o = bus.BranchTarget_o;
        std::cout << "[PC_MUX] BranchRedirect -> PCNext = 0x"
            << std::hex << bus.PCNext_o << std::dec << "\n";
    }
    else {
        bus.PCNext_o = bus.PCPlus8_o;
    }
}

void PC_MUX::clockEdge(TomasuloBus& /*bus*/) {
    // Pure combinational — nothing to latch.
}

void PC_MUX::reset() {
    std::cout << "[PC_MUX] reset()\n";
}