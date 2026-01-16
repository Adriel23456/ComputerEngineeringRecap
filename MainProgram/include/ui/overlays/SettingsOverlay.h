// ============================================================================
// File: include/ui/overlays/SettingsOverlay.h
// ============================================================================

#pragma once

/**
 * @file SettingsOverlay.h
 * @brief Settings overlay UI coordinator.
 *
 * Manages the settings overlay window and coordinates between
 * different settings panels (video, audio, credits).
 *
 * @note Follows:
 *   - SRP: Coordinates overlay display, delegates to panels
 *   - OCP: New panels can be added without modifying existing code
 */

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

 // Forward declarations
class ConfigManager;
class AudioManager;
class VideoSettingsPanel;
class AudioSettingsPanel;
class CreditsPanel;

/**
 * @class SettingsOverlay
 * @brief Main settings overlay controller.
 *
 * Manages overlay visibility, animation, and panel switching.
 * Delegates actual settings UI to specialized panel classes.
 */
class SettingsOverlay {
public:
    /**
     * @brief Constructs the settings overlay.
     * @param window Pointer to render window.
     * @param config Pointer to configuration manager.
     * @param audio Pointer to audio manager.
     */
    SettingsOverlay(sf::RenderWindow* window, ConfigManager* config, AudioManager* audio);

    /**
     * @brief Destructor.
     */
    ~SettingsOverlay();

    // ========================================================================
    // Visibility Control
    // ========================================================================

    /** @brief Toggles overlay visibility. */
    void toggle();

    /** @brief Opens the overlay. */
    void open();

    /** @brief Closes the overlay. */
    void close();

    /** @brief Checks if overlay is visible. */
    bool isOpen() const { return m_isOpen; }

    // ========================================================================
    // Update Loop
    // ========================================================================

    /**
     * @brief Handles input events.
     * @param event SFML event to process.
     */
    void handleEvent(const sf::Event& event);

    /**
     * @brief Updates overlay animation.
     * @param deltaTime Time since last frame.
     */
    void update(float deltaTime);

    /**
     * @brief Renders the overlay.
     */
    void render();

    // ========================================================================
    // Window Recreation
    // ========================================================================

    /** @brief Rebuilds UI elements after window recreation. */
    void rebuildAfterRecreate();

    /**
     * @brief Checks and consumes apply request flag.
     * @return true if display settings were applied.
     */
    bool consumeApplyRequested();

private:
    /** @brief Renders the modal background with fade effect. */
    void renderModalBackground(float alpha);

    /** @brief Renders the main overlay window. */
    void renderMainWindow(float alpha);

    /** @brief Renders the settings view. */
    void renderSettingsView();

    /** @brief Renders the credits view. */
    void renderCreditsView();

    // Dependencies
    sf::RenderWindow* m_window;
    ConfigManager* m_config;
    AudioManager* m_audio;

    // Panels (composition for SRP)
    std::unique_ptr<VideoSettingsPanel> m_videoPanel;
    std::unique_ptr<AudioSettingsPanel> m_audioPanel;
    std::unique_ptr<CreditsPanel>       m_creditsPanel;

    // State
    bool  m_isOpen = false;
    bool  m_applyRequested = false;
    bool  m_showingCredits = false;
    float m_animationTimer = 0.0f;

    // UI constants
    static constexpr float BUTTON_HEIGHT = 56.0f;
    static constexpr float ANIMATION_SPEED = 3.0f;
};