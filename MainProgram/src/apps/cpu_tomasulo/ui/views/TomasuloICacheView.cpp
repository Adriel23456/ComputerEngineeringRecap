// ============================================================================
// File: src/apps/cpu_tomasulo/ui/views/TomasuloICacheView.cpp
// ============================================================================

/**
 * @file TomasuloICacheView.cpp
 * @brief Implementation of TomasuloICacheView.
 *
 * Forwards line data from CpuTomasuloState::syncICacheView() into the
 * CacheTable widget, then renders the widget inside a scrollable child.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloICacheView.h"
#include <imgui.h>

 // ============================================================================
 // Data Forwarding
 // ============================================================================

void TomasuloICacheView::setBySetWay(
    int setIndex, int wayIndex,
    const std::string& tag,
    const std::array<uint8_t, CacheTable::kLineSizeBytes>& data,
    bool valid) {
    m_table.setLineBySetWay(setIndex, wayIndex, tag, data, valid);
}

// ============================================================================
// Render
// ============================================================================

void TomasuloICacheView::render() {
    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "L1 Instruction Cache (I-Cache)");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(1, 5));

    float remainingHeight = ImGui::GetContentRegionAvail().y;
    if (remainingHeight < 50.0f) remainingHeight = 50.0f;

    ImGuiWindowFlags childFlags =
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##ICacheScrollable", ImVec2(available.x, remainingHeight),
        false, childFlags);
    {
        m_table.render("##ICACHE_TABLE");
    }
    ImGui::EndChild();
}