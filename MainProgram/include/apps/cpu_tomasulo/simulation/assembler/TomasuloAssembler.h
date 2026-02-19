// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/assembler/TomasuloAssembler.h
// ============================================================================

#pragma once

/**
 * @file TomasuloAssembler.h
 * @brief Two-pass assembler for the Tomasulo CPU instruction set.
 *
 * Converts human-readable assembly source into a vector of 64-bit
 * encoded instruction words suitable for loading directly into TomasuloRAM.
 *
 * Instruction encoding (64-bit word):
 *   [63:56] Opcode          (8 bits)
 *   [55:52] Rd              (4 bits)
 *   [51:48] Rn              (4 bits)
 *   [47:44] Rm              (4 bits)
 *   [43:12] Immediate/Offset (32 bits, interpretation depends on opcode)
 *   [11: 0] Reserved
 *
 * Supported features:
 *   - Integer arithmetic (register and immediate)
 *   - Floating-point arithmetic (register and immediate)
 *   - Shift / logical operations
 *   - INC / DEC
 *   - Integer and floating-point comparisons
 *   - Branches with forward and backward label resolution
 *   - LDR / STR / LDRB / STRB with bracket syntax and optional offset
 *   - MOV / MOVI / MVNI / FMOVI / FMVNI / MOVL (load label address)
 *   - NOP, SWI
 *
 * Pass 1: strips comments, collects labels, counts instruction PCs.
 * Pass 2: encodes each instruction; unresolved labels produce error entries.
 *
 * @note
 *   - SRP: Only handles parsing and encoding — no I/O, UI, or RAM access.
 *   - The assemble() method is const and stateless; it may be called
 *     concurrently from multiple threads if needed.
 */

#include <string>
#include <vector>
#include <cstdint>

 /** @brief Result returned by TomasuloAssembler::assemble(). */
struct AssemblyResult {
    bool                     success = false;
    std::vector<uint64_t>    instructions;     ///< Encoded 64-bit words (valid on success).
    std::vector<std::string> errors;           ///< Per-line error messages (non-empty on failure).
    std::string              summary;          ///< Human-readable one-line status.
};

class TomasuloAssembler {
public:
    /**
     * @brief Assembles multi-line assembly source into 64-bit instruction words.
     *
     * Both passes operate on in-memory strings; no file I/O is performed.
     *
     * @param source  Assembly source code (UTF-8, any line ending).
     * @return        AssemblyResult with encoded instructions or a list of errors.
     */
    AssemblyResult assemble(const std::string& source) const;

private:
    // ── String Utilities ──────────────────────────────────────────
    static std::string              toUpper(const std::string& s);
    static std::string              trim(const std::string& s);

    /** @brief Splits a comma-separated operand list, trimming each token. */
    static std::vector<std::string> splitOperands(const std::string& s);

    // ── Parsing Utilities ─────────────────────────────────────────
    /** @brief Returns true if the token starts with '#'. */
    static bool     isImmediate(const std::string& token);

    /** @brief Parses "#<decimal or 0x hex>" → signed long. */
    static long     parseIntImm(const std::string& token);

    /** @brief Reinterprets a float's bit pattern as uint32. */
    static uint32_t floatToU32(float f);
};