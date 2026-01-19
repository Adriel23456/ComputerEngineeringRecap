// ============================================================================
// File: src/apps/quicksort_visualizer/QuicksortVisualizerStateFactory.cpp
// ============================================================================

/**
 * @file QuicksortVisualizerStateFactory.cpp
 * @brief Factory function for creating QuicksortVisualizerState.
 *
 * Provides a clean factory interface to create the Quicksort Visualizer state
 * without exposing implementation details to the main menu.
 *
 * @note Follows:
 *   - DIP: Main menu depends on abstract State, not concrete implementation
 *   - SRP: Factory only creates the state, no other logic
 */

#include "apps/quicksort_visualizer/QuicksortVisualizerState.h"
#include "core/fsm/State.h"
#include <memory>

// Forward declaration
class AudioManager;

/**
 * @brief Creates a new QuicksortVisualizerState instance.
 *
 * @param stateManager Pointer to the application's state manager.
 * @param window Pointer to the main render window.
 * @param audio Pointer to the audio manager for SFX volume control.
 * @return Unique pointer to the newly created QuicksortVisualizerState.
 */
std::unique_ptr<State> CreateQuicksortVisualizerState(StateManager* stateManager,
    sf::RenderWindow* window,
    AudioManager* audio) {
    return std::make_unique<QuicksortVisualizerState>(stateManager, window, audio);
}