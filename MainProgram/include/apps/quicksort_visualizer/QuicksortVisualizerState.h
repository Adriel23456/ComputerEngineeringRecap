// ============================================================================
// File: include/apps/quicksort_visualizer/QuicksortVisualizerState.h
// ============================================================================

#pragma once

/**
 * @file QuicksortVisualizerState.h
 * @brief Main state coordinator for Quicksort Visualization.
 *
 * Coordinates all components:
 * - Logic thread (OpenMP)
 * - Grid transform (pan/zoom math)
 * - Grid panel (rendering)
 * - Control panel (buttons)
 * - Input handler (mouse events)
 *
 * @note Design Principles:
 *   - SRP: Coordinates components, doesn't implement their logic
 *   - OCP: New components can be added without modification
 *   - DIP: Depends on component abstractions
 */

#include "core/fsm/State.h"
#include <memory>
#include <string>
#include <cstdint>

 // Forward declarations
namespace sf { class RenderWindow; }
class StateManager;

namespace quicksort {
    class LogicThreadController;
    class GridTransform;

    namespace ui {
        class GridPanel;
        class ControlPanel;
    }
    namespace input {
        class GridInputHandler;
    }
}

/**
 * @class QuicksortVisualizerState
 * @brief Main application state for Quicksort algorithm visualization.
 */
class QuicksortVisualizerState : public State {
public:
    // ========================================================================
    // Construction / Destruction
    // ========================================================================

    QuicksortVisualizerState(StateManager* stateManager, sf::RenderWindow* window);
    ~QuicksortVisualizerState() override;

    // Prevent copying
    QuicksortVisualizerState(const QuicksortVisualizerState&) = delete;
    QuicksortVisualizerState& operator=(const QuicksortVisualizerState&) = delete;

    // ========================================================================
    // State Interface Implementation
    // ========================================================================

    void handleEvent(sf::Event& event) override;
    void update(float deltaTime) override;
    void render() override;
    void renderBackground() override;

private:
    // ========================================================================
    // Initialization
    // ========================================================================

    bool initializeComponents();
    bool initializeLogicThread();
    bool initializeVisualization();
    void setupControlCallbacks();
    void logInitStatus(const std::string& componentName, bool success);

    // ========================================================================
    // Button Callbacks
    // ========================================================================

    void onRandomValues();
    void onSetAmount();
    void onStartQuicksort();
    void onResetView();

    // ========================================================================
    // Rendering
    // ========================================================================

    void renderShaderBackground();

    // ========================================================================
    // Components
    // ========================================================================

    // Threading
    std::unique_ptr<quicksort::LogicThreadController> m_logicThread;

    // Visualization logic (no UI dependencies)
    std::unique_ptr<quicksort::GridTransform> m_gridTransform;

    // UI components
    std::unique_ptr<quicksort::ui::GridPanel> m_gridPanel;
    std::unique_ptr<quicksort::ui::ControlPanel> m_controlPanel;

    // Input handling
    std::unique_ptr<quicksort::input::GridInputHandler> m_inputHandler;

    // State
    bool m_initialized;
    uint32_t m_elementCount;

    // Layout constants
    static constexpr float CONTROL_PANEL_HEIGHT_RATIO = 0.15f;
    static constexpr float PANEL_PADDING = 10.0f;
};