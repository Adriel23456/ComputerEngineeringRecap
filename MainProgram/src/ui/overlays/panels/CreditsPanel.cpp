// ============================================================================
// File: src/ui/overlays/panels/CreditsPanel.cpp
// ============================================================================

/**
 * @file CreditsPanel.cpp
 * @brief Implementation of CreditsPanel.
 */

#include "ui/overlays/panels/CreditsPanel.h"
#include "imgui.h"

 // ============================================================================
 // Construction
 // ============================================================================

CreditsPanel::CreditsPanel() {
    m_creditsText =
        "===========\n"
        "CPU with Thread-Level Parallelism (TLP)\n"
        "Team: Andres Guzman, Adriel Chaves, and Daniel Duarte\n"
        "Course: Computer Architecture II ~ Project 1\n"
        "Institution: Instituto Tecnologico de Costa Rica (TEC)\n"
        "===========\n"
        "---------------------------\n"
        "Other Projects\n"
        "Author: Adriel Chaves\n"
        "---------------------------\n";
}

// ============================================================================
// Rendering
// ============================================================================

bool CreditsPanel::render(float fullWidth, float buttonHeight) {
    bool backClicked = false;

    ImGui::SetWindowFontScale(1.8f);
    ImGui::TextUnformatted("Credits");
    ImGui::SetWindowFontScale(1.2f);
    ImGui::Separator();
    ImGui::Spacing();

    // Scrollable credits area
    ImVec2 available = ImGui::GetContentRegionAvail();
    ImVec2 scrollSize(available.x, available.y - buttonHeight - 16.0f);

    if (scrollSize.y < 80.0f) {
        scrollSize.y = 80.0f;
    }

    ImGui::BeginChild("##CreditsScroll", scrollSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    ImGui::PushTextWrapPos(0.0f);
    ImGui::TextUnformatted(m_creditsText.c_str());
    ImGui::PopTextWrapPos();
    ImGui::EndChild();

    ImGui::Spacing();

    // Back button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));

    if (ImGui::Button("Back", ImVec2(fullWidth, buttonHeight))) {
        backClicked = true;
    }

    ImGui::PopStyleColor(3);

    return backClicked;
}