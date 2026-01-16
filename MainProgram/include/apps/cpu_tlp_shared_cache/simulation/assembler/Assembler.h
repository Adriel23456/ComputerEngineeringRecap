// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/assembler/Assembler.h
// ============================================================================

#pragma once

/**
 * @file Assembler.h
 * @brief Assembly language to binary instruction converter.
 *
 * Converts human-readable assembly source code into 64-bit
 * machine instructions for the CPU TLP simulation.
 *
 * @note Follows:
 *   - SRP: Only handles assembly parsing and encoding
 *   - Encapsulation: Internal parsing helpers are private
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <cstdint>
#include <algorithm>
#include <stdexcept>

 /**
  * @class Assembler
  * @brief Converts assembly source files to binary instructions.
  *
  * Supports:
  * - Integer and floating-point operations
  * - Register and immediate operands
  * - Branch labels with forward/backward references
  * - Memory load/store instructions
  */
class Assembler {
public:
    using u64 = uint64_t;

    /**
     * @brief Assembles source file to instruction vector.
     * @param path Path to assembly source file.
     * @return Vector of 64-bit encoded instructions.
     * @throws std::runtime_error on parse errors.
     */
    std::vector<u64> assembleFile(const std::string& path);

private:
    // ========================================================================
    // String Utilities
    // ========================================================================

    /**
     * @brief Converts string to uppercase.
     * @param s Input string.
     * @return Uppercase copy.
     */
    std::string up(const std::string& s);

    /**
     * @brief Removes leading/trailing whitespace.
     * @param s Input string.
     * @return Trimmed copy.
     */
    std::string trim(const std::string& s);

    /**
     * @brief Splits operand string by commas.
     * @param s Operand string.
     * @return Vector of individual operands.
     */
    std::vector<std::string> splitOperands(const std::string& s);

    // ========================================================================
    // Parsing Utilities
    // ========================================================================

    /**
     * @brief Checks if token is an immediate value.
     * @param token Token to check.
     * @return true if immediate (starts with #).
     */
    bool isImmediate(const std::string& token);

    /**
     * @brief Parses integer immediate value.
     * @param token Immediate token (with or without #).
     * @return Parsed integer value.
     */
    long parseIntImm(const std::string& token);

    /**
     * @brief Converts float to 32-bit representation.
     * @param f Float value.
     * @return IEEE 754 32-bit representation.
     */
    uint32_t floatToU32(float f);
};