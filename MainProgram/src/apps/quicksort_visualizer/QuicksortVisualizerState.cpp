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
#include "apps/quicksort_visualizer/data/ElementCollection.h"
#include "apps/quicksort_visualizer/data/SwapQueue.h"
#include "apps/quicksort_visualizer/ui/GridPanel.h"
#include "apps/quicksort_visualizer/ui/ControlPanel.h"
#include "apps/quicksort_visualizer/ui/ElementRenderer.h"
#include "apps/quicksort_visualizer/ui/AmountInputPopup.h"
#include "apps/quicksort_visualizer/input/GridInputHandler.h"
#include "apps/quicksort_visualizer/animation/SwapAnimator.h"
#include "apps/quicksort_visualizer/audio/SwapSoundGenerator.h"
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
    , m_elements(nullptr)
    , m_gridPanel(nullptr)
    , m_controlPanel(nullptr)
    , m_elementRenderer(nullptr)
    , m_amountPopup(nullptr)
    , m_inputHandler(nullptr)
    , m_swapAnimator(nullptr)
    , m_soundGenerator(nullptr)
    , m_initialized(false)
    , m_sortingActive(false)
    , m_elementCount(quicksort::config::GridConfig::DEFAULT_ELEMENT_COUNT)
    , m_currentElementWidth(20.0f)
    , m_currentElementGap(2.0f)
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

    // Stop any running sort
    if (m_logicThread) {
        m_logicThread->cancelSorting();
    }

    // Shutdown in reverse order of creation
    m_soundGenerator.reset();
    m_swapAnimator.reset();
    m_inputHandler.reset();
    m_amountPopup.reset();
    m_elementRenderer.reset();
    m_controlPanel.reset();
    m_gridPanel.reset();
    m_elements.reset();
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

    // Initialize logic thread
    if (!initializeLogicThread()) {
        return false;
    }

    // Initialize data structures
    if (!initializeDataStructures()) {
        return false;
    }

    // Initialize visualization components
    if (!initializeVisualization()) {
        return false;
    }

    // Initialize animation
    if (!initializeAnimation()) {
        return false;
    }

    // Initialize audio
    if (!initializeAudio()) {
        return false;
    }

    // Setup callbacks
    setupControlCallbacks();
    setupPopupCallbacks();
    setupAnimatorCallbacks();

    // Generate initial random values and reset view for first time only
    regenerateElements();
    if (m_gridTransform) {
        m_gridTransform->resetView();
    }

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

bool QuicksortVisualizerState::initializeDataStructures() {
    m_elements = std::make_unique<quicksort::data::ElementCollection>();

    if (!m_elements) {
        logInitStatus("Element Collection", false);
        return false;
    }
    logInitStatus("Element Collection", true);

    return true;
}

bool QuicksortVisualizerState::initializeVisualization() {
    m_gridTransform = std::make_unique<quicksort::GridTransform>(
        quicksort::config::GridConfig::DEFAULT_GRID_WIDTH,
        quicksort::config::GridConfig::DEFAULT_GRID_HEIGHT
    );

    if (!m_gridTransform) {
        logInitStatus("Grid Transform", false);
        return false;
    }
    logInitStatus("Grid Transform", true);

    m_gridPanel = std::make_unique<quicksort::ui::GridPanel>(*m_gridTransform);

    if (!m_gridPanel) {
        logInitStatus("Grid Panel", false);
        return false;
    }
    logInitStatus("Grid Panel", true);

    m_elementRenderer = std::make_unique<quicksort::ui::ElementRenderer>(*m_gridTransform);

    if (!m_elementRenderer) {
        logInitStatus("Element Renderer", false);
        return false;
    }
    logInitStatus("Element Renderer", true);

    m_controlPanel = std::make_unique<quicksort::ui::ControlPanel>();

    if (!m_controlPanel) {
        logInitStatus("Control Panel", false);
        return false;
    }
    logInitStatus("Control Panel", true);

    m_amountPopup = std::make_unique<quicksort::ui::AmountInputPopup>();

    if (!m_amountPopup) {
        logInitStatus("Amount Input Popup", false);
        return false;
    }
    logInitStatus("Amount Input Popup", true);

    m_inputHandler = std::make_unique<quicksort::input::GridInputHandler>(*m_gridTransform);

    if (!m_inputHandler) {
        logInitStatus("Input Handler", false);
        return false;
    }
    logInitStatus("Input Handler", true);

    return true;
}

bool QuicksortVisualizerState::initializeAnimation() {
    m_swapAnimator = std::make_unique<quicksort::animation::SwapAnimator>();

    if (!m_swapAnimator) {
        logInitStatus("Swap Animator", false);
        return false;
    }

    // Set animation speed (adjust for visual appeal)
    m_swapAnimator->setSpeed(2.0f);

    logInitStatus("Swap Animator", true);
    return true;
}

bool QuicksortVisualizerState::initializeAudio() {
    m_soundGenerator = std::make_unique<quicksort::audio::SwapSoundGenerator>();

    if (!m_soundGenerator || !m_soundGenerator->initialize()) {
        logInitStatus("Swap Sound Generator", false);
        return false;
    }

    // Set initial volume (integrate with your existing SFX system)
    m_soundGenerator->setVolume(0.3f);
    m_soundGenerator->setEnabled(true);

    logInitStatus("Swap Sound Generator", true);
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

void QuicksortVisualizerState::setupPopupCallbacks() {
    quicksort::ui::AmountInputCallbacks callbacks;

    callbacks.onAccept = [this](uint32_t newAmount) { onAmountAccepted(newAmount); };
    callbacks.onCancel = [this]() { onAmountCancelled(); };

    m_amountPopup->setCallbacks(callbacks);
    m_amountPopup->setRange(
        quicksort::config::GridConfig::MIN_ELEMENT_COUNT,
        quicksort::config::GridConfig::MAX_ELEMENT_COUNT
    );
}

void QuicksortVisualizerState::setupAnimatorCallbacks() {
    // Set callback for when swap should be applied to element data
    m_swapAnimator->setSwapCallback([this](uint32_t indexA, uint32_t indexB) {
        onSwapData(indexA, indexB);
        });

    // Set callback for playing swap sound
    m_swapAnimator->setSoundCallback([this](const quicksort::data::SwapOperation& swap) {
        onPlaySwapSound(swap);
        });
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
    if (m_sortingActive) return;

    std::cout << "[QuicksortVisualizer] Generating " << m_elementCount << " random values...\n";
    regenerateElements();
}

void QuicksortVisualizerState::onSetAmount() {
    if (m_sortingActive) return;

    std::cout << "[QuicksortVisualizer] Opening amount input dialog...\n";

    if (m_amountPopup) {
        m_amountPopup->open(m_elementCount);
    }
}

void QuicksortVisualizerState::onStartQuicksort() {
    if (m_sortingActive) return;

    std::cout << "[QuicksortVisualizer] Starting quicksort...\n";

    if (!m_logicThread || !m_elements || m_elements->isEmpty()) {
        std::cerr << "[QuicksortVisualizer] Cannot start: no elements\n";
        return;
    }

    // Reset animator
    if (m_swapAnimator) {
        m_swapAnimator->reset();
    }

    // Set element count for sound scaling
    if (m_soundGenerator) {
        m_soundGenerator->setElementCount(m_elements->getCount());
    }

    // Extract values for sorting
    std::vector<double> values = extractValuesForSorting();

    // Start sorting on logic thread
    if (m_logicThread->startSorting(values)) {
        m_sortingActive = true;

        // Update control panel
        if (m_controlPanel) {
            m_controlPanel->setSortingActive(true);
        }
    }
}

void QuicksortVisualizerState::onResetView() {
    std::cout << "[QuicksortVisualizer] Resetting view...\n";
    if (m_gridTransform) {
        m_gridTransform->resetView();
    }
}

// ============================================================================
// Popup Callbacks
// ============================================================================

void QuicksortVisualizerState::onAmountAccepted(uint32_t newAmount) {
    std::cout << "[QuicksortVisualizer] Amount accepted: " << newAmount << "\n";

    m_elementCount = newAmount;

    if (m_controlPanel) {
        m_controlPanel->setElementCount(m_elementCount);
    }

    regenerateElements();
}

void QuicksortVisualizerState::onAmountCancelled() {
    std::cout << "[QuicksortVisualizer] Amount input cancelled.\n";
}

// ============================================================================
// Animation Callbacks
// ============================================================================

void QuicksortVisualizerState::onSwapData(uint32_t indexA, uint32_t indexB) {
    // Apply swap to element collection
    if (m_elements) {
        m_elements->swapElements(indexA, indexB);
    }
}

void QuicksortVisualizerState::onPlaySwapSound(const quicksort::data::SwapOperation& swap) {
    // Play sound for this swap
    if (m_soundGenerator) {
        m_soundGenerator->playSwapSound(swap);
    }
}

// ============================================================================
// Sorting Logic
// ============================================================================

void QuicksortVisualizerState::processSortingUpdate(float deltaTime) {
    if (!m_sortingActive) return;

    // Update animator
    if (m_swapAnimator) {
        m_swapAnimator->update(deltaTime);

        // Update renderer with animation state
        if (m_elementRenderer) {
            m_elementRenderer->setAnimationState(m_swapAnimator->getState());
        }

        // If not currently animating, try to get next swap from queue
        if (!m_swapAnimator->isAnimating()) {
            // Get swap queue from logic thread
            auto& swapQueue = m_logicThread->getSwapQueue();

            // Try to get next swap
            auto nextSwap = swapQueue.tryPop();

            if (nextSwap.has_value()) {
                // Start animating this swap
                m_swapAnimator->startSwap(nextSwap.value());
            }
            else if (swapQueue.isFullyProcessed()) {
                // All done!
                onSortingComplete();
            }
        }
    }
}

void QuicksortVisualizerState::onSortingComplete() {
    std::cout << "[QuicksortVisualizer] Sorting visualization complete!\n";

    m_sortingActive = false;

    // Clear animation state
    if (m_elementRenderer) {
        m_elementRenderer->clearAnimationState();
    }

    // Reset logic thread for next sort
    if (m_logicThread) {
        m_logicThread->reset();
    }

    // Update control panel
    if (m_controlPanel) {
        m_controlPanel->setSortingActive(false);
    }
}

// ============================================================================
// Helper Methods
// ============================================================================

void QuicksortVisualizerState::regenerateElements() {
    if (!m_elements) {
        return;
    }

    m_elements->generateRandom(m_elementCount);

    m_currentElementWidth = m_elements->getRecommendedElementWidth();
    m_currentElementGap = m_elements->getRecommendedElementGap();

    if (m_elementRenderer) {
        m_elementRenderer->setElementGap(m_currentElementGap);
    }

    updateGridSize();

    std::cout << "[QuicksortVisualizer] Generated " << m_elements->getCount()
        << " elements (width=" << m_currentElementWidth
        << "px, gap=" << m_currentElementGap << "px)\n";
}

void QuicksortVisualizerState::updateGridSize() {
    if (!m_elements || !m_gridTransform) {
        return;
    }

    float gridWidth = m_elements->calculateGridWidth(m_currentElementWidth, m_currentElementGap);
    float gridHeight = quicksort::config::GridConfig::DEFAULT_GRID_HEIGHT;

    m_gridTransform->setGridSize(gridWidth, gridHeight);

    std::cout << "[QuicksortVisualizer] Grid size updated to: "
        << gridWidth << "x" << gridHeight << "\n";
}

std::vector<double> QuicksortVisualizerState::extractValuesForSorting() const {
    std::vector<double> values;

    if (m_elements && !m_elements->isEmpty()) {
        uint32_t count = m_elements->getCount();
        values.reserve(count);

        for (uint32_t i = 0; i < count; ++i) {
            values.push_back(static_cast<double>(m_elements->getElement(i).value));
        }
    }

    return values;
}

// ============================================================================
// State Interface
// ============================================================================

void QuicksortVisualizerState::handleEvent(sf::Event& event) {
    // Allow ESC to cancel sorting
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            if (m_sortingActive && m_logicThread) {
                std::cout << "[QuicksortVisualizer] Cancelling sort...\n";
                m_logicThread->cancelSorting();
                onSortingComplete();
            }
            return;
        }
    }

    // Don't process other events if popup is open
    if (m_amountPopup && m_amountPopup->isOpen()) {
        return;
    }

    // Don't allow back navigation during sorting
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::BackSpace && !m_sortingActive) {
            m_stateManager->queueNextState(
                std::make_unique<MainMenuState>(m_stateManager, m_window)
            );
            return;
        }

        if (event.key.code == sf::Keyboard::R) {
            onResetView();
            return;
        }
    }
}

void QuicksortVisualizerState::update(float deltaTime) {
    // Process sorting animation
    processSortingUpdate(deltaTime);

    // Don't process grid input if popup is open
    if (!m_amountPopup || !m_amountPopup->isOpen()) {
        if (m_inputHandler && m_gridPanel) {
            m_inputHandler->processInput(
                m_gridPanel->isContentHovered(),
                m_gridPanel->getContentPosition(),
                m_gridPanel->getContentSize()
            );
        }
    }

    // Update grid transform animations
    if (m_gridTransform) {
        m_gridTransform->update(deltaTime);
    }

    // Update control panel state
    if (m_controlPanel && m_logicThread) {
        m_controlPanel->setThreadState(m_logicThread->getState());
        m_controlPanel->setSortingActive(m_sortingActive);
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

        m_gridPanel->render(gridPos, gridSize, m_elements.get(), m_elementRenderer.get(),
            m_currentElementWidth);
    }

    // Render control panel
    if (m_controlPanel) {
        float controlY = displaySize.y - controlPanelHeight - PANEL_PADDING;
        ImVec2 controlPos(PANEL_PADDING, controlY);
        ImVec2 controlSize(displaySize.x - (PANEL_PADDING * 2), controlPanelHeight);
        m_controlPanel->render(controlPos, controlSize);
    }

    // Render popup (if open)
    if (m_amountPopup) {
        m_amountPopup->render();
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