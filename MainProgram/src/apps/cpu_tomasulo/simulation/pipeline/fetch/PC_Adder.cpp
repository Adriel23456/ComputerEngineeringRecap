// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_Adder.cpp
// ============================================================================

/**
 * @file PC_Adder.cpp
 * @brief Component 3 implementation — PC + 8 (combinational).
 *
 * Adds 8 to PCCurrent_o each cycle to produce the sequential next-PC
 * (PCPlus8_o), which is then fed into the PC_MUX.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/fetch/PC_Adder.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"

void PC_Adder::evaluate(TomasuloBus& bus) {
    // One instruction = 8 bytes (64-bit word), so sequential fetch advances by 8.
    bus.PCPlus8_o = bus.PCCurrent_o + 8;
}

void PC_Adder::clockEdge(TomasuloBus& /*bus*/) {
    // Pure combinational — nothing to latch.
}

void PC_Adder::reset() {
    // No internal state.
}