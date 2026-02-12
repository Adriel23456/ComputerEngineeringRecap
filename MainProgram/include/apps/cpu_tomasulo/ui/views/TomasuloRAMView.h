#pragma once

/**
 * @file TomasuloRAMView.h
 * @brief RAM visualization and control view for Tomasulo CPU.
 *
 * Displays 50KB of RAM with paging, Reset, and Load controls.
 *
 * @note Follows:
 *   - SRP: Only handles RAM display and basic controls
 *   - OCP: New functionality (e.g., search, goto address) can be
 *          added without modifying existing code
 *   - DIP: Depends on ITomasuloView abstraction
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/widgets/TomasuloRamTable.h"

 /**
  * @class TomasuloRAMView
  * @brief View for displaying and managing 50KB Tomasulo RAM.
  *
  * Features:
  * - Paged RAM table (512 rows per page, 13 pages)
  * - Reset button (clears all RAM to zero)
  * - Load button (placeholder for binary file loading)
  * - Trimmed address display, full hex values
  */
class TomasuloRAMView : public ITomasuloView {
public:
    TomasuloRAMView() = default;

    /**
     * @brief Renders RAM table with paging and control buttons.
     */
    void render() override;

    /**
     * @brief Provides direct access to the RAM table widget.
     * @return Reference to the internal TomasuloRamTable.
     */
    TomasuloRamTable& getTable() { return m_table; }
    const TomasuloRamTable& getTable() const { return m_table; }

private:
    TomasuloRamTable m_table;  ///< Paged RAM visualization widget

    /**
     * @brief Renders the Reset and Load control buttons.
     * @param availableWidth Width available for the buttons.
     */
    void renderControlButtons(float availableWidth);
};