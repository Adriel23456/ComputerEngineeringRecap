// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/TomasuloCycleCoordinator.h
// ============================================================================

#pragma once

/**
 * @file TomasuloCycleCoordinator.h
 * @brief Orchestrates a single simulation cycle across all pipeline components.
 *
 * Each cycle is split into two phases:
 *
 *   Phase 1 — Combinational evaluation:
 *     Each registered component's evaluate(bus) is called in the order
 *     components were added. Callers MUST add components in topological
 *     dependency order (see TomasuloCPU::buildPipeline()).
 *
 *   Phase 2 — Rising clock edge:
 *     All components' clockEdge(bus) are called in the same order,
 *     latching the newly computed combinational outputs into registers.
 *
 * One-shot bus signals (BranchRedirect_o, Flush_o, etc.) are cleared at
 * the top of executeCycle() so early-evaluated components see fresh values
 * rather than stale signals from the previous cycle.
 *
 * resetAll():
 *   Clears the bus, sets RESET=true, calls reset() on every component,
 *   runs one evaluate pass so outputs reflect reset state, then clears RESET
 *   and zeroes the cycle counter.
 *
 * @note
 *   - SRP: Only responsible for evaluation ordering and clock dispatch.
 *   - Components are stored as raw pointers; ownership remains with TomasuloCPU.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <vector>
#include <cstdint>

class TomasuloCycleCoordinator {
public:
    TomasuloCycleCoordinator();

    /**
     * @brief Registers a pipeline component.
     *
     * Components are evaluated in insertion order. The caller is responsible
     * for adding them in correct topological dependency order.
     *
     * @param comp  Non-owning pointer to the component. Must outlive this coordinator.
     */
    void addComponent(ITomasuloComponent* comp);

    /**
     * @brief Executes one full simulation cycle (evaluate + clockEdge).
     *
     * One-shot bus signals are cleared before evaluation begins.
     *
     * @param bus  Shared wire bus (modified in place).
     * @return     Cycle count after this cycle completes.
     */
    uint64_t executeCycle(TomasuloBus& bus);

    /**
     * @brief Resets all components and the cycle counter.
     *
     * Clears the bus, drives RESET=true, calls reset() + evaluate() on all
     * components, then clears RESET and resets the cycle counter to 0.
     *
     * @param bus  Shared wire bus (cleared and re-evaluated in place).
     */
    void resetAll(TomasuloBus& bus);

    uint64_t cycleCount()    const { return m_cycleCount; }
    bool     isCycleComplete() const { return m_cycleComplete; }

private:
    std::vector<ITomasuloComponent*> m_components;
    uint64_t m_cycleCount = 0;
    bool     m_cycleComplete = true;
};