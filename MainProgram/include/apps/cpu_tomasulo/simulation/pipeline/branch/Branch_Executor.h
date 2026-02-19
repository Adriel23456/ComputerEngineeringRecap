// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Executor.h
// ============================================================================

#pragma once

/**
 * @file Branch_Executor.h
 * @brief Branch Executor (Component 35).
 *
 * Evaluates branch conditions from architectural flags and determines
 * whether a branch was correctly predicted.
 *
 * Condition evaluation table (NZCV):
 *   B     (0x45) — always taken
 *   BEQ   (0x46) — Z == 1
 *   BNE   (0x47) — Z == 0
 *   BLT   (0x48) — N != V
 *   BGT   (0x49) — Z == 0 && N == V
 *   BUN   (0x4A) — V == 1
 *   BORD  (0x4B) — V == 0
 *
 * Results are written directly to ROB via BrExResult_* bus signals.
 * Unlike ALU/FPU results, branch results do NOT go through the CDB.
 *
 * Purely combinational — clockEdge() is a no-op.
 *
 * @note
 *   - SRP: Only evaluates branch conditions and misprediction detection.
 *   - DIP: Reads from BrEx_* abstraction set by Branch_Arbiter.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class Branch_Executor : public ITomasuloComponent {
public:
    const char* name() const override { return "Branch_Executor"; }

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< Purely combinational — no state.
    void reset() override;
};