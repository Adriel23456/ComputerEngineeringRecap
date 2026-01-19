// ============================================================================
// File: src/core/Application.cpp
// ============================================================================

/**
 * @file Application.cpp
 * @brief Implementation of the Application class.
 *
 * Contains the main application loop and subsystem coordination.
 * All specific functionality is delegated to specialized managers.
 */

#include "core/Application.h"
#include "core/fsm/StateManager.h"
#include "core/config/ConfigManager.h"
#include "systems/audio/AudioManager.h"
#include "ui/overlays/SettingsOverlay.h"
#include "states/MainMenuState.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imguiThemes.h"
#include <iostream>
#include <algorithm>

 // ============================================================================
 // Construction / Destruction
 // ============================================================================

Application::Application(const std::string& configPath)
    : m_configPath(configPath)
{
}

Application::~Application() {
    shutdown();
}

// ============================================================================
// Initialization
// ============================================================================

/**
 * @brief Initializes all application subsystems in correct order.
 *
 * Initialization order is critical:
 * 1. Configuration (needed by all other systems)
 * 2. Window (needed for rendering)
 * 3. ImGui (needs window)
 * 4. Audio (independent but needs config)
 * 5. State Manager (needs window)
 * 6. Overlay (needs all above)
 *
 * @return true if all systems initialized successfully.
 */
bool Application::initialize() {
    // Step 1: Load configuration
    m_config = std::make_unique<ConfigManager>();
    if (!m_config->load(m_configPath)) {
        std::cerr << "[Application] Config not found, using defaults.\n";
        m_config->setDefault();
        m_config->save(m_configPath);
    }

    // Step 2: Create render window
    sf::VideoMode videoMode(
        m_config->getResolutionWidth(),
        m_config->getResolutionHeight()
    );
    m_window.create(
        videoMode,
        "Programs of Computer Engineering",
        m_config->getWindowStyle()
    );
    m_window.setFramerateLimit(m_config->getFramerate());
    m_window.setVerticalSyncEnabled(m_config->isVSyncEnabled());

    // Step 3: Initialize ImGui
    initImGui();

    // Step 4: Initialize audio system
    m_audio = std::make_unique<AudioManager>();
    m_audio->setBGMVolume(m_config->getBGMVolume() / 100.f);
    m_audio->setSFXVolume(m_config->getSFXVolume() / 100.f);
    m_audio->init("Music/Mainmenu.mp3");
    m_audio->loadSFX("enterSettings", "SoundEffects/Settings.wav");
    m_audio->loadSFX("exitSettings", "SoundEffects/Settings.wav");
    m_audio->playBGM(true);

    // Step 5: Initialize state manager with initial state
    m_stateManager = std::make_unique<StateManager>();
    m_stateManager->setCurrentState(
        std::make_unique<MainMenuState>(m_stateManager.get(), &m_window, m_audio.get())
    );

    // Step 6: Create settings overlay
    m_settingsOverlay = std::make_unique<SettingsOverlay>(
        &m_window, m_config.get(), m_audio.get()
    );

    m_running = true;
    std::cout << "[Application] Initialization complete.\n";
    return true;
}

// ============================================================================
// Main Loop
// ============================================================================

/**
 * @brief Main application loop.
 *
 * Executes the standard game loop pattern:
 * Process Events -> Update -> Render -> Repeat
 *
 * @return Exit code (0 for normal termination).
 */
int Application::run() {
    if (!m_running) {
        std::cerr << "[Application] Cannot run - not initialized.\n";
        return -1;
    }

    while (m_window.isOpen()) {
        // Calculate delta time with safety clamp to prevent physics issues
        sf::Time dt = m_clock.restart();
        float deltaTime = std::clamp(dt.asSeconds(), 0.f, 1.f);

        processEvents();
        update(deltaTime);
        render();

        // Handle deferred window recreation (after settings apply)
        if (m_settingsOverlay->consumeApplyRequested()) {
            recreateWindow();
        }

        // Apply pending state transitions at safe point
        if (m_stateManager->hasNextState()) {
            m_stateManager->applyNextState();
        }
    }

    return 0;
}

// ============================================================================
// Event Processing
// ============================================================================

/**
 * @brief Processes all pending SFML events.
 *
 * Events are routed to appropriate handlers:
 * - System events (close, resize) handled here
 * - Overlay toggle on Escape key
 * - Other events forwarded to current state (if overlay closed)
 */
void Application::processEvents() {
    ImGuiIO& io = ImGui::GetIO();
    sf::Event event;

    while (m_window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(m_window, event);

        // Handle window close request
        if (event.type == sf::Event::Closed) {
            m_window.close();
            continue;
        }

        // Handle window resize - update viewport
        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(
                0.f, 0.f,
                static_cast<float>(event.size.width),
                static_cast<float>(event.size.height)
            );
            m_window.setView(sf::View(visibleArea));
            continue;
        }

        // Toggle settings overlay with Escape key
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            m_settingsOverlay->toggle();
            continue;
        }

        // Forward events to current state when appropriate
        bool imguiWantsInput = io.WantCaptureMouse || io.WantCaptureKeyboard;
        if (!m_settingsOverlay->isOpen() && !imguiWantsInput) {
            if (auto* state = m_stateManager->getCurrentState()) {
                state->handleEvent(event);
            }
        }
    }
}

// ============================================================================
// Update
// ============================================================================

/**
 * @brief Updates all active systems.
 * @param deltaTime Time since last frame in seconds.
 */
void Application::update(float deltaTime) {
    ImGui::SFML::Update(m_window, sf::seconds(deltaTime));

    // Update current state only when overlay is closed
    if (!m_settingsOverlay->isOpen()) {
        if (auto* state = m_stateManager->getCurrentState()) {
            state->update(deltaTime);
        }
    }

    m_settingsOverlay->update(deltaTime);
}

// ============================================================================
// Rendering
// ============================================================================

/**
 * @brief Renders the current frame.
 *
 * Rendering order:
 * 1. Clear buffer
 * 2. State (full or background only depending on overlay)
 * 3. Settings overlay
 * 4. ImGui
 * 5. Display
 */
void Application::render() {
    m_window.clear();

    if (auto* state = m_stateManager->getCurrentState()) {
        if (m_settingsOverlay->isOpen()) {
            state->renderBackground();
        }
        else {
            state->render();
        }
    }

    m_settingsOverlay->render();
    ImGui::SFML::Render(m_window);
    m_window.display();
}

// ============================================================================
// Window Management
// ============================================================================

/**
 * @brief Recreates the window with current configuration.
 *
 * Required when display settings change (resolution, fullscreen, etc.).
 * ImGui context must be recreated as well.
 */
void Application::recreateWindow() {
    ImGui::SFML::Shutdown();

    sf::VideoMode videoMode(
        m_config->getResolutionWidth(),
        m_config->getResolutionHeight()
    );
    m_window.create(
        videoMode,
        "Programs of Computer Engineering",
        m_config->getWindowStyle()
    );
    m_window.setFramerateLimit(m_config->getFramerate());
    m_window.setVerticalSyncEnabled(m_config->isVSyncEnabled());

    initImGui();
    m_settingsOverlay->rebuildAfterRecreate();
}

/**
 * @brief Initializes ImGui with application styling.
 *
 * Sets up the ImGui context, applies custom theme,
 * and configures input handling.
 */
void Application::initImGui() {
    ImGui::SFML::Init(m_window);
    ImGui::StyleColorsDark();
    imguiThemes::embraceTheDarkness();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.FontGlobalScale = 2.0f;
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 1.0f;
}

// ============================================================================
// Shutdown
// ============================================================================

/**
 * @brief Shuts down all subsystems in reverse initialization order.
 *
 * Ensures proper cleanup of all resources before application exit.
 */
void Application::shutdown() {
    if (!m_running) return;

    std::cout << "[Application] Shutting down...\n";
    ImGui::SFML::Shutdown();
    m_running = false;
}