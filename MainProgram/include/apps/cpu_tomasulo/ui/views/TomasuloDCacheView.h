#pragma once

/**
 * @file TomasuloDCacheView.h
 * @brief Data cache visualization view for Tomasulo CPU.
 *
 * Displays a 2KB, 4-way set-associative D-Cache with 8 sets,
 * 64-byte lines, and 55-bit tags. Reuses CacheTable widget.
 *
 * @note Follows:
 *   - SRP: Only handles D-Cache view layout
 *   - DRY: Reuses CacheTable (identical geometry to I-Cache)
 *   - LSP: Drop-in replacement for any ITomasuloView
 *   - DIP: Depends on ITomasuloView abstraction
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/widgets/CacheTable.h"

 /**
  * @class TomasuloDCacheView
  * @brief View for displaying the Tomasulo CPU data cache.
  *
  * Features:
  * - Set/Way navigation with colored selection buttons
  * - 16-segment data display per cache line (64 bytes)
  * - Tag, Hex, Decimal, Double columns
  */
class TomasuloDCacheView : public ITomasuloView {
public:
    TomasuloDCacheView() = default;

    /**
     * @brief Renders D-Cache with navigation.
     */
    void render() override;

    /**
     * @brief Provides direct access to the cache table widget.
     */
    CacheTable& getTable() { return m_table; }
    const CacheTable& getTable() const { return m_table; }

    /**
     * @brief Sets a cache line by set and way.
     * @param setIndex Set index (0-7).
     * @param wayIndex Way index (0-3).
     * @param tag Tag string (55-bit hex).
     * @param data Line data (64 bytes).
     * @param valid Valid bit.
     */
    void setBySetWay(int setIndex, int wayIndex,
        const std::string& tag,
        const std::array<uint8_t, CacheTable::kLineSizeBytes>& data,
        bool valid);

private:
    CacheTable m_table;  ///< Cache visualization widget
};