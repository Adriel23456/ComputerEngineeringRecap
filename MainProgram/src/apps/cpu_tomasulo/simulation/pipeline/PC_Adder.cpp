/**
 * @file PC_Adder.cpp
 * @brief Component 3 implementation — PC + 8 (combinational).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/PC_Adder.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"

void PC_Adder::evaluate(TomasuloBus& bus) {
    bus.PCPlus8_o = bus.PCCurrent_o + 8;
}

void PC_Adder::clockEdge(TomasuloBus& /*bus*/) {
    // Pure combinational — nothing to latch.
}

void PC_Adder::reset() {
    // Nothing to reset.
}