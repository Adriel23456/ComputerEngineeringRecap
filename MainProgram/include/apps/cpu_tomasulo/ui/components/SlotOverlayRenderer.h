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

    /// Signature: (slotIndex) → ImU32 border color
    using ColorCallback = std::function<ImU32(int)>;

    /// Draw all slot overlays.  `origin` and `scale` define world→screen.
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
        const float baseSz = ImGui::GetFontSize();

        for (std::size_t i = 0; i < N; ++i) {
            const auto& d = defs[i];

            ImVec2 tl(origin.x + scale * d.x, origin.y + scale * d.y);
            ImVec2 br(tl.x + scale * d.w, tl.y + scale * d.h);

            dl->AddRectFilled(tl, br, FILL, CORNER_R);
            dl->AddRect(tl, br, colorFn(static_cast<int>(i)), CORNER_R, 0, BORDER_T);

            const float fontPx = std::max(10.0f, baseSz * scale * 1.1f);
            const float maxW = (br.x - tl.x) - 4.0f * scale;

            const auto& txt = labels[i];
            ImVec2 textSz = font->CalcTextSizeA(fontPx, FLT_MAX, 0.0f, txt.c_str());
            if (textSz.x > maxW && maxW > 0.0f)
                textSz = font->CalcTextSizeA(fontPx, maxW, maxW, txt.c_str());

            ImVec2 textPos(
                tl.x + ((br.x - tl.x) - textSz.x) * 0.5f,
                tl.y + ((br.y - tl.y) - textSz.y) * 0.5f);

            dl->AddText(font, fontPx, textPos, TEXT_COLOR, txt.c_str(),
                nullptr, maxW > 0.0f ? maxW : 0.0f);
        }
    }
};