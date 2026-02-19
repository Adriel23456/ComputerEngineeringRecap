// ============================================================================
// File: src/apps/cpu_tomasulo/ui/widgets/CacheTable.cpp
// ============================================================================

/**
 * @file CacheTable.cpp
 * @brief Implementation of CacheTable.
 *
 * Handles:
 *   - Data mutation via setLine / setLineBySetWay.
 *   - Set/Way button navigation rendering.
 *   - 16-segment data table (tag, hex, decimal, double).
 */

#include "apps/cpu_tomasulo/ui/widgets/CacheTable.h"
#include <imgui.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <algorithm>

 // ============================================================================
 // Data Setters
 // ============================================================================

void CacheTable::setLine(int index, const std::string& tag,
    const std::array<uint8_t, kLineSizeBytes>& data,
    bool valid) {
    if (index < 0 || index >= kTotalLines) return;
    m_lines[index].tag = tag;
    m_lines[index].data = data;
    m_lines[index].valid = valid;
}

void CacheTable::setLineBySetWay(int set, int way, const std::string& tag,
    const std::array<uint8_t, kLineSizeBytes>& data,
    bool valid) {
    if (set < 0 || set >= kSets || way < 0 || way >= kWays) return;
    setLine(set * kWays + way, tag, data, valid);
}

void CacheTable::resetAll() {
    for (auto& line : m_lines) {
        line.tag = "0x00000000000000";
        line.data.fill(0);
        line.valid = false;
    }
    m_selectedSet = 0;
    m_selectedWay = 0;
}

// ============================================================================
// Extraction / Formatting Helpers
// ============================================================================

uint64_t CacheTable::extract8Bytes(
    const std::array<uint8_t, kLineSizeBytes>& data, int segment) {
    if (segment < 0 || segment >= kSegments) return 0;

    uint64_t value = 0;
    int      offset = segment * 8;
    for (int i = 0; i < 8; ++i) {
        value |= (static_cast<uint64_t>(data[offset + i]) << (i * 8));
    }
    return value;
}

std::string CacheTable::format8BytesAsHex(
    const std::array<uint8_t, kLineSizeBytes>& data, int segment) {
    uint64_t value = extract8Bytes(data, segment);
    char buf[24];
    std::snprintf(buf, sizeof(buf), "0x%016llX",
        static_cast<unsigned long long>(value));
    return std::string(buf);
}

// ============================================================================
// Navigation Controls
// ============================================================================

void CacheTable::renderNavigationControls(float /*availableWidth*/) {
    const float btnW = 60.0f;
    const float btnH = 32.0f;
    const float spacing = 8.0f;

    // ── Set selector ─────────────────────────────────────────
    ImGui::Text("Set:");
    ImGui::SameLine();

    for (int s = 0; s < kSets; ++s) {
        if (s > 0) ImGui::SameLine(0, spacing);

        bool selected = (m_selectedSet == s);
        if (selected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.9f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.8f, 1.0f));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        }

        char label[16];
        std::snprintf(label, sizeof(label), "%d##set", s);
        if (ImGui::Button(label, ImVec2(btnW, btnH))) m_selectedSet = s;
        ImGui::PopStyleColor(3);
    }

    ImGui::Dummy(ImVec2(1, 10));

    // ── Way selector ─────────────────────────────────────────
    ImGui::Text("Way:");
    ImGui::SameLine();

    for (int w = 0; w < kWays; ++w) {
        if (w > 0) ImGui::SameLine(0, spacing);

        bool selected = (m_selectedWay == w);
        if (selected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.9f, 0.6f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 1.0f, 0.7f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.8f, 0.5f, 1.0f));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        }

        char label[16];
        std::snprintf(label, sizeof(label), "%d##way", w);
        if (ImGui::Button(label, ImVec2(btnW, btnH))) m_selectedWay = w;
        ImGui::PopStyleColor(3);
    }

    // ── Current selection label ───────────────────────────────
    ImGui::SameLine(0, 25.0f);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.3f, 1.0f),
        "Viewing: Set %d, Way %d", m_selectedSet, m_selectedWay);
}

// ============================================================================
// Data Table  (16 segments × 8 bytes per line)
// ============================================================================

void CacheTable::renderDataTable(const char* id, float tableHeight) {
    ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_SizingStretchProp;

    if (ImGui::BeginTable(id, 4, flags, ImVec2(0, tableHeight))) {
        ImGui::TableSetupScrollFreeze(0, 1);

        const float charW = ImGui::CalcTextSize("W").x;
        const float tagWidth = charW * 18.0f;
        const float dataWidth = charW * 27.0f;

        ImGui::TableSetupColumn("Tag (55 bits)",
            ImGuiTableColumnFlags_WidthFixed, tagWidth);
        ImGui::TableSetupColumn("Data (Hex, 64B)",
            ImGuiTableColumnFlags_WidthFixed, dataWidth);
        ImGui::TableSetupColumn("Decimal (int64, 2's comp)",
            ImGuiTableColumnFlags_WidthStretch, 1.5f);
        ImGui::TableSetupColumn("Double (IEEE 754, 64-bit)",
            ImGuiTableColumnFlags_WidthStretch, 1.5f);
        ImGui::TableHeadersRow();

        const CacheLine& line = getCurrentLine();

        for (int seg = 0; seg < kSegments; ++seg) {
            ImGui::TableNextRow();

            // ── Tag column (shown only in the first row) ──────
            ImGui::TableSetColumnIndex(0);
            if (seg == 0) {
                ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f),
                    "Set %d / Way %d", m_selectedSet, m_selectedWay);
                ImGui::Text("%s", line.tag.c_str());
            }
            else {
                ImGui::Dummy(ImVec2(0, 0));
            }

            // ── Data column (8 bytes, grouped as two 4-byte halves) ──
            ImGui::TableSetColumnIndex(1);
            {
                std::ostringstream oss;
                oss << std::hex << std::uppercase << std::setfill('0');
                int offset = seg * 8;
                for (int i = 0; i < 8; ++i) {
                    if (i > 0 && i % 4 == 0) oss << " ";
                    oss << std::setw(2) << static_cast<int>(line.data[offset + i]);
                }
                char segLabel[32];
                std::snprintf(segLabel, sizeof(segLabel), "[%2d] ", seg);
                ImGui::Text("%s%s", segLabel, oss.str().c_str());
            }

            // ── Decimal column (signed 64-bit) ────────────────
            ImGui::TableSetColumnIndex(2);
            {
                uint64_t bits = extract8Bytes(line.data, seg);
                int64_t  asSigned = static_cast<int64_t>(bits);
                ImGui::Text("%lld", static_cast<long long>(asSigned));
            }

            // ── Double column (IEEE 754) ──────────────────────
            ImGui::TableSetColumnIndex(3);
            {
                uint64_t bits = extract8Bytes(line.data, seg);
                double   dbl = 0.0;
                std::memcpy(&dbl, &bits, sizeof(double));
                char dblStr[64];
                std::snprintf(dblStr, sizeof(dblStr), "%.17g", dbl);
                ImGui::TextUnformatted(dblStr);
            }
        }

        ImGui::EndTable();
    }
}

// ============================================================================
// Main Render
// ============================================================================

void CacheTable::render(const char* id) {
    ImVec2 available = ImGui::GetContentRegionAvail();

    const float CTRL_HEIGHT = 170.0f;
    const float GAP = 5.0f;
    float tableHeight = available.y - CTRL_HEIGHT - GAP;
    if (tableHeight < 50.0f) tableHeight = 50.0f;

    // ── Navigation controls ───────────────────────────────────
    ImGuiWindowFlags ctrlFlags =
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::BeginChild("##CacheControls", ImVec2(available.x, CTRL_HEIGHT),
        true, ctrlFlags);
    {
        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f),
            "Cache Line Navigator  (2KB | 8 Sets x 4 Ways | 64B Lines | 55-bit Tag)");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(1, 8));
        renderNavigationControls(available.x);
    }
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(1, GAP));

    // ── Data table ────────────────────────────────────────────
    ImGui::BeginChild("##CacheTableArea", ImVec2(available.x, tableHeight), false);
    {
        renderDataTable(id, tableHeight);
    }
    ImGui::EndChild();
}