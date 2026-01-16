// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/ui/widgets/MemCacheTable.cpp
// ============================================================================

/**
 * @file MemCacheTable.cpp
 * @brief Implementation of MemCacheTable.
 */

#include "apps/cpu_tlp_shared_cache/ui/widgets/MemCacheTable.h"
#include <algorithm>
#include <cctype>

namespace {

    /**
     * @brief Trims leading whitespace from string.
     */
    std::string trimLeft(const std::string& s) {
        auto it = std::find_if(s.begin(), s.end(), [](unsigned char c) {
            return !std::isspace(c);
            });
        return (it == s.end()) ? std::string() : std::string(it, s.end());
    }

} // anonymous namespace

const char* MemCacheTable::deriveMESI(const std::string& stateIn) {
    std::string s = trimLeft(stateIn);
    if (s.empty()) return "Null";

    // Accept textual M/E/S/I
    char c = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
    if (c == 'M') return "M";
    if (c == 'E') return "E";
    if (c == 'S') return "S";
    if (c == 'I') return "I";

    // Accept 2-bit encoding (typical MESI mapping)
    if (s.size() >= 2 &&
        std::isdigit(static_cast<unsigned char>(s[0])) &&
        std::isdigit(static_cast<unsigned char>(s[1]))) {
        if (s[0] == '0' && s[1] == '0') return "I";
        if (s[0] == '0' && s[1] == '1') return "S";
        if (s[0] == '1' && s[1] == '0') return "E";
        if (s[0] == '1' && s[1] == '1') return "M";
    }

    return "?";
}

void MemCacheTable::render(const char* id) {
    ImVec2 available = ImGui::GetContentRegionAvail();

    ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_SizingStretchProp |
        ImGuiTableFlags_ScrollY;

    // Column weight proportions
    const float wTagWeight = 14.0f;
    const float wDataWeight = 32.0f;
    const float wStateWeight = 4.0f;
    const float wMESIWeight = 4.0f;

    if (ImGui::BeginTable("##mem_table", 4, flags, ImVec2(available.x, available.y))) {
        ImGui::TableSetupScrollFreeze(0, 1);

        ImGui::TableSetupColumn("Tag", ImGuiTableColumnFlags_WidthStretch, wTagWeight);
        ImGui::TableSetupColumn("Data", ImGuiTableColumnFlags_WidthStretch, wDataWeight);
        ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthStretch, wStateWeight);
        ImGui::TableSetupColumn("MESI", ImGuiTableColumnFlags_WidthStretch, wMESIWeight);
        ImGui::TableHeadersRow();

        for (int set = 0; set < kSets; ++set) {
            // Set header row
            ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("set%d", set);

            // Apply header background to all columns
            ImU32 bg = ImGui::GetColorU32(ImGuiCol_TableHeaderBg);
            for (int c = 0; c < 4; ++c) {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, bg, c);
            }

            // Two ways per set
            for (int way = 0; way < kWaysPerSet; ++way) {
                int r = idx(set, way);
                const Row& row = m_rows[r];
                const char* mesi = deriveMESI(row.state);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted(row.tag.c_str());

                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(row.data.c_str());

                ImGui::TableSetColumnIndex(2);
                ImGui::TextUnformatted(row.state.c_str());

                ImGui::TableSetColumnIndex(3);
                ImGui::TextUnformatted(mesi);
            }
        }

        ImGui::EndTable();
    }
}

// ============================================================================
// Setters by Linear Index
// ============================================================================

void MemCacheTable::setRow(int row, const std::string& tag,
    const std::string& data, const std::string& state) {
    int r = rowClamp(row);
    m_rows[r].tag = tag;
    m_rows[r].data = data;
    m_rows[r].state = state;
}

void MemCacheTable::setTag(int row, const std::string& tag) {
    m_rows[rowClamp(row)].tag = tag;
}

void MemCacheTable::setData(int row, const std::string& data) {
    m_rows[rowClamp(row)].data = data;
}

void MemCacheTable::setState(int row, const std::string& state) {
    m_rows[rowClamp(row)].state = state;
}

// ============================================================================
// Setters by Set/Way
// ============================================================================

void MemCacheTable::setBySetWay(int setIndex, int wayIndex,
    const std::string& tag, const std::string& data,
    const std::string& state) {
    int r = idx(setIndex, wayIndex);
    setRow(r, tag, data, state);
}

void MemCacheTable::setTagBySetWay(int setIndex, int wayIndex, const std::string& tag) {
    m_rows[idx(setIndex, wayIndex)].tag = tag;
}

void MemCacheTable::setDataBySetWay(int setIndex, int wayIndex, const std::string& data) {
    m_rows[idx(setIndex, wayIndex)].data = data;
}

void MemCacheTable::setStateBySetWay(int setIndex, int wayIndex, const std::string& state) {
    m_rows[idx(setIndex, wayIndex)].state = state;
}