// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/decode/Instruction_Decoder.cpp
// ============================================================================

/**
 * @file Instruction_Decoder.cpp
 * @brief Component 6 implementation — pure combinational bit-slicer.
 *
 * Decodes the 64-bit instruction word fetched by I_Cache into its
 * constituent fields and places them on the bus for downstream consumers.
 *
 * Encoding layout (64-bit word):
 *   [63:56] Opcode         (8 bits)  -> Op_in_o
 *   [55:52] Rd / Dest      (4 bits)  -> Dest_in_o
 *   [51:48] Rn             (4 bits)  -> Rn_in_o
 *   [47:44] Rm             (4 bits)  -> Rm_in_o
 *   [43:12] Imm / Offset  (32 bits)  -> Imm_in_o
 *   [11:0]  Reserved
 */

#include "apps/cpu_tomasulo/simulation/pipeline/decode/Instruction_Decoder.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ----------------------------------------------------------------------------
 // evaluate
 // Extracts each instruction field from the 64-bit word and writes it to the
 // corresponding bus output. All subsequent decode stages read from these fields.
 // ----------------------------------------------------------------------------
void Instruction_Decoder::evaluate(TomasuloBus& bus) {
    uint64_t instr = bus.InstrF_o;

    bus.Op_in_o = static_cast<uint8_t> ((instr >> 56) & 0xFF);       // [63:56]
    bus.Dest_in_o = static_cast<uint8_t> ((instr >> 52) & 0xF);        // [55:52]
    bus.Rn_in_o = static_cast<uint8_t> ((instr >> 48) & 0xF);        // [51:48]
    bus.Rm_in_o = static_cast<uint8_t> ((instr >> 44) & 0xF);        // [47:44]
    bus.Imm_in_o = static_cast<uint32_t>((instr >> 12) & 0xFFFFFFFF); // [43:12]
}

void Instruction_Decoder::clockEdge(TomasuloBus& /*bus*/) {
    // Pure combinational — nothing to latch.
}

void Instruction_Decoder::reset() {
    std::cout << "[Decoder] reset()\n";
}