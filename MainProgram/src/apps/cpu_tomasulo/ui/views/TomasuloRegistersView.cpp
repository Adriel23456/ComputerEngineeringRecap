/**
 * @file TomasuloRegistersView.cpp
 * @brief Implementation of TomasuloRegistersView.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloRegistersView.h"
#include <imgui.h>

void TomasuloRegistersView::render() {
    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f),
        "Register File (16 x 64-bit) - Tomasulo Rename Tags");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(1, 5));

    float remainingHeight = ImGui::GetContentRegionAvail().y;
    if (remainingHeight < 50.0f) remainingHeight = 50.0f;

    ImGuiWindowFlags childFlags =
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##RegistersScrollable",
        ImVec2(available.x, remainingHeight),
        false, childFlags);
    {
        m_table.render("##TOMASULO_REG_TABLE");
    }
    ImGui::EndChild();
}