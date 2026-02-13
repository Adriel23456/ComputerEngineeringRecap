#pragma once

/**
 * @file TomasuloAssembler.h
 * @brief Assembly language to binary instruction converter for Tomasulo CPU.
 *
 * Two-pass assembler:
 *   - Pass 1: Collect labels, count instructions
 *   - Pass 2: Encode 64-bit binary words with resolved references
 *
 * @note SRP: Only handles parsing and encoding — no I/O or UI.
 */

#include <string>
#include <vector>
#include <cstdint>

 /**
  * @struct AssemblyResult
  * @brief Output of a successful or failed assembly attempt.
  */
struct AssemblyResult {
    bool                     success = false;
    std::vector<uint64_t>    instructions;      ///< Encoded 64-bit words
    std::vector<std::string> errors;            ///< Human-readable error list
    std::string              summary;           ///< One-line status message
};

/**
 * @class TomasuloAssembler
 * @brief Converts assembly source text into 64-bit machine instructions.
 *
 * Supports:
 * - Integer and floating-point arithmetic (register & immediate)
 * - Shift and logical operations
 * - INC / DEC
 * - Comparisons (integer & float)
 * - Branches with label resolution
 * - Memory load / store with bracket syntax
 * - MOV / MOVI / MOVL variants
 * - NOP, SWI
 */
class TomasuloAssembler {
public:
    /**
     * @brief Assembles source text (multi-line string).
     * @param source Assembly source code.
     * @return AssemblyResult with encoded instructions or error list.
     */
    AssemblyResult assemble(const std::string& source) const;

private:
    // ── String helpers ──────────────────────────────────────────
    static std::string              toUpper(const std::string& s);
    static std::string              trim(const std::string& s);
    static std::vector<std::string> splitOperands(const std::string& s);

    // ── Parsing helpers ─────────────────────────────────────────
    static bool     isImmediate(const std::string& token);
    static long     parseIntImm(const std::string& token);
    static uint32_t floatToU32(float f);
};