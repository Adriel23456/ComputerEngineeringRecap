#pragma once

/**
 * @file TomasuloROBView.h
 * @brief Reorder Buffer visualization view for Tomasulo CPU.
 *
 * Displays a 32-entry ROB with status indicators,
 * a core-fields table, and a detail panel for selected entries.
 *
 * @note Follows:
 *   - SRP: Only handles ROB view layout
 *   - LSP: Drop-in replacement for any ITomasuloView
 *   - DIP: Depends on ITomasuloView abstraction
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/widgets/ROBTable.h"

 /**
  * @class TomasuloROBView
  * @brief View for displaying the Tomasulo CPU reorder buffer.
  *
  * Features:
  * - Head/Tail/Count status bar
  * - 32-entry table with color-coded Head/Tail rows
  * - Click-to-select entry detail panel (flags, branch, store)
  */
class TomasuloROBView : public ITomasuloView {
public:
    TomasuloROBView() = default;

    /**
     * @brief Renders ROB with status, table, and detail panel.
     */
    void render() override;

    /**
     * @brief Provides direct access to the ROB table widget.
     */
    ROBTable& getTable() { return m_table; }
    const ROBTable& getTable() const { return m_table; }

private:
    ROBTable m_table;  ///< ROB visualization widget
};