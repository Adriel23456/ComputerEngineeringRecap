#pragma once

/**
 * @file TomasuloAnalysisView.h
 * @brief Simulation analysis metrics view for Tomasulo CPU.
 *
 * Displays runtime statistics: committed instructions, latency
 * cycles, reservation station usage, branch mispredictions, etc.
 *
 * @note Follows:
 *   - SRP: Only handles analysis metrics display
 *   - OCP: New metrics can be added without modifying render logic
 *   - LSP: Drop-in replacement for any ITomasuloView
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include <cstdint>
#include <array>

 /**
  * @class TomasuloAnalysisView
  * @brief View panel for Tomasulo CPU simulation analysis metrics.
  *
  * Tracks and displays:
  * - Committed instructions (total and memory-related)
  * - Cycles lost to cache and RAM latency
  * - Total cycle count
  * - Per-station usage counters (SB, LB, RS)
  * - Branch misprediction count
  */
class TomasuloAnalysisView : public ITomasuloView {
public:
    TomasuloAnalysisView() = default;

    /**
     * @brief Renders the analysis metrics table.
     */
    void render() override;

    // ════════════════════════════════════════════════════════════
    // Metric Setters
    // ════════════════════════════════════════════════════════════

    // --- Instruction Commit ---
    void setCommittedInstructions(uint64_t v) { m_committedInstr = v; }
    void setCommittedMemoryInstructions(uint64_t v) { m_committedMemInstr = v; }

    // --- Latency ---
    void setCyclesLostCacheLatency(uint64_t v) { m_cyclesLostCache = v; }
    void setCyclesLostRAMLatency(uint64_t v) { m_cyclesLostRAM = v; }
    void setTotalCycles(uint64_t v) { m_totalCycles = v; }

    // --- Store Buffers ---
    void setUsesSB0(uint64_t v) { m_usesSB[0] = v; }
    void setUsesSB1(uint64_t v) { m_usesSB[1] = v; }

    // --- Load Buffers ---
    void setUsesLB0(uint64_t v) { m_usesLB[0] = v; }
    void setUsesLB1(uint64_t v) { m_usesLB[1] = v; }
    void setUsesLB2(uint64_t v) { m_usesLB[2] = v; }

    // --- Reservation Stations ---
    void setUsesIntALU0(uint64_t v) { m_usesRS[0] = v; }
    void setUsesIntALU1(uint64_t v) { m_usesRS[1] = v; }
    void setUsesFpALU0(uint64_t v) { m_usesRS[2] = v; }
    void setUsesIntMUL0(uint64_t v) { m_usesRS[3] = v; }
    void setUsesFpMUL0(uint64_t v) { m_usesRS[4] = v; }
    void setUsesBranch0(uint64_t v) { m_usesRS[5] = v; }

    // --- Branch ---
    void setBranchMispredictions(uint64_t v) { m_branchMispredictions = v; }

    // --- Generic indexed access ---
    void setUsesSB(int idx, uint64_t v) { if (idx >= 0 && idx < 2) m_usesSB[idx] = v; }
    void setUsesLB(int idx, uint64_t v) { if (idx >= 0 && idx < 3) m_usesLB[idx] = v; }
    void setUsesRS(int idx, uint64_t v) { if (idx >= 0 && idx < 6) m_usesRS[idx] = v; }

    /**
     * @brief Resets all metrics to zero.
     */
    void resetAll();

private:
    // Instruction commit
    uint64_t m_committedInstr = 0;
    uint64_t m_committedMemInstr = 0;

    // Latency
    uint64_t m_cyclesLostCache = 0;
    uint64_t m_cyclesLostRAM = 0;
    uint64_t m_totalCycles = 0;

    // Store Buffers (SB0, SB1)
    std::array<uint64_t, 2> m_usesSB = { 0, 0 };

    // Load Buffers (LB0, LB1, LB2)
    std::array<uint64_t, 3> m_usesLB = { 0, 0, 0 };

    // Reservation Stations (IntALU0, IntALU1, FpALU0, IntMUL0, FpMUL0, Branch0)
    std::array<uint64_t, 6> m_usesRS = { 0, 0, 0, 0, 0, 0 };

    // Branch
    uint64_t m_branchMispredictions = 0;
};