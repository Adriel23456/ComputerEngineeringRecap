/**
 * @file TomasuloCycleCoordinator.cpp
 * @brief Implementation of TomasuloCycleCoordinator.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloCycleCoordinator.h"
#include <iostream>

TomasuloCycleCoordinator::TomasuloCycleCoordinator() = default;

void TomasuloCycleCoordinator::addComponent(ITomasuloComponent* comp) {
    m_components.push_back(comp);
    std::cout << "[Coordinator] Registered component: " << comp->name() << "\n";
}

uint64_t TomasuloCycleCoordinator::executeCycle(TomasuloBus& bus) {
    m_cycleComplete = false;

    std::cout << "\n================================================\n";
    std::cout << "  CYCLE " << (m_cycleCount + 1) << " BEGIN\n";
    std::cout << "==================================================\n";

    // ── Phase 1: Combinational evaluation (topological order) ──
    for (auto* comp : m_components) {
        comp->evaluate(bus);
    }

    // ── Phase 2: Rising clock edge on ALL components ───────────
    bus.CLK = true;
    for (auto* comp : m_components) {
        comp->clockEdge(bus);
    }

    ++m_cycleCount;
    m_cycleComplete = true;

    std::cout << "==================================================\n";
    std::cout << "  CYCLE " << m_cycleCount << " END (PC=0x"
        << std::hex << bus.PCCurrent_o << std::dec << ")\n";
    std::cout << "==================================================\n";

    return m_cycleCount;
}

void TomasuloCycleCoordinator::resetAll(TomasuloBus& bus) {
    bus.clearOutputs();
    bus.RESET = true;

    for (auto* comp : m_components) {
        comp->reset();
    }

    // Run one evaluate pass so outputs reflect reset state
    for (auto* comp : m_components) {
        comp->evaluate(bus);
    }

    bus.RESET = false;
    m_cycleCount = 0;
    m_cycleComplete = true;

    std::cout << "[Coordinator] All components reset. Cycle counter = 0.\n";
}