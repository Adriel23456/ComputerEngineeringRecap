// ============================================================================
// File: src/ui/overlays/panels/VideoSettingsPanel.cpp
// ============================================================================

/**
 * @file VideoSettingsPanel.cpp
 * @brief Implementation of VideoSettingsPanel.
 */

#include "ui/overlays/panels/VideoSettingsPanel.h"
#include "core/config/ConfigManager.h"
#include "imgui.h"
#include <set>
#include <cmath>
#include <algorithm>

 // Static member definition
constexpr const char* VideoSettingsPanel::SCREEN_MODES[];

// ============================================================================
// Construction
// ============================================================================

VideoSettingsPanel::VideoSettingsPanel(sf::RenderWindow* window, ConfigManager* config)
    : m_window(window)
    , m_config(config)
{
    buildResolutionList();

    m_pendingVSync = m_config->isVSyncEnabled();

    // Determine current screen mode index
    std::string currentMode = m_config->getScreenModeString();
    if (currentMode == "fullscreen") {
        m_screenModeIndex = 1;
    }
    else if (currentMode == "borderless") {
        m_screenModeIndex = 2;
    }
    else {
        m_screenModeIndex = 0;
    }
}

// ============================================================================
// Rendering
// ============================================================================

bool VideoSettingsPanel::render(float columnWidth, float buttonHeight) {
    bool applyClicked = false;

    ImGui::Text("Video Settings");
    ImGui::Separator();
    ImGui::Spacing();

    const float arrowButtonWidth = 40.0f;
    const float displayWidth = columnWidth - (arrowButtonWidth * 2 + 10);

    // Resolution selector
    ImGui::BeginGroup();
    ImGui::Text("Resolution");

    // Left arrow
    ImGui::PushButtonRepeat(true);
    if (ImGui::Button("<##ResLeft", ImVec2(arrowButtonWidth, 30))) {
        m_resolutionIndex--;
        if (m_resolutionIndex < 0) {
            m_resolutionIndex = static_cast<int>(m_resolutionList.size()) - 1;
        }
    }
    ImGui::PopButtonRepeat();

    ImGui::SameLine();

    // Resolution display
    const auto& currentRes = m_resolutionList[m_resolutionIndex];
    std::string aspectRatio = getAspectRatioString(currentRes.width, currentRes.height);
    std::string resolutionText = std::to_string(currentRes.width) + " x " +
        std::to_string(currentRes.height) + " (" + aspectRatio + ")";

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    ImGui::Button(resolutionText.c_str(), ImVec2(displayWidth, 30));
    ImGui::PopStyleColor();

    ImGui::SameLine();

    // Right arrow
    ImGui::PushButtonRepeat(true);
    if (ImGui::Button(">##ResRight", ImVec2(arrowButtonWidth, 30))) {
        m_resolutionIndex++;
        if (m_resolutionIndex >= static_cast<int>(m_resolutionList.size())) {
            m_resolutionIndex = 0;
        }
    }
    ImGui::PopButtonRepeat();

    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(30, 0));
    ImGui::SameLine();

    // Screen mode selector
    ImGui::BeginGroup();
    ImGui::Text("Screen Mode");

    ImGui::PushButtonRepeat(true);
    if (ImGui::Button("<##ModeLeft", ImVec2(arrowButtonWidth, 30))) {
        m_screenModeIndex = (m_screenModeIndex - 1 + 3) % 3;
    }
    ImGui::PopButtonRepeat();

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    ImGui::Button(SCREEN_MODES[m_screenModeIndex], ImVec2(displayWidth, 30));
    ImGui::PopStyleColor();

    ImGui::SameLine();

    ImGui::PushButtonRepeat(true);
    if (ImGui::Button(">##ModeRight", ImVec2(arrowButtonWidth, 30))) {
        m_screenModeIndex = (m_screenModeIndex + 1) % 3;
    }
    ImGui::PopButtonRepeat();

    ImGui::EndGroup();

    // VSync and Apply row
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::BeginGroup();
    ImGui::Checkbox("VSync", &m_pendingVSync);
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(30, 0));
    ImGui::SameLine();

    // Apply button
    ImGui::BeginGroup();
    bool canApply = hasDisplayChangesPending();

    if (!canApply) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    }

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.4f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.5f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05f, 0.35f, 0.15f, 1.0f));

    if (ImGui::Button("Apply Changes", ImVec2(columnWidth, buttonHeight)) && canApply) {
        applyClicked = true;
    }

    ImGui::PopStyleColor(3);

    if (!canApply) {
        ImGui::PopStyleVar();
    }

    ImGui::EndGroup();

    return applyClicked;
}

// ============================================================================
// Resolution Management
// ============================================================================

void VideoSettingsPanel::rebuildResolutions() {
    buildResolutionList();
}

void VideoSettingsPanel::buildResolutionList() {
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    std::set<std::pair<unsigned int, unsigned int>> uniqueResolutions;

    for (const auto& mode : modes) {
        uniqueResolutions.insert({ mode.width, mode.height });
    }

    // Add current window resolution
    auto windowSize = m_window->getSize();
    uniqueResolutions.insert({ windowSize.x, windowSize.y });

    m_resolutionList.clear();
    for (const auto& [width, height] : uniqueResolutions) {
        m_resolutionList.push_back({ width, height });
    }

    // Fallback if no resolutions found
    if (m_resolutionList.empty()) {
        m_resolutionList.push_back({ 1920, 1080 });
        m_resolutionList.push_back({ 1600, 900 });
        m_resolutionList.push_back({ 1280, 720 });
    }

    // Find current resolution in list
    m_resolutionIndex = 0;
    for (int i = 0; i < static_cast<int>(m_resolutionList.size()); ++i) {
        if (sameResolution(m_resolutionList[i].width, m_resolutionList[i].height,
            m_config->getResolutionWidth(), m_config->getResolutionHeight())) {
            m_resolutionIndex = i;
            break;
        }
    }
}

std::string VideoSettingsPanel::getAspectRatioString(unsigned int w, unsigned int h) const {
    float ratio = static_cast<float>(w) / static_cast<float>(h);
    constexpr float epsilon = 0.01f;

    // Common aspect ratios
    if (std::abs(ratio - 16.0f / 9.0f) < epsilon)  return "16:9";
    if (std::abs(ratio - 16.0f / 10.0f) < epsilon) return "16:10";
    if (std::abs(ratio - 4.0f / 3.0f) < epsilon)   return "4:3";
    if (std::abs(ratio - 21.0f / 9.0f) < epsilon)  return "21:9";
    if (std::abs(ratio - 32.0f / 9.0f) < epsilon)  return "32:9";
    if (std::abs(ratio - 5.0f / 4.0f) < epsilon)   return "5:4";
    if (std::abs(ratio - 3.0f / 2.0f) < epsilon)   return "3:2";

    // Calculate GCD for custom ratio
    auto gcd = [](unsigned int a, unsigned int b) -> unsigned int {
        while (b) {
            unsigned int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
        };

    unsigned int divisor = gcd(w, h);
    return std::to_string(w / divisor) + ":" + std::to_string(h / divisor);
}

bool VideoSettingsPanel::sameResolution(unsigned int w1, unsigned int h1,
    unsigned int w2, unsigned int h2) {
    return w1 == w2 && h1 == h2;
}

// ============================================================================
// Settings Application
// ============================================================================

bool VideoSettingsPanel::hasDisplayChangesPending() const {
    const auto& pendingRes = m_resolutionList[m_resolutionIndex];

    bool resolutionChanged = !sameResolution(
        pendingRes.width, pendingRes.height,
        m_config->getResolutionWidth(), m_config->getResolutionHeight()
    );

    bool vsyncChanged = (m_pendingVSync != m_config->isVSyncEnabled());

    bool screenModeChanged = (std::string(SCREEN_MODES[m_screenModeIndex]) !=
        m_config->getScreenModeString());

    return resolutionChanged || vsyncChanged || screenModeChanged;
}

void VideoSettingsPanel::applyChanges() {
    const auto& selectedRes = m_resolutionList[m_resolutionIndex];

    m_config->setResolution(selectedRes.width, selectedRes.height);
    m_config->setVSyncEnabled(m_pendingVSync);
    m_config->setWindowStyle(SCREEN_MODES[m_screenModeIndex]);
}