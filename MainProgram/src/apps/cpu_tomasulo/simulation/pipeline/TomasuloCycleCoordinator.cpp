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

    // ── Clear one-shot signals from previous cycle ─────────────
    // These are set by Commit_Unit (late in evaluate order) but
    // read by PC_MUX (early in evaluate order). Without clearing,
    // early components see stale values from the previous cycle.
    bus.BranchRedirect_o = false;
    bus.BranchRedirect_i = false;
    bus.Flush_o = false;
    bus.Flush_PC_i = false;

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