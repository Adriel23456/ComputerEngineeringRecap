// ============================================================================
// File: include/apps/cpu_tomasulo/ui/widgets/CacheTable.h
// ============================================================================

#pragma once

/**
 * @file CacheTable.h
 * @brief Cache line visualization widget for the Tomasulo CPU.
 *
 * Displays a 2 KB, 4-way set-associative cache:
 *   - 8 sets × 4 ways × 64-byte lines × 55-bit tags
 *
 * The selected set/way is chosen via button navigation controls.
 * The active line is shown as 16 × 8-byte segments with tag, hex,
 * signed decimal, and IEEE 754 double columns.
 *
 * Shared by both I-Cache and D-Cache views (DRY).
 * Data is pushed in via setLineBySetWay() while the sim lock is held.
 *
 * @note
 *   - SRP: Only handles cache table display and set/way navigation.
 *   - OCP: Constants are compile-time; segment rendering is data-driven.
 *   - DRY: Single widget reused by TomasuloICacheView and TomasuloDCacheView.
 */

#include <array>
#include <string>
#include <cstdint>

class CacheTable {
public:
    // ── Geometry Constants ───────────────────────────────────────
    static constexpr int kSets = 8;                      ///< Number of sets.
    static constexpr int kWays = 4;                      ///< Ways per set.
    static constexpr int kTotalLines = kSets * kWays;          ///< 32 cache lines total.
    static constexpr int kLineSizeBytes = 64;                    ///< Bytes per cache line.
    static constexpr int kSegments = kLineSizeBytes / 8;     ///< 16 segments of 8 bytes.
    static constexpr int kTagBits = 55;                     ///< Tag width in bits.

    // ── Cache Line ───────────────────────────────────────────────

    /** @brief Stores the tag, raw data, and valid bit for one cache line. */
    struct CacheLine {
        std::string                        tag;   ///< 55-bit tag as a hex string.
        std::array<uint8_t, kLineSizeBytes> data; ///< 64 bytes of line data.
        bool                               valid; ///< Valid bit.

        CacheLine() : tag("0x00000000000000"), valid(false) { data.fill(0); }
    };

    // ── Construction ─────────────────────────────────────────────
    CacheTable() : m_selectedSet(0), m_selectedWay(0) { m_lines.fill(CacheLine()); }

    // ── Data Setters ─────────────────────────────────────────────

    /**
     * @brief Sets a cache line by its linear index.
     * @param index  Linear index [0, kTotalLines).
     * @param tag    Tag formatted as a hex string.
     * @param data   64-byte line data.
     * @param valid  Valid bit.
     */
    void setLine(int index, const std::string& tag,
        const std::array<uint8_t, kLineSizeBytes>& data,
        bool valid);

    /**
     * @brief Sets a cache line by set and way indices.
     * @param set    Set index [0, kSets).
     * @param way    Way index [0, kWays).
     * @param tag    Tag formatted as a hex string.
     * @param data   64-byte line data.
     * @param valid  Valid bit.
     */
    void setLineBySetWay(int set, int way, const std::string& tag,
        const std::array<uint8_t, kLineSizeBytes>& data,
        bool valid);

    /** @brief Resets all cache lines to invalid/zeroed state. */
    void resetAll();

    // ── Rendering ────────────────────────────────────────────────

    /**
     * @brief Renders the navigation controls and data table.
     * @param id  ImGui widget ID string.
     */
    void render(const char* id);

    // ── Navigation State ─────────────────────────────────────────
    int  getSelectedSet() const { return m_selectedSet; }
    int  getSelectedWay() const { return m_selectedWay; }

    void setSelectedSet(int set) { if (set >= 0 && set < kSets) m_selectedSet = set; }
    void setSelectedWay(int way) { if (way >= 0 && way < kWays) m_selectedWay = way; }

private:
    std::array<CacheLine, kTotalLines> m_lines;
    int m_selectedSet;
    int m_selectedWay;

    // ── Private Helpers ──────────────────────────────────────────

    /**
     * @brief Extracts 8 bytes from the line as a little-endian uint64.
     * @param data     Line data array.
     * @param segment  Segment index [0, kSegments).
     */
    static uint64_t extract8Bytes(const std::array<uint8_t, kLineSizeBytes>& data,
        int segment);

    /**
     * @brief Formats 8 bytes of a segment as a full 16-digit hex string.
     */
    static std::string format8BytesAsHex(const std::array<uint8_t, kLineSizeBytes>& data,
        int segment);

    /** @brief Returns a const reference to the currently selected cache line. */
    const CacheLine& getCurrentLine() const {
        return m_lines[m_selectedSet * kWays + m_selectedWay];
    }

    /**
     * @brief Renders the Set and Way button rows.
     * @param availableWidth  Width of the parent region.
     */
    void renderNavigationControls(float availableWidth);

    /**
     * @brief Renders the 16-segment data table for the active cache line.
     * @param id           ImGui table ID.
     * @param tableHeight  Height available for the table.
     */
    void renderDataTable(const char* id, float tableHeight);
};