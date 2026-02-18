#pragma once

#include <string>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <iomanip>

class TomasuloDisassembler {
public:
    static std::string disassemble(uint64_t instruction) {
        if (instruction == 0) return "NOP";

        uint8_t opcode = (instruction >> 56) & 0xFF;
        uint8_t rd = (instruction >> 52) & 0xF;
        uint8_t rn = (instruction >> 48) & 0xF;
        uint8_t rm = (instruction >> 44) & 0xF;
        uint32_t imm = (instruction >> 12) & 0xFFFFFFFF;

        const char* mn = mnemonic(opcode);
        std::ostringstream o;
        o << mn;

        switch (opcode) {
            // Rd, Rn, Rm
        case 0x00: case 0x01: case 0x02: case 0x03:
        case 0x04: case 0x05: case 0x06: case 0x07:
        case 0x08: case 0x09: case 0x0A: case 0x0B:
        case 0x0C: case 0x0D:
        case 0x1E: case 0x1F: case 0x20: case 0x21: case 0x22:
            o << " " << reg(rd) << ", " << reg(rn) << ", " << reg(rm);
            break;
            // Rd, Rn, #imm (int)
        case 0x0E: case 0x0F: case 0x10: case 0x11:
        case 0x12: case 0x13: case 0x14: case 0x15:
        case 0x16: case 0x17: case 0x18: case 0x19:
        case 0x1A: case 0x1B:
        case 0x3B: case 0x3C: case 0x3D: case 0x3E:
            o << " " << reg(rd) << ", " << reg(rn) << ", " << fmtImm(imm, false);
            break;
            // Rd, Rn, #imm (float)
        case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
        case 0x42: case 0x43: case 0x44:
            o << " " << reg(rd) << ", " << reg(rn) << ", " << fmtImm(imm, true);
            break;
            // Rd, Rm (unary)
        case 0x28: case 0x29: case 0x2A: case 0x2B:
        case 0x2C: case 0x2D: case 0x2E: case 0x2F: case 0x30:
        case 0x31: case 0x32:
            o << " " << reg(rd) << ", " << reg(rm);
            break;
            // Rd, #imm (MOVI/MVNI int)
        case 0x33: case 0x34:
            o << " " << reg(rd) << ", " << fmtImm(imm, false);
            break;
            // Rd only (INC/DEC)
        case 0x1C: case 0x1D:
            o << " " << reg(rd);
            break;
            // Rd, #imm (float mov/unary)
        case 0x35: case 0x36:
        case 0x52: case 0x53: case 0x54: case 0x55:
        case 0x56: case 0x57: case 0x58: case 0x59: case 0x5A:
            o << " " << reg(rd) << ", " << fmtImm(imm, true);
            break;
            // Rn, Rm (comparisons)
        case 0x37: case 0x38: case 0x39: case 0x3A:
        case 0x3F: case 0x40: case 0x41:
            o << " " << reg(rn) << ", " << reg(rm);
            break;
            // Branches
        case 0x45: case 0x46: case 0x47: case 0x48:
        case 0x49: case 0x4A: case 0x4B:
            o << " +" << (int32_t)imm;
            break;
            // LDR/LDRB
        case 0x4E: case 0x50: {
            int32_t off = (int32_t)imm;
            o << " " << reg(rd) << ", [" << reg(rn);
            if (off) o << ", #" << off;
            o << "]";
            break;
        }
                 // STR/STRB
        case 0x4F: case 0x51: {
            int32_t off = (int32_t)imm;
            o << " " << reg(rm) << ", [" << reg(rn);
            if (off) o << ", #" << off;
            o << "]";
            break;
        }
                 // SWI/NOP
        case 0x4C: case 0x4D:
            break;
        default:
            o << " ???";
            break;
        }
        return o.str();
    }

private:
    static const char* mnemonic(uint8_t op) {
        static const char* table[] = {
            "ADD","SUB","ADC","SBC","MUL","DIV","AND","ORR",
            "EOR","BIC","LSL","LSR","ASR","ROR","ADDI","SUBI",
            "ADCI","SBCI","MULI","DIVI","ANDI","ORRI","EORI","BICI",
            "LSLI","LSRI","ASRI","RORI","INC","DEC","FADD","FSUB",
            "FMUL","FDIV","FCOPYSIGN","FADDI","FSUBI","FMULI","FDIVI","FCOPYSIGNI",
            "FSQRT","FNEG","FABS","CDTI","CDTD","RTNR","RTZ","RTP",
            "RTNE","MOV","MVN","MOVI","MVNI","FMOVI","FMVNI","CMP",
            "CMN","TST","TEQ","CMPI","CMNI","TSTI","TEQI","FCMP",
            "FCMN","FCMPS","FCMPI","FCMNI","FCMPSI","B","BEQ","BNE",
            "BLT","BGT","BUN","BORD","SWI","NOP","LDR","STR",
            "LDRB","STRB","FSQRTI","FNEGI","FABSI","CDTII","CDTDI","RTNRI",
            "RTZI","RTPI","RTNEI"
        };
        if (op <= 0x5A) return table[op];
        return "???";
    }

    static std::string reg(uint8_t r) {
        if (r <= 12) return "R" + std::to_string(r);
        return "R?";
    }

    static std::string fmtImm(uint32_t imm, bool isFloat) {
        std::ostringstream o;
        if (isFloat) {
            float f; std::memcpy(&f, &imm, 4);
            o << "#" << f;
        }
        else {
            o << "#" << (int32_t)imm;
        }
        return o.str();
    }
};