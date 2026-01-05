#pragma once
#include <SFML/Graphics.hpp>

class StateManager;

/**
 * @interface IState
 * @brief Pure interface for application states following ISP principle.
 *
 * Defines the contract that all states must fulfill. Each state represents
 * a distinct screen or mode in the application (menu, game, settings, etc.).
 *
 * @note Follows Interface Segregation Principle - minimal required interface.
 */
class IState {
public:
    virtual ~IState() = default;

    /**
     * @brief Handles input events for this state.
     * @param event The SFML event to process.
     */
    virtual void handleEvent(const sf::Event& event) = 0;

    /**
     * @brief Updates state logic.
     * @param deltaTime Time elapsed since last frame in seconds.
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Renders the complete state to the target.
     * @param target The render target to draw on.
     */
    virtual void render(sf::RenderTarget& target) = 0;

    /**
     * @brief Renders only the background layer.
     *
     * Used when an overlay (like settings) is displayed on top.
     * Default implementation calls render().
     *
     * @param target The render target to draw on.
     */
    virtual void renderBackground(sf::RenderTarget& target) { render(target); }

    /**
     * @brief Called when this state becomes active.
     */
    virtual void onEnter() {}

    /**
     * @brief Called when this state is about to be replaced.
     */
    virtual void onExit() {}
};