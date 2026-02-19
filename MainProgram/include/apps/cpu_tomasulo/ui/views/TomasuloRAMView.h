// ============================================================================
// File: include/apps/cpu_tomasulo/ui/views/TomasuloRAMView.h
// ============================================================================

#pragma once

/**
 * @file TomasuloRAMView.h
 * @brief RAM visualization and control view for the Tomasulo CPU.
 *
 * Displays 50 KB of RAM via TomasuloRamTable (paged, read-only display)
 * and provides two control buttons:
 *   - RESET RAM : clears all RAM via the registered reset callback.
 *   - Load .bin : opens a file dialog and dispatches the path to the
 *                 load-binary callback.
 *
 * Both mutations go through callbacks injected by
 * CpuTomasuloState::wireCallbacks() so this view never touches
 * simulation data directly.
 *
 * @note
 *   - SRP: Only handles RAM display layout and button UI.
 *   - OCP: New buttons can be added without modifying existing logic.
 *   - DIP: Mutations go through callback abstractions.
 */

#include "apps/cpu_tomasulo/ui/views/ITomasuloView.h"
#include "apps/cpu_tomasulo/ui/widgets/TomasuloRamTable.h"

#include <string>
#include <functional>

class TomasuloRAMView : public ITomasuloView {
public:
    TomasuloRAMView() = default;

    void update(float deltaTime) override;
    void render() override;

    // ── Widget Access ────────────────────────────────────────────
    TomasuloRamTable& getTable() { return m_table; }
    const TomasuloRamTable& getTable() const { return m_table; }

    // ── Callbacks ────────────────────────────────────────────────

    /**
     * @brief Registers the callback for the RESET RAM button.
     *
     * The callback should clear simulation RAM via the controller.
     */
    void setResetCallback(std::function<void()> callback);

    /**
     * @brief Registers the callback for the Load .bin button.
     *
     * The callback receives the selected file path and should load
     * the binary into simulation RAM at the upper address.
     */
    void setLoadBinaryCallback(std::function<void(const std::string&)> callback);

    // ── Status Display ───────────────────────────────────────────

    /**
     * @brief Sets the status-bar message and its auto-clear duration.
     * @param message          Text to display.
     * @param durationSeconds  Duration before the message clears.
     */
    void setStatusMessage(const std::string& message,
        float durationSeconds = 5.0f);

private:
    /** @brief Renders the Reset and Load buttons inside their child region. */
    void renderControlButtons(float availableWidth);

    // ── State ────────────────────────────────────────────────────
    TomasuloRamTable m_table;

    std::function<void()>                   m_resetCallback;
    std::function<void(const std::string&)> m_loadBinaryCallback;

    std::string m_statusMessage;
    float       m_statusTimer = 0.0f;
};