// ============================================================================
// File: include/apps/cpu_tomasulo/ui/widgets/ROBTable.h
// ============================================================================

#pragma once

/**
 * @file ROBTable.h
 * @brief Reorder Buffer visualization widget for the Tomasulo CPU.
 *
 * Displays a 32-entry circular ROB split into three regions:
 *   1. Status bar      — Head, Tail, Count, Full/Empty indicator.
 *   2. Main table      — One row per entry: Busy, Ready, Type, DestReg,
 *                        Value, PC, Exception, SourceStation.
 *   3. Detail panel    — Flags, Branch, and Store sub-fields for the
 *                        currently selected entry.
 *
 * Head is highlighted blue, Tail green, and the selected entry orange.
 *
 * Data is pushed entry-by-entry via getEntry() references by
 * CpuTomasuloState::syncROBView() while the simulation mutex is held.
 *
 * @note
 *   - SRP: Only handles ROB display and entry selection.
 *   - OCP: Entry count is a compile-time constant.
 */

#include <array>
#include <string>
#include <cstdint>

class ROBTable {
public:
    static constexpr int kEntryCount = 32;

    // ── ROB Entry ────────────────────────────────────────────────

    /** @brief All fields for a single ROB entry. */
    struct ROBEntry {
        // Core fields (main table)
        bool     busy = false;
        bool     ready = false;
        uint8_t  type = 0;   ///< 3-bit instruction type.
        uint8_t  destReg = 0;   ///< 4-bit destination register index.
        uint64_t value = 0;   ///< 64-bit result value.
        uint8_t  exception = 0;   ///< 4-bit exception code.
        uint64_t pc = 0;   ///< 64-bit program counter.
        uint8_t  sourceStation = 0;   ///< 4-bit source station ID.

        // Flags fields (detail panel)
        uint8_t  flagsResult = 0;   ///< 4-bit NZCV result.
        bool     flagsValid = false;
        bool     modifiesFlags = false;

        // Branch fields (detail panel)
        bool     predicted = false;
        bool     branchTaken = false;
        uint64_t branchTarget = 0;   ///< 64-bit branch target address.
        bool     mispredict = false;

        // Store fields (detail panel)
        uint64_t storeAddr = 0;   ///< 64-bit effective store address.
        uint64_t storeData = 0;   ///< 64-bit data to be written.
        bool     storeReady = false;
    };

    // ── Construction ─────────────────────────────────────────────
    ROBTable();

    // ── Data Access ──────────────────────────────────────────────

    /**
     * @brief Returns a mutable reference to an entry for in-place update.
     * @param index  Entry index [0, kEntryCount). Returns a dummy on out-of-range.
     */
    ROBEntry& getEntry(int index);
    const ROBEntry& getEntry(int index) const;

    void setHead(int head);    ///< Sets the ROB head pointer (wraps mod kEntryCount).
    void setTail(int tail);    ///< Sets the ROB tail pointer (wraps mod kEntryCount).
    void setCount(int count);  ///< Sets the live entry count (clamped to [0, kEntryCount]).

    int  getHead()  const { return m_head; }
    int  getTail()  const { return m_tail; }
    int  getCount() const { return m_count; }
    bool isFull()   const { return m_count == kEntryCount; }

    /** @brief Resets all entries, pointers, and the selection to their default state. */
    void resetAll();

    // ── Rendering ────────────────────────────────────────────────

    /**
     * @brief Renders the complete ROB widget (status bar + table + detail panel).
     * @param id  ImGui widget ID string.
     */
    void render(const char* id);

    // ── Entry Selection ──────────────────────────────────────────
    int  getSelectedEntry() const { return m_selectedEntry; }

    /** @brief Sets the selected entry index. Pass -1 to clear selection. */
    void setSelectedEntry(int idx);

private:
    std::array<ROBEntry, kEntryCount> m_entries;
    int m_head = 0;
    int m_tail = 0;
    int m_count = 0;
    int m_selectedEntry = -1;

    // ── Rendering Helpers ────────────────────────────────────────

    /** @brief Renders the Head / Tail / Count / Free status bar. */
    void renderStatusBar(float availableWidth);

    /**
     * @brief Renders the main 32-row entry table.
     * @param id           ImGui table ID.
     * @param tableHeight  Height available for the table.
     */
    void renderMainTable(const char* id, float tableHeight);

    /**
     * @brief Renders the flags / branch / store detail panel for the selected entry.
     * @param availableWidth  Width of the parent region.
     * @param panelHeight     Height available for the panel.
     */
    void renderDetailPanel(float availableWidth, float panelHeight);

    // ── String Conversion Helpers ────────────────────────────────
    static const char* typeToString(uint8_t type);           ///< e.g. "INT_ALU"
    static const char* destRegToString(uint8_t reg);         ///< e.g. "R3", "UPPER"
    static const char* exceptionToString(uint8_t exc);       ///< e.g. "Overflow"
    static const char* sourceStationToString(uint8_t ss);    ///< e.g. "LB0"

    /** @brief Formats a uint64 as trimmed hex (e.g. "0x1A00"). */
    static std::string formatHexTrimmed(uint64_t v);

    /** @brief Formats a uint64 as zero-padded 16-digit hex (e.g. "0x0000000000001A00"). */
    static std::string formatHexFull(uint64_t v);
};