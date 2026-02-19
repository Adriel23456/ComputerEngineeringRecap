// ============================================================================
// File: include/apps/cpu_tomasulo/ui/components/ShaderBackground.h
// ============================================================================

#pragma once

/**
 * @file ShaderBackground.h
 * @brief Animated GLSL fragment-shader background for the Tomasulo main view.
 *
 * Responsibility: Owns the SFML shader, render-texture, and elapsed-time clock.
 *                 Renders a fullscreen animated wormhole effect into an ImDrawList
 *                 rectangle each frame.
 *
 * Usage:
 *   ShaderBackground bg;
 *   bg.draw(screenPos, screenSize, ImGui::GetWindowDrawList());
 */

#include "imgui.h"
#include <SFML/Graphics.hpp>
#include <string>

class ShaderBackground {
public:
    ShaderBackground() = default;

    /**
     * @brief Renders the animated background into @p dl at the given screen rect.
     * @param screenPos   Top-left corner in ImGui screen space.
     * @param screenSize  Width/height of the area to fill.
     * @param dl          ImDrawList to render into (must not be null).
     */
    void draw(const ImVec2& screenPos, const ImVec2& screenSize, ImDrawList* dl);

private:
    /** @brief Compiles the fragment shader on first use. */
    void ensureShader();

    /** @brief Reallocates the render-texture if the requested size changed. */
    void ensureRT(unsigned w, unsigned h);

    sf::Shader        m_shader;
    bool              m_shaderLoaded = false;
    sf::Clock         m_clock;

    sf::RenderTexture m_rt;
    sf::Vector2u      m_rtSize{ 0u, 0u };

    static const std::string FRAG_SOURCE; ///< Inline GLSL source for the wormhole shader.
};