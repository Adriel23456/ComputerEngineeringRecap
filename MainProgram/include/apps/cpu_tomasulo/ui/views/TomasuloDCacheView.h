// ============================================================================
// File: include/apps/cpu_tomasulo/ui/views/TomasuloDCacheView.h
// ============================================================================

#pragma once

/**
 * @file TomasuloDCacheView.h
 * @brief Data cache visualization view for the Tomasulo CPU.
 *
 * Wraps a CacheTable widget configured for a 2 KB, 4-way set-associative
 * D-Cache (8 sets × 4 ways × 64-byte lines, 55-bit tags).
 *
 * Data is pushed in by CpuTomasuloState::syncDCacheView() each frame
 * while the simulation mutex is held. The view itself is stateless
 * with respect to the simulation.
 *
 * @note
 *   - SRP: Only handles D-Cache view layout and delegation to CacheTable.
 *   - DRY: Reuses CacheTable — geometry is identical to I-Cache.
 *   - LSP: Drop-in replacement for any ITomasuloView.
 *   - DIP: Depends on ITomasuloView abstraction.
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/widgets/CacheTable.h"

class TomasuloDCacheView : public ITomasuloView {
public:
    TomasuloDCacheView() = default;

    void render() override;

    // ── Widget Access ────────────────────────────────────────────
    CacheTable& getTable() { return m_table; }
    const CacheTable& getTable() const { return m_table; }

    /**
     * @brief Pushes a single cache line into the widget.
     * @param setIndex  Set index (0–7).
     * @param wayIndex  Way index (0–3).
     * @param tag       55-bit tag formatted as a hex string.
     * @param data      64-byte line data.
     * @param valid     Valid bit for this line.
     */
    void setBySetWay(int setIndex, int wayIndex,
        const std::string& tag,
        const std::array<uint8_t, CacheTable::kLineSizeBytes>& data,
        bool valid);

private:
    CacheTable m_table;  ///< Cache visualization widget.
};