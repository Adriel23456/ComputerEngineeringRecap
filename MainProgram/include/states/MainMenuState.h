// ============================================================================
// File: include/states/MainMenuState.h
// ============================================================================

#pragma once

/**
 * @file MainMenuState.h
 * @brief Main menu state for application entry.
 *
 * Displays the main menu with program selection options
 * and an animated shader background.
 *
 * @note Follows:
 *   - SRP: Only handles main menu display and navigation
 *   - LSP: Properly extends State base class
 */

#include "core/fsm/State.h"
#include <vector>
#include <string>

// Forward declarations
class AudioManager;

 /**
  * @class MainMenuState
  * @brief Application main menu state.
  *
  * Provides the entry point UI where users can select
  * which program/simulation to run.
  */
class MainMenuState : public State {
public:
    /**
     * @brief Constructs the main menu state.
     * @param stateManager Pointer to state manager for transitions.
     * @param window Pointer to render window.
     */
    MainMenuState(StateManager* stateManager, sf::RenderWindow* window,
        AudioManager* audio = nullptr);

    ~MainMenuState() override = default;

    /**
     * @brief Handles input events.
     * @param event SFML event to process.
     */
    void handleEvent(sf::Event& event) override;

    /**
     * @brief Updates menu state.
     * @param deltaTime Time since last frame.
     */
    void update(float deltaTime) override;

    /**
     * @brief Renders the complete menu.
     */
    void render() override;

    /**
     * @brief Renders only the animated background.
     */
    void renderBackground() override;

private:
    /**
     * @brief Creates an auto-fitting button for menu items.
     * @param label Button text.
     * @param targetWidth Desired button width.
     * @param height Button height.
     * @return true if button was clicked.
     */
    bool createAutoFitButton(const char* label, float targetWidth, float height);

    /**
     * @brief Renders the menu UI panel.
     */
    void renderMenuPanel();

    /**
     * @brief Handles menu item selection.
     * @param itemName Name of the selected menu item.
     */
    void handleMenuSelection(const std::string& itemName);

    std::vector<std::string> m_menuItems; ///< List of menu options
    AudioManager* m_audio;  ///< Pointer to audio manager for passing to child states
};