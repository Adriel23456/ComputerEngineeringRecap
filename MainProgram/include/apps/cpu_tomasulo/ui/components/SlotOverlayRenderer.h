// ============================================================================
// File: include/apps/cpu_tomasulo/ui/components/SlotOverlayRenderer.h
// ============================================================================

#pragma once

/**
 * @file SlotOverlayRenderer.h
 * @brief Renders labeled slot overlays on the Tomasulo pipeline diagram.
 *
 * Responsibility: Stateless utility class. Given a set of slot definitions,
 *                 live label strings, and a world-to-screen transform, it draws
 *                 the overlay rectangles and auto-scaled text into an ImDrawList.
 *
 * Usage:
 *   SlotOverlayRenderer renderer;
 *   renderer.draw(dl, origin, scale, slotDefs, labels,
 *                 [](int i) { return IM_COL32(0, 255, 0, 255); });
 */

#include "imgui.h"
#include <array>
#include <string>
#include <functional>

class SlotOverlayRenderer {
public:
    /** @brief Describes a single slot's position and size in world space. */
    struct SlotDef {
        float       x, y, w, h;  ///< World-space rect (pixels on the source diagram).
        const char* name;         ///< Human-readable identifier (for debugging).
    };

    /** @brief Callback that returns an ImU32 border color for a given slot index. */
    using ColorCallback = std::function<ImU32(int)>;

    /**
     * @brief Draws all slot overlays onto @p dl.
     *
     * @tparam N        Number of slots (deduced from the arrays).
     * @param dl        ImDrawList to render into.
     * @param origin    Screen-space top-left of the world image.
     * @param scale     World-to-screen scale factor (fitScale * zoom).
     * @param defs      Array of slot definitions in world space.
     * @param labels    Array of live label strings, one per slot.
     * @param colorFn   Returns the border color for a given slot index.
     */
    template <std::size_t N>
    void draw(ImDrawList* dl,
        const ImVec2& origin,
        float                            scale,
        const std::array<SlotDef, N>& defs,
        const std::array<std::string, N>& labels,
        const ColorCallback& colorFn) const
    {
        const ImU32  FILL = IM_COL32(0, 0, 0, 210);
        const ImU32  TEXT_COLOR = IM_COL32(255, 255, 255, 255);
        const float  CORNER_R = 4.0f;
        const float  BORDER_T = 1.5f;

        ImFont* font = ImGui::GetFont();

        for (std::size_t i = 0; i < N; ++i) {
            const auto& d = defs[i];

            ImVec2 tl(origin.x + scale * d.x, origin.y + scale * d.y);
            ImVec2 br(tl.x + scale * d.w, tl.y + scale * d.h);

            // Background fill + border
            dl->AddRectFilled(tl, br, FILL, CORNER_R);
            dl->AddRect(tl, br, colorFn(static_cast<int>(i)), CORNER_R, 0, BORDER_T);

            const float boxW = br.x - tl.x;
            const float boxH = br.y - tl.y;
            const float pad = 4.0f * scale;
            const float maxW = boxW - pad;
            if (maxW <= 0.0f) continue;

            const auto& txt = labels[i];
            if (txt.empty()) continue;

            // Start with a height-based font size, then shrink to fit width
            float fontPx = boxH * 0.75f;
            fontPx = std::max(8.0f, std::min(fontPx, 40.0f));

            ImVec2 textSz = font->CalcTextSizeA(fontPx, FLT_MAX, 0.0f, txt.c_str());

            if (textSz.x > maxW) {
                fontPx *= (maxW / textSz.x);
                fontPx = std::max(6.0f, fontPx);
                textSz = font->CalcTextSizeA(fontPx, FLT_MAX, 0.0f, txt.c_str());
            }

            // Center text within the box
            ImVec2 textPos(
                tl.x + (boxW - textSz.x) * 0.5f,
                tl.y + (boxH - textSz.y) * 0.5f);

            dl->AddText(font, fontPx, textPos, TEXT_COLOR, txt.c_str());
        }
    }
};