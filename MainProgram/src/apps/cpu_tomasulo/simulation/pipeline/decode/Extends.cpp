// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/decode/Extends.cpp
// ============================================================================

/**
 * @file Extends.cpp
 * @brief Component 7 implementation — Immediate extension (32->64 bit).
 *
 * Integer immediates: sign-extended from bit 31 into bits [63:32].
 * FP immediates: IEEE 754 single-to-double bit-level conversion.
 *
 * Extends evaluates before Control_Unit in topological order, so it cannot
 * rely on bus.IsFPALU_o / bus.IsFPMUL_o being valid yet. The FP opcode
 * classification is therefore derived directly from bus.Op_in_o.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/decode/Extends.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ----------------------------------------------------------------------------
 // signExtend32
 // Replicates bit 31 of @p imm into bits [63:32] of the 64-bit result.
 // ----------------------------------------------------------------------------
uint64_t Extends::signExtend32(uint32_t imm) {
    if (imm & 0x80000000U)
        return 0xFFFFFFFF00000000ULL | static_cast<uint64_t>(imm);  // Negative
    else
        return static_cast<uint64_t>(imm);                           // Positive
}

// ----------------------------------------------------------------------------
// float32ToDouble64Bits
// Converts an IEEE 754 single-precision bit pattern to double-precision bits
// without going through floating-point arithmetic:
//   sign     : bit 63  <- bit 31
//   exponent : bits [62:52] — rebias from 127 to 1023 (or preserve ±Inf/NaN)
//   mantissa : bits [51:0]  — left-shift 23-bit mantissa by 29 positions
// ----------------------------------------------------------------------------
uint64_t Extends::float32ToDouble64Bits(uint32_t imm) {
    uint64_t sign = (imm >> 31) & 1;
    uint32_t exponent = (imm >> 23) & 0xFF;
    uint64_t mantissa = imm & 0x7FFFFF;

    uint64_t doubleExp;
    uint64_t doubleMan = mantissa << 29;  // 23-bit mantissa -> 52-bit position

    if (exponent == 0x00) {
        doubleExp = 0x000;          // Zero or denormal
    }
    else if (exponent == 0xFF) {
        doubleExp = 0x7FF;          // ±Inf or NaN
    }
    else {
        doubleExp = static_cast<uint64_t>(exponent) - 127 + 1023;  // Rebias
    }

    return (sign << 63) | (doubleExp << 52) | doubleMan;
}

// ----------------------------------------------------------------------------
// evaluate
// Writes ImmExt_o with either sign-extended or float32->double64 bits,
// depending on whether the current opcode is an FP instruction.
// The FP classification must be derived locally from the opcode rather than
// from bus.IsFPALU_o / bus.IsFPMUL_o (which Control_Unit sets later).
// ----------------------------------------------------------------------------
void Extends::evaluate(TomasuloBus& bus) {
    uint8_t op = bus.Op_in_o;

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
        // FMOVI, FMVNI (bit-level FP move immediates)
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
    // Pure combinational — no state to latch.
}

void Extends::reset() {
    std::cout << "[Extends] reset()\n";
}