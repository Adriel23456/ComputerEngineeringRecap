/**
 * @file Control_Unit.cpp
 * @brief Component 8 implementation — Issue stage control logic.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/Control_Unit.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Opcode Decode
 // ============================================================================

Control_Unit::DecodeResult Control_Unit::decode(uint8_t op) {
    DecodeResult d{};

    switch (op) {
        // Integer ALU (3-reg)
    case 0x00: case 0x01: case 0x02: case 0x03:
    case 0x06: case 0x07: case 0x08: case 0x09:
    case 0x0A: case 0x0B: case 0x0C: case 0x0D:
        d.isIntALU = true;
        d.modifiesFlags = true;
        break;

        // Integer ALU (imm)
    case 0x0E: case 0x0F: case 0x10: case 0x11:
    case 0x14: case 0x15: case 0x16: case 0x17:
    case 0x18: case 0x19: case 0x1A: case 0x1B:
        d.isIntALU = true; d.useImm = true;
        d.modifiesFlags = true;
        break;

        // INC / DEC
    case 0x1C: case 0x1D:
        d.isIntALU = true; d.useImm = true;
        d.modifiesFlags = true;
        break;

        // Integer MUL/DIV (3-reg)
    case 0x04: case 0x05:
        d.isIntMUL = true;
        d.modifiesFlags = true;
        break;

        // Integer MUL/DIV (imm)
    case 0x12: case 0x13:
        d.isIntMUL = true; d.useImm = true;
        d.modifiesFlags = true;
        break;

        // FP ALU (3-reg)
    case 0x1E: case 0x1F: case 0x22:
        d.isFPALU = true;
        break;

        // FP ALU (imm)
    case 0x23: case 0x24: case 0x27:
        d.isFPALU = true; d.useImm = true;
        break;

        // FP Unary (2-reg) — goes to FPALU
    case 0x29: case 0x2A: case 0x2B: case 0x2C:
    case 0x2D: case 0x2E: case 0x2F: case 0x30:
        d.isFPALU = true;
        break;

        // FP Unary (imm) — goes to FPALU
    case 0x53: case 0x54: case 0x55: case 0x56:
    case 0x57: case 0x58: case 0x59: case 0x5A:
        d.isFPALU = true; d.useImm = true;
        break;

        // FP MUL/DIV (3-reg)
    case 0x20: case 0x21:
        d.isFPMUL = true;
        break;

        // FP MUL/DIV (imm)
    case 0x25: case 0x26:
        d.isFPMUL = true; d.useImm = true;
        break;

        // FSQRT (2-reg) — goes to FPMUL
    case 0x28:
        d.isFPMUL = true;
        break;

        // FSQRTI (imm) — goes to FPMUL
    case 0x52:
        d.isFPMUL = true; d.useImm = true;
        break;

        // MOV, MVN (2-reg)
    case 0x31: case 0x32:
        d.isIntALU = true;
        d.modifiesFlags = false; // MOV/MVN don't modify flags
        break;

        // MOVI, MVNI (imm)
    case 0x33: case 0x34:
        d.isIntALU = true; d.useImm = true;
        d.modifiesFlags = false;
        break;

        // FMOVI, FMVNI (imm) — handled by IntALU (bit moves)
    case 0x35: case 0x36:
        d.isIntALU = true; d.useImm = true;
        d.modifiesFlags = false;
        break;

        // CMP, CMN, TST, TEQ (2-reg)
    case 0x37: case 0x38: case 0x39: case 0x3A:
        d.isIntALU = true; d.isCMPonly = true;
        d.modifiesFlags = true;
        break;

        // CMP, CMN, TST, TEQ (imm)
    case 0x3B: case 0x3C: case 0x3D: case 0x3E:
        d.isIntALU = true; d.isCMPonly = true; d.useImm = true;
        d.modifiesFlags = true;
        break;

        // FCMP, FCMN, FCMPS (2-reg)
    case 0x3F: case 0x40: case 0x41:
        d.isFPALU = true; d.isCMPonly = true;
        d.modifiesFlags = true;
        break;

        // FCMP, FCMN, FCMPS (imm)
    case 0x42: case 0x43: case 0x44:
        d.isFPALU = true; d.isCMPonly = true; d.useImm = true;
        d.modifiesFlags = true;
        break;

        // Branches
    case 0x45:
        d.isBranch = true; d.isUnconditional = true;
        break;
    case 0x46: case 0x47: case 0x48: case 0x49:
    case 0x4A: case 0x4B:
        d.isBranch = true;
        d.needsFlags = true;
        break;

        // Special
    case 0x4C: d.isSWI = true; break;
    case 0x4D: d.isNOP = true; break;

        // Memory Load
    case 0x4E: case 0x50:
        d.isLoad = true;
        break;

        // Memory Store
    case 0x4F: case 0x51:
        d.isStore = true;
        break;

    default:
        d.isNOP = true; // Unknown opcodes treated as NOP
        std::cout << "[Control_Unit] WARNING: Unknown opcode 0x"
            << std::hex << (int)op << std::dec << " treated as NOP\n";
        break;
    }

    // NeedsFlags for ADC, SBC, ADCI, SBCI
    if (op == 0x02 || op == 0x03 || op == 0x10 || op == 0x11)
        d.needsFlags = true;

    // WritesRd — everything except CMP-only, Store, Branch, NOP, SWI
    d.writesRd = !d.isCMPonly && !d.isStore && !d.isBranch && !d.isNOP && !d.isSWI;

    return d;
}

uint8_t Control_Unit::allocTypeFromDecode(const DecodeResult& d) {
    if (d.isCMPonly)  return 0b111;
    if (d.isIntALU)   return 0b000;
    if (d.isIntMUL)   return 0b001;
    if (d.isFPALU)    return 0b010;
    if (d.isFPMUL)    return 0b011;
    if (d.isLoad)     return 0b100;
    if (d.isStore)    return 0b101;
    if (d.isBranch)   return 0b110;
    return 0b000; // NOP/SWI default
}

// ============================================================================
// Evaluate (combinational)
// ============================================================================

void Control_Unit::evaluate(TomasuloBus& bus) {
    // ── Default all outputs ────────────────────────────────────
    bus.StallIF_o = false;
    bus.ROBAlloc_o = false;
    bus.AllocSB0_o = false;
    bus.AllocSB1_o = false;
    bus.AllocLB0_o = false;
    bus.AllocLB1_o = false;
    bus.AllocLB2_o = false;
    bus.AllocRSIntALU0_o = false;
    bus.AllocRSIntALU1_o = false;
    bus.AllocRSFPALU0_o = false;
    bus.AllocRSIntMUL0_o = false;
    bus.AllocRSFPMUL0_o = false;
    bus.AllocRSBranch0_o = false;
    bus.TagWrEn_o = false;
    bus.FlagsTagWrEn_o = false;
    bus.ModifiesFlags_o = false;
    bus.NeedsFlags_o = false;
    bus.UseImm_o = false;
    bus.IsCMPonly_o = false;
    bus.AllocPredicted_o = false;
    bus.IsFPALU_o = false;
    bus.IsFPMUL_o = false;
    bus.AllocType_o = 0;
    bus.AllocSourceStation_o = 0;

    // ── STEP 1: Check if instruction is available ──────────────
    if (!bus.InstReady_o) {
        bus.StallIF_o = true;
        return;
    }

    // ── STEP 2: Decode ─────────────────────────────────────────
    uint8_t op = bus.Op_in_o;
    DecodeResult d = decode(op);

    // Propagate decode results to bus
    bus.IsFPALU_o = d.isFPALU;
    bus.IsFPMUL_o = d.isFPMUL;
    bus.ModifiesFlags_o = d.modifiesFlags;
    bus.NeedsFlags_o = d.needsFlags;
    bus.UseImm_o = d.useImm;
    bus.IsCMPonly_o = d.isCMPonly;
    bus.AllocPredicted_o = false; // Always predict not-taken
    bus.AllocType_o = allocTypeFromDecode(d);

    // ── STEP 3: Check ROB availability ─────────────────────────
    if (bus.ROBFull_o) {
        bus.StallIF_o = true;
        return;
    }

    // ── STEP 4: NOP / SWI — allocate ROB only ──────────────────
    if (d.isNOP || d.isSWI) {
        bus.ROBAlloc_o = true;
        bus.AllocSourceStation_o = 0x0F;
        return;
    }

    // ── STEP 5: Resource allocation ────────────────────────────
    bool allocated = false;

    if (d.isIntALU) {
        if (!bus.RS_IntALU0_Busy_o) {
            bus.AllocRSIntALU0_o = true; bus.AllocSourceStation_o = 0b0101; allocated = true;
        }
        else if (!bus.RS_IntALU1_Busy_o) {
            bus.AllocRSIntALU1_o = true; bus.AllocSourceStation_o = 0b0110; allocated = true;
        }
    }
    else if (d.isIntMUL) {
        if (!bus.RS_IntMUL0_Busy_o) {
            bus.AllocRSIntMUL0_o = true; bus.AllocSourceStation_o = 0b1000; allocated = true;
        }
    }
    else if (d.isFPALU) {
        if (!bus.RS_FPALU0_Busy_o) {
            bus.AllocRSFPALU0_o = true; bus.AllocSourceStation_o = 0b0111; allocated = true;
        }
    }
    else if (d.isFPMUL) {
        if (!bus.RS_FPMUL0_Busy_o) {
            bus.AllocRSFPMUL0_o = true; bus.AllocSourceStation_o = 0b1001; allocated = true;
        }
    }
    else if (d.isLoad) {
        if (!bus.LB0_Busy_o) {
            bus.AllocLB0_o = true; bus.AllocSourceStation_o = 0b0010; allocated = true;
        }
        else if (!bus.LB1_Busy_o) {
            bus.AllocLB1_o = true; bus.AllocSourceStation_o = 0b0011; allocated = true;
        }
        else if (!bus.LB2_Busy_o) {
            bus.AllocLB2_o = true; bus.AllocSourceStation_o = 0b0100; allocated = true;
        }
    }
    else if (d.isStore) {
        if (!bus.SB0_Busy_o) {
            bus.AllocSB0_o = true; bus.AllocSourceStation_o = 0b0000; allocated = true;
        }
        else if (!bus.SB1_Busy_o) {
            bus.AllocSB1_o = true; bus.AllocSourceStation_o = 0b0001; allocated = true;
        }
    }
    else if (d.isBranch) {
        if (!bus.RS_Branch0_Busy_o) {
            bus.AllocRSBranch0_o = true; bus.AllocSourceStation_o = 0b1010; allocated = true;
        }
    }

    // ── STEP 6: Stall if no resource available ─────────────────
    if (!allocated) {
        bus.StallIF_o = true;
        bus.ROBAlloc_o = false;
        return;
    }

    // ── STEP 7: Allocation succeeded ───────────────────────────
    bus.ROBAlloc_o = true;

    // Tag write enable — only if instruction writes to a destination register
    bus.TagWrEn_o = d.writesRd;
    bus.FlagsTagWrEn_o = d.modifiesFlags;
}

void Control_Unit::clockEdge(TomasuloBus& /*bus*/) {
    // Pure combinational — nothing to latch.
}

void Control_Unit::reset() {
    std::cout << "[Control_Unit] reset()\n";
}