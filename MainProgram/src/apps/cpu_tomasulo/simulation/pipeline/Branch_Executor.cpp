/**
 * @file Branch_Executor.cpp
 * @brief Component 35 implementation -- Branch Executor.
 *
 * Evaluates branch condition from flags. Results go directly to ROB.
 * B=always, BEQ=Z, BNE=!Z, BLT=N!=V, BGT=!Z&&N==V, BUN=V, BORD=!V.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/Branch_Executor.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void Branch_Executor::evaluate(TomasuloBus& bus) {
    bus.BrExResult_Valid_o = false;

    if (!bus.BrEx_Start_o) return;

    uint8_t flags = bus.BrEx_FlagsValue_o;
    uint8_t N = (flags >> 3) & 1;
    uint8_t Z = (flags >> 2) & 1;
    // uint8_t C = (flags >> 1) & 1;  // Not used by any branch
    uint8_t V = flags & 1;

    bool taken = false;
    switch (bus.BrEx_Op_o) {
    case 0x45: taken = true; break;                   // B
    case 0x46: taken = (Z == 1); break;               // BEQ
    case 0x47: taken = (Z == 0); break;               // BNE
    case 0x48: taken = (N != V); break;               // BLT
    case 0x49: taken = (Z == 0) && (N == V); break;   // BGT
    case 0x4A: taken = (V == 1); break;               // BUN
    case 0x4B: taken = (V == 0); break;               // BORD
    }

    bool predicted = bus.BrEx_PredTaken_o;
    bool mispredict = (taken != predicted);

    bus.BrExResult_Valid_o = true;
    bus.BrExResult_ROBTag_o = bus.BrEx_ROBTag_o;
    bus.BrExResult_Taken_o = taken;
    bus.BrExResult_Target_o = bus.BrEx_TargetPC_o;
    bus.BrExResult_Mispredict_o = mispredict;
}

void Branch_Executor::reset() {
    std::cout << "[Branch_Executor] reset()\n";
}