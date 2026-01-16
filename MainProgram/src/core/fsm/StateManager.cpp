// ============================================================================
// File: src/core/fsm/StateManager.cpp
// ============================================================================

/**
 * @file StateManager.cpp
 * @brief Implementation of the StateManager class.
 *
 * Provides safe state transition management using a deferred
 * transition pattern to prevent mid-frame state change issues.
 */

#include "core/fsm/StateManager.h"

 /**
  * @brief Constructs state manager with null states.
  */
StateManager::StateManager()
    : m_currentState(nullptr)
    , m_nextState(nullptr)
{
}

/**
 * @brief Destructor - unique_ptr handles cleanup automatically.
 */
StateManager::~StateManager() = default;

/**
 * @brief Immediately replaces the current state.
 * @param newState New state to set (ownership transferred).
 */
void StateManager::setCurrentState(std::unique_ptr<State> newState) {
    m_currentState = std::move(newState);
}

/**
 * @brief Queues a state for deferred transition.
 * @param newState State to queue (ownership transferred).
 */
void StateManager::queueNextState(std::unique_ptr<State> newState) {
    m_nextState = std::move(newState);
}

/**
 * @brief Checks for pending state transition.
 * @return true if a state is queued.
 */
bool StateManager::hasNextState() const {
    return static_cast<bool>(m_nextState);
}

/**
 * @brief Applies queued state if present.
 *
 * Transfers ownership from queue to current state.
 * Queue is automatically cleared after transfer.
 */
void StateManager::applyNextState() {
    if (m_nextState) {
        setCurrentState(std::move(m_nextState));
    }
}

/**
 * @brief Returns pointer to current state.
 * @return Raw pointer to current state (non-owning).
 */
State* StateManager::getCurrentState() const {
    return m_currentState.get();
}