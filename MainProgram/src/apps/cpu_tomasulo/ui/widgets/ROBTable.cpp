// ============================================================================
// File: src/apps/cpu_tomasulo/ui/widgets/ROBTable.cpp
// ============================================================================

/**
 * @file ROBTable.cpp
 * @brief Implementation of ROBTable.
 *
 * Handles:
 *   - Entry storage and pointer management.
 *   - Status bar (Head / Tail / Count / Free).
 *   - Main 32-row table with click-to-select and row coloring.
 *   - Detail panel (flags, branch, store) for the selected entry.
 */

#include "apps/cpu_tomasulo/ui/widgets/ROBTable.h"
#include <imgui.h>
#include <cstdio>
#include <cstring>
#include <algorithm>

 // ============================================================================
 // Construction / Reset
 // ============================================================================

ROBTable::ROBTable() { resetAll(); }

void ROBTable::resetAll() {
    for (auto& e : m_entries) e = ROBEntry{};
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
// Type / Register / Exception / Source Station String Tables
// ============================================================================

const char* ROBTable::typeToString(uint8_t type) {
    switch (type) {
    case 0: return "INT_ALU";
    case 1: return "INT_MUL";
    case 2: return "FP_ALU";
    case 3: return "FP_MUL";
    case 4: return "LOAD";
    case 5: return "STORE";
    case 6: return "BRANCH";
    case 7: return "CMP/FLG";
    default: return "???";
    }
}

const char* ROBTable::destRegToString(uint8_t reg) {
    static char buf[8];
    if (reg <= 12) { std::snprintf(buf, sizeof(buf), "R%d", reg); return buf; }
    else if (reg == 13) return "UPPER";
    else if (reg == 14) return "LOWER";
    else if (reg == 15) return "PEID";
    return "---";
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
    default: {
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

void ROBTable::renderStatusBar(float /*availableWidth*/) {
    ImGui::TextColored(ImVec4(0.3f, 0.6f, 1.0f, 1.0f), "HEAD: %d", m_head);
    ImGui::SameLine(0, 25.0f);
    ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.5f, 1.0f), "TAIL: %d", m_tail);
    ImGui::SameLine(0, 25.0f);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.3f, 1.0f), "COUNT: %d / %d", m_count, kEntryCount);
    ImGui::SameLine(0, 25.0f);

    if (isFull()) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "[FULL]");
    }
    else if (m_count == 0) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "[EMPTY]");
    }
    else {
        ImGui::TextColored(ImVec4(0.5f, 0.8f, 0.5f, 1.0f),
            "[%d FREE]", kEntryCount - m_count);
    }

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

            bool isHead = (i == m_head);
            bool isTail = (i == m_tail && m_count < kEntryCount);
            bool isSelected = (i == m_selectedEntry);

            // ── Row background highlight ──────────────────────
            ImU32 rowColor = 0;
            if (isHead && isTail) rowColor = IM_COL32(40, 80, 80, 80);  // teal
            else if (isHead)           rowColor = IM_COL32(30, 50, 120, 80);  // blue
            else if (isTail)           rowColor = IM_COL32(30, 100, 50, 80);  // green
            if (isSelected)            rowColor = IM_COL32(120, 80, 20, 100);  // orange

            if (rowColor != 0)
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, rowColor);

            // ── Index + click-to-select ───────────────────────
            ImGui::TableSetColumnIndex(0);
            {
                char selId[16];
                std::snprintf(selId, sizeof(selId), "%d##rob", i);

                bool wasSelected = (m_selectedEntry == i);
                if (ImGui::Selectable(selId, wasSelected,
                    ImGuiSelectableFlags_SpanAllColumns |
                    ImGuiSelectableFlags_AllowItemOverlap)) {
                    m_selectedEntry = (m_selectedEntry == i) ? -1 : i;
                }

                // Head / Tail markers
                ImGui::SameLine();
                if (isHead && isTail) ImGui::TextColored(ImVec4(0.3f, 0.9f, 0.9f, 1.0f), "HT");
                else if (isHead)           ImGui::TextColored(ImVec4(0.3f, 0.6f, 1.0f, 1.0f), "H");
                else if (isTail)           ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.5f, 1.0f), "T");
            }

            // ── Busy ─────────────────────────────────────────
            ImGui::TableSetColumnIndex(1);
            e.busy
                ? ImGui::TextColored(ImVec4(1.0f, 0.9f, 0.3f, 1.0f), "Yes")
                : ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "No");

            // ── Ready ────────────────────────────────────────
            ImGui::TableSetColumnIndex(2);
            if (!e.busy)  ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            else if (e.ready)  ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "Ready");
            else               ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "Wait");

            // ── Type ─────────────────────────────────────────
            ImGui::TableSetColumnIndex(3);
            if (!e.busy) {
                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            }
            else if (e.sourceStation == 15) {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.3f, 1.0f), "SPECIAL");
            }
            else {
                ImGui::Text("%s", typeToString(e.type));
            }

            // ── Dest ─────────────────────────────────────────
            ImGui::TableSetColumnIndex(4);
            if (!e.busy) {
                ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
            }
            else if (e.type == 5 || e.type == 6 || e.type == 7 || e.sourceStation == 15) {
                ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "NaN");
            }
            else {
                ImGui::Text("%s", destRegToString(e.destReg));
            }

            // ── Value ────────────────────────────────────────
            ImGui::TableSetColumnIndex(5);
            e.busy
                ? ImGui::TextUnformatted(formatHexFull(e.value).c_str())
                : ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");

            // ── PC ───────────────────────────────────────────
            ImGui::TableSetColumnIndex(6);
            e.busy
                ? ImGui::Text("%s", formatHexTrimmed(e.pc).c_str())
                : ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");

            // ── Exception ────────────────────────────────────
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

            // ── Source Station ───────────────────────────────
            ImGui::TableSetColumnIndex(8);
            e.busy
                ? ImGui::Text("%s", sourceStationToString(e.sourceStation))
                : ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "---");
        }

        ImGui::EndTable();
    }
}

// ============================================================================
// Detail Panel  (flags / branch / store for the selected entry)
// ============================================================================

void ROBTable::renderDetailPanel(float /*availableWidth*/, float /*panelHeight*/) {
    if (m_selectedEntry < 0) {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
            "Click a ROB entry above to view details.");
        return;
    }

    const ROBEntry& e = m_entries[m_selectedEntry];

    // ── Header ───────────────────────────────────────────────
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

    // ── Three-column detail table ─────────────────────────────
    ImGuiTableFlags tflags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX;

    if (ImGui::BeginTable("##ROBDetailCols", 3, tflags)) {
        const float colMin =
            ImGui::CalcTextSize("StoreData:     0x0000000000000000").x + 30.0f;

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
            ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "FlagsResult:   ---");
        }

        // ── Branch Info ───────────────────────────────────────
        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(ImVec4(0.3f, 0.8f, 1.0f, 1.0f), "Branch Info");
        ImGui::Separator();
        ImGui::Text("Predicted:     %s", e.predicted ? "Taken" : "NotTaken");
        ImGui::Text("BranchTaken:   %s", e.branchTaken ? "Yes" : "No");
        ImGui::Text("BranchTarget:  %s", formatHexTrimmed(e.branchTarget).c_str());
        e.mispredict
            ? ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Mispredict:    YES")
            : ImGui::Text("Mispredict:    No");

        // ── Store Info ────────────────────────────────────────
        ImGui::TableSetColumnIndex(2);
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.7f, 1.0f), "Store Info");
        ImGui::Separator();
        ImGui::Text("StoreReady:    %s", e.storeReady ? "Yes" : "No");
        ImGui::Text("StoreAddr:     %s", formatHexTrimmed(e.storeAddr).c_str());
        ImGui::Text("StoreData:     %s", formatHexFull(e.storeData).c_str());

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
    bool        hasSelection = (m_selectedEntry >= 0);

    // Proportional split: 60 % table / 40 % detail when an entry is selected;
    // detail collapses to a single hint line when nothing is selected.
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
    renderMainTable(id, tableHeight);
    ImGui::EndChild();

    ImGui::Dummy(ImVec2(1, GAP));

    // ── Detail panel ──────────────────────────────────────────
    ImGui::BeginChild("##ROBDetailPanel", ImVec2(available.x, detailH), true);
    renderDetailPanel(available.x, detailH);
    ImGui::EndChild();
}