// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/ui/views/AnalysisDataView.cpp
// ============================================================================

/**
 * @file AnalysisDataView.cpp
 * @brief Implementation of AnalysisDataView.
 */

#include "apps/cpu_tlp_shared_cache/ui/views/AnalysisDataView.h"
#include <imgui.h>

void AnalysisDataView::render() {
    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_ScrollX |
        ImGuiTableFlags_SizingStretchProp;

    if (ImGui::BeginTable("##analysis_data_table", 2, flags, ImVec2(available.x, available.y))) {
        ImGui::TableSetupScrollFreeze(0, 1);

        // Column proportions: 70% metric name, 30% value
        ImGui::TableSetupColumn("Metric", ImGuiTableColumnFlags_WidthStretch, 0.70f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.30f);
        ImGui::TableHeadersRow();

        // Helper lambda for rendering metric rows
        auto renderRow = [](const char* name, uint64_t value) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(name);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%llu", static_cast<unsigned long long>(value));
            };

        // Render all metrics
        renderRow("Number of cache misses:", m_cacheMisses);
        renderRow("Number of invalidations of relevant lines:", m_invalidations);
        renderRow("Number of read/write operations:", m_readWriteOps);
        renderRow("Number of transactions in MESI:", m_transactionsMESI);
        renderRow("Traffic of read/write in PE0:", m_trafficPE[0]);
        renderRow("Traffic of read/write in PE1:", m_trafficPE[1]);
        renderRow("Traffic of read/write in PE2:", m_trafficPE[2]);
        renderRow("Traffic of read/write in PE3:", m_trafficPE[3]);

        ImGui::EndTable();
    }
}