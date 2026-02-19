// ============================================================================
// File: src/apps/cpu_tomasulo/ui/views/TomasuloMainView.cpp
// ============================================================================

/**
 * @file TomasuloMainView.cpp
 * @brief Implementation of TomasuloMainView.
 *
 * Composites four focused components each frame:
 *   1. ShaderBackground   — full-canvas GLSL backdrop
 *   2. Diagram texture    — the PNG loaded via TextureCache
 *   3. BorderRenderer     — decorative 3-D frame around the image
 *   4. SlotOverlayRenderer — labeled boxes on pipeline slots
 *
 * Camera input → animate → clamp is ticked here so all transforms
 * are consistent within a single render call.
 */

#include "apps/cpu_tomasulo/ui/views/TomasuloMainView.h"
#include "apps/cpu_tomasulo/ui/components/BorderRenderer.h"
#include "core/resources/TextureCache.h"

#include "imgui.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

 // ============================================================================
 // Slot Layout  (image-pixel coordinates, world space)
 // ============================================================================

const std::array<TomasuloMainView::SlotDef, TomasuloMainView::SLOT_COUNT>
TomasuloMainView::s_slotDefs = { {
    {  185.0f,  165.0f, 200.0f, 30.0f, "Issue"      },
    {  660.0f,  145.0f, 110.0f, 25.0f, "SB0"        },
    {  660.0f,  235.0f, 110.0f, 25.0f, "SB1"        },
    {  660.0f,  325.0f, 110.0f, 25.0f, "LB0"        },
    {  660.0f,  415.0f, 110.0f, 25.0f, "LB1"        },
    {  660.0f,  505.0f, 110.0f, 25.0f, "LB2"        },
    {  660.0f,  605.0f, 140.0f, 25.0f, "RS_IntALU0" },
    {  660.0f,  695.0f, 140.0f, 25.0f, "RS_IntALU1" },
    {  660.0f,  785.0f, 140.0f, 25.0f, "RS_FPALU0"  },
    {  660.0f,  875.0f, 140.0f, 25.0f, "RS_IntMUL0" },
    {  660.0f,  965.0f, 140.0f, 25.0f, "RS_FPMUL0"  },
    {  660.0f, 1055.0f, 140.0f, 25.0f, "RS_Branch0" },
    { 1265.0f,  415.0f, 180.0f, 28.0f, "Commit"     },
} };

// ============================================================================
// Construction / Label Helpers
// ============================================================================

TomasuloMainView::TomasuloMainView() { clearLabels(); }

void TomasuloMainView::clearLabels() {
    for (auto& l : m_labels) l = "---";
}

void TomasuloMainView::setLabel(Slot slot, const std::string& text) {
    if (slot >= 0 && slot < SLOT_COUNT) m_labels[slot] = text;
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
        if (!m_tex)
            std::cout << "[TomasuloMainView] Could not load texture: " << path << "\n";
    }
}

// ============================================================================
// Slot Border Colors
// ============================================================================

ImU32 TomasuloMainView::slotBorderColor(int i) {
    constexpr ImU32 ISSUE_CLR = IM_COL32(100, 200, 255, 220);  // cyan   — issue stage
    constexpr ImU32 EXECUTE_CLR = IM_COL32(255, 200, 100, 220);  // amber  — execute stage
    constexpr ImU32 COMMIT_CLR = IM_COL32(100, 255, 150, 220);  // green  — commit stage

    if (i == 0)            return ISSUE_CLR;
    if (i >= 1 && i <= 11) return EXECUTE_CLR;
    return COMMIT_CLR;
}

// ============================================================================
// Reset Button
//
// Drawn on the foreground draw list so it is always above canvas content.
// Input is handled via manual hit-test to avoid conflicting with the
// InvisibleButton that owns pan/zoom input.
// ============================================================================

void TomasuloMainView::drawResetButton(const ImVec2& canvasP0,
    const ImVec2& canvasSize) {
    const float btnW = 80.0f;
    const float btnH = 30.0f;
    const float margin = 14.0f;
    const float round = 6.0f;

    const ImVec2 btnP0(
        canvasP0.x + canvasSize.x - btnW - margin,
        canvasP0.y + canvasSize.y - btnH - margin);
    const ImVec2 btnP1(btnP0.x + btnW, btnP0.y + btnH);

    // ── Hit test ──────────────────────────────────────────────
    const ImVec2 mouse = ImGui::GetIO().MousePos;
    const bool   hovered =
        mouse.x >= btnP0.x && mouse.x <= btnP1.x &&
        mouse.y >= btnP0.y && mouse.y <= btnP1.y;
    const bool clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);

    if (clicked) m_camera.reset();

    // ── Draw ──────────────────────────────────────────────────
    ImDrawList* fg = ImGui::GetForegroundDrawList();

    ImU32 bgColor;
    if (clicked)       bgColor = IM_COL32(80, 140, 200, 230);
    else if (hovered)  bgColor = IM_COL32(60, 80, 120, 210);
    else               bgColor = IM_COL32(30, 30, 40, 180);

    fg->AddRectFilled(btnP0, btnP1, bgColor, round);
    fg->AddRect(btnP0, btnP1, IM_COL32(210, 240, 255, 80), round, 0, 1.0f);

    // ── Centered label ────────────────────────────────────────
    const char* label = "RESET";
    const ImVec2 textSz = ImGui::CalcTextSize(label);
    const ImVec2 textPos(
        btnP0.x + (btnW - textSz.x) * 0.5f,
        btnP0.y + (btnH - textSz.y) * 0.5f);
    fg->AddText(textPos, IM_COL32(210, 240, 255, 255), label);

    // Hint cursor so the button feels interactive
    if (hovered) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
}

// ============================================================================
// Main Render
// ============================================================================

void TomasuloMainView::render() {
    ensureLoaded();

    if (!m_tex) {
        ImGui::TextWrapped(
            "Could not load 'Assets/CPU_TOMASULO/DiagramVersions/GeneralViewOfCPU.png'.");
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    const float dt = io.DeltaTime;
    ImDrawList* dl = ImGui::GetWindowDrawList();

    const ImVec2 canvasP0 = ImGui::GetCursorScreenPos();
    ImVec2       canvasSize = ImGui::GetContentRegionAvail();
    if (canvasSize.x < 2.0f) canvasSize.x = 2.0f;
    if (canvasSize.y < 2.0f) canvasSize.y = 2.0f;

    // ── Animated background ───────────────────────────────────
    m_background.draw(canvasP0, canvasSize, dl);

    // ── Invisible button — captures mouse input for pan/zoom ──
    ImGui::InvisibleButton("##TomasuloCanvas", canvasSize,
        ImGuiButtonFlags_MouseButtonLeft |
        ImGuiButtonFlags_MouseButtonRight |
        ImGuiButtonFlags_MouseButtonMiddle);
    const bool hovered = ImGui::IsItemHovered();

    // ── World dimensions ──────────────────────────────────────
    const sf::Vector2u texSz = m_tex->getSize();
    const float imgW = (float)texSz.x;
    const float imgH = (float)texSz.y;

    const float fitScale = Camera2D::computeFitScale(
        canvasSize.x, canvasSize.y, imgW, imgH, m_camera.config.fitFill);

    // ── Camera init (once) ────────────────────────────────────
    if (!m_camera.isInitialized())
        m_camera.init(imgW, imgH);

    // ── Input → clamp → animate ───────────────────────────────
    if (hovered)
        m_camera.handleInput(canvasP0, canvasSize, fitScale);

    m_camera.clampTarget(imgW, imgH, canvasSize.x, canvasSize.y, fitScale);
    m_camera.animate(dt);

    // ── World-to-screen transform ─────────────────────────────
    const float  scale = m_camera.currentScale(fitScale);
    const ImVec2 imgP0 = m_camera.worldOrigin(canvasP0, canvasSize, fitScale);
    const ImVec2 imgP1(imgP0.x + imgW * scale, imgP0.y + imgH * scale);

    // ── Draw world (clipped to canvas bounds) ─────────────────
    dl->PushClipRect(canvasP0,
        { canvasP0.x + canvasSize.x, canvasP0.y + canvasSize.y },
        true);

    // Diagram image
    const ImTextureID imgId = (ImTextureID)(intptr_t)m_tex->getNativeHandle();
    dl->AddImage(imgId, imgP0, imgP1, { 0,0 }, { 1,1 }, IM_COL32(255, 255, 255, 255));

    // 3-D border frame
    const float pad = 10.0f;
    BorderRenderer::drawCool3DBorder(dl,
        { imgP0.x - pad, imgP0.y - pad },
        { imgP1.x + pad, imgP1.y + pad });

    // Slot overlays with live labels
    m_overlay.draw(dl, imgP0, scale, s_slotDefs, m_labels, slotBorderColor);

    dl->PopClipRect();

    // ── RESET button (above clip rect, always visible) ────────
    drawResetButton(canvasP0, canvasSize);
}