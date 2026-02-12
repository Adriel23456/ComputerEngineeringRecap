/**
 * @file TomasuloRegistersView.cpp
 * @brief Implementation of TomasuloRegistersView.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloRegistersView.h"
#include <imgui.h>

 // ============================================================================
 // Main Render
 // ============================================================================

void TomasuloRegistersView::render() {
    ImVec2 available = ImGui::GetContentRegionAvail();

    // Header
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Register File (16 x 64-bit)");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(1, 5));

    // All remaining height goes to the register table
    float remainingHeight = ImGui::GetContentRegionAvail().y;
    if (remainingHeight < 50.0f) remainingHeight = 50.0f;

    ImGuiWindowFlags childFlags =
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##RegistersScrollable", ImVec2(available.x, remainingHeight),
        false, childFlags);
    {
        m_table.render("##TOMASULO_REG_TABLE");
    }
    ImGui::EndChild();
}