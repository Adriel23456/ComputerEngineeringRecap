// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_MUX.cpp
// ============================================================================

/**
 * @file PC_MUX.cpp
 * @brief Component 2 implementation — PC MUX (combinational).
 *
 * Selects between the sequential next-PC (PCPlus8_o) and the branch
 * redirect target (BranchTarget_o) based on BranchRedirect_o.
 * The result is placed on PCNext_o for PC_C to latch on the next edge.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/fetch/PC_MUX.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Evaluate (combinational)
 // ============================================================================

void PC_MUX::evaluate(TomasuloBus& bus) {
    if (bus.BranchRedirect_o) {
        // Misprediction or unconditional taken branch: override the sequential PC.
        bus.PCNext_o = bus.BranchTarget_o;
        std::cout << "[PC_MUX] BranchRedirect -> PCNext = 0x"
            << std::hex << bus.PCNext_o << std::dec << "\n";
    }
    else {
        // No redirect: advance sequentially by 8.
        bus.PCNext_o = bus.PCPlus8_o;
    }
}

void PC_MUX::clockEdge(TomasuloBus& /*bus*/) {
    // Pure combinational — nothing to latch.
}

// ============================================================================
// Reset
// ============================================================================

void PC_MUX::reset() {
    std::cout << "[PC_MUX] reset()\n";
}