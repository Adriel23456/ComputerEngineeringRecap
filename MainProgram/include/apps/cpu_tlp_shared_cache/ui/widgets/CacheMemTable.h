// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/widgets/CacheMemTable.h
// ============================================================================

#pragma once

/**
 * @file CacheMemTable.h
 * @brief Cache memory visualization table widget.
 *
 * Displays cache line contents with set/way navigation and
 * multiple data format views (hex, decimal, double).
 *
 * @note Follows:
 *   - SRP: Only handles cache visualization
 *   - Encapsulation: Internal data structures hidden
 */

#include <array>
#include <string>
#include <cstdint>

 /**
  * @class CacheMemTable
  * @brief Interactive cache line viewer with navigation.
  *
  * Features:
  * - Set/Way selection buttons
  * - 32-byte line display in 4x8-byte segments
  * - Multiple data formats: hex, signed decimal, IEEE double
  * - MESI state color coding
  */
class CacheMemTable {
public:
    /**
     * @struct CacheLine
     * @brief Cache line data storage.
     */
    struct CacheLine {
        std::string tag;                  ///< Tag as hex string
        std::array<uint8_t, 32> data;     ///< Line data (32 bytes)
        std::string mesi;                 ///< MESI state (I/S/E/M)

        CacheLine() : tag("0x00000000000000"), mesi("I") {
            data.fill(0);
        }
    };

    CacheMemTable() : m_selectedSet(0), m_selectedWay(0) {
        m_lines.fill(CacheLine());
    }

    // ========================================================================
    // Data Setters
    // ========================================================================

    /**
     * @brief Sets line by linear index.
     * @param index Linear index (0-15).
     * @param tag Tag string.
     * @param data Line data.
     * @param mesi MESI state.
     */
    void setLine(int index, const std::string& tag,
        const std::array<uint8_t, 32>& data,
        const std::string& mesi);

    /**
     * @brief Sets line by set and way indices.
     * @param set Set index (0-7).
     * @param way Way index (0-1).
     * @param tag Tag string.
     * @param data Line data.
     * @param mesi MESI state.
     */
    void setLineBySetWay(int set, int way, const std::string& tag,
        const std::array<uint8_t, 32>& data,
        const std::string& mesi);

    // ========================================================================
    // Rendering
    // ========================================================================

    /**
     * @brief Renders the cache table with navigation.
     * @param id ImGui widget ID.
     */
    void render(const char* id);

    // ========================================================================
    // Navigation
    // ========================================================================

    /** @brief Gets currently selected set. */
    int getSelectedSet() const { return m_selectedSet; }

    /** @brief Gets currently selected way. */
    int getSelectedWay() const { return m_selectedWay; }

    /** @brief Sets selected set index. */
    void setSelectedSet(int set) {
        if (set >= 0 && set < kSets) m_selectedSet = set;
    }

    /** @brief Sets selected way index. */
    void setSelectedWay(int way) {
        if (way >= 0 && way < kWays) m_selectedWay = way;
    }

private:
    static constexpr int kSets = 8;
    static constexpr int kWays = 2;
    static constexpr int kTotalLines = kSets * kWays;

    std::array<CacheLine, kTotalLines> m_lines;

    int m_selectedSet;  ///< Currently selected set
    int m_selectedWay;  ///< Currently selected way

    // ========================================================================
    // Helper Methods
    // ========================================================================

    /**
     * @brief Converts line data to hex string.
     */
    std::string dataToHex(const std::array<uint8_t, 32>& data);

    /**
     * @brief Extracts 8-byte segment as uint64.
     * @param data Line data.
     * @param segment Segment index (0-3).
     * @return 64-bit value in little-endian.
     */
    uint64_t extract8Bytes(const std::array<uint8_t, 32>& data, int segment);

    /**
     * @brief Formats 8-byte segment as hex string.
     */
    std::string format8BytesAsHex(const std::array<uint8_t, 32>& data, int segment);

    /**
     * @brief Gets currently selected line.
     */
    const CacheLine& getCurrentLine() const {
        int index = m_selectedSet * kWays + m_selectedWay;
        return m_lines[index];
    }
};