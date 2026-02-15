/**
 * @file Extends.cpp
 * @brief Component 7 implementation — Immediate extension.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/Extends.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

uint64_t Extends::signExtend32(uint32_t imm) {
    if (imm & 0x80000000U)
        return 0xFFFFFFFF00000000ULL | static_cast<uint64_t>(imm);
    else
        return static_cast<uint64_t>(imm);
}

uint64_t Extends::float32ToDouble64Bits(uint32_t imm) {
    // IEEE 754 single -> double bit-level conversion
    uint64_t sign = (imm >> 31) & 1;
    uint32_t exponent = (imm >> 23) & 0xFF;
    uint64_t mantissa = imm & 0x7FFFFF;

    uint64_t doubleExp;
    uint64_t doubleMan = mantissa << 29; // 23-bit -> 52-bit mantissa (shift left 29)

    if (exponent == 0x00) {
        // Zero or denormalized
        doubleExp = 0x000;
    }
    else if (exponent == 0xFF) {
        // Inf or NaN
        doubleExp = 0x7FF;
    }
    else {
        // Normal: rebias exponent (127 -> 1023)
        doubleExp = static_cast<uint64_t>(exponent) - 127 + 1023;
    }

    return (sign << 63) | (doubleExp << 52) | doubleMan;
}

void Extends::evaluate(TomasuloBus& bus) {
    bool isFP = bus.IsFPALU_o || bus.IsFPMUL_o;

    // FMOVI(0x35) and FMVNI(0x36) route through IntALU but carry FP immediates
    uint8_t op = bus.Op_in_o;
    if (op == 0x35 || op == 0x36) {
        isFP = true;
    }

    if (isFP) {
        bus.ImmExt_o = float32ToDouble64Bits(bus.Imm_in_o);
    }
    else {
        bus.ImmExt_o = signExtend32(bus.Imm_in_o);
    }
}

void Extends::clockEdge(TomasuloBus& /*bus*/) {
    // Pure combinational
}

void Extends::reset() {
    std::cout << "[Extends] reset()\n";
}