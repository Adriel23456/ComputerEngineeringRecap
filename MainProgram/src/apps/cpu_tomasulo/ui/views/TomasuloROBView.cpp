// ============================================================================
// File: src/apps/cpu_tomasulo/ui/views/TomasuloROBView.cpp
// ============================================================================

/**
 * @file TomasuloROBView.cpp
 * @brief Implementation of TomasuloROBView.
 *
 * Renders a header then delegates to ROBTable, which displays
 * head/tail/count status, the 32-entry table, and a detail panel
 * for the selected entry. Data is pushed by
 * CpuTomasuloState::syncROBView() each frame under the sim lock.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloROBView.h"
#include <imgui.h>

 // ============================================================================
 // Render
 // ============================================================================

void TomasuloROBView::render() {
    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f),
        "Reorder Buffer (ROB)  -  32 Entries");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(1, 5));

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