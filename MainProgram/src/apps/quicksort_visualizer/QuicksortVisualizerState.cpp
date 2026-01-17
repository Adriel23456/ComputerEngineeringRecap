// ============================================================================
// File: src/apps/quicksort_visualizer/QuicksortVisualizerState.cpp
// ============================================================================

/**
 * @file QuicksortVisualizerState.cpp
 * @brief Implementation of the QuicksortVisualizerState class.
 */

#include "apps/quicksort_visualizer/QuicksortVisualizerState.h"
#include "apps/quicksort_visualizer/threading/LogicThreadController.h"
#include "apps/quicksort_visualizer/visualization/GridTransform.h"
#include "apps/quicksort_visualizer/visualization/GridConfig.h"
#include "apps/quicksort_visualizer/ui/GridPanel.h"
#include "apps/quicksort_visualizer/ui/ControlPanel.h"
#include "apps/quicksort_visualizer/input/GridInputHandler.h"
#include "core/fsm/StateManager.h"
#include "states/MainMenuState.h"
#include "imgui.h"
#include <SFML/Graphics.hpp>
#include <iostream>

 // ============================================================================
 // Shader Background
 // ============================================================================

namespace {

    const std::string QUICKSORT_BACKGROUND_SHADER = R"(
        uniform vec2 iResolution;
        uniform float iTime;
        
        void main() {
            vec2 fragCoord = gl_FragCoord.xy;
            vec2 uv = fragCoord / iResolution;
            
            float gradient = uv.y * 0.3 + 0.1;
            float wave = sin(uv.x * 3.0 + iTime * 0.5) * 0.02;
            
            vec3 baseColor = vec3(0.08, 0.10, 0.14);
            vec3 accentColor = vec3(0.12, 0.15, 0.22);
            
            vec3 finalColor = mix(baseColor, accentColor, gradient + wave);
            
            gl_FragColor = vec4(finalColor, 1.0);
        }
    )";

    sf::Shader g_backgroundShader;
    bool g_shaderLoaded = false;
    sf::Clock g_shaderClock;

    sf::Shader& getBackgroundShader() {
        if (!g_shaderLoaded) {
            if (g_backgroundShader.loadFromMemory(QUICKSORT_BACKGROUND_SHADER, sf::Shader::Fragment)) {
                g_shaderLoaded = true;
            }
        }
        return g_backgroundShader;
    }

} // anonymous namespace

// ============================================================================
// Construction / Destruction
// ============================================================================

QuicksortVisualizerState::QuicksortVisualizerState(StateManager* stateManager,
    sf::RenderWindow* window)
    : State(stateManager, window)
    , m_logicThread(nullptr)
    , m_gridTransform(nullptr)
    , m_gridPanel(nullptr)
    , m_controlPanel(nullptr)
    , m_inputHandler(nullptr)
    , m_initialized(false)
    , m_elementCount(quicksort::config::GridConfig::DEFAULT_ELEMENT_COUNT)
{
    std::cout << "[QuicksortVisualizer] ========================================\n";
    std::cout << "[QuicksortVisualizer] Initializing Quicksort Visualizer State\n";
    std::cout << "[QuicksortVisualizer] ========================================\n";

    m_initialized = initializeComponents();

    if (m_initialized) {
        std::cout << "[QuicksortVisualizer] State initialization COMPLETE.\n";
    }
    else {
        std::cerr << "[QuicksortVisualizer] State initialization FAILED.\n";
    }
}

QuicksortVisualizerState::~QuicksortVisualizerState() {
    std::cout << "[QuicksortVisualizer] Shutting down...\n";

    // Shutdown in reverse order of creation
    m_inputHandler.reset();
    m_controlPanel.reset();
    m_gridPanel.reset();
    m_gridTransform.reset();

    if (m_logicThread) {
        m_logicThread->shutdown();
        m_logicThread.reset();
    }

    std::cout << "[QuicksortVisualizer] Shutdown complete.\n";
}

// ============================================================================
// Initialization
// ============================================================================

bool QuicksortVisualizerState::initializeComponents() {
    // Verify window
    if (m_window == nullptr) {
        logInitStatus("RenderWindow", false);
        return false;
    }
    logInitStatus("RenderWindow reference", true);

    // Initialize shader
    getBackgroundShader();
    logInitStatus("Background shader", g_shaderLoaded);

    // Initialize logic thread (Step 2)
    if (!initializeLogicThread()) {
        return false;
    }

    // Initialize visualization components (Step 3)
    if (!initializeVisualization()) {
        return false;
    }

    // Setup control panel callbacks
    setupControlCallbacks();

    std::cout << "[QuicksortVisualizer] All components initialized.\n";
    return true;
}

bool QuicksortVisualizerState::initializeLogicThread() {
    m_logicThread = std::make_unique<quicksort::LogicThreadController>();

    if (!m_logicThread || !m_logicThread->initialize()) {
        logInitStatus("OpenMP Logic Thread", false);
        return false;
    }

    logInitStatus("OpenMP Logic Thread", true);
    return true;
}

bool QuicksortVisualizerState::initializeVisualization() {
    // Create grid transform (logic layer - no UI)
    m_gridTransform = std::make_unique<quicksort::GridTransform>(
        quicksort::config::GridConfig::DEFAULT_GRID_WIDTH,
        quicksort::config::GridConfig::DEFAULT_GRID_HEIGHT
    );

    if (!m_gridTransform) {
        logInitStatus("Grid Transform", false);
        return false;
    }
    logInitStatus("Grid Transform", true);

    // Create grid panel (UI layer)
    m_gridPanel = std::make_unique<quicksort::ui::GridPanel>(*m_gridTransform);

    if (!m_gridPanel) {
        logInitStatus("Grid Panel", false);
        return false;
    }
    logInitStatus("Grid Panel", true);

    // Create control panel (UI layer)
    m_controlPanel = std::make_unique<quicksort::ui::ControlPanel>();

    if (!m_controlPanel) {
        logInitStatus("Control Panel", false);
        return false;
    }
    logInitStatus("Control Panel", true);

    // Create input handler
    m_inputHandler = std::make_unique<quicksort::input::GridInputHandler>(*m_gridTransform);

    if (!m_inputHandler) {
        logInitStatus("Input Handler", false);
        return false;
    }
    logInitStatus("Input Handler", true);

    return true;
}

void QuicksortVisualizerState::setupControlCallbacks() {
    quicksort::ui::ControlPanelCallbacks callbacks;

    callbacks.onRandomValues = [this]() { onRandomValues(); };
    callbacks.onSetAmount = [this]() { onSetAmount(); };
    callbacks.onStartQuicksort = [this]() { onStartQuicksort(); };
    callbacks.onResetView = [this]() { onResetView(); };

    m_controlPanel->setCallbacks(callbacks);
    m_controlPanel->setElementCount(m_elementCount);

    if (m_logicThread) {
        m_controlPanel->setThreadState(m_logicThread->getState());
    }
}

void QuicksortVisualizerState::logInitStatus(const std::string& componentName, bool success) {
    if (success) {
        std::cout << "[QuicksortVisualizer] [OK] " << componentName << "\n";
    }
    else {
        std::cerr << "[QuicksortVisualizer] [FAIL] " << componentName << "\n";
    }
}

// ============================================================================
// Button Callbacks
// ============================================================================

void QuicksortVisualizerState::onRandomValues() {
    std::cout << "[QuicksortVisualizer] Generating random values...\n";
    // Future: Generate random values (Step 4)
}

void QuicksortVisualizerState::onSetAmount() {
    std::cout << "[QuicksortVisualizer] Set amount dialog...\n";
    // Future: Open dialog to set count (Step 4)
}

void QuicksortVisualizerState::onStartQuicksort() {
    std::cout << "[QuicksortVisualizer] Starting quicksort...\n";
    // Future: Begin sorting (Step 6)
}

void QuicksortVisualizerState::onResetView() {
    std::cout << "[QuicksortVisualizer] Resetting view...\n";
    if (m_gridTransform) {
        m_gridTransform->resetView();
    }
}

// ============================================================================
// State Interface
// ============================================================================

void QuicksortVisualizerState::handleEvent(sf::Event& event) {
    // Back navigation - this still works because it's a key event
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace) {
            m_stateManager->queueNextState(
                std::make_unique<MainMenuState>(m_stateManager, m_window)
            );
            return;
        }

        // Reset view with R key
        if (event.key.code == sf::Keyboard::R) {
            onResetView();
            return;
        }
    }

    // Note: Mouse input is now handled through ImGui in update(), not through SFML events
}

void QuicksortVisualizerState::update(float deltaTime) {
    // Process input using ImGui mouse state (must be done after ImGui::NewFrame in main loop)
    if (m_inputHandler && m_gridPanel) {
        m_inputHandler->processInput(
            m_gridPanel->isContentHovered(),
            m_gridPanel->getContentPosition(),
            m_gridPanel->getContentSize()
        );
    }

    // Update grid transform animations
    if (m_gridTransform) {
        m_gridTransform->update(deltaTime);
    }

    // Update control panel state
    if (m_controlPanel && m_logicThread) {
        m_controlPanel->setThreadState(m_logicThread->getState());
    }
}

void QuicksortVisualizerState::render() {
    renderShaderBackground();

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    float controlPanelHeight = displaySize.y * CONTROL_PANEL_HEIGHT_RATIO;
    float gridPanelHeight = displaySize.y - controlPanelHeight - (PANEL_PADDING * 3);

    // Render grid panel
    if (m_gridPanel) {
        ImVec2 gridPos(PANEL_PADDING, PANEL_PADDING);
        ImVec2 gridSize(displaySize.x - (PANEL_PADDING * 2), gridPanelHeight);
        m_gridPanel->render(gridPos, gridSize);
    }

    // Render control panel
    if (m_controlPanel) {
        float controlY = displaySize.y - controlPanelHeight - PANEL_PADDING;
        ImVec2 controlPos(PANEL_PADDING, controlY);
        ImVec2 controlSize(displaySize.x - (PANEL_PADDING * 2), controlPanelHeight);
        m_controlPanel->render(controlPos, controlSize);
    }
}

void QuicksortVisualizerState::renderBackground() {
    renderShaderBackground();
}

void QuicksortVisualizerState::renderShaderBackground() {
    if (!g_shaderLoaded) {
        m_window->clear(sf::Color(20, 26, 36));
        return;
    }

    sf::Shader& shader = getBackgroundShader();
    shader.setUniform("iTime", g_shaderClock.getElapsedTime().asSeconds());
    shader.setUniform("iResolution", sf::Vector2f(m_window->getSize()));

    sf::RectangleShape rect(sf::Vector2f(m_window->getSize()));
    m_window->draw(rect, &shader);
}