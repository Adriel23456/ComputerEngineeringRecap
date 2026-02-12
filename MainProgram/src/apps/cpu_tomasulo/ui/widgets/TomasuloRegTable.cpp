/**
 * @file TomasuloRegTable.cpp
 * @brief Implementation of TomasuloRegTable.
 */

#include "apps/cpu_tomasulo/ui/widgets/TomasuloRegTable.h"
#include <imgui.h>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>

 // ============================================================================
 // Construction
 // ============================================================================

TomasuloRegTable::TomasuloRegTable() {
    // Register names with 4-bit binary codes
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

    resetAll();
}

// ============================================================================
// Reset
// ============================================================================

void TomasuloRegTable::resetAll() {
    for (auto& v : m_values) {
        v = 0x0000000000000000ULL;
    }
    m_values[LOWER] = 0xFFFFFFFFFFFFFFFFULL;  // All ones default

    for (int i = 0; i < kRegCount; ++i) {
        m_valueText[i] = formatHex(m_values[i]);
    }
}

// ============================================================================
// Read Access
// ============================================================================

uint64_t TomasuloRegTable::getValueByIndex(int idx) const {
    if (idx < 0 || idx >= kRegCount) return 0ULL;
    return m_values[idx];
}

const std::string& TomasuloRegTable::getHexTextByIndex(int idx) const {
    static const std::string kZero = "0x0000000000000000";
    if (idx < 0 || idx >= kRegCount) return kZero;
    return m_valueText[idx];
}

// ============================================================================
// Write Access
// ============================================================================

void TomasuloRegTable::setValueByIndex(int idx, uint64_t val) {
    if (idx < 0 || idx >= kRegCount) return;
    m_values[idx] = val;
    m_valueText[idx] = formatHex(val);
}

void TomasuloRegTable::setValueByName(const std::string& key, uint64_t val) {
    int idx = indexFromName(key);
    if (idx >= 0) {
        setValueByIndex(idx, val);
    }
}

// ============================================================================
// Rendering
// ============================================================================

void TomasuloRegTable::render(const char* id) {
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
            ImGui::TableNextRow();

            // Register name
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(m_names[i].c_str());

            // Hex value with context menu
            ImGui::TableSetColumnIndex(1);
            {
                const std::string& hx = m_valueText[i];
                ImGui::TextUnformatted(hx.c_str());

                std::string ctxId = "##ctx_reg_" + std::to_string(i);
                if (ImGui::BeginPopupContextItem(ctxId.c_str())) {
                    if (ImGui::MenuItem("Copy HEX")) {
                        ImGui::SetClipboardText(hx.c_str());
                    }
                    ImGui::EndPopup();
                }
            }

            // Decimal (signed 64-bit)
            ImGui::TableSetColumnIndex(2);
            {
                int64_t asSigned = static_cast<int64_t>(m_values[i]);
                ImGui::Text("%lld", static_cast<long long>(asSigned));
            }

            // Double (IEEE 754)
            ImGui::TableSetColumnIndex(3);
            {
                double dbl = 0.0;
                uint64_t bits = m_values[i];
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
// Helpers
// ============================================================================

std::string TomasuloRegTable::formatHex(uint64_t v) {
    char b[24];
    std::snprintf(b, sizeof(b), "0x%016llX", static_cast<unsigned long long>(v));
    return std::string(b);
}

int TomasuloRegTable::indexFromName(const std::string& keyIn) {
    // Convert to uppercase for comparison
    std::string k;
    k.reserve(keyIn.size());
    for (char c : keyIn) {
        k.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
    }

    if (k == "UPPER") return UPPER;
    if (k == "LOWER") return LOWER;
    if (k == "PEID")  return PEID;

    // Check REG0–REG12
    if (k.rfind("REG", 0) == 0 && k.size() >= 4) {
        try {
            int n = std::stoi(k.substr(3));
            if (n >= 0 && n <= 12) return n;
        }
        catch (...) {
            return -1;
        }
    }

    return -1;
}