// ============================================================================
// File: src/apps/cpu_tomasulo/ui/components/BorderRenderer.cpp
// ============================================================================

/**
 * @file BorderRenderer.cpp
 * @brief Implementation of BorderRenderer::drawCool3DBorder.
 */

#include "apps/cpu_tomasulo/ui/components/BorderRenderer.h"

void BorderRenderer::drawCool3DBorder(ImDrawList* dl,
    const ImVec2& p0,
    const ImVec2& p1) {
    if (!dl) return;

    const float rounding = 14.0f;
    const float glow = 10.0f;
    const float bevel = 5.0f;
    const float stroke = 2.5f;

    const ImU32 mainStroke = IM_COL32(210, 240, 255, 220);
    const ImU32 innerStroke = IM_COL32(40, 60, 80, 200);

    // ── Drop shadow ─────────────────────────────────────────────
    dl->AddRectFilled(
        ImVec2(p0.x + 6.0f, p0.y + 7.0f),
        ImVec2(p1.x + 6.0f, p1.y + 7.0f),
        IM_COL32(0, 0, 0, 80), rounding + 2.0f);

    // ── Neon glow rings (alternating cyan / magenta) ─────────────
    for (int i = 0; i < 3; ++i) {
        const float g = glow + i * 6.0f;
        const float a = 1.0f - (float)i / 3.0f;
        const ImU32 c = (i % 2 == 0)
            ? IM_COL32(120, 220, 255, (int)(24 * a))
            : IM_COL32(255, 120, 220, (int)(18 * a));
        dl->AddRect(ImVec2(p0.x - g, p0.y - g),
            ImVec2(p1.x + g, p1.y + g),
            c, rounding + g, 0, 2.0f + (float)i);
    }

    // ── Main stroke ──────────────────────────────────────────────
    dl->AddRect(p0, p1, mainStroke, rounding, 0, stroke);

    // ── Bevel highlights (top-left bright, bottom-right dark) ────
    {
        const ImU32 hi = IM_COL32(255, 255, 255, 90);
        const ImU32 lo = IM_COL32(0, 0, 0, 120);
        const ImVec2 q0(p0.x + 1.5f, p0.y + 1.5f);
        const ImVec2 q1(p1.x - 1.5f, p1.y - 1.5f);

        dl->AddLine({ q0.x + rounding, q0.y }, { q1.x - rounding, q0.y }, hi, bevel);
        dl->AddLine({ q0.x, q0.y + rounding }, { q0.x, q1.y - rounding }, hi, bevel);
        dl->AddLine({ q0.x + rounding, q1.y }, { q1.x - rounding, q1.y }, lo, bevel);
        dl->AddLine({ q1.x, q0.y + rounding }, { q1.x, q1.y - rounding }, lo, bevel);
    }

    // ── Inner inset stroke ───────────────────────────────────────
    dl->AddRect(ImVec2(p0.x + 4.0f, p0.y + 4.0f),
        ImVec2(p1.x - 4.0f, p1.y - 4.0f),
        innerStroke, rounding - 3.0f, 0, 1.5f);

    // ── Corner tick marks ────────────────────────────────────────
    {
        const ImU32 tick = IM_COL32(160, 255, 220, 220);
        const float L = 18.0f;

        auto drawCorner = [&](float cx, float cy, float sx, float sy) {
            dl->AddLine({ cx,          cy }, { cx + sx * L, cy }, tick, 2.0f);
            dl->AddLine({ cx,          cy }, { cx,          cy + sy * L }, tick, 2.0f);
            };
        drawCorner(p0.x + 10, p0.y + 10, 1, 1);
        drawCorner(p1.x - 10, p0.y + 10, -1, 1);
        drawCorner(p0.x + 10, p1.y - 10, 1, -1);
        drawCorner(p1.x - 10, p1.y - 10, -1, -1);
    }
}