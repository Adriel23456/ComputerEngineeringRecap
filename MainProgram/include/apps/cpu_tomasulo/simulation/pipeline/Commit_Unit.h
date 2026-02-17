#pragma once

/**
 * @file Commit_Unit.h
 * @brief Component 41: Commit Unit.
 *
 * Retires instructions IN PROGRAM ORDER from the ROB head.
 * Only committed results become architecturally visible.
 * Handles register writeback, flag commit, store commit,
 * branch redirect, flush, and exception signaling.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class Commit_Unit : public ITomasuloComponent {
public:
    const char* name() const override { return "Commit_Unit"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    bool m_storeCommitInProgress = false;
    bool m_storeCommitDone = false;

    void activateFree(TomasuloBus& bus, uint8_t stationID) const;
};