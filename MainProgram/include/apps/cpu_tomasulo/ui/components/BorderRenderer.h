// ============================================================================
// File: include/apps/cpu_tomasulo/ui/components/BorderRenderer.h
// ============================================================================

#pragma once

/**
 * @file BorderRenderer.h
 * @brief Draws a decorative 3D border around the Tomasulo diagram image.
 *
 * Responsibility: Pure stateless rendering utility — no stored state,
 *                 only ImDrawList draw calls.
 *
 * Features:
 *   - Drop shadow
 *   - Multi-ring neon glow
 *   - Main stroke with bevel highlights (top-left bright, bottom-right dark)
 *   - Inner inset stroke
 *   - Corner tick marks
 */

#include "imgui.h"

namespace BorderRenderer {
    /**
     * @brief Draws a 3D decorative border between screen corners @p p0 and @p p1.
     * @param dl  ImDrawList to render into (must not be null).
     * @param p0  Top-left corner in screen space.
     * @param p1  Bottom-right corner in screen space.
     */
    void drawCool3DBorder(ImDrawList* dl, const ImVec2& p0, const ImVec2& p1);
}