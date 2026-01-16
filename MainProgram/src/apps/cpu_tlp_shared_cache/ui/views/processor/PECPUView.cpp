// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/ui/views/processor/PECPUView.cpp
// ============================================================================

/**
 * @file PECPUView.cpp
 * @brief Implementation of PECPUView.
 */

#include "apps/cpu_tlp_shared_cache/ui/views/processor/PECPUView.h"
#include "core/resources/TextureCache.h"
#include "imgui.h"
#include <iostream>
#include <algorithm>

PECPUView::PECPUView(int peIndex)
    : m_peIndex(peIndex)
{
}

void PECPUView::ensureLoaded() {
    if (!m_tex) {
        const std::string fullPath = std::string(RESOURCES_PATH) + "Assets/CPU_TLP/CPU_Pipeline.png";
        m_tex = TextureCache::instance().get(fullPath);

        if (!m_tex) {
            std::cout << "[PE" << m_peIndex << "CPUView] Could not load texture: "
                << fullPath << "\n";
        }

        // Configure viewer: no zoom, 80% visible, min 50% visible when dragging
        m_viewer.setZoomEnabled(false);
        m_viewer.setLockedVisibleFraction(0.80f);
        m_viewer.setMinVisibleFraction(0.50f);
    }
}

void PECPUView::setLabel(size_t index, const std::string& text) {
    if (index < m_labels.size()) {
        m_labels[index] = text;
    }
}

void PECPUView::setLabels(const std::array<std::string, 5>& labels) {
    m_labels = labels;
}

void PECPUView::render() {
    ensureLoaded();

    if (!m_tex) {
        ImGui::TextWrapped("Could not load 'Assets/CPU_TLP/CPU_Pipeline.png'.");
        return;
    }

    // Generate unique viewer ID for this PE
    char viewerId[32];
    std::snprintf(viewerId, sizeof(viewerId), "##PE%dCPU_Viewer", m_peIndex);

    m_viewer.renderWithOverlay(*m_tex, viewerId,
        [this](const ImVec2& origin, float scale, ImDrawList* dl) {
            renderOverlay(origin, scale, dl);
        }
    );
}

void PECPUView::renderOverlay(const ImVec2& origin, float scale, ImDrawList* dl) {
    // Pipeline stage box layout (in image pixels)
    constexpr float Y_IMG = 550.0f;
    constexpr float X0_IMG = 1800.0f;
    constexpr float DX_IMG = 1400.0f;
    constexpr float EXTRA_BETWEEN_2_3 = 250.0f;
    constexpr int COUNT = 5;

    // Box dimensions (in image pixels)
    constexpr float BOX_W_IMG = 1200.0f;
    constexpr float BOX_H_IMG = 180.0f;

    // Box colors
    const ImU32 FILL = IM_COL32(0, 0, 0, 220);
    const ImU32 BORDER = IM_COL32(255, 255, 255, 220);
    const float CORNER_RADIUS = 8.0f;
    const float BORDER_THICKNESS = 2.0f;

    // Font settings
    ImFont* font = ImGui::GetFont();
    float baseFontSize = ImGui::GetFontSize();
    float fontPx = std::max(16.0f, baseFontSize * scale * 1.6f);

    for (int i = 0; i < COUNT; ++i) {
        // Calculate position with extra spacing after stage 2
        float xImg = X0_IMG + DX_IMG * i + ((i >= 2) ? EXTRA_BETWEEN_2_3 : 0.0f);

        ImVec2 topLeft(origin.x + scale * xImg, origin.y + scale * Y_IMG);
        ImVec2 bottomRight(topLeft.x + scale * BOX_W_IMG, topLeft.y + scale * BOX_H_IMG);

        // Draw box
        dl->AddRectFilled(topLeft, bottomRight, FILL, CORNER_RADIUS);
        dl->AddRect(topLeft, bottomRight, BORDER, CORNER_RADIUS, 0, BORDER_THICKNESS);

        // Draw centered text
        const std::string& txt = m_labels[i];
        ImVec2 textSize = font->CalcTextSizeA(fontPx, FLT_MAX, 0.0f, txt.c_str());
        ImVec2 textPos(
            topLeft.x + ((bottomRight.x - topLeft.x) - textSize.x) * 0.5f,
            topLeft.y + ((bottomRight.y - topLeft.y) - textSize.y) * 0.5f + 1.0f
        );

        dl->AddText(font, fontPx, textPos, IM_COL32(255, 255, 255, 255), txt.c_str());
    }
}