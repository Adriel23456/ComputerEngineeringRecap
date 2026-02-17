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
    uint8_t op = bus.Op_in_o;

    // Determine FP status directly from opcode — do NOT use
    // bus.IsFPALU_o / bus.IsFPMUL_o since Control_Unit evaluates AFTER us.
    bool isFP = false;
    switch (op) {
        // FP ALU (3-reg)
    case 0x1E: case 0x1F: case 0x22:
        // FP ALU (imm)
    case 0x23: case 0x24: case 0x27:
        // FP Unary (2-reg)
    case 0x29: case 0x2A: case 0x2B: case 0x2C:
    case 0x2D: case 0x2E: case 0x2F: case 0x30:
        // FP Unary (imm)
    case 0x53: case 0x54: case 0x55: case 0x56:
    case 0x57: case 0x58: case 0x59: case 0x5A:
        // FP MUL/DIV (3-reg)
    case 0x20: case 0x21:
        // FP MUL/DIV (imm)
    case 0x25: case 0x26:
        // FSQRT / FSQRTI
    case 0x28: case 0x52:
        // FCMP, FCMN, FCMPS (2-reg)
    case 0x3F: case 0x40: case 0x41:
        // FCMP, FCMN, FCMPS (imm)
    case 0x42: case 0x43: case 0x44:
        // FMOVI, FMVNI
    case 0x35: case 0x36:
        isFP = true;
        break;
    default:
        isFP = false;
        break;
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