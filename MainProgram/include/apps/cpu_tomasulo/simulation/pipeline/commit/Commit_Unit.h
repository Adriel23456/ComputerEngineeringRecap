// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/commit/Commit_Unit.h
// ============================================================================

#pragma once

/**
 * @file Commit_Unit.h
 * @brief Component 41: Commit Unit.
 *
 * Retires instructions IN PROGRAM ORDER from the ROB head.
 * Only committed results become architecturally visible.
 * Handles register writeback, flag commit, store commit,
 * branch redirect, flush, and exception signaling.
 *
 * Store commit is a two-phase handshake with Memory_Arbiter:
 *   Phase 1 (evaluate): StoreCommit_Req_o is held HIGH with the address/data.
 *   Phase 2 (clockEdge): StoreCommit_Done_o is latched; ROB pop occurs next cycle.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class Commit_Unit : public ITomasuloComponent {
public:
    const char* name() const override { return "Commit_Unit"; }
    /** @brief Drives all commit-stage outputs based on the current ROB head. */
    void evaluate(TomasuloBus& bus) override;
    /** @brief Latches store-commit progress flags on the clock edge. */
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    bool m_storeCommitInProgress = false; ///< True after StoreCommit_Req_o is first raised.
    bool m_storeCommitDone = false;       ///< True once StoreCommit_Done_o has been latched.

    /**
     * @brief Asserts the Free signal for the station identified by @p stationID.
     * @param stationID  4-bit source-station code stored in the ROB entry.
     */
    void activateFree(TomasuloBus& bus, uint8_t stationID) const;
};