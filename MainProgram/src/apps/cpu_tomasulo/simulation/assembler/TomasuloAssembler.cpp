/**
 * @file TomasuloAssembler.cpp
 * @brief Implementation of the Tomasulo CPU assembler.
 *
 * Two-pass assembly:
 *   - Pass 1: Collect labels and count instructions
 *   - Pass 2: Generate 64-bit binary encoding with resolved references
 *
 * Encoding layout (64-bit word):
 *   [63:56] Opcode   (8 bits)
 *   [55:52] Rd       (4 bits)
 *   [51:48] Rn       (4 bits)
 *   [47:44] Rm       (4 bits)
 *   [43:12] Immediate / Offset (32 bits)
 *   [11:0]  Reserved
 */

#include "apps/cpu_tomasulo/simulation/assembler/TomasuloAssembler.h"

#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <unordered_map>
#include <tuple>

 // ============================================================================
 // String Utilities
 // ============================================================================

std::string TomasuloAssembler::toUpper(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return r;
}

std::string TomasuloAssembler::trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

std::vector<std::string> TomasuloAssembler::splitOperands(const std::string& s) {
    std::vector<std::string> result;
    std::string t = trim(s);
    if (t.empty()) return result;

    size_t start = 0;
    size_t pos;
    while ((pos = t.find(',', start)) != std::string::npos) {
        std::string tok = trim(t.substr(start, pos - start));
        if (!tok.empty()) result.push_back(tok);
        start = pos + 1;
    }
    std::string last = trim(t.substr(start));
    if (!last.empty()) result.push_back(last);
    return result;
}

// ============================================================================
// Parsing Utilities
// ============================================================================

bool TomasuloAssembler::isImmediate(const std::string& token) {
    return !token.empty() && token[0] == '#';
}

long TomasuloAssembler::parseIntImm(const std::string& token) {
    if (token.size() <= 1)
        throw std::runtime_error("Empty immediate: " + token);

    std::string num = token.substr(1); // strip '#'

    if (num.size() >= 2 && num[0] == '0' && (num[1] == 'x' || num[1] == 'X'))
        return std::stol(num, nullptr, 16);

    return std::stol(num, nullptr, 10);
}

uint32_t TomasuloAssembler::floatToU32(float f) {
    uint32_t u;
    std::memcpy(&u, &f, sizeof(u));
    return u;
}

// ============================================================================
// Main Assembly
// ============================================================================

AssemblyResult TomasuloAssembler::assemble(const std::string& source) const {

    // ════════════════════════════════════════════════════════════
    // Opcode Table
    // ════════════════════════════════════════════════════════════

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

        // Increment / Decrement
        {"INC", 0x1C}, {"DEC", 0x1D},

        // Float operations (register)
        {"FADD", 0x1E}, {"FSUB", 0x1F}, {"FMUL", 0x20}, {"FDIV", 0x21},
        {"FCOPYSIGN", 0x22},

        // Float operations (immediate)
        {"FADDI", 0x23}, {"FSUBI", 0x24}, {"FMULI", 0x25}, {"FDIVI", 0x26},
        {"FCOPYSIGNI", 0x27},

        // Unary float
        {"FSQRT", 0x28}, {"FNEG", 0x29}, {"FABS", 0x2A},
        {"CDTI", 0x2B}, {"CDTD", 0x2C}, {"RTNR", 0x2D},
        {"RTZ", 0x2E}, {"RTP", 0x2F}, {"RTNE", 0x30},

        // MOV variants
        {"MOV", 0x31}, {"MVN", 0x32}, {"MOVI", 0x33}, {"MVNI", 0x34},
        {"FMOVI", 0x35}, {"FMVNI", 0x36},

        // MOVL - load label address (reuses MOVI opcode)
        {"MOVL", 0x33},

        // Integer comparisons (register)
        {"CMP", 0x37}, {"CMN", 0x38}, {"TST", 0x39}, {"TEQ", 0x3A},

        // Integer comparisons (immediate)
        {"CMPI", 0x3B}, {"CMNI", 0x3C}, {"TSTI", 0x3D}, {"TEQI", 0x3E},

        // Float comparisons (register)
        {"FCMP", 0x3F}, {"FCMN", 0x40}, {"FCMPS", 0x41},

        // Float comparisons (immediate)
        {"FCMPI", 0x42}, {"FCMNI", 0x43}, {"FCMPSI", 0x44},

        // Branches
        {"B", 0x45}, {"BEQ", 0x46}, {"BNE", 0x47}, {"BLT", 0x48},
        {"BGT", 0x49}, {"BUN", 0x4A}, {"BORD", 0x4B},

        // Special
        {"SWI", 0x4C}, {"NOP", 0x4D},

        // Memory
        {"LDR", 0x4E}, {"STR", 0x4F}, {"LDRB", 0x50}, {"STRB", 0x51},

        // Unary float (immediate)
        {"FSQRTI", 0x52}, {"FNEGI", 0x53}, {"FABSI", 0x54},
        {"CDTII", 0x55}, {"CDTDI", 0x56}, {"RTNRI", 0x57},
        {"RTZI", 0x58}, {"RTPI", 0x59}, {"RTNEI", 0x5A},
    };

    // ════════════════════════════════════════════════════════════
    // Register Table
    // ════════════════════════════════════════════════════════════

    static const std::unordered_map<std::string, uint8_t> REGS = {
        {"REG0",  0x0}, {"REG1",  0x1}, {"REG2",  0x2}, {"REG3",  0x3},
        {"REG4",  0x4}, {"REG5",  0x5}, {"REG6",  0x6}, {"REG7",  0x7},
        {"REG8",  0x8}, {"REG9",  0x9}, {"REG10", 0xA}, {"REG11", 0xB},
        {"REG12", 0xC}, {"UPPER", 0xD}, {"LOWER", 0xE}, {"PEID",  0xF},
    };

    // ════════════════════════════════════════════════════════════
    // Split source into lines
    // ════════════════════════════════════════════════════════════

    struct LineInfo { int lineNumber; std::string text; };
    std::vector<LineInfo> lines;

    {
        std::istringstream stream(source);
        std::string raw;
        int lineNo = 1;

        while (std::getline(stream, raw)) {
            // Strip \r
            if (!raw.empty() && raw.back() == '\r') raw.pop_back();

            // Strip comments (;)
            auto sc = raw.find(';');
            if (sc != std::string::npos) raw = raw.substr(0, sc);

            // Strip full-line # comments
            std::string tr = trim(raw);
            if (!tr.empty() && tr[0] == '#') raw.clear();

            lines.push_back({ lineNo++, raw });
        }
    }

    // ════════════════════════════════════════════════════════════
    // Pass 1 — labels & instruction list
    // ════════════════════════════════════════════════════════════

    using InstrTuple = std::tuple<int, std::string, int>; // (PC, text, srcLine)
    std::vector<InstrTuple> instructions;
    std::unordered_map<std::string, int> labels;
    int pc = 0;

    for (auto& ln : lines) {
        std::string s = trim(ln.text);
        if (s.empty()) continue;

        // Extract labels (may be multiple on one line)
        while (true) {
            auto colon = s.find(':');
            if (colon == std::string::npos) break;

            std::string label = trim(s.substr(0, colon));
            if (!label.empty()) labels[toUpper(label)] = pc;
            s = trim(s.substr(colon + 1));
        }

        if (!s.empty()) {
            instructions.emplace_back(pc, s, ln.lineNumber);
            ++pc;
        }
    }

    // ════════════════════════════════════════════════════════════
    // Pass 2 — encode
    // ════════════════════════════════════════════════════════════

    AssemblyResult result;
    result.instructions.reserve(instructions.size());

    for (const auto& [currentPC, text, srcLine] : instructions) {
        try {
            std::istringstream iss(text);
            std::string opcode, rest;
            iss >> opcode;
            std::getline(iss, rest);
            rest = trim(rest);

            std::string opcUp = toUpper(opcode);
            auto it = OPCODE_MAP.find(opcUp);
            if (it == OPCODE_MAP.end())
                throw std::runtime_error("Unknown opcode: '" + opcode + "'");

            uint8_t  opcVal = it->second;
            uint64_t word = (uint64_t(opcVal) & 0xFFULL) << 56;
            bool     encoded = false;

            // ── Memory instructions ─────────────────────────────
            if (opcVal == 0x4E || opcVal == 0x4F ||
                opcVal == 0x50 || opcVal == 0x51) {

                auto bOpen = rest.find('[');
                auto bClose = rest.find(']');
                if (bOpen == std::string::npos || bClose == std::string::npos)
                    throw std::runtime_error("Memory instruction requires bracket syntax.");

                std::string before = rest.substr(0, bOpen);
                auto comma = before.find(',');
                if (comma == std::string::npos)
                    throw std::runtime_error("Missing comma before '['");

                std::string regStr = toUpper(trim(before.substr(0, comma)));
                if (!REGS.count(regStr))
                    throw std::runtime_error("Invalid register: " + regStr);
                uint8_t reg = REGS.at(regStr);

                std::string inside = trim(rest.substr(bOpen + 1, bClose - bOpen - 1));
                uint8_t  baseReg = 0;
                uint32_t offsetU32 = 0;

                auto commaIn = inside.find(',');
                if (commaIn != std::string::npos) {
                    std::string baseStr = toUpper(trim(inside.substr(0, commaIn)));
                    std::string offsetStr = trim(inside.substr(commaIn + 1));
                    if (!REGS.count(baseStr))
                        throw std::runtime_error("Invalid base register: " + baseStr);
                    baseReg = REGS.at(baseStr);

                    if (!isImmediate(offsetStr))
                        throw std::runtime_error("Offset must be immediate: " + offsetStr);
                    offsetU32 = static_cast<uint32_t>(static_cast<int32_t>(parseIntImm(offsetStr)));
                }
                else {
                    std::string baseStr = toUpper(inside);
                    if (!REGS.count(baseStr))
                        throw std::runtime_error("Invalid base register: " + inside);
                    baseReg = REGS.at(baseStr);
                }

                if (opcVal == 0x4E || opcVal == 0x50) { // LDR / LDRB
                    word |= (uint64_t(reg) & 0xFULL) << 52;
                    word |= (uint64_t(baseReg) & 0xFULL) << 48;
                }
                else {                                  // STR / STRB
                    word |= (uint64_t(reg) & 0xFULL) << 44;
                    word |= (uint64_t(baseReg) & 0xFULL) << 48;
                }
                word |= (uint64_t(offsetU32) & 0xFFFFFFFFULL) << 12;
                encoded = true;
            }
            // ── All other instructions ──────────────────────────
            else {
                auto ops = splitOperands(rest);

                // MOVL: label address
                if (ops.size() == 2 && !isImmediate(ops[1]) && opcUp == "MOVL") {
                    std::string rd = toUpper(ops[0]);
                    if (!REGS.count(rd))
                        throw std::runtime_error("Invalid register for MOVL: " + ops[0]);
                    std::string target = toUpper(ops[1]);
                    if (!labels.count(target))
                        throw std::runtime_error("Label not found for MOVL: '" + ops[1] + "'");

                    long addrBytes = labels.at(target) * 8L;
                    uint32_t imm = static_cast<uint32_t>(static_cast<int32_t>(addrBytes));

                    word |= (uint64_t(REGS.at(rd)) & 0xFULL) << 52;
                    word |= (uint64_t(REGS.at(rd)) & 0xFULL) << 48;
                    word |= (uint64_t(imm) & 0xFFFFFFFFULL) << 12;
                    encoded = true;
                }
                // R-format: 3 register operands (Rd, Rn, Rm)
                else if (ops.size() == 3 && !isImmediate(ops[2])) {
                    std::string d = toUpper(ops[0]), n = toUpper(ops[1]), m = toUpper(ops[2]);
                    if (!REGS.count(d) || !REGS.count(n) || !REGS.count(m))
                        throw std::runtime_error("Invalid register in 3-operand instruction.");

                    word |= (uint64_t(REGS.at(d)) & 0xFULL) << 52;
                    word |= (uint64_t(REGS.at(n)) & 0xFULL) << 48;
                    word |= (uint64_t(REGS.at(m)) & 0xFULL) << 44;
                    encoded = true;
                }
                // R-format: 2 register operands
                else if (ops.size() == 2 && !isImmediate(ops[1])) {
                    std::string a0 = toUpper(ops[0]), a1 = toUpper(ops[1]);
                    if (!REGS.count(a0) || !REGS.count(a1))
                        throw std::runtime_error("Invalid register in 2-operand instruction.");

                    // Comparison instructions: (Rn, Rm)
                    if (opcVal == 0x37 || opcVal == 0x38 || opcVal == 0x39 || opcVal == 0x3A ||
                        opcVal == 0x3F || opcVal == 0x40 || opcVal == 0x41) {
                        word |= (uint64_t(REGS.at(a0)) & 0xFULL) << 48;
                        word |= (uint64_t(REGS.at(a1)) & 0xFULL) << 44;
                    }
                    else { // (Rd, Rm)
                        word |= (uint64_t(REGS.at(a0)) & 0xFULL) << 52;
                        word |= (uint64_t(REGS.at(a1)) & 0xFULL) << 44;
                    }
                    encoded = true;
                }
                // I-format: last operand is immediate
                else if (!ops.empty() && isImmediate(ops.back())) {
                    if (ops.size() < 2 || ops.size() > 3)
                        throw std::runtime_error("Immediate instruction requires 2 or 3 operands.");

                    std::string rdStr = toUpper(ops[0]);
                    if (!REGS.count(rdStr))
                        throw std::runtime_error("Invalid destination register: " + ops[0]);

                    uint8_t rd = REGS.at(rdStr);
                    uint8_t rn = rd;

                    if (ops.size() == 3) {
                        std::string rnStr = toUpper(ops[1]);
                        if (isImmediate(rnStr))
                            throw std::runtime_error("Second operand cannot be immediate.");
                        if (!REGS.count(rnStr))
                            throw std::runtime_error("Invalid source register: " + ops[1]);
                        rn = REGS.at(rnStr);
                    }

                    uint32_t immU32 = 0;
                    std::string immTok = ops.back();

                    bool isFPImm = (opcVal >= 0x23 && opcVal <= 0x27)   // FADDI..FCOPYSIGNI
                        || (opcVal == 0x35 || opcVal == 0x36)    // FMOVI, FMVNI
                        || (opcVal >= 0x42 && opcVal <= 0x44)    // FCMPI..FCMPSI
                        || (opcVal >= 0x52 && opcVal <= 0x5A);   // FSQRTI..RTNEI

                    if (isFPImm) {
                        float fv = std::stof(immTok.substr(1));  // strip '#'
                        immU32 = floatToU32(fv);
                    }
                    else {
                        long val = parseIntImm(immTok);
                        immU32 = static_cast<uint32_t>(static_cast<int32_t>(val));
                    }

                    word |= (uint64_t(rd) & 0xFULL) << 52;
                    word |= (uint64_t(rn) & 0xFULL) << 48;
                    word |= (uint64_t(immU32) & 0xFFFFFFFFULL) << 12;
                    encoded = true;
                }
                // Branches: label
                else if (ops.size() == 1 && !isImmediate(ops[0]) && opcUp[0] == 'B') {
                    std::string target = toUpper(ops[0]);
                    if (!labels.count(target))
                        throw std::runtime_error("Label not found: '" + ops[0] + "'");

                    long offsetInstr = static_cast<long>(labels.at(target)) - static_cast<long>(currentPC);
                    long offsetBytes = offsetInstr * 8;
                    uint32_t offU32 = static_cast<uint32_t>(static_cast<int32_t>(offsetBytes));
                    word |= (uint64_t(offU32) & 0xFFFFFFFFULL) << 12;
                    encoded = true;
                }
                // INC / DEC: single register
                else if (ops.size() == 1 && !isImmediate(ops[0]) &&
                    (opcVal == 0x1C || opcVal == 0x1D)) {
                    std::string rdStr = toUpper(ops[0]);
                    if (!REGS.count(rdStr))
                        throw std::runtime_error("Invalid register for INC/DEC: " + ops[0]);

                    uint8_t rd = REGS.at(rdStr);
                    word |= (uint64_t(rd) & 0xFULL) << 52;
                    word |= (uint64_t(rd) & 0xFULL) << 48;
                    word |= (uint64_t(1) & 0xFFFFFFFFULL) << 12;
                    encoded = true;
                }
                // No operands: NOP, SWI
                else if (ops.empty()) {
                    encoded = true;
                }

                if (!encoded)
                    throw std::runtime_error("Invalid instruction format or operand count.");
            }

            result.instructions.push_back(word);

        }
        catch (const std::exception& e) {
            std::ostringstream oss;
            oss << "[Line " << srcLine << "] " << e.what() << " -> \"" << text << "\"";
            result.errors.push_back(oss.str());
        }
    }

    // ════════════════════════════════════════════════════════════
    // Build summary
    // ════════════════════════════════════════════════════════════

    if (result.errors.empty()) {
        result.success = true;
        result.summary = "Success - " + std::to_string(result.instructions.size())
            + " instruction(s) assembled.";
    }
    else {
        result.success = false;
        std::ostringstream oss;
        oss << "Assembly failed with " << result.errors.size() << " error(s):\n";
        for (const auto& err : result.errors) oss << "  * " << err << "\n";
        result.summary = oss.str();
    }

    return result;
}