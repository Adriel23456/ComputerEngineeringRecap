/**
 * @file IntALU.cpp
 * @brief Component 27 implementation -- Integer ALU.
 *
 * Single-cycle combinational: ADD, SUB, ADC, SBC, AND, ORR, EOR, BIC,
 * LSL, LSR, ASR, ROR, INC, DEC, MOV, MVN, CMP, CMN, TST, TEQ.
 * Stalls if CDB is busy (IntALU_CDBStall_o == 1).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/IntALU.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>
#include <cstdint>

static uint64_t rotate_right(uint64_t val, unsigned amount) {
    amount %= 64;
    if (amount == 0) return val;
    return (val >> amount) | (val << (64 - amount));
}

void IntALU::evaluate(TomasuloBus& bus) {
    bus.IntALU_Free_o = !bus.IntALU_CDBStall_o;
    bus.IntALU_CDBReq_o = false;
    bus.IntALU_CDBFlagsValid_o = false;

    if (!bus.IntALU_Start_o) return;

    uint8_t  op = bus.IntALU_Op_o;
    uint64_t A = bus.IntALU_A_o;
    uint64_t B = bus.IntALU_B_o;
    uint64_t cin = 0;

    if (bus.IntALU_NeedsFlags_o) {
        cin = (bus.IntALU_FlagsIn_o >> 1) & 1;  // Carry is bit[1]
    }

    uint64_t result = 0;
    uint64_t temp = 0;
    bool useTempForFlags = false;
    bool carry_out = false;
    bool overflow = false;

    switch (op) {

        // ADD, ADDI
    case 0x00: case 0x0E: {
        result = A + B;
        carry_out = (result < A);
        overflow = (~(A ^ B) & (A ^ result)) >> 63;
        break;
    }

             // SUB, SUBI
    case 0x01: case 0x0F: {
        result = A - B;
        carry_out = (A >= B);
        overflow = ((A ^ B) & (A ^ result)) >> 63;
        break;
    }

             // ADC, ADCI
    case 0x02: case 0x10: {
        uint64_t sum = A + B;
        result = sum + cin;
        carry_out = (sum < A) || (result < sum);
        overflow = (~(A ^ B) & (A ^ result)) >> 63;
        break;
    }

             // SBC, SBCI
    case 0x03: case 0x11: {
        uint64_t sub = B + (cin ? 0 : 1);
        result = A - sub;
        carry_out = (A >= sub);
        overflow = ((A ^ B) & (A ^ result)) >> 63;
        break;
    }

             // INC
    case 0x1C: {
        result = A + 1;
        carry_out = (result == 0);
        overflow = (A == 0x7FFFFFFFFFFFFFFFULL);
        break;
    }

             // DEC
    case 0x1D: {
        result = A - 1;
        carry_out = (A != 0);
        overflow = (A == 0x8000000000000000ULL);
        break;
    }

             // AND, ANDI
    case 0x06: case 0x14: result = A & B; break;

        // ORR, ORRI
    case 0x07: case 0x15: result = A | B; break;

        // EOR, EORI
    case 0x08: case 0x16: result = A ^ B; break;

        // BIC, BICI
    case 0x09: case 0x17: result = A & (~B); break;

        // LSL, LSLI
    case 0x0A: case 0x18: {
        unsigned sh = B % 64;
        if (sh > 0) carry_out = (A >> (64 - sh)) & 1;
        result = A << sh;
        break;
    }

             // LSR, LSRI
    case 0x0B: case 0x19: {
        unsigned sh = B % 64;
        if (sh > 0) carry_out = (A >> (sh - 1)) & 1;
        result = A >> sh;
        break;
    }

             // ASR, ASRI
    case 0x0C: case 0x1A: {
        unsigned sh = B % 64;
        if (sh > 0) carry_out = (A >> (sh - 1)) & 1;
        result = (uint64_t)((int64_t)A >> sh);
        break;
    }

             // ROR, RORI
    case 0x0D: case 0x1B: {
        unsigned sh = B % 64;
        if (sh > 0) carry_out = (A >> (sh - 1)) & 1;
        result = rotate_right(A, sh);
        break;
    }

             // MOV, MOVI, FMOVI
    case 0x31: case 0x33: case 0x35: result = B; break;

        // MVN, MVNI, FMVNI
    case 0x32: case 0x34: result = ~B; break;                      // MVN, MVNI: bitwise NOT
    case 0x36: result = B ^ 0x8000000000000000ULL; break;           // FMVNI: FP negate (flip sign bit)

        // CMP, CMPI
    case 0x37: case 0x3B: {
        temp = A - B;
        carry_out = (A >= B);
        overflow = ((A ^ B) & (A ^ temp)) >> 63;
        useTempForFlags = true;
        result = 0;
        break;
    }

             // CMN, CMNI
    case 0x38: case 0x3C: {
        temp = A + B;
        carry_out = (temp < A);
        overflow = (~(A ^ B) & (A ^ temp)) >> 63;
        useTempForFlags = true;
        result = 0;
        break;
    }

             // TST, TSTI
    case 0x39: case 0x3D: {
        temp = A & B;
        useTempForFlags = true;
        result = 0;
        break;
    }

             // TEQ, TEQI
    case 0x3A: case 0x3E: {
        temp = A ^ B;
        useTempForFlags = true;
        result = 0;
        break;
    }

    default:
        result = 0;
        break;
    }

    // ---- Flags ----
    if (bus.IntALU_ModFlags_o) {
        uint64_t flagSrc = useTempForFlags ? temp : result;

        uint8_t N = (flagSrc >> 63) & 1;
        uint8_t Z = (flagSrc == 0);
        uint8_t C = carry_out;
        uint8_t V = overflow;

        uint8_t flags = (N << 3) | (Z << 2) | (C << 1) | V;

        bus.IntALU_CDBFlagsValid_o = true;
        bus.IntALU_CDBFlags_o = flags;
    }

    bus.IntALU_CDBReq_o = true;
    bus.IntALU_CDBROBTag_o = bus.IntALU_DestROBTag_o;
    bus.IntALU_CDBValue_o = result;
    bus.IntALU_CDBException_o = 0x0;

    std::cout << "[IntALU] op=0x" << std::hex << (int)op
        << " A=0x" << A << " B=0x" << B
        << " -> result=0x" << result << std::dec
        << " ROB#" << (int)bus.IntALU_DestROBTag_o << "\n";
}

void IntALU::reset() {
    std::cout << "[IntALU] reset()\n";
}