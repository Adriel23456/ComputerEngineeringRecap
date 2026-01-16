// ============================================================================
// File: include/core/Application.h
// ============================================================================

#pragma once

/**
 * @file Application.h
 * @brief Main application controller definition.
 *
 * Provides the central coordination point for all application
 * subsystems following the Single Responsibility Principle.
 */

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

 // Forward declarations to minimize header dependencies (DIP)
class StateManager;
class ConfigManager;
class AudioManager;
class SettingsOverlay;

/**
 * @class Application
 * @brief Main application controller following SRP.
 *
 * Orchestrates the application lifecycle: initialization, main loop,
 * and shutdown. Delegates specific responsibilities to specialized
 * manager classes.
 *
 * @note Design principles applied:
 *   - SRP: Only coordinates high-level application flow
 *   - DIP: Depends on manager abstractions (can be injected)
 *   - OCP: New systems can be added without modifying existing code
 */
class Application {
public:
    /**
     * @brief Constructs the application with configuration path.
     * @param configPath Path to the configuration JSON file.
     */
    explicit Application(const std::string& configPath);

    /**
     * @brief Destructor - ensures clean shutdown.
     */
    ~Application();

    // Prevent copying - application owns unique system resources
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    /**
     * @brief Initializes all subsystems.
     * @return true if initialization succeeded, false otherwise.
     */
    bool initialize();

    /**
     * @brief Runs the main application loop.
     * @return Exit code (0 for success).
     */
    int run();

    /**
     * @brief Cleanly shuts down all subsystems.
     */
    void shutdown();

private:
    /** @brief Processes all pending window events. */
    void processEvents();

    /** @brief Updates all active systems and states. */
    void update(float deltaTime);

    /** @brief Renders the current frame. */
    void render();

    /** @brief Recreates the window after settings change. */
    void recreateWindow();

    /** @brief Initializes the ImGui context and styling. */
    void initImGui();

    // Configuration
    std::string m_configPath; ///< Path to configuration file

    // Core systems (order matters for initialization/destruction)
    sf::RenderWindow m_window;                          ///< Main render window
    std::unique_ptr<ConfigManager> m_config;            ///< Configuration manager
    std::unique_ptr<AudioManager> m_audio;              ///< Audio system manager
    std::unique_ptr<StateManager> m_stateManager;       ///< Application state machine
    std::unique_ptr<SettingsOverlay> m_settingsOverlay; ///< Settings UI overlay

    // Runtime state
    sf::Clock m_clock;      ///< Delta time measurement clock
    bool m_running = false; ///< Application running flag
};