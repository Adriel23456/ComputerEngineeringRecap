/**
 * @file PC_C.cpp
 * @brief Component 1 implementation — Program Counter.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/PC_C.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void PC_C::evaluate(TomasuloBus& bus) {
    // Combinational: output the current register value
    bus.PCCurrent_o = m_register;
}

// PC_C.cpp — clockEdge
void PC_C::clockEdge(TomasuloBus& bus) {
    uint64_t prev = m_register;

    if (bus.RESET) {
        m_register = 0x0000000000000000ULL;
    }
    else if (bus.Flush_o) {
        m_register = bus.BranchTarget_o;   // <-- WAS bus.PCNext_o
    }
    else if (bus.ROBAlloc_o) {
        m_register = bus.PCNext_o;
    }
}

void PC_C::reset() {
    m_register = 0;
    std::cout << "[PC_C] reset()\n";
}