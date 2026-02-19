// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Arbiter.cpp
// ============================================================================

/**
 * @file Branch_Arbiter.cpp
 * @brief Implementation of Branch_Arbiter (Component 34).
 *
 * evaluate():
 *   If RSBr0_Req_o is asserted, grants the request and forwards all
 *   RS_Branch0 outputs (Op, TargetPC, FlagsValue, PredTaken, ROBTag)
 *   to the BrEx_* inputs consumed by Branch_Executor.
 *
 *   RSBr0_Grant_o is asserted so RS_Branch0 knows it was dispatched
 *   and can suppress duplicate requests on the next cycle.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/branch/Branch_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Evaluate
 // ============================================================================

void Branch_Arbiter::evaluate(TomasuloBus& bus) {
    bus.RSBr0_Grant_o = false;
    bus.BrEx_Start_o = false;

    if (bus.RSBr0_Req_o) {
        bus.RSBr0_Grant_o = true;
        bus.BrEx_Start_o = true;
        bus.BrEx_Op_o = bus.RSBr0_Op_o;
        bus.BrEx_TargetPC_o = bus.RSBr0_TargetPC_o;
        bus.BrEx_FlagsValue_o = bus.RSBr0_FlagsValue_o;
        bus.BrEx_PredTaken_o = bus.RSBr0_PredTaken_o;
        bus.BrEx_ROBTag_o = bus.RSBr0_ROBTag_o;
    }
}

// ============================================================================
// Reset
// ============================================================================

void Branch_Arbiter::reset() {
    std::cout << "[Branch_Arbiter] reset()\n";
}