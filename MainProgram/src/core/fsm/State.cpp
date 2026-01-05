#include "core/fsm/State.h"
#include "core/fsm/StateManager.h"

/**
 * @brief Constructs the base state with essential dependencies.
 * 
 * @param stateManager Pointer to the application's state manager.
 * @param window Pointer to the main render window.
 */
State::State(StateManager* stateManager, sf::RenderWindow* window)
    : m_stateManager(stateManager)
    , m_window(window)
{
}

/**
 * @brief Returns current window dimensions.
 * @return Window size as unsigned integer vector.
 */
sf::Vector2u State::getWindowSize() const {
    return m_window ? m_window->getSize() : sf::Vector2u(0, 0);
}