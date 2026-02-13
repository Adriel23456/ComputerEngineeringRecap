/**
 * @file TomasuloRegTable.cpp
 * @brief Implementation of TomasuloRegTable.
 *
 * Pure renderer — all values read from the bound TomasuloRegisterFile*.
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

    if (ImGui::BeginTable(id, 4, flags, ImVec2(available.x, available.y))) {
        ImGui::TableSetupScrollFreeze(0, 1);

        const float charW = ImGui::CalcTextSize("W").x;
        const float regMin = std::max(160.0f, charW * 18.0f);

        ImGui::TableSetupColumn("REG Number",
            ImGuiTableColumnFlags_WidthFixed, regMin);
        ImGui::TableSetupColumn("Value (Hex)",
            ImGuiTableColumnFlags_WidthStretch, 0.45f);
        ImGui::TableSetupColumn("Decimal (int64, 2's comp)",
            ImGuiTableColumnFlags_WidthStretch, 0.25f);
        ImGui::TableSetupColumn("Double (IEEE 754, 64-bit)",
            ImGuiTableColumnFlags_WidthStretch, 0.30f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < kRegCount; ++i) {
            uint64_t value = m_dataSource->get(i);

            ImGui::TableNextRow();

            // Register name
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(m_names[i].c_str());

            // Hex value + context menu
            ImGui::TableSetColumnIndex(1);
            {
                std::string hx = formatHex(value);
                ImGui::TextUnformatted(hx.c_str());

                std::string ctxId = "##ctx_reg_" + std::to_string(i);
                if (ImGui::BeginPopupContextItem(ctxId.c_str())) {
                    if (ImGui::MenuItem("Copy HEX"))
                        ImGui::SetClipboardText(hx.c_str());
                    ImGui::EndPopup();
                }
            }

            // Decimal (signed 64-bit)
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%lld",
                static_cast<long long>(static_cast<int64_t>(value)));

            // Double (IEEE 754)
            ImGui::TableSetColumnIndex(3);
            {
                double dbl = 0.0;
                uint64_t bits = value;
                std::memcpy(&dbl, &bits, sizeof(double));
                char dblStr[64];
                std::snprintf(dblStr, sizeof(dblStr), "%.17g", dbl);
                ImGui::TextUnformatted(dblStr);
            }
        }

        ImGui::EndTable();
    }
}