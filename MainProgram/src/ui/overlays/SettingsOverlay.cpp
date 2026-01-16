// ============================================================================
// File: src/ui/overlays/SettingsOverlay.cpp
// ============================================================================

/**
 * @file SettingsOverlay.cpp
 * @brief Implementation of SettingsOverlay.
 *
 * Coordinates the settings overlay window and delegates to
 * specialized panels for different settings categories.
 */

#include "ui/overlays/SettingsOverlay.h"
#include "ui/overlays/panels/VideoSettingsPanel.h"
#include "ui/overlays/panels/AudioSettingsPanel.h"
#include "ui/overlays/panels/CreditsPanel.h"
#include "core/config/ConfigManager.h"
#include "systems/audio/AudioManager.h"
#include "imgui.h"
#include <string>

 // ============================================================================
 // Construction / Destruction
 // ============================================================================

SettingsOverlay::SettingsOverlay(sf::RenderWindow* window, ConfigManager* config, AudioManager* audio)
    : m_window(window)
    , m_config(config)
    , m_audio(audio)
{
    m_videoPanel = std::make_unique<VideoSettingsPanel>(window, config);
    m_audioPanel = std::make_unique<AudioSettingsPanel>(config, audio);
    m_creditsPanel = std::make_unique<CreditsPanel>();
}

SettingsOverlay::~SettingsOverlay() = default;

// ============================================================================
// Visibility Control
// ============================================================================

void SettingsOverlay::toggle() {
    if (m_isOpen) {
        close();
    }
    else {
        open();
    }
}

void SettingsOverlay::open() {
    if (!m_isOpen) {
        m_isOpen = true;
        m_animationTimer = 0.0f;
        if (m_audio) {
            m_audio->playSFX("enterSettings");
        }
    }
}

void SettingsOverlay::close() {
    if (m_isOpen) {
        m_isOpen = false;
        if (m_audio) {
            m_audio->playSFX("exitSettings");
        }
    }
}

bool SettingsOverlay::consumeApplyRequested() {
    bool result = m_applyRequested;
    m_applyRequested = false;
    return result;
}

// ============================================================================
// Update Loop
// ============================================================================

void SettingsOverlay::handleEvent(const sf::Event& event) {
    (void)event; // Currently unused
}

void SettingsOverlay::update(float deltaTime) {
    if (m_isOpen && m_animationTimer < 1.0f) {
        m_animationTimer += deltaTime * ANIMATION_SPEED;
        if (m_animationTimer > 1.0f) {
            m_animationTimer = 1.0f;
        }
    }
}

void SettingsOverlay::rebuildAfterRecreate() {
    m_videoPanel->rebuildResolutions();
}

// ============================================================================
// Rendering
// ============================================================================

void SettingsOverlay::render() {
    if (!m_isOpen) {
        return;
    }

    // Calculate eased alpha for smooth animation
    float alpha = m_animationTimer;
    float easedAlpha = alpha * alpha * (3.0f - 2.0f * alpha);

    ImGui::PushID("##SettingsOverlayContext");

    renderModalBackground(easedAlpha);
    renderMainWindow(easedAlpha);

    ImGui::PopID();
}

void SettingsOverlay::renderModalBackground(float alpha) {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(displaySize, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.4f * alpha);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoFocusOnAppearing;

    ImGui::Begin("##ModalBackground", nullptr, flags);
    ImGui::End();
}

void SettingsOverlay::renderMainWindow(float alpha) {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 displaySize = io.DisplaySize;

    // Animated window size and position
    float scale = 0.85f + 0.15f * alpha;
    ImVec2 windowSize(displaySize.x * 0.95f * scale, displaySize.y * 0.95f * scale);
    ImVec2 windowPos((displaySize.x - windowSize.x) * 0.5f,
        (displaySize.y - windowSize.y) * 0.5f);

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowFocus();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, alpha));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30, 30));
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoDocking;

    if (ImGui::Begin("##SettingsMainOverlay", nullptr, flags)) {
        if (m_showingCredits) {
            renderCreditsView();
        }
        else {
            renderSettingsView();
        }
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void SettingsOverlay::renderSettingsView() {
    float fullWidth = ImGui::GetContentRegionAvail().x;
    float columnWidth = (fullWidth - 30) * 0.5f;

    // Title
    ImGui::SetWindowFontScale(1.8f);
    ImGui::TextUnformatted("Settings");
    ImGui::SetWindowFontScale(1.2f);
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    // Video Settings Panel
    if (m_videoPanel->render(columnWidth, BUTTON_HEIGHT)) {
        m_videoPanel->applyChanges();
        m_config->save(std::string(RESOURCES_PATH) + "config.json");
        m_applyRequested = true;
        close();
    }

    // Section separator
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    // Audio Settings Panel
    m_audioPanel->render(fullWidth);

    // Section separator
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    // Credits Button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.3f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.6f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.25f, 0.45f, 1.0f));

    if (ImGui::Button("Credits", ImVec2(fullWidth, BUTTON_HEIGHT))) {
        m_showingCredits = true;
    }

    ImGui::PopStyleColor(3);
    ImGui::Spacing();

    // Exit Game Button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));

    if (ImGui::Button("Exit Game", ImVec2(fullWidth, BUTTON_HEIGHT))) {
        m_window->close();
    }

    ImGui::PopStyleColor(3);
}

void SettingsOverlay::renderCreditsView() {
    float fullWidth = ImGui::GetContentRegionAvail().x;

    if (m_creditsPanel->render(fullWidth, BUTTON_HEIGHT)) {
        m_showingCredits = false;
    }
}