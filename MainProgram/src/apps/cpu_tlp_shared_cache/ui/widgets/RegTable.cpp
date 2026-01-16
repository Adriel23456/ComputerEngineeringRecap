// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/ui/widgets/RegTable.cpp
// ============================================================================

/**
 * @file RegTable.cpp
 * @brief Implementation of RegTable.
 */

#include "apps/cpu_tlp_shared_cache/ui/widgets/RegTable.h"
#include <imgui.h>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>

 // ============================================================================
 // Construction
 // ============================================================================

RegTable::RegTable(int peIndex) {
    // Register names with binary codes
    m_names[REG0] = "REG0 (0000)";
    m_names[REG1] = "REG1 (0001)";
    m_names[REG2] = "REG2 (0010)";
    m_names[REG3] = "REG3 (0011)";
    m_names[REG4] = "REG4 (0100)";
    m_names[REG5] = "REG5 (0101)";
    m_names[REG6] = "REG6 (0110)";
    m_names[REG7] = "REG7 (0111)";
    m_names[REG8] = "REG8 (1000)";
    m_names[PEID] = "PEID (1001)";
    m_names[UPPER_REG] = "UPPER_REG (1010)";
    m_names[LOWER_REG] = "LOWER_REG (1011)";

    // Default values
    for (auto& v : m_values) {
        v = 0x0000000000000000ULL;
    }
    m_values[LOWER_REG] = 0xFFFFFFFFFFFFFFFFULL;  // All ones
    m_values[PEID] = static_cast<uint64_t>(peIndex);

    // Initialize hex text
    for (int i = 0; i < kRegCount; ++i) {
        m_valueText[i] = formatHex(m_values[i]);
    }
}

// ============================================================================
// Public Read Access
// ============================================================================

uint64_t RegTable::getValueByIndex(int idx) const {
    if (idx < 0 || idx >= kRegCount) return 0ULL;
    return m_values[idx];
}

const std::string& RegTable::getHexTextByIndex(int idx) const {
    static const std::string kZero = "0x0000000000000000";
    if (idx < 0 || idx >= kRegCount) return kZero;
    return m_valueText[idx];
}

// ============================================================================
// Private Setters
// ============================================================================

void RegTable::setPEID(int peIndex) {
    m_values[PEID] = static_cast<uint64_t>(peIndex);
    m_valueText[PEID] = formatHex(m_values[PEID]);
}

void RegTable::setValueByIndex(int idx, uint64_t val) {
    if (idx < 0 || idx >= kRegCount) return;
    m_values[idx] = val;
    m_valueText[idx] = formatHex(val);
}

void RegTable::setValueByName(const std::string& key, uint64_t val) {
    int idx = indexFromName(key);
    if (idx >= 0) {
        setValueByIndex(idx, val);
    }
}

// ============================================================================
// Rendering
// ============================================================================

void RegTable::render(const char* id) {
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
        const float regMin = std::max(140.0f, charW * 18.0f);

        ImGui::TableSetupColumn("REG Number", ImGuiTableColumnFlags_WidthFixed, regMin);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.45f);
        ImGui::TableSetupColumn("Decimal (int64, 2's comp)", ImGuiTableColumnFlags_WidthStretch, 0.25f);
        ImGui::TableSetupColumn("Float (IEEE 754, 64-bit)", ImGuiTableColumnFlags_WidthStretch, 0.30f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < kRegCount; ++i) {
            ImGui::TableNextRow();

            // Register name
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(m_names[i].c_str());

            // Hex value (read-only with context menu)
            ImGui::TableSetColumnIndex(1);
            {
                const std::string& hx = m_valueText[i];
                ImGui::TextUnformatted(hx.c_str());

                std::string ctxId = "##ctx_val_" + std::to_string(i);
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

std::string RegTable::formatHex(uint64_t v) {
    char b[20];
    std::snprintf(b, sizeof(b), "0x%016llX", static_cast<unsigned long long>(v));
    return std::string(b);
}

int RegTable::indexFromName(const std::string& keyIn) {
    // Convert to uppercase for comparison
    std::string k;
    k.reserve(keyIn.size());
    for (char c : keyIn) {
        k.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
    }

    if (k == "PEID") return PEID;
    if (k == "UPPER_REG") return UPPER_REG;
    if (k == "LOWER_REG") return LOWER_REG;

    // Check REG0-REG8
    if (k.rfind("REG", 0) == 0 && k.size() >= 4) {
        int n = k[3] - '0';
        if (n >= 0 && n <= 8) return n;
    }

    return -1;
}