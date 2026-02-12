#pragma once

/**
 * @file ROBTable.h
 * @brief Reorder Buffer visualization table widget for Tomasulo CPU.
 *
 * Displays a 32-entry circular ROB with Head/Tail pointers,
 * a main table of core fields, and a detail panel for the
 * selected entry's branch, store, and flags information.
 *
 * @note Follows:
 *   - SRP: Only handles ROB table display and entry selection
 *   - OCP: Entry count is configurable via constant
 *   - Encapsulation: Internal layout logic is private
 */

#include <array>
#include <string>
#include <cstdint>

 /**
  * @class ROBTable
  * @brief Interactive ROB viewer with entry selection and detail panel.
  *
  * Features:
  * - 32 entries with Head/Tail/Count status bar
  * - Core fields table: Busy, Ready, Type, DestReg, Value, PC, Exception
  * - Detail panel for selected entry (branch, store, flags)
  * - Head row highlighted blue, Tail row highlighted green
  */
class ROBTable {
public:
    static constexpr int kEntryCount = 32;

    // ════════════════════════════════════════════════════════════
    // ROB Entry
    // ════════════════════════════════════════════════════════════

    /**
     * @struct ROBEntry
     * @brief Single ROB entry with all fields.
     */
    struct ROBEntry {
        // Core fields
        bool        busy = false;
        bool        ready = false;
        uint8_t     type = 0;       ///< 3-bit instruction type
        uint8_t     destReg = 0;       ///< 4-bit destination register
        uint64_t    value = 0;       ///< 64-bit result value
        uint8_t     exception = 0;       ///< 4-bit exception code
        uint64_t    pc = 0;       ///< 64-bit program counter
        uint8_t     sourceStation = 0;       ///< 4-bit source station ID

        // Flags fields
        uint8_t     flagsResult = 0;       ///< 4-bit flags result
        bool        flagsValid = false;
        bool        modifiesFlags = false;

        // Branch fields
        bool        predicted = false;
        bool        branchTaken = false;
        uint64_t    branchTarget = 0;       ///< 64-bit branch target address
        bool        mispredict = false;

        // Store fields
        uint64_t    storeAddr = 0;       ///< 64-bit store address
        uint64_t    storeData = 0;       ///< 64-bit store data
        bool        storeReady = false;
    };

    // ════════════════════════════════════════════════════════════
    // Construction
    // ════════════════════════════════════════════════════════════

    ROBTable();

    // ════════════════════════════════════════════════════════════
    // Data Access
    // ════════════════════════════════════════════════════════════

    /**
     * @brief Gets a mutable reference to an entry.
     * @param index Entry index (0–31).
     */
    ROBEntry& getEntry(int index);
    const ROBEntry& getEntry(int index) const;

    /**
     * @brief Sets Head pointer.
     */
    void setHead(int head);

    /**
     * @brief Sets Tail pointer.
     */
    void setTail(int tail);

    /**
     * @brief Sets entry count.
     */
    void setCount(int count);

    int getHead()  const { return m_head; }
    int getTail()  const { return m_tail; }
    int getCount() const { return m_count; }
    bool isFull()  const { return m_count == kEntryCount; }

    /**
     * @brief Resets all entries and pointers.
     */
    void resetAll();

    // ════════════════════════════════════════════════════════════
    // Rendering
    // ════════════════════════════════════════════════════════════

    /**
     * @brief Renders the complete ROB widget.
     * @param id ImGui widget ID.
     */
    void render(const char* id);

    /**
     * @brief Gets/sets the selected entry index (-1 = none).
     */
    int  getSelectedEntry() const { return m_selectedEntry; }
    void setSelectedEntry(int idx);

private:
    std::array<ROBEntry, kEntryCount> m_entries;
    int m_head = 0;
    int m_tail = 0;
    int m_count = 0;
    int m_selectedEntry = -1;

    // ════════════════════════════════════════════════════════════
    // Rendering Helpers
    // ════════════════════════════════════════════════════════════

    /**
     * @brief Renders the Head/Tail/Count/Full status bar.
     */
    void renderStatusBar(float availableWidth);

    /**
     * @brief Renders the main 32-entry table.
     * @param id ImGui table ID.
     * @param tableHeight Available height.
     */
    void renderMainTable(const char* id, float tableHeight);

    /**
     * @brief Renders the detail panel for the selected entry.
     * @param panelHeight Available height.
     */
    void renderDetailPanel(float availableWidth, float panelHeight);

    /**
     * @brief Converts instruction type code to display string.
     */
    static const char* typeToString(uint8_t type);

    /**
     * @brief Converts destination register code to display string.
     */
    static const char* destRegToString(uint8_t reg);

    /**
     * @brief Converts exception code to display string.
     */
    static const char* exceptionToString(uint8_t exc);

    /**
     * @brief Converts source station code to display string.
     */
    static const char* sourceStationToString(uint8_t ss);

    /**
     * @brief Formats a 64-bit value as trimmed hex (like RAM addresses).
     */
    static std::string formatHexTrimmed(uint64_t v);

    /**
     * @brief Formats a 64-bit value as full 16-digit hex.
     */
    static std::string formatHexFull(uint64_t v);
};