/**
 * @file ROBTable.cpp
 * @brief Implementation of ROBTable.
 */

#include "apps/cpu_tomasulo/ui/widgets/ROBTable.h"
#include <imgui.h>
#include <cstdio>
#include <cstring>
#include <algorithm>

 // ============================================================================
 // Construction / Reset
 // ============================================================================

ROBTable::ROBTable() {
    resetAll();
}

void ROBTable::resetAll() {
    for (auto& e : m_entries) {
        e = ROBEntry{};
    }
    m_head = 0;
    m_tail = 0;
    m_count = 0;
    m_selectedEntry = -1;
}

// ============================================================================
// Data Access
// ============================================================================

ROBTable::ROBEntry& ROBTable::getEntry(int index) {
    static ROBEntry dummy{};
    if (index < 0 || index >= kEntryCount) return dummy;
    return m_entries[index];
}

const ROBTable::ROBEntry& ROBTable::getEntry(int index) const {
    static const ROBEntry dummy{};
    if (index < 0 || index >= kEntryCount) return dummy;
    return m_entries[index];
}

void ROBTable::setHead(int head) { m_head = head % kEntryCount; }
void ROBTable::setTail(int tail) { m_tail = tail % kEntryCount; }
void ROBTable::setCount(int count) { m_count = std::clamp(count, 0, kEntryCount); }

void ROBTable::setSelectedEntry(int idx) {
    if (idx < -1 || idx >= kEntryCount) idx = -1;
    m_selectedEntry = idx;
}

// ============================================================================
// Type / Register / Exception / SourceStation Strings
// ============================================================================

const char* ROBTable::typeToString(uint8_t type) {
    switch (type) {
    case 0: return "ALU";
    case 1: return "LOAD";
    case 2: return "STORE";
    case 3: return "BRANCH";
    case 4: return "FP_ALU";
    case 5: return "FP_MUL";
    case 6: return "INT_MUL";
    case 7: return "OTHER";
    default: return "???";
    }
}

const char* ROBTable::destRegToString(uint8_t reg) {
    static char buf[8];
    if (reg <= 12) {
        std::snprintf(buf, sizeof(buf), "R%d", reg);
    }
    else if (reg == 13) {
        return "UPPER";
    }
    else if (reg == 14) {
        return "LOWER";
    }
    else if (reg == 15) {
        return "PEID";
    }
    else {
        return "---";
    }
    return buf;
}

const char* ROBTable::exceptionToString(uint8_t exc) {
    switch (exc) {
    case 0x0: return "None";
    case 0x1: return "Overflow";
    case 0x2: return "DivZero";
    case 0x3: return "InvalidOp";
    case 0x4: return "PageFault";
    case 0x5: return "ProtFault";
    case 0x6: return "Misalign";
    case 0x7: return "Breakpoint";
    default:
    {
        static char buf[8];
        std::snprintf(buf, sizeof(buf), "0x%X", exc);
        return buf;
    }
    }
}

const char* ROBTable::sourceStationToString(uint8_t ss) {
    static const char* names[] = {
        "SB0", "SB1", "LB0", "LB1", "LB2",
        "IntALU0", "IntALU1", "FpALU0",
        "IntMUL0", "FpMUL0", "Branch0",
        "---", "---", "---", "---", "---"
    };
    if (ss < 16) return names[ss];
    return "???";
}

// ============================================================================
// Formatting
// ============================================================================

std::string ROBTable::formatHexTrimmed(uint64_t v) {
    char buf[24];
    std::snprintf(buf, sizeof(buf), "0x%llX", static_cast<unsigned long long>(v));
    return std::string(buf);
}

std::string ROBTable::formatHexFull(uint64_t v) {
    char buf[24];
    std::snprintf(buf, sizeof(buf), "0x%016llX", static_cast<unsigned long long>(v));
    return std::string(buf);
}

// ============================================================================
// Status Bar
// ============================================================================

void ROBTable::renderStatusBar(float availableWidth) {
    // Head indicator (blue)
    ImGui::TextColored(ImVec4(0.3f, 0.6f, 1.0f, 1.0f), "HEAD: %d", m_head);

    ImGui::SameLine(0, 25.0f);

    // Tail indicator (green)
    ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.5f, 1.0f), "TAIL: %d", m_tail);

    ImGui::SameLine(0, 25.0f);

    // Count
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.3f, 1.0f), "COUNT: %d / %d",
        m_count, kEntryCount);

    ImGui::SameLine(0, 25.0f);

    // Full indicator
    if (isFull()) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "[FULL]");
    }
    else if (m_count == 0) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "[EMPTY]");
    }
    else {
        ImGui::TextColored(ImVec4(0.5f, 0.8f, 0.5f, 1.0f), "[%d FREE]",
            kEntryCount - m_count);
    }

    // Selected entry info
    if (m_selectedEntry >= 0) {
        ImGui::SameLine(0, 25.0f);
        ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f),
            "SELECTED: #%d", m_selectedEntry);
    }
}

// ============================================================================
// Main Table
// ============================================================================

void ROBTable::renderMainTable(const char* id, float tableHeight) {
    ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_SizingStretchProp;

    if (ImGui::BeginTable(id, 9, flags, ImVec2(0, tableHeight))) {
        ImGui::TableSetupScrollFreeze(0, 1);

        const float charW = ImGui::CalcTextSize("W").x;

        ImGui::TableSetupColumn("#",
            ImGuiTableColumnFlags_WidthFixed, charW * 4.0f);
        ImGui::TableSetupColumn("Busy",
            ImGuiTableColumnFlags_WidthFixed, charW * 5.0f);
        ImGui::TableSetupColumn("Ready",
            ImGuiTableColumnFlags_WidthFixed, charW * 6.0f);
        ImGui::TableSetupColumn("Type",
            ImGuiTableColumnFlags_WidthFixed, charW * 8.0f);
        ImGui::TableSetupColumn("Dest",
            ImGuiTableColumnFlags_WidthFixed, charW * 7.0f);
        ImGui::TableSetupColumn("Value (Hex)",
            ImGuiTableColumnFlags_WidthStretch, 1.0f);
        ImGui::TableSetupColumn("PC",
            ImGuiTableColumnFlags_WidthStretch, 0.6f);
        ImGui::TableSetupColumn("Exc",
            ImGuiTableColumnFlags_WidthFixed, charW * 10.0f);
        ImGui::TableSetupColumn("Source",
            ImGuiTableColumnFlags_WidthFixed, charW * 9.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < kEntryCount; ++i) {
            const ROBEntry& e = m_entries[i];

            ImGui::TableNextRow();

            // ── Row highlighting ──────────────────────────────
            bool isHead = (i == m_head);
            bool isTail = (i == m_tail && m_count < kEntryCount);
            bool isSelected = (i == m_selectedEntry);

            ImU32 rowColor = 0;
            if (isHead && isTail) {
                rowColor = IM_COL32(40, 80, 80, 80);   // Teal when Head==Tail
            }
            else if (isHead) {
                rowColor = IM_COL32(30, 50, 120, 80);   // Blue for Head
            }
            else if (isTail) {
                rowColor = IM_COL32(30, 100, 50, 80);   // Green for Tail
            }

            if (isSelected) {
                rowColor = IM_COL32(120, 80, 20, 100);  // Orange for selected
            }

            if (rowColor != 0) {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, rowColor);
            }

            // ── Index column ──────────────────────────────────
            ImGui::TableSetColumnIndex(0);
            {
                // Clickable selectable spanning the row
                char selectableId[16];
                std::snprintf(selectableId, sizeof(selectableId), "%d##rob", i);

                bool wasSelected = (m_selectedEntry == i);
                if (ImGui::Selectable(selectableId, wasSelected,
                    ImGuiSelectableFlags_SpanAllColumns |
                    ImGuiSelectableFlags_AllowItemOverlap)) {
                    m_selectedEntry = (m_selectedEntry == i) ? -1 : i;
                }

                // Head/Tail markers
                ImGui::SameLine();
                if (isHead && isTail) {
                    ImGui::TextColored(ImVec4(0.3f, 0.9f, 0.9f, 1.0f), "HT");
                }
                else if (isHead) {
                    ImGui::TextColored(ImVec4(0.3f, 0.6f, 1.0f, 1.0f), "H");
                }
                else if (isTail) {
                    ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.5f, 1.0f), "T");
                }
            }

            // ── Busy ──────────────────────────────────────────
            ImGui::TableSetColumnIndex(1);
            if (e.busy) {
                ImGui::TextColored(ImVec4(1.0f, 0.9f, 0.3f, 1.0f), "Yes");
            }
            else {
                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "No");
            }

            // ── Ready ─────────────────────────────────────────
            ImGui::TableSetColumnIndex(2);
            if (!e.busy) {
                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            }
            else if (e.ready) {
                ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "Ready");
            }
            else {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "Wait");
            }

            // ── Type ──────────────────────────────────────────
            ImGui::TableSetColumnIndex(3);
            if (e.busy) {
                ImGui::Text("%s", typeToString(e.type));
            }
            else {
                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            }

            // ── Dest ──────────────────────────────────────────
            ImGui::TableSetColumnIndex(4);
            if (e.busy) {
                ImGui::Text("%s", destRegToString(e.destReg));
            }
            else {
                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            }

            // ── Value ─────────────────────────────────────────
            ImGui::TableSetColumnIndex(5);
            if (e.busy) {
                std::string hex = formatHexFull(e.value);
                ImGui::TextUnformatted(hex.c_str());
            }
            else {
                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            }

            // ── PC ────────────────────────────────────────────
            ImGui::TableSetColumnIndex(6);
            if (e.busy) {
                std::string pc = formatHexTrimmed(e.pc);
                ImGui::Text("%s", pc.c_str());
            }
            else {
                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            }

            // ── Exception ─────────────────────────────────────
            ImGui::TableSetColumnIndex(7);
            if (!e.busy) {
                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            }
            else if (e.exception != 0) {
                ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "%s",
                    exceptionToString(e.exception));
            }
            else {
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "None");
            }

            // ── Source Station ─────────────────────────────────
            ImGui::TableSetColumnIndex(8);
            if (e.busy) {
                ImGui::Text("%s", sourceStationToString(e.sourceStation));
            }
            else {
                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            }
        }

        ImGui::EndTable();
    }
}

// ============================================================================
// Detail Panel
// ============================================================================

void ROBTable::renderDetailPanel(float availableWidth, float panelHeight) {
    if (m_selectedEntry < 0) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
            "Click a ROB entry above to view details.");
        return;
    }

    const ROBEntry& e = m_entries[m_selectedEntry];

    // Header
    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f),
        "Entry #%d Details", m_selectedEntry);

    if (m_selectedEntry == m_head) {
        ImGui::SameLine(0, 10.0f);
        ImGui::TextColored(ImVec4(0.3f, 0.6f, 1.0f, 1.0f), "[HEAD]");
    }
    if (m_selectedEntry == m_tail && m_count < kEntryCount) {
        ImGui::SameLine(0, 10.0f);
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.5f, 1.0f), "[TAIL]");
    }

    ImGui::Separator();
    ImGui::Dummy(ImVec2(1, 4));

    // Use an ImGui table for the 3-column layout — handles overflow & scrollbar properly
    ImGuiTableFlags tflags =
        ImGuiTableFlags_SizingFixedFit |
        ImGuiTableFlags_ScrollX;

    if (ImGui::BeginTable("##ROBDetailCols", 3, tflags)) {
        // Size columns from the widest possible content + padding
        const float colMin = ImGui::CalcTextSize("StoreData:     0x0000000000000000").x + 30.0f;

        ImGui::TableSetupColumn("Flags", ImGuiTableColumnFlags_WidthFixed, colMin);
        ImGui::TableSetupColumn("Branch", ImGuiTableColumnFlags_WidthFixed, colMin);
        ImGui::TableSetupColumn("Store", ImGuiTableColumnFlags_WidthFixed, colMin);

        ImGui::TableNextRow();

        // ── Flags Info ────────────────────────────────────────
        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.3f, 1.0f), "Flags Info");
        ImGui::Separator();
        ImGui::Text("ModifiesFlags: %s", e.modifiesFlags ? "Yes" : "No");
        ImGui::Text("FlagsValid:    %s", e.flagsValid ? "Yes" : "No");
        if (e.flagsValid) {
            char flagsBin[8];
            std::snprintf(flagsBin, sizeof(flagsBin), "%c%c%c%c",
                (e.flagsResult & 0x8) ? '1' : '0',
                (e.flagsResult & 0x4) ? '1' : '0',
                (e.flagsResult & 0x2) ? '1' : '0',
                (e.flagsResult & 0x1) ? '1' : '0');
            ImGui::Text("FlagsResult:   0x%X (%s)", e.flagsResult, flagsBin);
        }
        else {
            ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f),
                "FlagsResult:   ---");
        }

        // ── Branch Info ───────────────────────────────────────
        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(ImVec4(0.3f, 0.8f, 1.0f, 1.0f), "Branch Info");
        ImGui::Separator();
        ImGui::Text("Predicted:     %s", e.predicted ? "Taken" : "NotTaken");
        ImGui::Text("BranchTaken:   %s", e.branchTaken ? "Yes" : "No");
        {
            std::string target = formatHexTrimmed(e.branchTarget);
            ImGui::Text("BranchTarget:  %s", target.c_str());
        }
        if (e.mispredict) {
            ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
                "Mispredict:    YES");
        }
        else {
            ImGui::Text("Mispredict:    No");
        }

        // ── Store Info ────────────────────────────────────────
        ImGui::TableSetColumnIndex(2);
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.7f, 1.0f), "Store Info");
        ImGui::Separator();
        ImGui::Text("StoreReady:    %s", e.storeReady ? "Yes" : "No");
        {
            std::string addr = formatHexTrimmed(e.storeAddr);
            ImGui::Text("StoreAddr:     %s", addr.c_str());
        }
        {
            std::string data = formatHexFull(e.storeData);
            ImGui::Text("StoreData:     %s", data.c_str());
        }

        ImGui::EndTable();
    }
}

// ============================================================================
// Main Render
// ============================================================================

void ROBTable::render(const char* id) {
    ImVec2 available = ImGui::GetContentRegionAvail();

    const float STATUS_HEIGHT = 30.0f;
    const float GAP = 5.0f;

    bool hasSelection = (m_selectedEntry >= 0);

    // Proportional split: table gets 60%, detail gets 40% when selected
    // When no selection, detail collapses to a single hint line
    float usable = available.y - STATUS_HEIGHT - GAP * 3;

    float detailH, tableHeight;
    if (hasSelection) {
        detailH = usable * 0.40f;
        tableHeight = usable - detailH;
    }
    else {
        detailH = 55.0f;
        tableHeight = usable - detailH;
    }
    if (tableHeight < 80.0f) tableHeight = 80.0f;
    if (detailH < 55.0f) detailH = 55.0f;

    // ── Status bar ────────────────────────────────────────────
    renderStatusBar(available.x);

    ImGui::Dummy(ImVec2(1, GAP));

    // ── Main table ────────────────────────────────────────────
    ImGui::BeginChild("##ROBTableArea", ImVec2(available.x, tableHeight), false);
    {
        renderMainTable(id, tableHeight);
    }
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(1, GAP));

    // ── Detail panel ──────────────────────────────────────────
    ImGui::BeginChild("##ROBDetailPanel", ImVec2(available.x, detailH), true);
    {
        renderDetailPanel(available.x, detailH);
    }
    ImGui::EndChild();
}