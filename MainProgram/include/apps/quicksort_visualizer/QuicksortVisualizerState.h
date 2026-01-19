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
 * - Element collection (sort data)
 * - Element renderer (visualization)
 * - Amount input popup (element count dialog)
 * - Swap animator (animation state machine)
 * - Swap sound generator (audio feedback)
 *
 * @note Design Principles:
 *   - SRP: Coordinates components, doesn't implement their logic
 *   - OCP: New components can be added without modification
 *   - DIP: Depends on component abstractions
 */

#include "core/fsm/State.h"
#include "apps/quicksort_visualizer/data/SwapOperation.h"
#include <memory>
#include <string>
#include <cstdint>

 // Forward declarations
namespace sf { class RenderWindow; }
class StateManager;

namespace quicksort {
    class LogicThreadController;
    class GridTransform;

    namespace data {
        class ElementCollection;
        class SwapQueue;
    }
    namespace ui {
        class GridPanel;
        class ControlPanel;
        class ElementRenderer;
        class AmountInputPopup;
    }
    namespace input {
        class GridInputHandler;
    }
    namespace animation {
        class SwapAnimator;
    }
    namespace audio {
        class SwapSoundGenerator;
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
    bool initializeDataStructures();
    bool initializeAnimation();
    bool initializeAudio();
    void setupControlCallbacks();
    void setupPopupCallbacks();
    void setupAnimatorCallbacks();
    void logInitStatus(const std::string& componentName, bool success);

    // ========================================================================
    // Button Callbacks
    // ========================================================================

    void onRandomValues();
    void onSetAmount();
    void onStartQuicksort();
    void onResetView();

    // ========================================================================
    // Popup Callbacks
    // ========================================================================

    void onAmountAccepted(uint32_t newAmount);
    void onAmountCancelled();

    // ========================================================================
    // Animation Callbacks
    // ========================================================================

    void onSwapData(uint32_t indexA, uint32_t indexB);
    void onPlaySwapSound(const quicksort::data::SwapOperation& swap);

    // ========================================================================
    // Sorting Logic
    // ========================================================================

    /**
     * @brief Processes swap queue and updates animations.
     * @param deltaTime Time since last update.
     */
    void processSortingUpdate(float deltaTime);

    /**
     * @brief Called when sorting completes.
     */
    void onSortingComplete();

    // ========================================================================
    // Helper Methods
    // ========================================================================

    void updateGridSize();
    void regenerateElements();

    /**
     * @brief Extracts values from elements for sorting.
     * @return Vector of double values.
     */
    std::vector<double> extractValuesForSorting() const;

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

    // Data structures
    std::unique_ptr<quicksort::data::ElementCollection> m_elements;

    // UI components
    std::unique_ptr<quicksort::ui::GridPanel> m_gridPanel;
    std::unique_ptr<quicksort::ui::ControlPanel> m_controlPanel;
    std::unique_ptr<quicksort::ui::ElementRenderer> m_elementRenderer;
    std::unique_ptr<quicksort::ui::AmountInputPopup> m_amountPopup;

    // Input handling
    std::unique_ptr<quicksort::input::GridInputHandler> m_inputHandler;

    // Animation
    std::unique_ptr<quicksort::animation::SwapAnimator> m_swapAnimator;

    // Audio
    std::unique_ptr<quicksort::audio::SwapSoundGenerator> m_soundGenerator;

    // State
    bool m_initialized;
    bool m_sortingActive;
    uint32_t m_elementCount;
    float m_currentElementWidth;
    float m_currentElementGap;

    // Layout constants
    static constexpr float CONTROL_PANEL_HEIGHT_RATIO = 0.15f;
    static constexpr float PANEL_PADDING = 10.0f;
};