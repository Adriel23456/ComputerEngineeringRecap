#pragma once

/**
 * @file CacheTable.h
 * @brief Cache visualization table widget for Tomasulo CPU.
 *
 * Displays a 2KB, 4-way set-associative cache with
 * 8 sets, 64-byte lines, and 55-bit tags.
 *
 * @note Follows:
 *   - SRP: Only handles cache table display and set/way navigation
 *   - OCP: Constants are configurable; segment rendering is data-driven
 *   - DRY: Shared by both I-Cache and D-Cache views
 *   - Encapsulation: Internal data structures and formatting are private
 */

#include <array>
#include <string>
#include <cstdint>

 /**
  * @class CacheTable
  * @brief Interactive cache line viewer with set/way navigation.
  *
  * Features:
  * - Set (0-7) and Way (0-3) selection buttons
  * - 64-byte line displayed as 16 x 8-byte segments
  * - Multiple data formats: hex, signed decimal, IEEE 754 double
  * - 55-bit tag display
  * - Valid bit indicator per line
  */
class CacheTable {
public:
    // ════════════════════════════════════════════════════════════
    // Constants
    // ════════════════════════════════════════════════════════════

    static constexpr int kSets = 8;                        ///< Number of sets
    static constexpr int kWays = 4;                        ///< Ways per set
    static constexpr int kTotalLines = kSets * kWays;            ///< 32 cache lines
    static constexpr int kLineSizeBytes = 64;                       ///< Bytes per cache line
    static constexpr int kSegments = kLineSizeBytes / 8;       ///< 16 segments of 8 bytes
    static constexpr int kTagBits = 55;                       ///< Tag width in bits

    // ════════════════════════════════════════════════════════════
    // Cache Line
    // ════════════════════════════════════════════════════════════

    /**
     * @struct CacheLine
     * @brief Single cache line.
     */
    struct CacheLine {
        std::string tag;                                ///< Tag as hex string
        std::array<uint8_t, kLineSizeBytes> data;       ///< Line data (64 bytes)
        bool valid;                                     ///< Valid bit

        CacheLine() : tag("0x00000000000000"), valid(false) {
            data.fill(0);
        }
    };

    // ════════════════════════════════════════════════════════════
    // Construction
    // ════════════════════════════════════════════════════════════

    CacheTable() : m_selectedSet(0), m_selectedWay(0) {
        m_lines.fill(CacheLine());
    }

    // ════════════════════════════════════════════════════════════
    // Data Setters
    // ════════════════════════════════════════════════════════════

    /**
     * @brief Sets line by linear index.
     * @param index Linear index (0 to kTotalLines-1).
     * @param tag Tag string.
     * @param data Line data (64 bytes).
     * @param valid Valid bit.
     */
    void setLine(int index, const std::string& tag,
        const std::array<uint8_t, kLineSizeBytes>& data,
        bool valid);

    /**
     * @brief Sets line by set and way indices.
     * @param set Set index (0-7).
     * @param way Way index (0-3).
     * @param tag Tag string.
     * @param data Line data (64 bytes).
     * @param valid Valid bit.
     */
    void setLineBySetWay(int set, int way, const std::string& tag,
        const std::array<uint8_t, kLineSizeBytes>& data,
        bool valid);

    /**
     * @brief Resets all cache lines to invalid/empty state.
     */
    void resetAll();

    // ════════════════════════════════════════════════════════════
    // Rendering
    // ════════════════════════════════════════════════════════════

    /**
     * @brief Renders the cache table with set/way navigation.
     * @param id ImGui widget ID.
     */
    void render(const char* id);

    // ════════════════════════════════════════════════════════════
    // Navigation
    // ════════════════════════════════════════════════════════════

    int getSelectedSet() const { return m_selectedSet; }
    int getSelectedWay() const { return m_selectedWay; }

    void setSelectedSet(int set) {
        if (set >= 0 && set < kSets) m_selectedSet = set;
    }
    void setSelectedWay(int way) {
        if (way >= 0 && way < kWays) m_selectedWay = way;
    }

private:
    std::array<CacheLine, kTotalLines> m_lines;
    int m_selectedSet;
    int m_selectedWay;

    // ════════════════════════════════════════════════════════════
    // Helpers
    // ════════════════════════════════════════════════════════════

    /**
     * @brief Extracts 8-byte segment from line data as uint64 (little-endian).
     * @param data Line data.
     * @param segment Segment index (0-15).
     */
    static uint64_t extract8Bytes(const std::array<uint8_t, kLineSizeBytes>& data,
        int segment);

    /**
     * @brief Formats 8-byte segment as full hex string (0x0000000000000000).
     */
    static std::string format8BytesAsHex(const std::array<uint8_t, kLineSizeBytes>& data,
        int segment);

    /**
     * @brief Gets the currently selected cache line.
     */
    const CacheLine& getCurrentLine() const {
        int index = m_selectedSet * kWays + m_selectedWay;
        return m_lines[index];
    }

    /**
     * @brief Renders the set/way navigation controls.
     * @param availableWidth Width of the control area.
     */
    void renderNavigationControls(float availableWidth);

    /**
     * @brief Renders the data table for the selected cache line.
     * @param id ImGui table ID.
     * @param tableHeight Height available for the table.
     */
    void renderDataTable(const char* id, float tableHeight);
};