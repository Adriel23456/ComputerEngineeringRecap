#pragma once
/**
 * @file BorderRenderer.h
 * @brief Draws the decorative 3D border around the diagram image.
 *
 * Responsibility: Pure rendering utility — no state, just draw commands.
 */

#include "imgui.h"

namespace BorderRenderer {
    void drawCool3DBorder(ImDrawList* dl, const ImVec2& p0, const ImVec2& p1);
}