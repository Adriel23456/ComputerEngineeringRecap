#pragma once

/**
 * @file TomasuloRegistersView.h
 * @brief Register file visualization view for Tomasulo CPU.
 *
 * Displays 16 registers (REG0–REG12, UPPER, LOWER, PEID).
 *
 * @note Follows:
 *   - SRP: Only handles register view layout
 *   - LSP: Drop-in replacement for any ITomasuloView
 *   - DIP: Depends on ITomasuloView abstraction
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/widgets/TomasuloRegTable.h"

 /**
  * @class TomasuloRegistersView
  * @brief View for displaying Tomasulo CPU registers.
  *
  * Features:
  * - 16 registers with 4-bit binary codes
  * - Hex, Decimal, Double columns
  * - Context menu to copy values
  */
class TomasuloRegistersView : public ITomasuloView {
public:
    TomasuloRegistersView() = default;

    /**
     * @brief Renders the register view.
     */
    void render() override;

    /**
     * @brief Provides direct access to the register table widget.
     */
    TomasuloRegTable& getTable() { return m_table; }
    const TomasuloRegTable& getTable() const { return m_table; }

private:
    TomasuloRegTable m_table;  ///< Register table widget
};