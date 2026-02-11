/**
 * @file TomasuloMainView.cpp
 * @brief Implementation of TomasuloMainView.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloMainView.h"
#include "core/resources/TextureCache.h"
#include "imgui.h"
#include <iostream>
#include <algorithm>
#include <cstdio>

 // ============================================================================
 // Slot Layout Definitions (image-pixel coordinates, 1465x1123)
 // ============================================================================

const std::array<TomasuloMainView::SlotDef, TomasuloMainView::SLOT_COUNT>
TomasuloMainView::s_slotDefs = { {
        // ── Issue (1) ───────────────────────────────────────────────────
        { 568.0f,  205.0f,  200.0f, 30.0f, "Issue" },

        // ── Execute (11) — SB0 starts at y=105, then 125px spacing ─────

        // SB0, SB1
        { 1060.0f,  145.0f,  110.0f, 25.0f, "SB0" },
        { 1060.0f,  235.0f,  110.0f, 25.0f, "SB1" },

        // LB0, LB1, LB2
        { 1060.0f,  325.0f,  110.0f, 25.0f, "LB0" },
        { 1060.0f,  415.0f,  110.0f, 25.0f, "LB1" },
        { 1060.0f,  505.0f,  110.0f, 25.0f, "LB2" },

        // RS_IntALU0, RS_IntALU1
        { 1060.0f,  605.0f,  140.0f, 25.0f, "RS_IntALU0" },
        { 1060.0f,  695.0f,  140.0f, 25.0f, "RS_IntALU1" },

        // RS_FPALU0
        { 1060.0f,  785.0f,  140.0f, 25.0f, "RS_FPALU0" },

        // RS_IntMUL0
        { 1060.0f, 875.0f,  140.0f, 25.0f, "RS_IntMUL0" },

        // RS_FPMUL0
        { 1060.0f, 965.0f,  140.0f, 25.0f, "RS_FPMUL0" },

        // RS_Branch0
        { 1060.0f, 1055.0f,  140.0f, 25.0f, "RS_Branch0" },

        // ── Commit (2 CDB) ─────────────────────────────────────────────
        { 1645.0f, 155.0f,  180.0f, 28.0f, "CDB_A" },
        { 1645.0f, 195.0f,  180.0f, 28.0f, "CDB_B" },
    } };

// ============================================================================
// Construction
// ============================================================================

TomasuloMainView::TomasuloMainView() {
    clearLabels();
}

void TomasuloMainView::clearLabels() {
    for (auto& l : m_labels) {
        l = "---";
    }
}

// ============================================================================
// Label Setters
// ============================================================================

void TomasuloMainView::setLabel(Slot slot, const std::string& text) {
    if (slot >= 0 && slot < SLOT_COUNT) {
        m_labels[slot] = text;
    }
}

void TomasuloMainView::setLabels(const std::array<std::string, SLOT_COUNT>& labels) {
    m_labels = labels;
}

// ============================================================================
// Texture Loading
// ============================================================================

void TomasuloMainView::ensureLoaded() {
    if (!m_tex) {
        const std::string path = std::string(RESOURCES_PATH)
            + "Assets/CPU_TOMASULO/DiagramVersions/GeneralViewOfCPU.png";

        m_tex = TextureCache::instance().get(path);

        if (!m_tex) {
            std::cout << "[TomasuloMainView] Could not load texture: "
                << path << "\n";
        }

        // Configure viewer: no zoom by default, show 90% of image,
        // allow dragging down to 50% visible
        m_viewer.setZoomEnabled(false);
        m_viewer.setLockedVisibleFraction(0.90f);
        m_viewer.setMinVisibleFraction(0.50f);
    }
}

// ============================================================================
// Rendering
// ============================================================================

void TomasuloMainView::render() {
    ensureLoaded();

    if (!m_tex) {
        ImGui::TextWrapped(
            "Could not load 'Assets/CPU_TOMASULO/DiagramVersions/GeneralViewOfCPU.png'.");
        return;
    }

    m_viewer.renderWithOverlay(*m_tex, "##TomasuloMainViewer",
        [this](const ImVec2& origin, float scale, ImDrawList* dl) {
            renderOverlay(origin, scale, dl);
        }
    );
}

// ============================================================================
// Overlay Drawing
// ============================================================================

void TomasuloMainView::renderOverlay(const ImVec2& origin, float scale,
    ImDrawList* dl) {
    // Box styling
    const ImU32 FILL = IM_COL32(0, 0, 0, 210);
    const ImU32 TEXT_COLOR = IM_COL32(255, 255, 255, 255);
    const float CORNER_RADIUS = 4.0f;
    const float BORDER_THICK = 1.5f;

    // Section border colors
    const ImU32 ISSUE_BORDER = IM_COL32(100, 200, 255, 220);
    const ImU32 EXECUTE_BORDER = IM_COL32(255, 200, 100, 220);
    const ImU32 COMMIT_BORDER = IM_COL32(100, 255, 150, 220);

    ImFont* font = ImGui::GetFont();
    float baseFontSz = ImGui::GetFontSize();

    for (int i = 0; i < SLOT_COUNT; ++i) {
        const SlotDef& def = s_slotDefs[i];

        ImVec2 topLeft(
            origin.x + scale * def.x,
            origin.y + scale * def.y
        );
        ImVec2 bottomRight(
            topLeft.x + scale * def.w,
            topLeft.y + scale * def.h
        );

        // Choose border color by stage
        ImU32 borderColor;
        if (i == ISSUE)                                     borderColor = ISSUE_BORDER;
        else if (i >= SB0 && i <= RS_BRANCH0)               borderColor = EXECUTE_BORDER;
        else                                                 borderColor = COMMIT_BORDER;

        dl->AddRectFilled(topLeft, bottomRight, FILL, CORNER_RADIUS);
        dl->AddRect(topLeft, bottomRight, borderColor, CORNER_RADIUS, 0, BORDER_THICK);

        float fontPx = std::max(10.0f, baseFontSz * scale * 1.1f);

        const std::string& txt = m_labels[i];
        ImVec2 textSz = font->CalcTextSizeA(fontPx, FLT_MAX, 0.0f, txt.c_str());

        float maxTextW = (bottomRight.x - topLeft.x) - 4.0f * scale;
        if (textSz.x > maxTextW && maxTextW > 0.0f) {
            textSz = font->CalcTextSizeA(fontPx, maxTextW, maxTextW, txt.c_str());
        }

        ImVec2 textPos(
            topLeft.x + ((bottomRight.x - topLeft.x) - textSz.x) * 0.5f,
            topLeft.y + ((bottomRight.y - topLeft.y) - textSz.y) * 0.5f
        );

        dl->AddText(font, fontPx, textPos, TEXT_COLOR, txt.c_str(),
            nullptr, maxTextW > 0.0f ? maxTextW : 0.0f);
    }
}