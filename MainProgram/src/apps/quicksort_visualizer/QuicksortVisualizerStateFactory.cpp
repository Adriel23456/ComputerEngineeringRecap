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

 /**
  * @brief Creates a new QuicksortVisualizerState instance.
  *
  * This factory function is declared in the main menu state to avoid
  * tight coupling between the menu and the visualizer implementation.
  *
  * @param stateManager Pointer to the application's state manager.
  * @param window Pointer to the main render window.
  * @return Unique pointer to the newly created QuicksortVisualizerState.
  */
std::unique_ptr<State> CreateQuicksortVisualizerState(StateManager* stateManager,
    sf::RenderWindow* window) {
    return std::make_unique<QuicksortVisualizerState>(stateManager, window);
}