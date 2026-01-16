// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/widgets/RamTable.h
// ============================================================================

#pragma once

/**
 * @file RamTable.h
 * @brief RAM visualization table widget.
 *
 * Displays shared memory contents in a scrollable table with
 * multiple data format views (hex, decimal, double).
 *
 * @note Follows:
 *   - SRP: Only handles RAM table display
 *   - Encapsulation: Internal formatting is private
 */

#include <array>
#include <string>
#include <cstdint>

 /**
  * @class RamTable
  * @brief Table widget for displaying RAM contents.
  *
  * Features:
  * - 512 rows (4KB / 8 bytes per row)
  * - Address, hex, decimal, and double columns
  * - Context menu for clipboard copy
  */
class RamTable {
public:
    static constexpr int kRows = 512;                         ///< Number of 64-bit words
    static constexpr size_t kBytesPerRow = 8;                 ///< Bytes per row
    static constexpr uint64_t kBaseAddr = 0x0000000000000000ULL; ///< Starting address
    static constexpr uint64_t kStep = 0x8ULL;                 ///< Address increment
    static constexpr uint64_t kMaxAddr = kBaseAddr + (kRows - 1) * kStep; ///< Max address

    /**
     * @brief Renders the RAM table.
     * @param id ImGui widget ID.
     */
    void render(const char* id);

    /**
     * @brief Sets value by byte address (must be 8-byte aligned).
     * @param address Byte address (multiple of 8).
     * @param value 64-bit value to set.
     */
    void setDataByAddress(uint64_t address, uint64_t value);

    /**
     * @brief Sets value by row index.
     * @param rowIndex Row index (0-511).
     * @param value 64-bit value to set.
     */
    void setDataByIndex(int rowIndex, uint64_t value);

private:
    std::array<uint64_t, kRows> m_values{};

    /**
     * @brief Formats address as hex string.
     */
    static std::string formatAddress(uint64_t addr64);

    /**
     * @brief Formats value as hex string.
     */
    static std::string formatHex(uint64_t v);
};