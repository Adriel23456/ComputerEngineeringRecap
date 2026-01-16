// ============================================================================
// File: include/core/fsm/State.h
// ============================================================================

#pragma once

/**
 * @file State.h
 * @brief Abstract base class for application states.
 *
 * Provides the interface and common functionality that all states
 * must implement. Each state represents a distinct screen or mode
 * in the application (menu, game, settings, etc.).
 *
 * @note Follows:
 *   - SRP: Only manages state lifecycle and dependency access
 *   - OCP: Open for extension via virtual methods
 *   - LSP: All derived states are substitutable
 */

#include <SFML/Graphics.hpp>

class StateManager;

/**
 * @class State
 * @brief Abstract base class for all application states.
 *
 * Defines the interface for state lifecycle management including
 * event handling, updates, and rendering.
 */
class State {
public:
    /**
     * @brief Constructs a state with required dependencies.
     * @param stateManager Pointer to the state manager for transitions.
     * @param window Pointer to the render window.
     */
    State(StateManager* stateManager, sf::RenderWindow* window);

    /**
     * @brief Virtual destructor for proper cleanup.
     */
    virtual ~State() = default;

    /**
     * @brief Handles input events for this state.
     * @param event The SFML event to process.
     */
    virtual void handleEvent(sf::Event& event) = 0;

    /**
     * @brief Updates state logic each frame.
     * @param deltaTime Time elapsed since last frame in seconds.
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Renders the complete state.
     */
    virtual void render() = 0;

    /**
     * @brief Renders only the background layer.
     *
     * Called when an overlay (like settings) is displayed on top.
     * Default implementation does nothing - override if needed.
     */
    virtual void renderBackground() {}

    /**
     * @brief Called when this state becomes active.
     *
     * Override to perform initialization when entering the state.
     */
    virtual void onEnter() {}

    /**
     * @brief Called when this state is about to be replaced.
     *
     * Override to perform cleanup when exiting the state.
     */
    virtual void onExit() {}

protected:
    /**
     * @brief Gets the current window size.
     * @return Window dimensions as unsigned integer vector.
     */
    sf::Vector2u getWindowSize() const;

    StateManager* m_stateManager;  ///< Manager for state transitions
    sf::RenderWindow* m_window;    ///< Window for rendering and input
};