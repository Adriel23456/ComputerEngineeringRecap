#pragma once

/**
 * @file TomasuloCycleCoordinator.h
 * @brief Orchestrates a single simulation cycle across all components.
 *
 * Evaluation order follows the topological dependency graph:
 *   Phase 1: PC_C -> PC_Adder -> (need Control_Unit for stall) -> PC_MUX
 *   Phase 2: I_Cache -> Decoder -> Extends -> Control_Unit -> RegFile -> ROB -> Flags
 *   Phase 3: RS/Buffers (alloc + ready check)
 *   Phase 4: Arbiters -> Execution Units -> CDB Arbiter -> CDB broadcast
 *   Phase 5: Commit_Unit -> flush/redirect signals
 *   Phase 6: Clock edge on ALL sequential components
 *
 * @note SRP: Only coordinates evaluation order.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <vector>
#include <cstdint>
#include <memory>

class TomasuloCycleCoordinator {
public:
    TomasuloCycleCoordinator();

    /**
     * @brief Register a component. Components are evaluated in insertion order
     *        for the combinational phase, then all get clockEdge().
     *        The caller MUST add components in correct topological order.
     */
    void addComponent(ITomasuloComponent* comp);

    /**
     * @brief Execute one full simulation cycle.
     * @param bus The shared wire bus.
     * @return The cycle number after execution.
     */
    uint64_t executeCycle(TomasuloBus& bus);

    /** @brief Reset all components and the cycle counter. */
    void resetAll(TomasuloBus& bus);

    uint64_t cycleCount() const { return m_cycleCount; }

    /** @brief True if the cycle has completed (always true after executeCycle returns). */
    bool isCycleComplete() const { return m_cycleComplete; }

private:
    std::vector<ITomasuloComponent*> m_components;
    uint64_t m_cycleCount = 0;
    bool     m_cycleComplete = true;
};