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

void PC_C::clockEdge(TomasuloBus& bus) {
    uint64_t prev = m_register;

    if (bus.RESET) {
        m_register = 0x0000000000000000ULL;
        std::cout << "[PC_C] RESET -> PC = 0x0\n";
    }
    else if (bus.Flush_o) {
        // Flush overrides stall — accept redirect
        m_register = bus.PCNext_o;
        std::cout << "[PC_C] FLUSH -> PC = 0x" << std::hex << m_register << std::dec << "\n";
    }
    else if (!bus.StallIF_o) {
        m_register = bus.PCNext_o;
    }
    // else: stall — hold value

    if (m_register != prev) {
        std::cout << "[PC_C] clockEdge: PC 0x" << std::hex << prev
            << " -> 0x" << m_register << std::dec << "\n";
    }
}

void PC_C::reset() {
    m_register = 0;
    std::cout << "[PC_C] reset()\n";
}