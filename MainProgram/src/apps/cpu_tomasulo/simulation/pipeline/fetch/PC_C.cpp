// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/fetch/PC_C.cpp
// ============================================================================

/**
 * @file PC_C.cpp
 * @brief Component 1 implementation — Program Counter register.
 *
 * The PC is a simple 64-bit register.  Its update priority (highest first):
 *   1. RESET        → 0x0000000000000000
 *   2. Flush_o      → BranchTarget_o  (branch misprediction redirect)
 *   3. ROBAlloc_o   → PCNext_o        (sequential advance / taken prediction)
 */

#include "apps/cpu_tomasulo/simulation/pipeline/fetch/PC_C.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Evaluate (combinational)
 // ============================================================================

void PC_C::evaluate(TomasuloBus& bus) {
    // Drive the registered value onto the bus every cycle.
    bus.PCCurrent_o = m_register;
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void PC_C::clockEdge(TomasuloBus& bus) {
    if (bus.RESET) {
        // Hard reset — jump to address 0.
        m_register = 0x0000000000000000ULL;
    }
    else if (bus.Flush_o) {
        // Branch misprediction: redirect to the resolved target.
        m_register = bus.BranchTarget_o;
    }
    else if (bus.ROBAlloc_o) {
        // Normal advance: PC moves to the value selected by the PC_MUX.
        m_register = bus.PCNext_o;
    }
    // If none of the above are asserted (e.g. stall), the PC retains its value.
}

// ============================================================================
// Reset
// ============================================================================

void PC_C::reset() {
    m_register = 0;
    std::cout << "[PC_C] reset()\n";
}