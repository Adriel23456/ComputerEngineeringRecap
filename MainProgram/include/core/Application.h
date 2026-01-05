#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

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
 * @note Follows:
 *   - SRP: Only coordinates high-level application flow
 *   - DIP: Depends on manager abstractions (can be injected)
 */
class Application {
public:
    /**
     * @brief Constructs the application with configuration path.
     * @param configPath Path to the configuration JSON file.
     */
    explicit Application(const std::string& configPath);

    ~Application();

    // Prevent copying - application is a singleton-like resource
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

    /** @brief Initializes the ImGui context. */
    void initImGui();

    // Core systems
    std::string m_configPath;
    sf::RenderWindow m_window;                          ///< Main render window (value, not pointer)
    std::unique_ptr<ConfigManager> m_config;            ///< Configuration manager
    std::unique_ptr<AudioManager> m_audio;              ///< Audio system
    std::unique_ptr<StateManager> m_stateManager;       ///< State machine
    std::unique_ptr<SettingsOverlay> m_settingsOverlay; ///< Settings UI overlay

    sf::Clock m_clock;      ///< Delta time clock
    bool m_running = false; ///< Application running flag
};