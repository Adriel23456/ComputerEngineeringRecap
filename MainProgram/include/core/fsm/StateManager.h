// ============================================================================
// File: include/core/fsm/StateManager.h
// ============================================================================

#pragma once

#include "core/fsm/State.h"
#include <memory>

/**
 * @file StateManager.h
 * @brief State machine manager for application state transitions.
 *
 * Manages the lifecycle of application states using a queued transition
 * pattern to ensure safe state changes during the game loop.
 *
 * @note Follows:
 *   - SRP: Only manages state transitions
 *   - OCP: New states can be added without modifying this class
 */

 /**
  * @class StateManager
  * @brief Manages application state transitions safely.
  *
  * Implements a deferred state transition pattern where state changes
  * are queued and applied at safe points in the application loop,
  * preventing issues with mid-frame state changes.
  */
class StateManager {
public:
    /**
     * @brief Constructs the state manager with no active state.
     */
    StateManager();

    /**
     * @brief Destructor - automatically cleans up owned states.
     */
    ~StateManager();

    // Prevent copying - state manager owns unique resources
    StateManager(const StateManager&) = delete;
    StateManager& operator=(const StateManager&) = delete;

    /**
     * @brief Immediately sets the current state.
     *
     * Use with caution - prefer queueNextState() for safe transitions
     * during the main loop. This method is primarily for initial state setup.
     *
     * @param newState The state to set as current (ownership transferred).
     */
    void setCurrentState(std::unique_ptr<State> newState);

    /**
     * @brief Queues a state for transition at the next safe point.
     *
     * The queued state will be applied when applyNextState() is called,
     * typically at the end of the main loop iteration.
     *
     * @param newState The state to queue (ownership transferred).
     */
    void queueNextState(std::unique_ptr<State> newState);

    /**
     * @brief Gets the currently active state.
     * @return Pointer to current state, or nullptr if none.
     */
    State* getCurrentState() const;

    /**
     * @brief Checks if a state transition is pending.
     * @return true if a state is queued for transition.
     */
    bool hasNextState() const;

    /**
     * @brief Applies the queued state transition.
     *
     * If a state is queued, it becomes the current state and
     * the queue is cleared. Safe to call when no state is queued.
     */
    void applyNextState();

private:
    std::unique_ptr<State> m_currentState; ///< Currently active state
    std::unique_ptr<State> m_nextState;    ///< Queued state for deferred transition
};