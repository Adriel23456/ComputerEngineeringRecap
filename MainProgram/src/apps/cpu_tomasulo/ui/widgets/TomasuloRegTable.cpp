/**
 * @file TomasuloRegTable.cpp
 * @brief Implementation of TomasuloRegTable.
 *
 * Pure renderer — reads Value, Qi, and Qi_valid from the bound
 * TomasuloRegisterFile* and displays them in an ImGui table.
 */

#include "apps/cpu_tomasulo/ui/widgets/TomasuloRegTable.h"
#include "apps/cpu_tomasulo/simulation/processor/TomasuloRegisterFile.h"

#include <imgui.h>
#include <cstdio>
#include <cstring>

 // ============================================================================
 // Construction
 // ============================================================================

TomasuloRegTable::TomasuloRegTable() {
    m_names[REG0] = "REG0  (0000)";
    m_names[REG1] = "REG1  (0001)";
    m_names[REG2] = "REG2  (0010)";
    m_names[REG3] = "REG3  (0011)";
    m_names[REG4] = "REG4  (0100)";
    m_names[REG5] = "REG5  (0101)";
    m_names[REG6] = "REG6  (0110)";
    m_names[REG7] = "REG7  (0111)";
    m_names[REG8] = "REG8  (1000)";
    m_names[REG9] = "REG9  (1001)";
    m_names[REG10] = "REG10 (1010)";
    m_names[REG11] = "REG11 (1011)";
    m_names[REG12] = "REG12 (1100)";
    m_names[UPPER] = "UPPER (1101)";
    m_names[LOWER] = "LOWER (1110)";
    m_names[PEID] = "PEID  (1111)";
}

// ============================================================================
// Data Source Binding
// ============================================================================

void TomasuloRegTable::bindDataSource(const TomasuloRegisterFile* regs) {
    m_dataSource = regs;
}

// ============================================================================
// Formatting
// ============================================================================

std::string TomasuloRegTable::formatHex(uint64_t v) {
    char b[24];
    std::snprintf(b, sizeof(b), "0x%016llX",
        static_cast<unsigned long long>(v));
    return std::string(b);
}

// ============================================================================
// Rendering
// ============================================================================

void TomasuloRegTable::render(const char* id) {
    if (!m_dataSource) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
            "Register data source not bound.");
        return;
    }

    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_SizingStretchProp;

    // 6 columns: Register | Qi | Qi Valid | Hex | Decimal | Double
    if (ImGui::BeginTable(id, 6, flags, ImVec2(available.x, available.y))) {
        ImGui::TableSetupScrollFreeze(0, 1);

        const float charW = ImGui::CalcTextSize("W").x;
        const float regMin = std::max(160.0f, charW * 18.0f);

        ImGui::TableSetupColumn("REG Number",
            ImGuiTableColumnFlags_WidthFixed, regMin);
        ImGui::TableSetupColumn("Qi (ROB#)",
            ImGuiTableColumnFlags_WidthFixed, charW * 10.0f);
        ImGui::TableSetupColumn("Qi Valid",
            ImGuiTableColumnFlags_WidthFixed, charW * 9.0f);
        ImGui::TableSetupColumn("Value (Hex)",
            ImGuiTableColumnFlags_WidthStretch, 0.40f);
        ImGui::TableSetupColumn("Decimal (int64)",
            ImGuiTableColumnFlags_WidthStretch, 0.25f);
        ImGui::TableSetupColumn("Double (IEEE 754)",
            ImGuiTableColumnFlags_WidthStretch, 0.25f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < kRegCount; ++i) {
            const TomasuloRegEntry& entry = m_dataSource->getEntry(i);

            ImGui::TableNextRow();

            // ── Col 0: Register name ────────────────────────────
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(m_names[i].c_str());

            // ── Col 1: Qi (ROB index) ───────────────────────────
            ImGui::TableSetColumnIndex(1);
            if (entry.qi_valid) {
                ImGui::Text("ROB[%u]", static_cast<unsigned>(entry.qi));
            }
            else {
                ImGui::TextColored(ImVec4(0.45f, 0.45f, 0.45f, 1.0f), "--");
            }

            // ── Col 2: Qi Valid ─────────────────────────────────
            ImGui::TableSetColumnIndex(2);
            if (entry.qi_valid) {
                ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.3f, 1.0f), "Waiting");
            }
            else {
                ImGui::TextColored(ImVec4(0.3f, 0.9f, 0.4f, 1.0f), "Ready");
            }

            // ── Col 3: Hex value + context menu ─────────────────
            ImGui::TableSetColumnIndex(3);
            {
                std::string hx = formatHex(entry.value);
                ImGui::TextUnformatted(hx.c_str());

                std::string ctxId = "##ctx_reg_" + std::to_string(i);
                if (ImGui::BeginPopupContextItem(ctxId.c_str())) {
                    if (ImGui::MenuItem("Copy HEX"))
                        ImGui::SetClipboardText(hx.c_str());
                    ImGui::EndPopup();
                }
            }

            // ── Col 4: Decimal (signed 64-bit) ──────────────────
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%lld",
                static_cast<long long>(static_cast<int64_t>(entry.value)));

            // ── Col 5: Double (IEEE 754) ────────────────────────
            ImGui::TableSetColumnIndex(5);
            {
                double dbl = 0.0;
                uint64_t bits = entry.value;
                std::memcpy(&dbl, &bits, sizeof(double));
                char dblStr[64];
                std::snprintf(dblStr, sizeof(dblStr), "%.17g", dbl);
                ImGui::TextUnformatted(dblStr);
            }
        }

        ImGui::EndTable();
    }
}