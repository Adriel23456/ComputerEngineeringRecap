// ============================================================================
// File: include/apps/cpu_tomasulo/ui/views/TomasuloRegistersView.h
// ============================================================================

#pragma once

/**
 * @file TomasuloRegistersView.h
 * @brief Register file visualization view for the Tomasulo CPU.
 *
 * Wraps a TomasuloRegTable widget that reads 16 × 64-bit registers
 * directly from the simulation's TomasuloRegisterFile via a data-source
 * binding established by CpuTomasuloState::bindDataSources().
 *
 * No per-frame sync is needed — the table reads from the bound pointer
 * while the simulation mutex is held inside renderContentPanel.
 *
 * @note
 *   - SRP: Only handles register view layout.
 *   - DIP: Depends on ITomasuloView abstraction.
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/widgets/TomasuloRegTable.h"

class TomasuloRegistersView : public ITomasuloView {
public:
    TomasuloRegistersView() = default;

    void render() override;

    // ── Widget Access ────────────────────────────────────────────
    TomasuloRegTable& getTable() { return m_table; }
    const TomasuloRegTable& getTable() const { return m_table; }

private:
    TomasuloRegTable m_table;  ///< Register file visualization widget.
};