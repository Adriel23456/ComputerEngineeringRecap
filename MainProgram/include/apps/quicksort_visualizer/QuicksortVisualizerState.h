// ============================================================================
// File: include/apps/quicksort_visualizer/QuicksortVisualizerState.h
// ============================================================================

#pragma once

/**
 * @file QuicksortVisualizerState.h
 * @brief Main state coordinator for Quicksort Visualization.
 */

#include "core/fsm/State.h"
#include "apps/quicksort_visualizer/data/SwapOperation.h"
#include <memory>
#include <string>
#include <cstdint>
#include <vector>

 // Forward declarations
namespace sf { class RenderWindow; }
class StateManager;
class AudioManager;

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
    QuicksortVisualizerState(StateManager* stateManager, sf::RenderWindow* window,
        AudioManager* audioManager = nullptr);
    ~QuicksortVisualizerState() override;

    QuicksortVisualizerState(const QuicksortVisualizerState&) = delete;
    QuicksortVisualizerState& operator=(const QuicksortVisualizerState&) = delete;

    void handleEvent(sf::Event& event) override;
    void update(float deltaTime) override;
    void render() override;
    void renderBackground() override;

private:
    // Initialization
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

    // Button Callbacks
    void onRandomValues();
    void onSetAmount();
    void onStartQuicksort();
    void onResetView();

    // Popup Callbacks
    void onAmountAccepted(uint32_t newAmount);
    void onAmountCancelled();

    // Animation Callbacks
    void onSwapData(uint32_t indexA, uint32_t indexB);
    void onPlaySwapSound(const quicksort::data::SwapOperation& swap);
    void onPlayVerificationSound(uint32_t index, uint32_t total);
    void onVerificationComplete();

    // Sorting Logic
    void processSortingUpdate(float deltaTime);
    void onSortingComplete();
    void startVerificationSweep();

    // Helper Methods
    void updateGridSize();
    void regenerateElements();
    std::vector<double> extractValuesForSorting() const;

    // Rendering
    void renderShaderBackground();

    // Components
    std::unique_ptr<quicksort::LogicThreadController> m_logicThread;
    std::unique_ptr<quicksort::GridTransform> m_gridTransform;
    std::unique_ptr<quicksort::data::ElementCollection> m_elements;
    std::unique_ptr<quicksort::ui::GridPanel> m_gridPanel;
    std::unique_ptr<quicksort::ui::ControlPanel> m_controlPanel;
    std::unique_ptr<quicksort::ui::ElementRenderer> m_elementRenderer;
    std::unique_ptr<quicksort::ui::AmountInputPopup> m_amountPopup;
    std::unique_ptr<quicksort::input::GridInputHandler> m_inputHandler;
    std::unique_ptr<quicksort::animation::SwapAnimator> m_swapAnimator;
    std::unique_ptr<quicksort::audio::SwapSoundGenerator> m_soundGenerator;

    // External references
    AudioManager* m_audioManager;

    // State
    bool m_initialized;
    bool m_sortingActive;
    bool m_verificationActive;
    uint32_t m_elementCount;
    float m_currentElementWidth;
    float m_currentElementGap;

    // Layout constants
    static constexpr float CONTROL_PANEL_HEIGHT_RATIO = 0.15f;
    static constexpr float PANEL_PADDING = 10.0f;
};