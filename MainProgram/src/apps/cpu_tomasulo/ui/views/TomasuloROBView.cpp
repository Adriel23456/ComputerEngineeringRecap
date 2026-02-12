/**
 * @file TomasuloROBView.cpp
 * @brief Implementation of TomasuloROBView.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloROBView.h"
#include <imgui.h>

 // ============================================================================
 // Main Render
 // ============================================================================

void TomasuloROBView::render() {
    ImVec2 available = ImGui::GetContentRegionAvail();

    // Header
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f),
        "Reorder Buffer (ROB)  -  32 Entries");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(1, 5));

    // All remaining height goes to the ROB table widget
    float remainingHeight = ImGui::GetContentRegionAvail().y;
    if (remainingHeight < 50.0f) remainingHeight = 50.0f;

    ImGuiWindowFlags childFlags =
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##ROBScrollable", ImVec2(available.x, remainingHeight),
        false, childFlags);
    {
        m_table.render("##ROB_TABLE");
    }
    ImGui::EndChild();
}