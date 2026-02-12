#pragma once
/**
 * @file ShaderBackground.h
 * @brief Animated fragment-shader background drawn behind the world.
 *
 * Responsibility: Owns the shader, render-texture, and timing.
 *                 Renders into an ImDrawList rectangle.
 */

#include "imgui.h"
#include <SFML/Graphics.hpp>
#include <string>

class ShaderBackground {
public:
    ShaderBackground() = default;

    /// Draw the animated background into `dl` at the given screen rect.
    void draw(const ImVec2& screenPos, const ImVec2& screenSize, ImDrawList* dl);

private:
    void ensureShader();
    void ensureRT(unsigned w, unsigned h);

    sf::Shader        m_shader;
    bool              m_shaderLoaded = false;
    sf::Clock         m_clock;

    sf::RenderTexture m_rt;
    sf::Vector2u      m_rtSize{ 0u, 0u };

    static const std::string FRAG_SOURCE;
};