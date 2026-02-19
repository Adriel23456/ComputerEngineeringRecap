// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/decode/Instruction_Decoder.h
// ============================================================================

#pragma once

/**
 * @file Instruction_Decoder.h
 * @brief Component 6: Instruction Decoder (combinational bit-slicer).
 *
 * Splits the 64-bit instruction word fetched by I_Cache into its
 * constituent fields and writes them onto the bus.
 *
 * Encoding layout:
 *   [63:56] Opcode  (8 bits)  -> Op_in_o
 *   [55:52] Rd      (4 bits)  -> Dest_in_o
 *   [51:48] Rn      (4 bits)  -> Rn_in_o
 *   [47:44] Rm      (4 bits)  -> Rm_in_o
 *   [43:12] Imm/Off (32 bits) -> Imm_in_o
 *   [11:0]  Reserved
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class Instruction_Decoder : public ITomasuloComponent {
public:
    const char* name() const override { return "Instruction_Decoder"; }
    /** @brief Slices InstrF_o into Op/Dest/Rn/Rm/Imm fields on the bus. */
    void evaluate(TomasuloBus& bus) override;
    /** @brief Pure combinational — no state to latch. */
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;
};