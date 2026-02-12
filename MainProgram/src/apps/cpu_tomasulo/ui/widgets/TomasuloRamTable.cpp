/**
 * @file TomasuloRamTable.cpp
 * @brief Implementation of TomasuloRamTable.
 */

#include "apps/cpu_tomasulo/ui/widgets/TomasuloRamTable.h"
#include <imgui.h>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>

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
// Data Access
// ============================================================================

void TomasuloRamTable::setDataByIndex(int rowIndex, uint64_t value) {
    if (rowIndex >= 0 && rowIndex < kTotalRows) {
        m_values[rowIndex] = value;
    }
}

void TomasuloRamTable::setDataByAddress(uint64_t address, uint64_t value) {
    if ((address % kStep) != 0) {
        std::cerr << "[TomasuloRAM] setDataByAddress: address not 8-byte aligned: "
            << formatAddress(address) << std::endl;
        return;
    }

    uint64_t maxAddr = kBaseAddr + static_cast<uint64_t>(kTotalRows - 1) * kStep;
    if (address > maxAddr) {
        std::cerr << "[TomasuloRAM] setDataByAddress: address out of range: "
            << formatAddress(address) << std::endl;
        return;
    }

    int rowIndex = static_cast<int>((address - kBaseAddr) / kStep);
    setDataByIndex(rowIndex, value);
}

uint64_t TomasuloRamTable::getDataByIndex(int rowIndex) const {
    if (rowIndex >= 0 && rowIndex < kTotalRows) {
        return m_values[rowIndex];
    }
    return 0;
}

void TomasuloRamTable::resetAll() {
    m_values.fill(0);
    m_currentPage = 0;
}

void TomasuloRamTable::setPage(int page) {
    m_currentPage = std::clamp(page, 0, kTotalPages - 1);
}

// ============================================================================
// Page Helpers
// ============================================================================

int TomasuloRamTable::rowsOnPage(int page) {
    if (page < 0 || page >= kTotalPages) return 0;
    int startRow = page * kRowsPerPage;
    int remaining = kTotalRows - startRow;
    return std::min(remaining, kRowsPerPage);
}

int TomasuloRamTable::firstRowOfPage(int page) {
    return page * kRowsPerPage;
}

// ============================================================================
// Page Navigation
// ============================================================================

void TomasuloRamTable::renderPageNavigation(float availableWidth) {
    // ── Layout constants ──────────────────────────────────────
    const float NAV_BUTTON_W = 50.0f;
    const float NAV_BUTTON_H = 28.0f;
    const float PAGE_INPUT_W = 80.0f;
    const float GAP = 6.0f;
    const float MARGIN = 8.0f;

    // Fixed total width of the navigation controls (4 buttons + input + gaps)
    const float navTotalW = NAV_BUTTON_W * 4 + PAGE_INPUT_W + GAP * 4;

    // The info text gets whatever space is left
    const float textRegionW = availableWidth - navTotalW - MARGIN * 2;

    // ── Compute page info ─────────────────────────────────────
    int firstRow = firstRowOfPage(m_currentPage);
    int rows = rowsOnPage(m_currentPage);
    int lastRow = firstRow + rows - 1;

    uint64_t addrStart = kBaseAddr + static_cast<uint64_t>(firstRow) * kStep;
    uint64_t addrEnd = kBaseAddr + static_cast<uint64_t>(lastRow) * kStep;

    char pageInfo[128];
    std::snprintf(pageInfo, sizeof(pageInfo),
        "Page %d / %d   |   Rows %d - %d   |   %s - %s   |   %d rows",
        m_currentPage + 1, kTotalPages,
        firstRow, lastRow,
        formatAddress(addrStart).c_str(),
        formatAddress(addrEnd).c_str(),
        rows);

    // ── Info text (fixed-width region, auto-scaled) ───────────
    {
        ImVec2 fullTextSize = ImGui::CalcTextSize(pageInfo);
        float scale = 1.0f;
        if (textRegionW > 0.0f && fullTextSize.x > textRegionW) {
            scale = textRegionW / fullTextSize.x;
            scale = std::max(scale, 0.55f); // Don't shrink below 55%
        }

        if (scale < 1.0f) ImGui::SetWindowFontScale(scale);

        // Vertically center the text with the nav buttons
        float textH = ImGui::GetFontSize() * scale;
        float offsetY = (NAV_BUTTON_H - textH) * 0.5f;
        if (offsetY > 0.0f) {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offsetY);
        }

        ImGui::TextColored(ImVec4(0.7f, 0.8f, 0.9f, 1.0f), "%s", pageInfo);

        if (scale < 1.0f) ImGui::SetWindowFontScale(1.0f);
    }

    // ── Navigation buttons (right-anchored at fixed position) ─
    float navStartX = availableWidth - navTotalW - MARGIN;
    if (navStartX < 0.0f) navStartX = 0.0f;

    ImGui::SameLine(navStartX);

    // |<< First
    {
        bool disabled = (m_currentPage == 0);
        if (disabled) ImGui::BeginDisabled();
        if (ImGui::Button("|<<", ImVec2(NAV_BUTTON_W, NAV_BUTTON_H))) {
            m_currentPage = 0;
        }
        if (disabled) ImGui::EndDisabled();
    }

    ImGui::SameLine(0.0f, GAP);

    // < Prev
    {
        bool disabled = (m_currentPage == 0);
        if (disabled) ImGui::BeginDisabled();
        if (ImGui::Button("<", ImVec2(NAV_BUTTON_W, NAV_BUTTON_H))) {
            m_currentPage = std::max(0, m_currentPage - 1);
        }
        if (disabled) ImGui::EndDisabled();
    }

    ImGui::SameLine(0.0f, GAP);

    // Direct page input
    {
        ImGui::PushItemWidth(PAGE_INPUT_W);
        int displayPage = m_currentPage + 1; // 1-based for display
        if (ImGui::InputInt("##PageInput", &displayPage, 0, 0,
            ImGuiInputTextFlags_EnterReturnsTrue)) {
            setPage(displayPage - 1);
        }
        ImGui::PopItemWidth();
    }

    ImGui::SameLine(0.0f, GAP);

    // > Next
    {
        bool disabled = (m_currentPage >= kTotalPages - 1);
        if (disabled) ImGui::BeginDisabled();
        if (ImGui::Button(">", ImVec2(NAV_BUTTON_W, NAV_BUTTON_H))) {
            m_currentPage = std::min(kTotalPages - 1, m_currentPage + 1);
        }
        if (disabled) ImGui::EndDisabled();
    }

    ImGui::SameLine(0.0f, GAP);

    // >>| Last
    {
        bool disabled = (m_currentPage >= kTotalPages - 1);
        if (disabled) ImGui::BeginDisabled();
        if (ImGui::Button(">>|", ImVec2(NAV_BUTTON_W, NAV_BUTTON_H))) {
            m_currentPage = kTotalPages - 1;
        }
        if (disabled) ImGui::EndDisabled();
    }
}

// ============================================================================
// Table Rendering
// ============================================================================

void TomasuloRamTable::renderTable(const char* id, float tableHeight) {
    ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_SizingStretchProp;

    ImVec2 available = ImGui::GetContentRegionAvail();

    if (ImGui::BeginTable(id, 4, flags, ImVec2(available.x, tableHeight))) {
        ImGui::TableSetupScrollFreeze(0, 1);

        const float charW = ImGui::CalcTextSize("W").x;
        const float addrMin = std::max(120.0f, charW * 14.0f); // Trimmed addresses are shorter
        const float hexMin = std::max(180.0f, charW * 20.0f);

        ImGui::TableSetupColumn("Address",
            ImGuiTableColumnFlags_WidthFixed, addrMin);
        ImGui::TableSetupColumn("Value (Hex)",
            ImGuiTableColumnFlags_WidthFixed, hexMin);
        ImGui::TableSetupColumn("Decimal (int64, 2's comp)",
            ImGuiTableColumnFlags_WidthStretch, 0.5f);
        ImGui::TableSetupColumn("Double (IEEE 754, 64-bit)",
            ImGuiTableColumnFlags_WidthStretch, 0.5f);
        ImGui::TableHeadersRow();

        int startRow = firstRowOfPage(m_currentPage);
        int rows = rowsOnPage(m_currentPage);

        for (int i = 0; i < rows; ++i) {
            int globalRow = startRow + i;
            uint64_t addr = kBaseAddr + static_cast<uint64_t>(globalRow) * kStep;
            uint64_t value = m_values[globalRow];

            ImGui::TableNextRow();

            // Address column (trimmed hex)
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", formatAddress(addr).c_str());

            // Hex value column (full 16 digits) with context menu
            ImGui::TableSetColumnIndex(1);
            {
                std::string hexStr = formatHex(value);
                ImGui::TextUnformatted(hexStr.c_str());

                std::string ctxId = "##ctx_hex_" + std::to_string(globalRow);
                if (ImGui::BeginPopupContextItem(ctxId.c_str())) {
                    if (ImGui::MenuItem("Copy HEX")) {
                        ImGui::SetClipboardText(hexStr.c_str());
                    }
                    if (ImGui::MenuItem("Copy Address")) {
                        ImGui::SetClipboardText(formatAddress(addr).c_str());
                    }
                    ImGui::EndPopup();
                }
            }

            // Decimal column (signed 64-bit, two's complement)
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

// ============================================================================
// Main Render
// ============================================================================

void TomasuloRamTable::render(const char* id) {
    ImVec2 available = ImGui::GetContentRegionAvail();

    const float NAV_HEIGHT = 36.0f;
    const float SPACING = 4.0f;

    // Navigation bar at top
    renderPageNavigation(available.x);

    ImGui::Dummy(ImVec2(1.0f, SPACING));

    // Table fills remaining height
    float tableHeight = available.y - NAV_HEIGHT - SPACING;
    if (tableHeight < 50.0f) tableHeight = 50.0f;

    renderTable(id, tableHeight);
}