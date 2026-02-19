// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/execution/IntALU.cpp
// ============================================================================

/**
 * @file IntALU.cpp
 * @brief Component 27 implementation -- Integer ALU.
 *
 * Single-cycle combinational: ADD, SUB, ADC, SBC, AND, ORR, EOR, BIC,
 * LSL, LSR, ASR, ROR, INC, DEC, MOV, MVN, CMP, CMN, TST, TEQ.
 * Stalls if CDB is busy (IntALU_CDBStall_o == 1).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/execution/IntALU.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>
#include <cstdint>

 // ============================================================================
 // Helpers
 // ============================================================================

 /** @brief Barrel-rotate a 64-bit value right by @p amount bits (wraps at 64). */
static uint64_t rotate_right(uint64_t val, unsigned amount) {
    amount %= 64;
    if (amount == 0) return val;
    return (val >> amount) | (val << (64 - amount));
}

// ============================================================================
// Evaluate (combinational)
// ============================================================================

void IntALU::evaluate(TomasuloBus& bus) {
    // The ALU is free whenever the CDB is not stalling its output.
    bus.IntALU_Free_o = !bus.IntALU_CDBStall_o;
    bus.IntALU_CDBReq_o = false;
    bus.IntALU_CDBFlagsValid_o = false;

    // Nothing to do if the reservation station has not dispatched to us.
    if (!bus.IntALU_Start_o) return;

    uint8_t  op = bus.IntALU_Op_o;
    uint64_t A = bus.IntALU_A_o;
    uint64_t B = bus.IntALU_B_o;
    uint64_t cin = 0;   ///< Carry-in (from architectural flags, bit[1] = C).

    // ADC / SBC need the current carry flag as an operand.
    if (bus.IntALU_NeedsFlags_o) {
        cin = (bus.IntALU_FlagsIn_o >> 1) & 1;  // Carry is bit[1]
    }

    uint64_t result = 0;
    uint64_t temp = 0;          ///< Intermediate result used only for flag computation (CMP/CMN/TST/TEQ).
    bool useTempForFlags = false; ///< When true, flags are derived from @p temp instead of @p result.
    bool carry_out = false;
    bool overflow = false;

    switch (op) {

        // ── Arithmetic ───────────────────────────────────────────────

        // ADD, ADDI
    case 0x00: case 0x0E: {
        result = A + B;
        carry_out = (result < A);                            // Unsigned overflow -> carry
        overflow = (~(A ^ B) & (A ^ result)) >> 63;        // Signed overflow
        break;
    }

             // SUB, SUBI
    case 0x01: case 0x0F: {
        result = A - B;
        carry_out = (A >= B);                                // Borrow did NOT occur -> carry
        overflow = ((A ^ B) & (A ^ result)) >> 63;
        break;
    }

             // ADC, ADCI  (add with carry-in)
    case 0x02: case 0x10: {
        uint64_t sum = A + B;
        result = sum + cin;
        carry_out = (sum < A) || (result < sum);             // Either addition wrapped
        overflow = (~(A ^ B) & (A ^ result)) >> 63;
        break;
    }

             // SBC, SBCI  (subtract with carry-in)
    case 0x03: case 0x11: {
        uint64_t sub = B + (cin ? 0 : 1);                   // cin=0 means borrow
        result = A - sub;
        carry_out = (A >= sub);
        overflow = ((A ^ B) & (A ^ result)) >> 63;
        break;
    }

             // INC  (increment, no second operand)
    case 0x1C: {
        result = A + 1;
        carry_out = (result == 0);                           // Wrapped around
        overflow = (A == 0x7FFFFFFFFFFFFFFFULL);            // Max positive signed
        break;
    }

             // DEC  (decrement, no second operand)
    case 0x1D: {
        result = A - 1;
        carry_out = (A != 0);                                // No borrow unless A was already 0
        overflow = (A == 0x8000000000000000ULL);            // Min negative signed
        break;
    }

             // ── Bitwise ──────────────────────────────────────────────────

             // AND, ANDI
    case 0x06: case 0x14: result = A & B;  break;

        // ORR, ORRI
    case 0x07: case 0x15: result = A | B;  break;

        // EOR, EORI
    case 0x08: case 0x16: result = A ^ B;  break;

        // BIC, BICI  (bit-clear: A AND NOT B)
    case 0x09: case 0x17: result = A & (~B); break;

        // ── Shift / Rotate ───────────────────────────────────────────

        // LSL, LSLI
    case 0x0A: case 0x18: {
        unsigned sh = B % 64;
        if (sh > 0) carry_out = (A >> (64 - sh)) & 1;  // Last bit shifted out
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

             // ASR, ASRI  (arithmetic right shift — sign-extends)
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

             // ── Move ─────────────────────────────────────────────────────

             // MOV, MOVI, FMOVI  (pass B through unchanged)
    case 0x31: case 0x33: case 0x35: result = B; break;

        // MVN, MVNI  (bitwise NOT of B)
    case 0x32: case 0x34: result = ~B; break;

        // FMVNI  (FP negate: flip IEEE-754 sign bit)
    case 0x36: result = B ^ 0x8000000000000000ULL; break;

        // ── Compare / Test (result discarded, only flags produced) ───

        // CMP, CMPI  (A - B, flags only)
    case 0x37: case 0x3B: {
        temp = A - B;
        carry_out = (A >= B);
        overflow = ((A ^ B) & (A ^ temp)) >> 63;
        useTempForFlags = true;
        result = 0;
        break;
    }

             // CMN, CMNI  (A + B, flags only)
    case 0x38: case 0x3C: {
        temp = A + B;
        carry_out = (temp < A);
        overflow = (~(A ^ B) & (A ^ temp)) >> 63;
        useTempForFlags = true;
        result = 0;
        break;
    }

             // TST, TSTI  (A AND B, flags only)
    case 0x39: case 0x3D: {
        temp = A & B;
        useTempForFlags = true;
        result = 0;
        break;
    }

             // TEQ, TEQI  (A EOR B, flags only)
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

    // ── Flags computation ─────────────────────────────────────────
    // Only written to bus when the instruction is flagged to modify NZCV.
    if (bus.IntALU_ModFlags_o) {
        uint64_t flagSrc = useTempForFlags ? temp : result;

        uint8_t N = (flagSrc >> 63) & 1;  // Negative
        uint8_t Z = (flagSrc == 0);        // Zero
        uint8_t C = carry_out;             // Carry / borrow
        uint8_t V = overflow;              // Signed overflow

        uint8_t flags = (N << 3) | (Z << 2) | (C << 1) | V;

        bus.IntALU_CDBFlagsValid_o = true;
        bus.IntALU_CDBFlags_o = flags;
    }

    // ── CDB output ────────────────────────────────────────────────
    bus.IntALU_CDBReq_o = true;
    bus.IntALU_CDBROBTag_o = bus.IntALU_DestROBTag_o;
    bus.IntALU_CDBValue_o = result;
    bus.IntALU_CDBException_o = 0x0;
}

// ============================================================================
// Reset
// ============================================================================

void IntALU::reset() {
    // Combinational unit — no internal state to clear.
    std::cout << "[IntALU] reset()\n";
}