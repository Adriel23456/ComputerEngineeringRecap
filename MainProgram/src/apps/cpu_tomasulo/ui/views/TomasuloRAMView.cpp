/**
 * @file TomasuloRAMView.cpp
 * @brief Implementation of TomasuloRAMView.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloRAMView.h"
#include <imgui.h>
#include <iostream>

 // ============================================================================
 // Main Render
 // ============================================================================

void TomasuloRAMView::render() {
    ImVec2 available = ImGui::GetContentRegionAvail();
    const ImGuiStyle& style = ImGui::GetStyle();

    const float SPACING = 8.0f;
    const float BUTTON_HEIGHT = 46.0f;

    // IMPORTANT:
    // Reserve a little more than BUTTON_HEIGHT to avoid bottom cropping.
    // This accounts for frame padding / item spacing / window padding.
    const float EXTRA_BUTTON_AREA_Y = 8.0f; // small, but fixes clipping nicely
    const float buttonAreaHeight = BUTTON_HEIGHT + EXTRA_BUTTON_AREA_Y;

    float tableHeight = available.y - (SPACING + buttonAreaHeight);
    if (tableHeight < 0.0f) tableHeight = 0.0f;

    // Render the paged RAM table
    ImGuiWindowFlags childFlags =
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##TomasuloRAMTableArea",
        ImVec2(available.x, tableHeight), false, childFlags);
    {
        m_table.render("##TOMASULO_RAM_TABLE");
    }
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(1.0f, SPACING));

    // Render control buttons
    renderControlButtons(available.x);
}

// ============================================================================
// Control Buttons
// ============================================================================

void TomasuloRAMView::renderControlButtons(float availableWidth) {
    const ImGuiStyle& style = ImGui::GetStyle();

    const float GAP = 10.0f;
    const float BUTTON_HEIGHT = 46.0f;
    const float buttonWidth = (availableWidth - GAP) * 0.5f;
    const ImVec2 buttonSize(buttonWidth, BUTTON_HEIGHT);

    // ── Reset button (red) ────────────────────────────────────
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.16f, 0.16f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.78f, 0.22f, 0.22f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.58f, 0.12f, 0.12f, 1.0f));

    if (ImGui::Button("RESET RAM", buttonSize)) {
        std::cout << "[TomasuloRAM] RESET RAM pressed - clearing all RAM\n";
        m_table.resetAll();
    }

    ImGui::PopStyleColor(3);
    ImGui::SameLine(0.0f, GAP);

    // ── Load button (green) — no-op for now ───────────────────
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.55f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.68f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.09f, 0.45f, 0.16f, 1.0f));

    if (ImGui::Button("Load", buttonSize)) {
        std::cout << "[TomasuloRAM] LOAD pressed - (not yet implemented)\n";
        // TODO: Implement binary file loading for Tomasulo RAM
    }

    ImGui::PopStyleColor(3);

    // Tiny spacer so the bottom of the buttons never touches/clips the view
    ImGui::Dummy(ImVec2(1.0f, 4.0f));
}
