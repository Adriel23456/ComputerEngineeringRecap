// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/ui/views/GeneralView.cpp
// ============================================================================

/**
 * @file GeneralView.cpp
 * @brief Implementation of GeneralView.
 */

#include "apps/cpu_tlp_shared_cache/ui/views/GeneralView.h"
#include "apps/cpu_tlp_shared_cache/CpuTLPControlAPI.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
#include <string>

GeneralView::GeneralView() = default;

void GeneralView::ensureLoaded() {
    if (!m_loaded) {
        const std::string path = std::string(RESOURCES_PATH) + "Assets/CPU_TLP/GeneralView.png";
        m_loaded = m_texture.loadFromFile(path);
        if (!m_loaded) {
            std::cout << "[GeneralView] Could not load image: " << path << "\n";
        }
    }
}

void GeneralView::render() {
    ensureLoaded();

    const float SPACING = 10.0f;
    const float BUTTON_HEIGHT = 46.0f;
    ImVec2 available = ImGui::GetContentRegionAvail();

    float imageHeight = available.y - BUTTON_HEIGHT - SPACING;
    if (imageHeight < 0.0f) {
        imageHeight = 0.0f;
    }

    // Architecture diagram
    if (m_loaded) {
        ImGui::Image(m_texture, sf::Vector2f(available.x, imageHeight));
    }
    else {
        ImGui::Dummy(ImVec2(available.x, imageHeight));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - imageHeight);
        ImGui::TextWrapped(
            "Could not load GeneralView image. "
            "Check 'resources/Assets/CPU_TLP/GeneralView.png'"
        );
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + imageHeight);
    }

    ImGui::Dummy(ImVec2(1.0f, SPACING));

    // Control buttons
    const float GAP = 10.0f;
    const float TEXT_PADDING = 30.0f;

    float wReset = ImGui::CalcTextSize("RESET").x + TEXT_PADDING;
    float wStep = ImGui::CalcTextSize("Step").x + TEXT_PADDING;
    float wStepUntil = ImGui::CalcTextSize("StepUntilNum").x + TEXT_PADDING;
    float wInfinite = ImGui::CalcTextSize("InfiniteStep").x + TEXT_PADDING;
    float wStop = ImGui::CalcTextSize("STOP").x + TEXT_PADDING;

    // RESET button (red)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.82f, 0.22f, 0.22f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.60f, 0.12f, 0.12f, 1.0f));

    if (ImGui::Button("RESET", ImVec2(wReset, BUTTON_HEIGHT))) {
        // Reset all PEs
        if (cpu_tlp_ui::onResetPE0) cpu_tlp_ui::onResetPE0();
        if (cpu_tlp_ui::onResetPE1) cpu_tlp_ui::onResetPE1();
        if (cpu_tlp_ui::onResetPE2) cpu_tlp_ui::onResetPE2();
        if (cpu_tlp_ui::onResetPE3) cpu_tlp_ui::onResetPE3();
        // Reset analysis data
        if (cpu_tlp_ui::onResetAnalysis) cpu_tlp_ui::onResetAnalysis();
    }

    ImGui::PopStyleColor(3);
    ImGui::SameLine(0.0f, GAP);

    // Step button
    if (ImGui::Button("Step", ImVec2(wStep, BUTTON_HEIGHT))) {
        if (cpu_tlp_ui::onStepPE0) cpu_tlp_ui::onStepPE0();
        if (cpu_tlp_ui::onStepPE1) cpu_tlp_ui::onStepPE1();
        if (cpu_tlp_ui::onStepPE2) cpu_tlp_ui::onStepPE2();
        if (cpu_tlp_ui::onStepPE3) cpu_tlp_ui::onStepPE3();
    }

    ImGui::SameLine(0.0f, GAP);

    // StepUntil group
    ImGui::BeginGroup();
    {
        if (m_untilSteps < 1) {
            m_untilSteps = 1;
        }

        float inputWidth = available.x * 0.15f;
        ImGui::PushItemWidth(inputWidth);

        int tmp = m_untilSteps;
        ImGui::InputScalar("##until_steps", ImGuiDataType_S32, &tmp,
            nullptr, nullptr, nullptr, ImGuiInputTextFlags_CharsDecimal);
        if (tmp < 1) tmp = 1;
        m_untilSteps = tmp;

        ImGui::PopItemWidth();
        ImGui::SameLine();

        if (ImGui::Button("StepUntilNum", ImVec2(wStepUntil, BUTTON_HEIGHT))) {
            if (cpu_tlp_ui::onStepUntilPE0) cpu_tlp_ui::onStepUntilPE0(m_untilSteps);
            if (cpu_tlp_ui::onStepUntilPE1) cpu_tlp_ui::onStepUntilPE1(m_untilSteps);
            if (cpu_tlp_ui::onStepUntilPE2) cpu_tlp_ui::onStepUntilPE2(m_untilSteps);
            if (cpu_tlp_ui::onStepUntilPE3) cpu_tlp_ui::onStepUntilPE3(m_untilSteps);
        }
    }
    ImGui::EndGroup();

    ImGui::SameLine(0.0f, GAP);

    // InfiniteStep button (green)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.55f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.68f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.09f, 0.45f, 0.16f, 1.0f));

    if (ImGui::Button("InfiniteStep", ImVec2(wInfinite, BUTTON_HEIGHT))) {
        if (cpu_tlp_ui::onStepIndefinitelyPE0) cpu_tlp_ui::onStepIndefinitelyPE0();
        if (cpu_tlp_ui::onStepIndefinitelyPE1) cpu_tlp_ui::onStepIndefinitelyPE1();
        if (cpu_tlp_ui::onStepIndefinitelyPE2) cpu_tlp_ui::onStepIndefinitelyPE2();
        if (cpu_tlp_ui::onStepIndefinitelyPE3) cpu_tlp_ui::onStepIndefinitelyPE3();
    }

    ImGui::PopStyleColor(3);
    ImGui::SameLine();

    // Stop button
    if (ImGui::Button("STOP", ImVec2(wStop, BUTTON_HEIGHT))) {
        if (cpu_tlp_ui::onStopPE0) cpu_tlp_ui::onStopPE0();
        if (cpu_tlp_ui::onStopPE1) cpu_tlp_ui::onStopPE1();
        if (cpu_tlp_ui::onStopPE2) cpu_tlp_ui::onStopPE2();
        if (cpu_tlp_ui::onStopPE3) cpu_tlp_ui::onStopPE3();
    }
}