// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/TomasuloCycleCoordinator.cpp
// ============================================================================

/**
 * @file TomasuloCycleCoordinator.cpp
 * @brief Implementation of TomasuloCycleCoordinator.
 *
 * executeCycle():
 *   1. Clear one-shot bus signals that must not persist across cycles
 *      (BranchRedirect, Flush). Without this, PC_MUX (evaluated early)
 *      would see stale redirect signals from the previous cycle.
 *   2. Call evaluate(bus) on every component in registration order.
 *   3. Assert CLK=true and call clockEdge(bus) on every component.
 *   4. Increment the cycle counter.
 *
 * resetAll():
 *   Clears bus outputs, sets RESET=true, calls reset() on all components,
 *   runs one evaluate pass to propagate reset state through combinational
 *   logic, then clears RESET and resets the cycle counter.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloCycleCoordinator.h"
#include <iostream>

 // ============================================================================
 // Construction
 // ============================================================================

TomasuloCycleCoordinator::TomasuloCycleCoordinator() = default;

void TomasuloCycleCoordinator::addComponent(ITomasuloComponent* comp) {
    m_components.push_back(comp);
    std::cout << "[Coordinator] Registered component: " << comp->name() << "\n";
}

// ============================================================================
// executeCycle
// ============================================================================

uint64_t TomasuloCycleCoordinator::executeCycle(TomasuloBus& bus) {
    m_cycleComplete = false;

    // ── Clear one-shot signals from the previous cycle ────────────
    // Commit_Unit writes these late in the evaluate order;
    // PC_MUX reads them early. Clearing here prevents false redirects.
    bus.BranchRedirect_o = false;
    bus.BranchRedirect_i = false;
    bus.Flush_o = false;
    bus.Flush_PC_i = false;

    // ── Phase 1: Combinational evaluation (topological order) ─────
    for (auto* comp : m_components)
        comp->evaluate(bus);

    // ── Phase 2: Rising clock edge ────────────────────────────────
    bus.CLK = true;
    for (auto* comp : m_components)
        comp->clockEdge(bus);

    ++m_cycleCount;
    m_cycleComplete = true;
    return m_cycleCount;
}

// ============================================================================
// resetAll
// ============================================================================

void TomasuloCycleCoordinator::resetAll(TomasuloBus& bus) {
    bus.clearOutputs();
    bus.RESET = true;

    for (auto* comp : m_components)
        comp->reset();

    // One evaluate pass propagates reset state through combinational outputs
    for (auto* comp : m_components)
        comp->evaluate(bus);

    bus.RESET = false;
    m_cycleCount = 0;
    m_cycleComplete = true;

    std::cout << "[Coordinator] All components reset. Cycle counter = 0.\n";
}