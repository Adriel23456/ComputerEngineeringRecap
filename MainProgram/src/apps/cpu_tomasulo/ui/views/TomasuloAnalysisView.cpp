/**
 * @file TomasuloAnalysisView.cpp
 * @brief Implementation of TomasuloAnalysisView.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloAnalysisView.h"
#include <imgui.h>

 // ============================================================================
 // Reset
 // ============================================================================

void TomasuloAnalysisView::resetAll() {
    m_committedInstr = 0;
    m_committedMemInstr = 0;
    m_cyclesLostCache = 0;
    m_cyclesLostRAM = 0;
    m_totalCycles = 0;
    m_usesSB.fill(0);
    m_usesLB.fill(0);
    m_usesRS.fill(0);
    m_branchMispredictions = 0;
}

// ============================================================================
// Render
// ============================================================================

void TomasuloAnalysisView::render() {
    ImVec2 available = ImGui::GetContentRegionAvail();

    // Header
    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Simulation Analysis");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(1, 5));

    float remainingHeight = ImGui::GetContentRegionAvail().y;
    if (remainingHeight < 50.0f) remainingHeight = 50.0f;

    ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_SizingStretchProp;

    if (ImGui::BeginTable("##AnalysisTable", 2, flags,
        ImVec2(available.x, remainingHeight))) {
        ImGui::TableSetupScrollFreeze(0, 1);

        ImGui::TableSetupColumn("Metric",
            ImGuiTableColumnFlags_WidthStretch, 0.70f);
        ImGui::TableSetupColumn("Value",
            ImGuiTableColumnFlags_WidthStretch, 0.30f);
        ImGui::TableHeadersRow();

        // Helper lambda
        auto row = [](const char* name, uint64_t value,
            ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted(name);
                ImGui::TableSetColumnIndex(1);
                ImGui::TextColored(color, "%llu",
                    static_cast<unsigned long long>(value));
            };

        // Helper for section headers
        auto sectionHeader = [](const char* title, ImVec4 color) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextColored(color, "%s", title);
            ImGui::TableSetColumnIndex(1);
            ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            };

        ImVec4 cyan = ImVec4(0.4f, 0.8f, 1.0f, 1.0f);
        ImVec4 green = ImVec4(0.3f, 1.0f, 0.5f, 1.0f);
        ImVec4 yellow = ImVec4(1.0f, 1.0f, 0.3f, 1.0f);
        ImVec4 orange = ImVec4(1.0f, 0.7f, 0.2f, 1.0f);
        ImVec4 red = ImVec4(1.0f, 0.4f, 0.3f, 1.0f);
        ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        // ── Execution Overview ────────────────────────────────
        sectionHeader("EXECUTION OVERVIEW", cyan);
        row("Total Cycles", m_totalCycles, yellow);
        row("Committed Instructions", m_committedInstr, green);
        row("Committed Memory Instructions", m_committedMemInstr, green);

        // ── Latency ───────────────────────────────────────────
        sectionHeader("LATENCY", cyan);
        row("Cycles Lost - Cache Latency", m_cyclesLostCache, orange);
        row("Cycles Lost - RAM Latency", m_cyclesLostRAM, orange);

        // ── Store Buffers ─────────────────────────────────────
        sectionHeader("STORE BUFFERS", cyan);
        row("SB0 Uses", m_usesSB[0], white);
        row("SB1 Uses", m_usesSB[1], white);

        // ── Load Buffers ──────────────────────────────────────
        sectionHeader("LOAD BUFFERS", cyan);
        row("LB0 Uses", m_usesLB[0], white);
        row("LB1 Uses", m_usesLB[1], white);
        row("LB2 Uses", m_usesLB[2], white);

        // ── Reservation Stations ──────────────────────────────
        sectionHeader("RESERVATION STATIONS", cyan);
        row("RS IntegerALU0 Uses", m_usesRS[0], white);
        row("RS IntegerALU1 Uses", m_usesRS[1], white);
        row("RS FP_ALU0 Uses", m_usesRS[2], white);
        row("RS IntegerMUL0 Uses", m_usesRS[3], white);
        row("RS FP_MUL0 Uses", m_usesRS[4], white);
        row("RS Branch0 Uses", m_usesRS[5], white);

        // ── Branch ────────────────────────────────────────────
        sectionHeader("BRANCH PREDICTION", cyan);
        row("Branch Mispredictions", m_branchMispredictions, red);

        ImGui::EndTable();
    }
}