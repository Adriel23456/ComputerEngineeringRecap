#pragma once

/**
 * @file TomasuloRegistersView.h
 * @brief Register file visualization view for Tomasulo CPU.
 *
 * Displays 16 registers via TomasuloRegTable, which reads
 * directly from the simulation's TomasuloRegisterFile.
 *
 * @note
 *   - SRP: Only handles register view layout
 *   - DIP: Depends on ITomasuloView abstraction
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/widgets/TomasuloRegTable.h"

class TomasuloRegistersView : public ITomasuloView {
public:
    TomasuloRegistersView() = default;

    void render() override;

    TomasuloRegTable& getTable() { return m_table; }
    const TomasuloRegTable& getTable() const { return m_table; }

private:
    TomasuloRegTable m_table;
};