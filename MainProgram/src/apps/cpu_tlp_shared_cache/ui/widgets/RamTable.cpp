// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/ui/widgets/RamTable.cpp
// ============================================================================

/**
 * @file RamTable.cpp
 * @brief Implementation of RamTable.
 */

#include "apps/cpu_tlp_shared_cache/ui/widgets/RamTable.h"
#include <imgui.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

std::string RamTable::formatAddress(uint64_t addr64) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "0x%016llX", static_cast<unsigned long long>(addr64));
    return std::string(buf);
}

std::string RamTable::formatHex(uint64_t v) {
    char buf[20];
    std::snprintf(buf, sizeof(buf), "0x%016llX", static_cast<unsigned long long>(v));
    return std::string(buf);
}

void RamTable::setDataByIndex(int rowIndex, uint64_t value) {
    if (rowIndex >= 0 && rowIndex < kRows) {
        m_values[rowIndex] = value;
    }
}

void RamTable::setDataByAddress(uint64_t address, uint64_t value) {
    // Verify 8-byte alignment
    if ((address % kStep) != 0) {
        std::cout << "[RAM] setDataByAddress: address not 8-byte aligned: "
            << formatAddress(address) << std::endl;
        return;
    }

    // Verify range
    if (address > kMaxAddr) {
        std::cout << "[RAM] setDataByAddress: address out of range: "
            << formatAddress(address) << std::endl;
        return;
    }

    int rowIndex = static_cast<int>(address / kStep);
    setDataByIndex(rowIndex, value);
}

void RamTable::render(const char* id) {
    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_SizingStretchProp;

    if (ImGui::BeginTable(id, 4, flags, ImVec2(available.x, available.y))) {
        ImGui::TableSetupScrollFreeze(0, 1);

        const float charW = ImGui::CalcTextSize("W").x;
        const float addrMin = std::max(180.0f, charW * 20.0f);
        const float hexMin = std::max(180.0f, charW * 20.0f);

        ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, addrMin);
        ImGui::TableSetupColumn("Value (Hex)", ImGuiTableColumnFlags_WidthFixed, hexMin);
        ImGui::TableSetupColumn("Decimal (int64, 2's comp)", ImGuiTableColumnFlags_WidthStretch, 0.5f);
        ImGui::TableSetupColumn("Double (IEEE 754, 64-bit)", ImGuiTableColumnFlags_WidthStretch, 0.5f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < kRows; ++i) {
            uint64_t addr = kBaseAddr + static_cast<uint64_t>(i) * kStep;
            uint64_t value = m_values[i];

            ImGui::TableNextRow();

            // Address column
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", formatAddress(addr).c_str());

            // Hex value column with context menu
            ImGui::TableSetColumnIndex(1);
            {
                std::string hexStr = formatHex(value);
                ImGui::TextUnformatted(hexStr.c_str());

                std::string ctxId = "##ctx_hex_" + std::to_string(i);
                if (ImGui::BeginPopupContextItem(ctxId.c_str())) {
                    if (ImGui::MenuItem("Copy HEX")) {
                        ImGui::SetClipboardText(hexStr.c_str());
                    }
                    ImGui::EndPopup();
                }
            }

            // Decimal column (signed 64-bit)
            ImGui::TableSetColumnIndex(2);
            {
                int64_t asSigned = static_cast<int64_t>(value);
                ImGui::Text("%lld", static_cast<long long>(asSigned));
            }

            // Double column (IEEE 754)
            ImGui::TableSetColumnIndex(3);
            {
                double dbl = 0.0;
                std::memcpy(&dbl, &value, sizeof(double));
                char dblStr[64];
                std::snprintf(dblStr, sizeof(dblStr), "%.17g", dbl);
                ImGui::TextUnformatted(dblStr);
            }
        }

        ImGui::EndTable();
    }
}