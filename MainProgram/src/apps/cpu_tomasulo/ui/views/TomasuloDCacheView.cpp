// ============================================================================
// File: src/apps/cpu_tomasulo/ui/views/TomasuloDCacheView.cpp
// ============================================================================

/**
 * @file TomasuloDCacheView.cpp
 * @brief Implementation of TomasuloDCacheView.
 *
 * Forwards line data from CpuTomasuloState::syncDCacheView() into the
 * CacheTable widget, then renders the widget inside a scrollable child.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloDCacheView.h"
#include <imgui.h>

 // ============================================================================
 // Data Forwarding
 // ============================================================================

void TomasuloDCacheView::setBySetWay(
    int setIndex, int wayIndex,
    const std::string& tag,
    const std::array<uint8_t, CacheTable::kLineSizeBytes>& data,
    bool valid) {
    m_table.setLineBySetWay(setIndex, wayIndex, tag, data, valid);
}

// ============================================================================
// Render
// ============================================================================

void TomasuloDCacheView::render() {
    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "L1 Data Cache (D-Cache)");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(1, 5));

    float remainingHeight = ImGui::GetContentRegionAvail().y;
    if (remainingHeight < 50.0f) remainingHeight = 50.0f;

    ImGuiWindowFlags childFlags =
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##DCacheScrollable", ImVec2(available.x, remainingHeight),
        false, childFlags);
    {
        m_table.render("##DCACHE_TABLE");
    }
    ImGui::EndChild();
}