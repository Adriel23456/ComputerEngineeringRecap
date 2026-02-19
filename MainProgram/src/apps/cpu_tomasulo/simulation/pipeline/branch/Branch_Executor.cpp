// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Executor.cpp
// ============================================================================

/**
 * @file Branch_Executor.cpp
 * @brief Implementation of Branch_Executor (Component 35).
 *
 * evaluate():
 *   Reads the 4-bit NZCV flags from BrEx_FlagsValue_o and the branch
 *   opcode from BrEx_Op_o. Evaluates the branch condition, detects
 *   misprediction (taken != predicted), and writes results to the
 *   BrExResult_* bus signals consumed by the ROB.
 *
 *   Branch results bypass the CDB entirely — they are written directly
 *   to the ROB entry (BranchTaken, BranchTarget, Mispredict fields).
 *
 * Condition encoding (NZCV bit positions: N=3, Z=2, C=1, V=0):
 *   B     0x45 — always taken
 *   BEQ   0x46 — Z == 1
 *   BNE   0x47 — Z == 0
 *   BLT   0x48 — N != V
 *   BGT   0x49 — Z == 0 && N == V
 *   BUN   0x4A — V == 1  (branch if unordered / FP unordered)
 *   BORD  0x4B — V == 0  (branch if ordered)
 */

#include "apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Executor.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Evaluate
 // ============================================================================

void Branch_Executor::evaluate(TomasuloBus& bus) {
    bus.BrExResult_Valid_o = false;

    if (!bus.BrEx_Start_o) return;

    uint8_t flags = bus.BrEx_FlagsValue_o;
    uint8_t N = (flags >> 3) & 1;
    uint8_t Z = (flags >> 2) & 1;
    // C = (flags >> 1) & 1  — not used by any branch condition
    uint8_t V = flags & 1;

    bool taken = false;
    switch (bus.BrEx_Op_o) {
    case 0x45: taken = true;                       break;  // B
    case 0x46: taken = (Z == 1);                   break;  // BEQ
    case 0x47: taken = (Z == 0);                   break;  // BNE
    case 0x48: taken = (N != V);                   break;  // BLT
    case 0x49: taken = (Z == 0) && (N == V);       break;  // BGT
    case 0x4A: taken = (V == 1);                   break;  // BUN
    case 0x4B: taken = (V == 0);                   break;  // BORD
    }

    bool mispredict = (taken != bus.BrEx_PredTaken_o);

    bus.BrExResult_Valid_o = true;
    bus.BrExResult_ROBTag_o = bus.BrEx_ROBTag_o;
    bus.BrExResult_Taken_o = taken;
    bus.BrExResult_Target_o = bus.BrEx_TargetPC_o;
    bus.BrExResult_Mispredict_o = mispredict;
}

// ============================================================================
// Reset
// ============================================================================

void Branch_Executor::reset() {
    std::cout << "[Branch_Executor] reset()\n";
}