// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/execution/FPALU_Arbiter.cpp
// ============================================================================

/**
 * @file FPALU_Arbiter.cpp
 * @brief Component 28 implementation -- FP ALU Arbiter.
 *
 * Single-RS pass-through: there is only one FP ALU reservation station
 * (RS_FPALU0), so no multi-way arbitration logic is needed. The arbiter
 * simply forwards RS_FPALU0's request to the FPALU execution unit when
 * FPALU_Free_o is asserted (stage 1 of the FPALU pipeline is empty).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/execution/FPALU_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ----------------------------------------------------------------------------
 // evaluate
 // Grants RS_FPALU0 and populates the FPALU dispatch signals when the unit
 // is free and the RS has a ready (fully operand-resolved) operation.
 // ----------------------------------------------------------------------------
void FPALU_Arbiter::evaluate(TomasuloBus& bus) {
    bus.RSFPAU0_Grant_o = false;
    bus.FPALU_Start_o = false;

    if (bus.FPALU_Free_o && bus.RSFPAU0_Req_o) {
        bus.RSFPAU0_Grant_o = true;
        bus.FPALU_Start_o = true;
        bus.FPALU_Op_o = bus.RSFPAU0_Op_o;
        bus.FPALU_A_o = bus.RSFPAU0_Vj_o;
        bus.FPALU_B_o = bus.RSFPAU0_Vk_o;
        bus.FPALU_DestROBTag_o = bus.RSFPAU0_DestROBTag_o;
        bus.FPALU_ModFlags_o = bus.RSFPAU0_ModFlags_o;
        bus.FPALU_NeedsFlags_o = bus.RSFPAU0_NeedsFlags_o;
        bus.FPALU_FlagsIn_o = bus.RSFPAU0_FlagsValue_o;
    }
}

void FPALU_Arbiter::reset() {
    std::cout << "[FPALU_Arbiter] reset()\n";
}