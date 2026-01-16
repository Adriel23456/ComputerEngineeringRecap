// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/assembler/Assembler.cpp
// ============================================================================

/**
 * @file Assembler.cpp
 * @brief Implementation of the assembly language assembler.
 *
 * Converts human-readable assembly source code into 64-bit machine
 * instructions for the CPU TLP simulation.
 *
 * @note Two-pass assembly:
 *   - Pass 1: Collect labels and count instructions
 *   - Pass 2: Generate binary encoding with resolved references
 */

#include "apps/cpu_tlp_shared_cache/simulation/assembler/Assembler.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>

 // ============================================================================
 // String Processing Utilities
 // ============================================================================

std::string Assembler::up(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return result;
}

std::string Assembler::trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::vector<std::string> Assembler::splitOperands(const std::string& s) {
    std::vector<std::string> result;
    std::string trimmedInput = trim(s);
    if (trimmedInput.empty()) return result;

    size_t start = 0;
    size_t pos = 0;

    while ((pos = trimmedInput.find(',', start)) != std::string::npos) {
        std::string token = trim(trimmedInput.substr(start, pos - start));
        if (!token.empty()) {
            result.push_back(token);
        }
        start = pos + 1;
    }

    std::string lastToken = trim(trimmedInput.substr(start));
    if (!lastToken.empty()) {
        result.push_back(lastToken);
    }

    return result;
}

bool Assembler::isImmediate(const std::string& token) {
    return !token.empty() && token[0] == '#';
}

long Assembler::parseIntImm(const std::string& token) {
    try {
        if (token.size() <= 1) {
            throw std::runtime_error("Empty immediate: " + token);
        }

        std::string numStr = token.substr(1);  // Remove '#'

        // Detect hexadecimal (0x or 0X prefix)
        if (numStr.size() >= 2 &&
            numStr[0] == '0' &&
            (numStr[1] == 'x' || numStr[1] == 'X')) {
            return std::stol(numStr, nullptr, 16);
        }
        else {
            // Decimal (supports negative values)
            return std::stol(numStr, nullptr, 10);
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Invalid integer immediate: " + token + " (" + e.what() + ")");
    }
}

uint32_t Assembler::floatToU32(float f) {
    uint32_t u;
    std::memcpy(&u, &f, sizeof(u));
    return u;
}

// ============================================================================
// Main Assembler Implementation
// ============================================================================

std::vector<Assembler::u64> Assembler::assembleFile(const std::string& path) {

    // ========================================================================
    // Opcode Table - Mnemonic to hex value mapping
    // ========================================================================

    static const std::unordered_map<std::string, uint8_t> OPCODE_MAP = {
        // Integer arithmetic (3 registers)
        {"ADD", 0x00}, {"SUB", 0x01}, {"ADC", 0x02}, {"SBC", 0x03},
        {"MUL", 0x04}, {"DIV", 0x05},

        // Logical operations (3 registers)
        {"AND", 0x06}, {"ORR", 0x07}, {"EOR", 0x08}, {"BIC", 0x09},

        // Shift operations (3 registers)
        {"LSL", 0x0A}, {"LSR", 0x0B}, {"ASR", 0x0C}, {"ROR", 0x0D},

        // Integer immediate operations
        {"ADDI", 0x0E}, {"SUBI", 0x0F}, {"ADCI", 0x10}, {"SBCI", 0x11},
        {"MULI", 0x12}, {"DIVI", 0x13}, {"ANDI", 0x14}, {"ORRI", 0x15},
        {"EORI", 0x16}, {"BICI", 0x17}, {"LSLI", 0x18}, {"LSRI", 0x19},
        {"ASRI", 0x1A}, {"RORI", 0x1B},

        // Increment/Decrement
        {"INC", 0x1C}, {"DEC", 0x1D},

        // Float operations (no immediate)
        {"FADD", 0x1E}, {"FSUB", 0x1F}, {"FMUL", 0x20}, {"FDIV", 0x21},
        {"FCOPYSIGN", 0x22},

        // Float operations (with immediate)
        {"FADDI", 0x23}, {"FSUBI", 0x24}, {"FMULI", 0x25}, {"FDIVI", 0x26},
        {"FCOPYSIGNI", 0x27},

        // Unary float operations
        {"FSQRT", 0x28}, {"FNEG", 0x29}, {"FABS", 0x2A},
        {"CDTI", 0x2B}, {"CDTD", 0x2C}, {"RTNR", 0x2D},
        {"RTZ", 0x2E}, {"RTP", 0x2F}, {"RTNE", 0x30},

        // MOV variants
        {"MOV", 0x31}, {"MVN", 0x32}, {"MOVI", 0x33}, {"MVNI", 0x34},
        {"FMOVI", 0x35}, {"FMVNI", 0x36},

        // MOVL - Load label address (uses MOVI opcode)
        {"MOVL", 0x33},

        // Integer comparisons (no immediate)
        {"CMP", 0x37}, {"CMN", 0x38}, {"TST", 0x39}, {"TEQ", 0x3A},

        // Integer comparisons (with immediate)
        {"CMPI", 0x3B}, {"CMNI", 0x3C}, {"TSTI", 0x3D}, {"TEQI", 0x3E},

        // Float comparisons (no immediate)
        {"FCMP", 0x3F}, {"FCMN", 0x40}, {"FCMPS", 0x41},

        // Float comparisons (with immediate)
        {"FCMPI", 0x42}, {"FCMNI", 0x43}, {"FCMPSI", 0x44},

        // Branches
        {"B", 0x45}, {"BEQ", 0x46}, {"BNE", 0x47}, {"BLT", 0x48},
        {"BGT", 0x49}, {"BUN", 0x4A}, {"BORD", 0x4B},

        // Special instructions
        {"SWI", 0x4C},  // Software Interrupt
        {"NOP", 0x4D},  // No Operation

        // Memory operations
        {"LDR", 0x4E}, {"STR", 0x4F}, {"LDRB", 0x50}, {"STRB", 0x51},

        // Unary float with immediate
        {"FSQRTI", 0x52}, {"FNEGI", 0x53}, {"FABSI", 0x54},
        {"CDTII", 0x55}, {"CDTDI", 0x56}, {"RTNRI", 0x57},
        {"RTZI", 0x58}, {"RTPI", 0x59}, {"RTNEI", 0x5A}
    };

    // ========================================================================
    // Register Table - Name to code mapping
    // ========================================================================

    static const std::unordered_map<std::string, uint8_t> REGS = {
        {"REG0", 0x0}, {"REG1", 0x1}, {"REG2", 0x2}, {"REG3", 0x3},
        {"REG4", 0x4}, {"REG5", 0x5}, {"REG6", 0x6}, {"REG7", 0x7},
        {"REG8", 0x8}, {"SIDS", 0x9}, {"UPPER_REG", 0xA}, {"LOWER_REG", 0xB}
    };

    // ========================================================================
    // Open Source File
    // ========================================================================

    std::ifstream ifs(path);
    if (!ifs) {
        throw std::runtime_error("Could not open file: " + path);
    }

    // ========================================================================
    // Pass 1: Collect Labels and Instructions
    // ========================================================================

    struct LineInfo {
        int lineNumber;
        std::string text;
    };

    std::vector<LineInfo> lines;
    std::string raw;
    int lineNumber = 1;

    // Read all lines, removing comments
    while (std::getline(ifs, raw)) {
        // Remove \r for Windows compatibility
        if (!raw.empty() && raw.back() == '\r') {
            raw.pop_back();
        }

        // Remove comments (everything after ';' or '#' at line start)
        size_t commentPos = raw.find_first_of(";");
        if (commentPos != std::string::npos) {
            raw = raw.substr(0, commentPos);
        }

        // Also handle '#' comments at start of line
        std::string trimmed = trim(raw);
        if (!trimmed.empty() && trimmed[0] == '#') {
            raw.clear();
        }

        lines.push_back({ lineNumber++, raw });
    }

    std::vector<std::tuple<int, std::string, int>> instructions;  // (PC, text, line)
    std::unordered_map<std::string, int> labels;                  // label -> PC
    int pc = 0;

    // Process lines to extract labels and instructions
    for (auto& ln : lines) {
        std::string s = trim(ln.text);
        if (s.empty()) continue;

        // Process all labels on the line (can be multiple)
        while (true) {
            size_t colonPos = s.find(':');
            if (colonPos == std::string::npos) break;

            std::string label = trim(s.substr(0, colonPos));
            if (!label.empty()) {
                labels[up(label)] = pc;
            }
            s = trim(s.substr(colonPos + 1));
        }

        // Remaining text after labels is an instruction
        if (!s.empty()) {
            instructions.emplace_back(pc, s, ln.lineNumber);
            ++pc;
        }
    }

    // ========================================================================
    // Pass 2: Encode Instructions
    // ========================================================================

    std::vector<u64> output;
    std::vector<std::string> errors;

    std::cout << "[Assembler] Starting assembly: " << path
        << " (" << instructions.size() << " instructions)\n";

    for (const auto& inst : instructions) {
        int currentPC = std::get<0>(inst);
        std::string text = std::get<1>(inst);
        int srcLine = std::get<2>(inst);

        try {
            // Separate opcode from operands
            std::stringstream lineSS(text);
            std::string opcode, rest;

            lineSS >> opcode;
            std::getline(lineSS, rest);
            rest = trim(rest);

            std::string opcodeUpper = up(opcode);
            if (OPCODE_MAP.find(opcodeUpper) == OPCODE_MAP.end()) {
                throw std::runtime_error("Unknown opcode: '" + opcode + "'");
            }

            uint8_t opcodeValue = OPCODE_MAP.at(opcodeUpper);
            u64 word = 0;
            word |= (u64(opcodeValue) & 0xFFULL) << 56;  // Opcode in bits [63:56]
            bool encoded = false;

            // ================================================================
            // Memory Instructions (LDR/STR/LDRB/STRB)
            // Special handling due to bracket syntax
            // ================================================================

            if (opcodeValue == 0x4E || opcodeValue == 0x4F ||
                opcodeValue == 0x50 || opcodeValue == 0x51) {

                size_t bracketOpen = rest.find('[');
                size_t bracketClose = rest.find(']');

                if (bracketOpen == std::string::npos || bracketClose == std::string::npos) {
                    throw std::runtime_error(
                        "Memory instruction requires: REG, [BASE] or REG, [BASE, #offset]");
                }

                // Extract first register (before '[')
                std::string beforeBracket = rest.substr(0, bracketOpen);
                size_t commaPos = beforeBracket.find(',');
                if (commaPos == std::string::npos) {
                    throw std::runtime_error("Missing comma before '['");
                }

                std::string regStr = trim(beforeBracket.substr(0, commaPos));
                std::string regStrUpper = up(regStr);

                if (REGS.count(regStrUpper) == 0) {
                    throw std::runtime_error("Invalid register: " + regStr);
                }
                uint8_t reg = REGS.at(regStrUpper);

                // Extract content inside brackets
                std::string insideBrackets = trim(
                    rest.substr(bracketOpen + 1, bracketClose - bracketOpen - 1));

                uint8_t baseReg = 0;
                uint32_t offsetU32 = 0;

                // Check for offset: [BASE, #offset]
                size_t commaInside = insideBrackets.find(',');
                if (commaInside != std::string::npos) {
                    std::string baseStr = trim(insideBrackets.substr(0, commaInside));
                    std::string offsetStr = trim(insideBrackets.substr(commaInside + 1));

                    std::string baseStrUpper = up(baseStr);
                    if (REGS.count(baseStrUpper) == 0) {
                        throw std::runtime_error("Invalid base register: " + baseStr);
                    }
                    baseReg = REGS.at(baseStrUpper);

                    if (!isImmediate(offsetStr)) {
                        throw std::runtime_error("Offset must be immediate: " + offsetStr);
                    }
                    long offsetVal = parseIntImm(offsetStr);
                    offsetU32 = static_cast<uint32_t>(static_cast<int32_t>(offsetVal));
                }
                else {
                    // Format: [BASE] (no offset)
                    std::string baseStrUpper = up(insideBrackets);
                    if (REGS.count(baseStrUpper) == 0) {
                        throw std::runtime_error("Invalid base register: " + insideBrackets);
                    }
                    baseReg = REGS.at(baseStrUpper);
                    offsetU32 = 0;
                }

                // Encode based on instruction type
                if (opcodeValue == 0x4E || opcodeValue == 0x50) {
                    // LDR/LDRB: first register -> Rd (bits 52-55)
                    word |= (u64(reg) & 0xFULL) << 52;
                    word |= (u64(baseReg) & 0xFULL) << 48;
                }
                else {
                    // STR/STRB: first register -> Rm (bits 44-47)
                    word |= (u64(reg) & 0xFULL) << 44;
                    word |= (u64(baseReg) & 0xFULL) << 48;
                }
                word |= (u64(offsetU32) & 0xFFFFFFFFULL) << 12;
                encoded = true;
            }
            // ================================================================
            // All Other Instructions
            // ================================================================
            else {
                auto ops = splitOperands(rest);

                // MOVL: Load label address
                if (ops.size() == 2 && !isImmediate(ops[1]) && opcodeUpper == "MOVL") {
                    std::string rdStr = up(ops[0]);
                    if (REGS.count(rdStr) == 0) {
                        throw std::runtime_error("Invalid destination register for MOVL: " + ops[0]);
                    }

                    std::string target = up(ops[1]);
                    if (labels.count(target) == 0) {
                        throw std::runtime_error("Label not found for MOVL: '" + ops[1] + "'");
                    }

                    uint8_t rd = REGS.at(rdStr);
                    long targetPC = labels.at(target);
                    long targetAddrBytes = targetPC * 8;  // Convert to bytes
                    uint32_t immU32 = static_cast<uint32_t>(static_cast<int32_t>(targetAddrBytes));

                    word |= (u64(rd) & 0xFULL) << 52;
                    word |= (u64(rd) & 0xFULL) << 48;
                    word |= (u64(immU32) & 0xFFFFFFFFULL) << 12;
                    encoded = true;
                }
                // R-Format: 3 register operands (Rd, Rn, Rm)
                else if (ops.size() == 3 && !isImmediate(ops[2])) {
                    std::string rd = up(ops[0]), rn = up(ops[1]), rm = up(ops[2]);
                    if (REGS.count(rd) == 0 || REGS.count(rn) == 0 || REGS.count(rm) == 0) {
                        throw std::runtime_error("Invalid register in 3-operand instruction.");
                    }

                    word |= (u64(REGS.at(rd)) & 0xFULL) << 52;  // Rd [55:52]
                    word |= (u64(REGS.at(rn)) & 0xFULL) << 48;  // Rn [51:48]
                    word |= (u64(REGS.at(rm)) & 0xFULL) << 44;  // Rm [47:44]
                    encoded = true;
                }
                // R-Format: 2 register operands
                else if (ops.size() == 2 && !isImmediate(ops[1])) {
                    std::string arg0 = up(ops[0]), arg1 = up(ops[1]);
                    if (REGS.count(arg0) == 0 || REGS.count(arg1) == 0) {
                        throw std::runtime_error("Invalid register in 2-operand instruction.");
                    }

                    // Comparison instructions: Rn, Rm
                    if (opcodeValue == 0x37 || opcodeValue == 0x38 ||
                        opcodeValue == 0x39 || opcodeValue == 0x3A ||
                        opcodeValue == 0x3F || opcodeValue == 0x40 ||
                        opcodeValue == 0x41) {
                        word |= (u64(REGS.at(arg0)) & 0xFULL) << 48;  // Rn
                        word |= (u64(REGS.at(arg1)) & 0xFULL) << 44;  // Rm
                        encoded = true;
                    }
                    // Other: Rd, Rm
                    else {
                        word |= (u64(REGS.at(arg0)) & 0xFULL) << 52;  // Rd
                        word |= (u64(REGS.at(arg1)) & 0xFULL) << 44;  // Rm
                        encoded = true;
                    }
                }
                // I-Format: Instructions with immediate
                else if (!ops.empty() && isImmediate(ops.back())) {
                    if (ops.size() < 2 || ops.size() > 3) {
                        throw std::runtime_error("Immediate instruction requires 2 or 3 operands.");
                    }

                    std::string rdStr = up(ops[0]);
                    if (REGS.count(rdStr) == 0) {
                        throw std::runtime_error("Invalid destination register: " + ops[0]);
                    }

                    uint8_t rd = REGS.at(rdStr);
                    uint8_t rn = rd;  // Default: Rn = Rd

                    if (ops.size() == 3) {
                        std::string rnStr = up(ops[1]);
                        if (isImmediate(rnStr)) {
                            throw std::runtime_error("Second operand cannot be immediate.");
                        }
                        if (REGS.count(rnStr) == 0) {
                            throw std::runtime_error("Invalid source register: " + ops[1]);
                        }
                        rn = REGS.at(rnStr);
                    }

                    uint32_t immU32 = 0;
                    std::string immToken = ops.back();

                    // Float immediate (has '.') or integer
                    if (immToken.find('.') != std::string::npos && opcodeUpper[0] == 'F') {
                        float fval = std::stof(immToken.substr(1));
                        immU32 = floatToU32(fval);
                    }
                    else {
                        long val = parseIntImm(immToken);
                        immU32 = static_cast<uint32_t>(static_cast<int32_t>(val));
                    }

                    word |= (u64(rd) & 0xFULL) << 52;
                    word |= (u64(rn) & 0xFULL) << 48;
                    word |= (u64(immU32) & 0xFFFFFFFFULL) << 12;
                    encoded = true;
                }
                // Branches: Jump to label
                else if (ops.size() == 1 && !isImmediate(ops[0]) && opcodeUpper[0] == 'B') {
                    std::string target = up(ops[0]);
                    if (labels.count(target) == 0) {
                        throw std::runtime_error("Label not found: '" + ops[0] + "'");
                    }

                    long offsetInstr = static_cast<long>(labels.at(target)) - static_cast<long>(currentPC);
                    long offsetBytes = offsetInstr * 8;  // Each instruction = 8 bytes

                    uint32_t offsetU32 = static_cast<uint32_t>(static_cast<int32_t>(offsetBytes));
                    word |= (u64(offsetU32) & 0xFFFFFFFFULL) << 12;
                    encoded = true;
                }
                // INC/DEC: Unary increment/decrement
                else if (ops.size() == 1 && !isImmediate(ops[0]) &&
                    (opcodeValue == 0x1C || opcodeValue == 0x1D)) {
                    std::string rdStr = up(ops[0]);
                    if (REGS.count(rdStr) == 0) {
                        throw std::runtime_error("Invalid register for INC/DEC: " + ops[0]);
                    }

                    uint8_t rd = REGS.at(rdStr);
                    uint32_t immU32 = 1;  // Always uses immediate = 1

                    word |= (u64(rd) & 0xFULL) << 52;
                    word |= (u64(rd) & 0xFULL) << 48;
                    word |= (u64(immU32) & 0xFFFFFFFFULL) << 12;
                    encoded = true;
                }
                // No operands: NOP, SWI
                else if (ops.empty()) {
                    encoded = true;
                }

                if (!encoded) {
                    throw std::runtime_error(
                        "Invalid instruction format or incorrect number of operands.");
                }
            }

            output.push_back(word);

        }
        catch (const std::exception& e) {
            std::stringstream errorSS;
            errorSS << "[Line " << srcLine << "] " << e.what() << " -> \"" << text << "\"";
            errors.push_back(errorSS.str());
        }
    }

    // ========================================================================
    // Report Errors or Success
    // ========================================================================

    if (!errors.empty()) {
        std::stringstream finalError;
        finalError << "Assembly failed with " << errors.size() << " error(s):\n";
        for (const auto& err : errors) {
            finalError << "  * " << err << "\n";
        }
        throw std::runtime_error(finalError.str());
    }

    std::cout << "[Assembler] OK - Assembly completed successfully: "
        << output.size() << " instructions generated.\n";

    return output;
}