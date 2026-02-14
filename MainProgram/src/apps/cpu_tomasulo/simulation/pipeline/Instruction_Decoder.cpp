/**
 * @file Instruction_Decoder.cpp
 * @brief Component 6 implementation — pure combinational decode.
 *
 * Encoding layout (64-bit word):
 *   [63:56] Opcode (8 bits)
 *   [55:52] Rd     (4 bits)
 *   [51:48] Rn     (4 bits)
 *   [47:44] Rm     (4 bits)
 *   [43:12] Immediate / Offset (32 bits)
 *   [11:0]  Reserved
 */

#include "apps/cpu_tomasulo/simulation/pipeline/Instruction_Decoder.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void Instruction_Decoder::evaluate(TomasuloBus& bus) {
    uint64_t instr = bus.InstrF_o;

    bus.Op_in_o = static_cast<uint8_t>((instr >> 56) & 0xFF);
    bus.Dest_in_o = static_cast<uint8_t>((instr >> 52) & 0xF);
    bus.Rn_in_o = static_cast<uint8_t>((instr >> 48) & 0xF);
    bus.Rm_in_o = static_cast<uint8_t>((instr >> 44) & 0xF);
    bus.Imm_in_o = static_cast<uint32_t>((instr >> 12) & 0xFFFFFFFF);

    if (bus.InstReady_o) {
        std::cout << "[Decoder] Op=0x" << std::hex << (int)bus.Op_in_o
            << " Rd=" << (int)bus.Dest_in_o
            << " Rn=" << (int)bus.Rn_in_o
            << " Rm=" << (int)bus.Rm_in_o
            << " Imm=0x" << bus.Imm_in_o << std::dec << "\n";
    }
}

void Instruction_Decoder::clockEdge(TomasuloBus& /*bus*/) {
    // Pure combinational — nothing to latch.
}

void Instruction_Decoder::reset() {
    std::cout << "[Decoder] reset()\n";
}