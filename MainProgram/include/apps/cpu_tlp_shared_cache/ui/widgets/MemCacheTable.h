// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/widgets/MemCacheTable.h
// ============================================================================

#pragma once

/**
 * @file MemCacheTable.h
 * @brief Generic cache table widget for memory display.
 *
 * Displays cache contents in a 4-column table format with
 * MESI state derivation from state field.
 *
 * @note Follows:
 *   - SRP: Only handles table rendering
 *   - Encapsulation: MESI derivation is internal
 */

#include <array>
#include <string>
#include <imgui.h>

 /**
  * @class MemCacheTable
  * @brief Generic 4-column cache table (Tag/Data/State/MESI).
  *
  * Supports 8 sets x 2 ways = 16 total lines with automatic
  * MESI state derivation from state field.
  */
class MemCacheTable {
public:
    static constexpr int kSets = 8;
    static constexpr int kWaysPerSet = 2;
    static constexpr int kRows = kSets * kWaysPerSet;

    /**
     * @struct Row
     * @brief Single cache line row data.
     */
    struct Row {
        std::string tag;    ///< Tag as hex string
        std::string data;   ///< Data as hex string
        std::string state;  ///< State (2-bit or letter)
    };

    /**
     * @brief Renders table filling available space.
     * @param id ImGui widget ID.
     */
    void render(const char* id);

    // ========================================================================
    // Row Setters (by linear index)
    // ========================================================================

    void setRow(int row, const std::string& tag,
        const std::string& data, const std::string& state);
    void setTag(int row, const std::string& tag);
    void setData(int row, const std::string& data);
    void setState(int row, const std::string& state);

    // ========================================================================
    // Row Setters (by set/way)
    // ========================================================================

    void setBySetWay(int setIndex, int wayIndex,
        const std::string& tag, const std::string& data,
        const std::string& state);
    void setTagBySetWay(int setIndex, int wayIndex, const std::string& tag);
    void setDataBySetWay(int setIndex, int wayIndex, const std::string& data);
    void setStateBySetWay(int setIndex, int wayIndex, const std::string& state);

    /**
     * @brief Gets row data.
     * @param row Linear row index.
     * @return Const reference to row data.
     */
    const Row& getRow(int row) const { return m_rows[rowClamp(row)]; }

private:
    std::array<Row, kRows> m_rows{};

    /**
     * @brief Clamps row index to valid range.
     */
    static int rowClamp(int r) {
        if (r < 0) return 0;
        if (r >= kRows) return kRows - 1;
        return r;
    }

    /**
     * @brief Converts set/way to linear index.
     */
    static int idx(int setIndex, int wayIndex) {
        if (setIndex < 0) setIndex = 0;
        if (setIndex >= kSets) setIndex = kSets - 1;
        if (wayIndex < 0) wayIndex = 0;
        if (wayIndex >= kWaysPerSet) wayIndex = kWaysPerSet - 1;
        return setIndex * kWaysPerSet + wayIndex;
    }

    /**
     * @brief Derives MESI string from state field.
     * @param state State string (letter or 2-bit).
     * @return MESI letter or "?" if unknown.
     */
    static const char* deriveMESI(const std::string& state);
};