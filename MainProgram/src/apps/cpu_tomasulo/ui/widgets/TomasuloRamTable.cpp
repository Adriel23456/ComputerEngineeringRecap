// ============================================================================
// File: src/apps/cpu_tomasulo/ui/widgets/TomasuloRamTable.cpp
// ============================================================================

/**
 * @file TomasuloRamTable.cpp
 * @brief Implementation of TomasuloRamTable.
 *
 * Pure renderer. All data reads go through the bound TomasuloRAM*.
 *
 * Responsibilities:
 *   - Page math (firstRow, rowsOnPage, totalPages).
 *   - Navigation bar with auto-scaled info text and |<< < [n] > >>| buttons.
 *   - Four-column table (Address | Hex | Decimal | Double).
 */

#include "apps/cpu_tomasulo/ui/widgets/TomasuloRamTable.h"
#include "apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"

#include <imgui.h>
#include <cstdio>
#include <cstring>
#include <algorithm>

 // ============================================================================
 // Data Source Binding
 // ============================================================================

void TomasuloRamTable::bindDataSource(const TomasuloRAM* ram) {
    m_dataSource = ram;
}

// ============================================================================
// Derived Constants
// ============================================================================

int TomasuloRamTable::totalRows() const {
    return m_dataSource ? static_cast<int>(m_dataSource->totalRows()) : 0;
}

int TomasuloRamTable::totalPages() const {
    int rows = totalRows();
    return (rows + kRowsPerPage - 1) / kRowsPerPage;
}

// ============================================================================
// Page Control
// ============================================================================

void TomasuloRamTable::setPage(int page) {
    int maxPage = totalPages() - 1;
    m_currentPage = std::clamp(page, 0, std::max(0, maxPage));
}

void TomasuloRamTable::resetPage() { m_currentPage = 0; }

// ============================================================================
// Formatting
// ============================================================================

std::string TomasuloRamTable::formatAddress(uint64_t addr) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "0x%llX", static_cast<unsigned long long>(addr));
    return std::string(buf);
}

std::string TomasuloRamTable::formatHex(uint64_t v) {
    char buf[24];
    std::snprintf(buf, sizeof(buf), "0x%016llX", static_cast<unsigned long long>(v));
    return std::string(buf);
}

// ============================================================================
// Page Math
// ============================================================================

int TomasuloRamTable::rowsOnPage(int page, int pages, int rows) {
    if (page < 0 || page >= pages) return 0;
    return std::min(rows - page * kRowsPerPage, kRowsPerPage);
}

int TomasuloRamTable::firstRowOfPage(int page) {
    return page * kRowsPerPage;
}

// ============================================================================
// Page Navigation Bar
// ============================================================================

void TomasuloRamTable::renderPageNavigation(float availableWidth) {
    if (!m_dataSource) return;

    const int tRows = totalRows();
    const int tPages = totalPages();

    const float NAV_BUTTON_W = 50.0f;
    const float NAV_BUTTON_H = 28.0f;
    const float PAGE_INPUT_W = 80.0f;
    const float GAP = 6.0f;
    const float MARGIN = 8.0f;

    const float navTotalW = NAV_BUTTON_W * 4 + PAGE_INPUT_W + GAP * 4;
    const float textRegionW = availableWidth - navTotalW - MARGIN * 2;

    int firstRow = firstRowOfPage(m_currentPage);
    int rows = rowsOnPage(m_currentPage, tPages, tRows);
    int lastRow = firstRow + rows - 1;

    uint64_t addrStart = static_cast<uint64_t>(firstRow) * TomasuloRAM::kStep;
    uint64_t addrEnd = static_cast<uint64_t>(lastRow) * TomasuloRAM::kStep;

    char pageInfo[128];
    std::snprintf(pageInfo, sizeof(pageInfo),
        "Page %d / %d   |   Rows %d - %d   |   %s - %s   |   %d rows",
        m_currentPage + 1, tPages,
        firstRow, lastRow,
        formatAddress(addrStart).c_str(),
        formatAddress(addrEnd).c_str(),
        rows);

    // ── Auto-scaled info text ─────────────────────────────────
    {
        ImVec2 fullTextSize = ImGui::CalcTextSize(pageInfo);
        float  scale = 1.0f;
        if (textRegionW > 0.0f && fullTextSize.x > textRegionW)
            scale = std::max(textRegionW / fullTextSize.x, 0.55f);

        if (scale < 1.0f) ImGui::SetWindowFontScale(scale);

        float offsetY = (NAV_BUTTON_H - ImGui::GetFontSize() * scale) * 0.5f;
        if (offsetY > 0.0f) ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);

        ImGui::TextColored(ImVec4(0.7f, 0.8f, 0.9f, 1.0f), "%s", pageInfo);

        if (scale < 1.0f) ImGui::SetWindowFontScale(1.0f);
    }

    // ── Navigation buttons ────────────────────────────────────
    float navStartX = availableWidth - navTotalW - MARGIN;
    if (navStartX < 0.0f) navStartX = 0.0f;
    ImGui::SameLine(navStartX);

    auto navBtn = [&](const char* label, bool disabled, auto action) {
        if (disabled) ImGui::BeginDisabled();
        if (ImGui::Button(label, ImVec2(NAV_BUTTON_W, NAV_BUTTON_H))) action();
        if (disabled) ImGui::EndDisabled();
        };

    navBtn("|<<", m_currentPage == 0, [&] { m_currentPage = 0; });
    ImGui::SameLine(0.0f, GAP);
    navBtn("<", m_currentPage == 0, [&] { m_currentPage = std::max(0, m_currentPage - 1); });
    ImGui::SameLine(0.0f, GAP);

    // Page number input
    ImGui::PushItemWidth(PAGE_INPUT_W);
    int displayPage = m_currentPage + 1;
    if (ImGui::InputInt("##PageInput", &displayPage, 0, 0,
        ImGuiInputTextFlags_EnterReturnsTrue)) {
        setPage(displayPage - 1);
    }
    ImGui::PopItemWidth();

    ImGui::SameLine(0.0f, GAP);
    navBtn(">", m_currentPage >= tPages - 1, [&] { m_currentPage = std::min(tPages - 1, m_currentPage + 1); });
    ImGui::SameLine(0.0f, GAP);
    navBtn(">>|", m_currentPage >= tPages - 1, [&] { m_currentPage = tPages - 1; });
}

// ============================================================================
// Table Rendering
// ============================================================================

void TomasuloRamTable::renderTable(const char* id, float tableHeight) {
    if (!m_dataSource) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
            "RAM data source not bound.");
        return;
    }

    ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_SizingStretchProp;

    ImVec2 available = ImGui::GetContentRegionAvail();

    if (ImGui::BeginTable(id, 4, flags, ImVec2(available.x, tableHeight))) {
        ImGui::TableSetupScrollFreeze(0, 1);

        const float charW = ImGui::CalcTextSize("W").x;
        const float addrMin = std::max(120.0f, charW * 14.0f);
        const float hexMin = std::max(180.0f, charW * 20.0f);

        ImGui::TableSetupColumn("Address",
            ImGuiTableColumnFlags_WidthFixed, addrMin);
        ImGui::TableSetupColumn("Value (Hex) 'BigEndian'",
            ImGuiTableColumnFlags_WidthFixed, hexMin);
        ImGui::TableSetupColumn("Decimal (int64, 2's comp)",
            ImGuiTableColumnFlags_WidthStretch, 0.5f);
        ImGui::TableSetupColumn("Double (IEEE 754, 64-bit)",
            ImGuiTableColumnFlags_WidthStretch, 0.5f);
        ImGui::TableHeadersRow();

        const int tRows = totalRows();
        const int tPages = totalPages();
        int startRow = firstRowOfPage(m_currentPage);
        int rows = rowsOnPage(m_currentPage, tPages, tRows);

        for (int i = 0; i < rows; ++i) {
            int      globalRow = startRow + i;
            uint64_t addr = static_cast<uint64_t>(globalRow) * TomasuloRAM::kStep;
            uint64_t value = m_dataSource->read(static_cast<size_t>(globalRow));

            ImGui::TableNextRow();

            // ── Address ───────────────────────────────────────
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", formatAddress(addr).c_str());

            // ── Hex value + right-click copy context menu ─────
            ImGui::TableSetColumnIndex(1);
            {
                std::string hexStr = formatHex(value);
                ImGui::TextUnformatted(hexStr.c_str());

                std::string ctxId = "##ctx_hex_" + std::to_string(globalRow);
                if (ImGui::BeginPopupContextItem(ctxId.c_str())) {
                    if (ImGui::MenuItem("Copy HEX"))
                        ImGui::SetClipboardText(hexStr.c_str());
                    if (ImGui::MenuItem("Copy Address"))
                        ImGui::SetClipboardText(formatAddress(addr).c_str());
                    ImGui::EndPopup();
                }
            }

            // ── Decimal (signed 64-bit) ───────────────────────
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%lld",
                static_cast<long long>(static_cast<int64_t>(value)));

            // ── Double (IEEE 754) ─────────────────────────────
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

// ============================================================================
// Main Render
// ============================================================================

void TomasuloRamTable::render(const char* id) {
    ImVec2 available = ImGui::GetContentRegionAvail();

    const float NAV_HEIGHT = 36.0f;
    const float SPACING = 6.0f;
    const float TABLE_BOTTOM_PAD = 6.0f;

    renderPageNavigation(available.x);
    ImGui::Dummy(ImVec2(1.0f, SPACING));

    float tableHeight = available.y - NAV_HEIGHT - SPACING - TABLE_BOTTOM_PAD;
    if (tableHeight < 50.0f) tableHeight = 50.0f;

    renderTable(id, tableHeight);
}