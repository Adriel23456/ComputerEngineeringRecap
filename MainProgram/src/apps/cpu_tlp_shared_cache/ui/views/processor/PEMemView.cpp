// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/ui/views/processor/PEMemView.cpp
// ============================================================================

/**
 * @file PEMemView.cpp
 * @brief Implementation of PEMemView.
 */

#include "apps/cpu_tlp_shared_cache/ui/views/processor/PEMemView.h"
#include <imgui.h>
#include <cstdio>

PEMemView::PEMemView(int peIndex)
    : m_peIndex(peIndex)
{
}

void PEMemView::render() {
    ImVec2 available = ImGui::GetContentRegionAvail();

    // Generate unique child ID
    char childId[32];
    std::snprintf(childId, sizeof(childId), "##PE%dMemScrollable", m_peIndex);

    ImGui::BeginChild(childId, ImVec2(available.x, available.y), false);

    // Header
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "PE%d L1 Cache Memory", m_peIndex);
    ImGui::Separator();
    ImGui::Dummy(ImVec2(1, 5));

    // Table with unique ID
    char tableId[32];
    std::snprintf(tableId, sizeof(tableId), "##PE%d_Cache_Table", m_peIndex);
    m_table.render(tableId);

    ImGui::EndChild();
}

void PEMemView::setBySetWay(int setIndex, int wayIndex,
    const std::string& tag,
    const std::array<uint8_t, 32>& data,
    const std::string& mesi) {
    m_table.setLineBySetWay(setIndex, wayIndex, tag, data, mesi);
}