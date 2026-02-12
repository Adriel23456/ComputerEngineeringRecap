#pragma once

/**
 * @file TomasuloRegTable.h
 * @brief Register file visualization table widget for Tomasulo CPU.
 *
 * Displays 16 registers (REG0–REG12, UPPER, LOWER, PEID) with
 * multiple data format views (hex, decimal, double).
 *
 * @note Follows:
 *   - SRP: Only handles register table display
 *   - Encapsulation: Internal formatting is private
 */

#include <array>
#include <cstdint>
#include <string>

 /**
  * @class TomasuloRegTable
  * @brief Table widget for displaying Tomasulo CPU registers.
  *
  * Features:
  * - 16 registers with 4-bit binary codes
  * - Read-only display with context menu copy
  * - Multiple format columns: hex, decimal, double
  */
class TomasuloRegTable {
public:
    static constexpr int kRegCount = 16;

    /**
     * @enum RegIndex
     * @brief Register indices matching 4-bit binary encoding.
     */
    enum RegIndex : int {
        REG0 = 0,   // 0000
        REG1 = 1,   // 0001
        REG2 = 2,   // 0010
        REG3 = 3,   // 0011
        REG4 = 4,   // 0100
        REG5 = 5,   // 0101
        REG6 = 6,   // 0110
        REG7 = 7,   // 0111
        REG8 = 8,   // 1000
        REG9 = 9,   // 1001
        REG10 = 10,  // 1010
        REG11 = 11,  // 1011
        REG12 = 12,  // 1100
        UPPER = 13,  // 1101
        LOWER = 14,  // 1110
        PEID = 15   // 1111
    };

    /**
     * @brief Constructs register table with default values.
     */
    TomasuloRegTable();

    // ════════════════════════════════════════════════════════════
    // Read Access
    // ════════════════════════════════════════════════════════════

    /**
     * @brief Gets register value by index.
     * @param idx Register index (0–15).
     * @return Register value, or 0 if invalid index.
     */
    uint64_t getValueByIndex(int idx) const;

    /**
     * @brief Gets register hex text by index.
     * @param idx Register index (0–15).
     * @return Hex string representation.
     */
    const std::string& getHexTextByIndex(int idx) const;

    // ════════════════════════════════════════════════════════════
    // Write Access
    // ════════════════════════════════════════════════════════════

    /**
     * @brief Sets register value by index.
     * @param idx Register index (0–15).
     * @param val 64-bit value.
     */
    void setValueByIndex(int idx, uint64_t val);

    /**
     * @brief Sets register value by name (case-insensitive).
     * @param key Register name (e.g., "REG0", "UPPER", "PEID").
     * @param val 64-bit value.
     */
    void setValueByName(const std::string& key, uint64_t val);

    /**
     * @brief Resets all registers to default values.
     */
    void resetAll();

    // ════════════════════════════════════════════════════════════
    // Rendering
    // ════════════════════════════════════════════════════════════

    /**
     * @brief Renders the register table.
     * @param id ImGui widget ID.
     */
    void render(const char* id);

private:
    std::array<std::string, kRegCount> m_names{};      ///< Register names
    std::array<uint64_t, kRegCount>    m_values{};     ///< Register values
    std::array<std::string, kRegCount> m_valueText{};  ///< Hex text cache

    /**
     * @brief Formats value as full 16-digit hex string.
     */
    static std::string formatHex(uint64_t v);

    /**
     * @brief Resolves register name to index (case-insensitive).
     * @return Index (0–15) or -1 if not found.
     */
    static int indexFromName(const std::string& keyIn);
};