#pragma once

/**
 * @file TomasuloRamTable.h
 * @brief Paged RAM visualization table widget for Tomasulo CPU.
 *
 * Displays 50KB of memory in a scrollable, paged table with
 * multiple data format views (hex, decimal, double).
 *
 * @note Follows:
 *   - SRP: Only handles RAM table display and paging
 *   - OCP: Page size and RAM size are configurable via constants
 *   - Encapsulation: Internal formatting is private
 */

#include <array>
#include <string>
#include <cstdint>
#include <cstring>

 /**
  * @class TomasuloRamTable
  * @brief Paged table widget for displaying 50KB RAM contents.
  *
  * Features:
  * - 6400 rows (50KB / 8 bytes per row)
  * - 512 rows per page, 13 pages total
  * - Address (trimmed hex), full hex value, decimal, and double columns
  * - Page navigation with First/Prev/Next/Last and direct page input
  * - Context menu for clipboard copy
  */
class TomasuloRamTable {
public:
    static constexpr uint64_t kRamSizeBytes = 50ULL * 1024ULL;            ///< 50 KB
    static constexpr size_t   kBytesPerRow = 8;                           ///< 8 bytes per 64-bit word
    static constexpr int      kTotalRows = static_cast<int>(kRamSizeBytes / kBytesPerRow); ///< 6400 rows
    static constexpr int      kRowsPerPage = 512;                         ///< Rows displayed per page
    static constexpr int      kTotalPages = (kTotalRows + kRowsPerPage - 1) / kRowsPerPage; ///< 13 pages
    static constexpr uint64_t kBaseAddr = 0x0000000000000000ULL;       ///< Starting address
    static constexpr uint64_t kStep = 0x8ULL;                      ///< Address increment per row

    TomasuloRamTable() : m_currentPage(0) {
        m_values.fill(0);
    }

    /**
     * @brief Renders the paged RAM table with navigation controls.
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
     * @param rowIndex Row index (0 to kTotalRows-1).
     * @param value 64-bit value to set.
     */
    void setDataByIndex(int rowIndex, uint64_t value);

    /**
     * @brief Gets value by row index.
     * @param rowIndex Row index (0 to kTotalRows-1).
     * @return 64-bit value at that row.
     */
    uint64_t getDataByIndex(int rowIndex) const;

    /**
     * @brief Resets all RAM values to zero.
     */
    void resetAll();

    /**
     * @brief Gets the current page index (0-based).
     */
    int currentPage() const { return m_currentPage; }

    /**
     * @brief Sets the current page index (clamped to valid range).
     */
    void setPage(int page);

private:
    std::array<uint64_t, kTotalRows> m_values{};
    int m_currentPage;

    /**
     * @brief Renders the page navigation bar.
     * @param availableWidth Width available for the navigation bar.
     */
    void renderPageNavigation(float availableWidth);

    /**
     * @brief Renders the table for the current page.
     * @param id ImGui table ID.
     * @param tableHeight Available height for the table.
     */
    void renderTable(const char* id, float tableHeight);

    /**
     * @brief Formats address as trimmed hex string (no leading zeros).
     * e.g., 0x0, 0x8, 0xD00
     */
    static std::string formatAddress(uint64_t addr);

    /**
     * @brief Formats value as full 16-digit hex string.
     * e.g., 0x0000000000000000
     */
    static std::string formatHex(uint64_t v);

    /**
     * @brief Returns the number of rows on a given page.
     */
    static int rowsOnPage(int page);

    /**
     * @brief Returns the global row index for the first row of a page.
     */
    static int firstRowOfPage(int page);
};