// ============================================================================
// File: src/states/MainMenuState.cpp
// ============================================================================

/**
 * @file MainMenuState.cpp
 * @brief Implementation of MainMenuState.
 *
 * Provides main menu rendering with shader-based animated background
 * and program selection functionality.
 */

#include "states/MainMenuState.h"
#include "states/ProgramState.h"
#include "core/fsm/StateManager.h"
#include "imgui.h"
#include <memory>
#include <algorithm>

 // Forward declaration for factory functions (now with AudioManager)
std::unique_ptr<State> CreateCpuTLPSharedCacheState(StateManager*, sf::RenderWindow*, AudioManager*);
std::unique_ptr<State> CreateQuicksortVisualizerState(StateManager*, sf::RenderWindow*, AudioManager*);
std::unique_ptr<State> CreateCpuTomasuloState(StateManager*, sf::RenderWindow*, AudioManager*);

// ============================================================================
// Shader Background Implementation
// ============================================================================

namespace {

    /**
     * @brief Fragment shader source for animated background.
     */
    const std::string BACKGROUND_SHADER_SOURCE = R"(
    uniform vec2 iResolution;
    uniform float iTime;
    
    vec3 palette(float t) {
        vec3 a = vec3(0.5, 0.5, 0.5);
        vec3 b = vec3(0.5, 0.5, 0.5);
        vec3 c = vec3(1.0, 1.0, 1.0);
        vec3 d = vec3(0.263, 0.416, 0.557);
        return a + b * cos(6.28318 * (c * t + d));
    }
    
    void main() {
        vec2 fragCoord = gl_FragCoord.xy;
        vec2 uv = (fragCoord * 2.0 - iResolution) / iResolution.y;
        vec2 uv0 = uv;
        vec3 finalColor = vec3(0.0);
        
        for (float i = 0.0; i < 4.0; i++) {
            uv = fract(uv * 1.5) - 0.5;
            float d = length(uv) * exp(-length(uv0));
            vec3 col = palette(length(uv0) + i * 0.4 + iTime * 0.4);
            d = sin(d * 8.0 + iTime) / 8.0;
            d = abs(d);
            d = pow(0.01 / d, 1.2);
            finalColor += col * d;
        }
        
        gl_FragColor = vec4(finalColor, 1.0);
    }
)";

    // Shader state (static to persist across frames)
    sf::Shader g_backgroundShader;
    bool g_shaderLoaded = false;
    sf::Clock g_shaderClock;

    /**
     * @brief Ensures shader is loaded and returns it.
     * @return Reference to the loaded shader.
     */
    sf::Shader& getBackgroundShader() {
        if (!g_shaderLoaded) {
            g_backgroundShader.loadFromMemory(BACKGROUND_SHADER_SOURCE, sf::Shader::Fragment);
            g_shaderLoaded = true;
        }
        return g_backgroundShader;
    }

} // anonymous namespace

// ============================================================================
// Construction
// ============================================================================

MainMenuState::MainMenuState(StateManager* stateManager, sf::RenderWindow* window,
    AudioManager* audio)
    : State(stateManager, window)
    , m_audio(audio)
{
    m_menuItems = {
        "CPU TLP with Shared Cache",
        "CPU with Tomasulo",
        "Quicksort",
    };
}

// ============================================================================
// Event Handling
// ============================================================================

void MainMenuState::handleEvent(sf::Event& event) {
    (void)event; // Menu uses ImGui for input
}

// ============================================================================
// Update
// ============================================================================

void MainMenuState::update(float /*deltaTime*/) {
    // Menu state has no per-frame logic
}

// ============================================================================
// Rendering
// ============================================================================

void MainMenuState::renderBackground() {
    sf::Shader& shader = getBackgroundShader();

    float currentTime = g_shaderClock.getElapsedTime().asSeconds();
    shader.setUniform("iTime", currentTime);
    shader.setUniform("iResolution", sf::Vector2f(m_window->getSize()));

    sf::RectangleShape backgroundRect(sf::Vector2f(m_window->getSize()));
    backgroundRect.setPosition(0, 0);
    m_window->draw(backgroundRect, &shader);
}

void MainMenuState::render() {
    renderBackground();
    renderMenuPanel();
}

void MainMenuState::renderMenuPanel() {
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImVec2 panelSize(displaySize.x * 0.8f, displaySize.y * 0.8f);
    ImVec2 panelPos((displaySize.x - panelSize.x) * 0.5f,
        (displaySize.y - panelSize.y) * 0.5f);

    ImGui::SetNextWindowPos(panelPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(panelSize, ImGuiCond_Always);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.85f));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoTitleBar;

    if (ImGui::Begin("##MainMenuPanel", nullptr, flags)) {
        ImGui::BeginChild("##MenuScrollRegion", ImVec2(0, 0), true,
            ImGuiWindowFlags_AlwaysVerticalScrollbar);

        float innerWidth = ImGui::GetContentRegionAvail().x;
        float buttonWidth = innerWidth * 0.95f;
        constexpr float BUTTON_HEIGHT = 64.f;

        for (const auto& item : m_menuItems) {
            if (createAutoFitButton(item.c_str(), buttonWidth, BUTTON_HEIGHT)) {
                handleMenuSelection(item);
            }
            ImGui::Dummy(ImVec2(1, 8));
        }

        ImGui::EndChild();
    }
    ImGui::End();

    ImGui::PopStyleColor();
}

// ============================================================================
// UI Helpers
// ============================================================================

bool MainMenuState::createAutoFitButton(const char* label, float targetWidth, float height) {
    ImVec2 textSize = ImGui::CalcTextSize(label);
    float paddingX = ImGui::GetStyle().FramePadding.x * 2.f;
    float available = targetWidth - paddingX;

    float scale = available > 0.f ? (available / std::max(1.f, textSize.x)) : 1.f;
    scale = std::clamp(scale, 0.7f, 1.4f);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);
    ImGui::SetWindowFontScale(scale);

    bool clicked = ImGui::Button(label, ImVec2(targetWidth, height));

    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleVar();

    return clicked;
}

// ============================================================================
// Navigation
// ============================================================================

void MainMenuState::handleMenuSelection(const std::string& itemName) {
    if (itemName == "CPU TLP with Shared Cache") {
        m_stateManager->queueNextState(
            CreateCpuTLPSharedCacheState(m_stateManager, m_window, m_audio)
        );
    }
    else if (itemName == "CPU with Tomasulo") {
        m_stateManager->queueNextState(
            CreateCpuTomasuloState(m_stateManager, m_window, m_audio)
        );
    }
    else if (itemName == "Quicksort") {
        m_stateManager->queueNextState(
            CreateQuicksortVisualizerState(m_stateManager, m_window, m_audio)
        );
    }
    // Here your gonna be adding the new cpu_tomasulo_simulation
    else {
        m_stateManager->queueNextState(
            std::make_unique<ProgramState>(m_stateManager, m_window, itemName)
        );
    }
}