// ============================================================================
// File: src/core/fsm/State.cpp
// ============================================================================

/**
 * @file State.cpp
 * @brief Implementation of the State base class.
 */

#include "core/fsm/State.h"
#include "core/fsm/StateManager.h"

State::State(StateManager* stateManager, sf::RenderWindow* window)
    : m_stateManager(stateManager)
    , m_window(window)
{
}

sf::Vector2u State::getWindowSize() const {
    return m_window ? m_window->getSize() : sf::Vector2u(0, 0);
}