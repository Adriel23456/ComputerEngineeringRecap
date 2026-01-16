// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/processor/pe/ControlUnit.cpp
// ============================================================================

/**
 * @file ControlUnit.cpp
 * @brief Implementation of ControlUnit.
 */

#include "apps/cpu_tlp_shared_cache/simulation/processor/pe/ControlUnit.h"

namespace cpu_tlp {

    ControlSignals ControlUnit::decode(uint8_t opcode) const {
        ControlSignals sig{};

        switch (opcode) {
            // 0x00-0x0D: Integer operations (register)
        case 0x00: case 0x01: case 0x02: case 0x03:
        case 0x04: case 0x05: case 0x06: case 0x07:
        case 0x08: case 0x09: case 0x0A: case 0x0B:
        case 0x0C: case 0x0D:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = opcode;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

            // 0x0E-0x1B: Integer operations with immediate
        case 0x0E: case 0x0F: case 0x10: case 0x11:
        case 0x12: case 0x13: case 0x14: case 0x15:
        case 0x16: case 0x17: case 0x18: case 0x19:
        case 0x1A: case 0x1B:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = opcode - 0x0E;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

            // 0x1C: INC Rd
        case 0x1C:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x00;  // ADD
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

            // 0x1D: DEC Rd
        case 0x1D:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x01;  // SUB
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

            // 0x1E-0x22: Floating-point operations (register)
        case 0x1E:  // FADD
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x0E;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x1F:  // FSUB
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x0F;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x20:  // FMUL
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x10;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x21:  // FDIV
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x11;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x22:  // FCOPYSIGN
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x12;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

            // 0x23-0x27: Floating-point with float immediate
        case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = opcode - 0x23 + 0x0E;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 1;
            break;

            // 0x28-0x30: More floating-point operations
        case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C:
        case 0x2D: case 0x2E: case 0x2F: case 0x30:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = opcode - 0x28 + 0x13;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

            // 0x31-0x32: MOV/MVN (register)
        case 0x31:  // MOV
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x1C;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x32:  // MVN
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x1D;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

            // 0x33: MOVI (immediate integer)
        case 0x33:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x1C;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

            // 0x34: MVNI (immediate integer)
        case 0x34:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x1D;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

            // 0x35: FMOVI (immediate float)
        case 0x35:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x1E;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 1;
            break;

            // 0x36: FMVNI (immediate float)
        case 0x36:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x1F;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 1;
            break;

            // 0x37-0x3E: Comparisons (no register write)
        case 0x37:  // CMP
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x01;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x38:  // CMN
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x00;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x39:  // TST
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x06;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x3A:  // TEQ
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x08;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x3B:  // CMPI
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x01;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

        case 0x3C:  // CMNI
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x00;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

        case 0x3D:  // TSTI
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x06;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

        case 0x3E:  // TEQI
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x08;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

            // 0x3F-0x44: Floating-point comparisons
        case 0x3F:  // FCMP
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x0F;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x40:  // FCMN
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x0E;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x41:  // FCMPS
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x20;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

        case 0x42:  // FCMPI
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x0F;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 1;
            break;

        case 0x43:  // FCMNI
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x0E;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 1;
            break;

        case 0x44:  // FCMPSI
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = 0x20;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 1;
            break;

            // 0x45-0x4B: Branches
        case 0x45: case 0x46: case 0x47: case 0x48:
        case 0x49: case 0x4A: case 0x4B:
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 1;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0;
            sig.BranchOp_D = opcode - 0x45 + 1;
            sig.BranchE = 1; sig.ImmOp = 1; sig.DataType = 0;
            break;

            // 0x4C: CRASH (SWI)
        case 0x4C:
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x21;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

            // 0x4D: NOP
        case 0x4D:
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x22;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;

            // 0x4E-0x51: Memory operations
        case 0x4E:  // LDR
            sig.RegWrite_D = 1; sig.MemOp_D = 1; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 1; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

        case 0x4F:  // STR
            sig.RegWrite_D = 0; sig.MemOp_D = 1; sig.C_WE_D = 1;
            sig.C_REQUEST_D = 1; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

        case 0x50:  // LDRI (ISB)
            sig.RegWrite_D = 1; sig.MemOp_D = 1; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 1; sig.C_ISB_D = 1; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

        case 0x51:  // STRI (ISB)
            sig.RegWrite_D = 0; sig.MemOp_D = 1; sig.C_WE_D = 1;
            sig.C_REQUEST_D = 1; sig.C_ISB_D = 1; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 0;
            break;

            // 0x52-0x5A: More floating-point with immediate
        case 0x52: case 0x53: case 0x54: case 0x55:
        case 0x56: case 0x57: case 0x58: case 0x59: case 0x5A:
            sig.RegWrite_D = 1; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 1; sig.ALUControl_D = opcode - 0x52 + 0x13;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 1; sig.DataType = 1;
            break;

            // Default: NOP
        default:
            sig.RegWrite_D = 0; sig.MemOp_D = 0; sig.C_WE_D = 0;
            sig.C_REQUEST_D = 0; sig.C_ISB_D = 0; sig.PCSrc_D = 0;
            sig.FlagsUpd_D = 0; sig.ALUControl_D = 0x22;
            sig.BranchOp_D = 0; sig.BranchE = 0; sig.ImmOp = 0; sig.DataType = 0;
            break;
        }

        return sig;
    }

} // namespace cpu_tlp