// ============================================================================
// File: include/apps/cpu_tomasulo/ui/views/TomasuloROBView.h
// ============================================================================

#pragma once

/**
 * @file TomasuloROBView.h
 * @brief Reorder Buffer visualization view for the Tomasulo CPU.
 *
 * Wraps a ROBTable widget that displays a 32-entry ROB with:
 *   - Head / Tail / Count status bar
 *   - Core-fields table with color-coded Head and Tail rows
 *   - Click-to-select detail panel (flags, branch, store fields)
 *
 * Data is pushed in by CpuTomasuloState::syncROBView() each frame
 * while the simulation mutex is held.
 *
 * @note
 *   - SRP: Only handles ROB view layout and delegation to ROBTable.
 *   - LSP: Drop-in replacement for any ITomasuloView.
 *   - DIP: Depends on ITomasuloView abstraction.
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/widgets/ROBTable.h"

class TomasuloROBView : public ITomasuloView {
public:
    TomasuloROBView() = default;

    void render() override;

    // ── Widget Access ────────────────────────────────────────────
    ROBTable& getTable() { return m_table; }
    const ROBTable& getTable() const { return m_table; }

private:
    ROBTable m_table;  ///< ROB visualization widget.
};