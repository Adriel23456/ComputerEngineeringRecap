// ============================================================================
// File: include/states/ProgramState.h
// ============================================================================

#pragma once

/**
 * @file ProgramState.h
 * @brief Generic program state placeholder.
 *
 * Provides a stub state for programs that haven't been
 * fully implemented yet.
 *
 * @note Follows:
 *   - SRP: Only displays placeholder UI
 *   - LSP: Properly extends State base class
 */

#include "core/fsm/State.h"
#include <string>

 /**
  * @class ProgramState
  * @brief Placeholder state for unimplemented programs.
  *
  * Displays a simple UI indicating the program name and
  * providing basic navigation hints.
  */
class ProgramState : public State {
public:
    /**
     * @brief Constructs a program state with a title.
     * @param stateManager Pointer to state manager.
     * @param window Pointer to render window.
     * @param title Display title for the program.
     */
    ProgramState(StateManager* stateManager, sf::RenderWindow* window, std::string title);

    /**
     * @brief Handles input events.
     * @param event SFML event to process.
     */
    void handleEvent(sf::Event& event) override;

    /**
     * @brief Updates state logic.
     * @param deltaTime Time since last frame.
     */
    void update(float deltaTime) override;

    /**
     * @brief Renders the placeholder UI.
     */
    void render() override;

    /**
     * @brief Renders a solid background color.
     */
    void renderBackground() override;

private:
    std::string m_title; ///< Program display title
};