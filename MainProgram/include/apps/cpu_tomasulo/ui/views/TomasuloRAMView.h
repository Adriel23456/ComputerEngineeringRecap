#pragma once

/**
 * @file TomasuloRAMView.h
 * @brief RAM visualization and control view for Tomasulo CPU.
 *
 * Displays 50KB of RAM with paging, plus control buttons:
 * - Reset: clears all RAM via async callback
 * - Load:  opens file dialog for .bin, dispatches to async callback
 *
 * @note
 *   - SRP: Only handles RAM display layout and button UI
 *   - OCP: New buttons can be added without modifying existing code
 *   - DIP: Mutations go through callbacks, not direct data writes
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/widgets/TomasuloRamTable.h"

#include <string>
#include <functional>

class TomasuloRAMView : public ITomasuloView {
public:
    TomasuloRAMView() = default;

    void render() override;

    // ── Widget access ───────────────────────────────────────────

    TomasuloRamTable& getTable() { return m_table; }
    const TomasuloRamTable& getTable() const { return m_table; }

    // ── Callbacks ───────────────────────────────────────────────

    /**
     * @brief Sets callback for the Reset button.
     *
     * The callback should clear simulation RAM (e.g., via the controller).
     */
    void setResetCallback(std::function<void()> callback);

    /**
     * @brief Sets callback for the Load button.
     *
     * The callback receives the selected .bin file path.
     * It should load the binary into simulation RAM at the UPPER address.
     */
    void setLoadBinaryCallback(std::function<void(const std::string&)> callback);

    // ── Status display ──────────────────────────────────────────

    void setStatusMessage(const std::string& message,
        float durationSeconds = 5.0f);

    void update(float deltaTime) override;

private:
    TomasuloRamTable m_table;

    std::function<void()>                  m_resetCallback;
    std::function<void(const std::string&)> m_loadBinaryCallback;

    std::string m_statusMessage;
    float       m_statusTimer = 0.0f;

    void renderControlButtons(float availableWidth);
};