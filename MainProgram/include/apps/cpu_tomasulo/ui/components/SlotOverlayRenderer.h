#pragma once
/**
 * @file SlotOverlayRenderer.h
 * @brief Renders labeled slot overlays on the Tomasulo diagram.
 *
 * Responsibility: Given slot definitions, labels, and a transform,
 *                 draw the overlay rectangles + text.
 */

#include "imgui.h"
#include <array>
#include <string>
#include <functional>

class SlotOverlayRenderer {
public:
    struct SlotDef {
        float       x, y, w, h;
        const char* name;
    };

    /// Signature: (slotIndex) -> ImU32 border color
    using ColorCallback = std::function<ImU32(int)>;

    /// Draw all slot overlays.  `origin` and `scale` define world->screen.
    template <std::size_t N>
    void draw(ImDrawList* dl,
        const ImVec2& origin,
        float scale,
        const std::array<SlotDef, N>& defs,
        const std::array<std::string, N>& labels,
        const ColorCallback& colorFn) const
    {
        const ImU32 FILL = IM_COL32(0, 0, 0, 210);
        const ImU32 TEXT_COLOR = IM_COL32(255, 255, 255, 255);
        const float CORNER_R = 4.0f;
        const float BORDER_T = 1.5f;

        ImFont* font = ImGui::GetFont();

        for (std::size_t i = 0; i < N; ++i) {
            const auto& d = defs[i];

            ImVec2 tl(origin.x + scale * d.x, origin.y + scale * d.y);
            ImVec2 br(tl.x + scale * d.w, tl.y + scale * d.h);

            dl->AddRectFilled(tl, br, FILL, CORNER_R);
            dl->AddRect(tl, br, colorFn(static_cast<int>(i)), CORNER_R, 0, BORDER_T);

            const float boxW = br.x - tl.x;
            const float boxH = br.y - tl.y;
            const float pad = 4.0f * scale;
            const float maxW = boxW - pad;
            if (maxW <= 0.0f) continue;

            const auto& txt = labels[i];
            if (txt.empty()) continue;

            // Start with height-based font size, then shrink to fit width
            float fontPx = boxH * 0.75f;
            fontPx = std::max(8.0f, std::min(fontPx, 40.0f));

            ImVec2 textSz = font->CalcTextSizeA(fontPx, FLT_MAX, 0.0f, txt.c_str());

            // Shrink if text overflows box width
            if (textSz.x > maxW) {
                fontPx *= (maxW / textSz.x);
                fontPx = std::max(6.0f, fontPx);
                textSz = font->CalcTextSizeA(fontPx, FLT_MAX, 0.0f, txt.c_str());
            }

            ImVec2 textPos(
                tl.x + (boxW - textSz.x) * 0.5f,
                tl.y + (boxH - textSz.y) * 0.5f);

            dl->AddText(font, fontPx, textPos, TEXT_COLOR, txt.c_str());
        }
    }
};