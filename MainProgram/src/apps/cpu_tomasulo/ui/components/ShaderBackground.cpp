/**
 * @file ShaderBackground.cpp
 */

#include "apps/cpu_tomasulo/ui/components/ShaderBackground.h"

const std::string ShaderBackground::FRAG_SOURCE = R"(
    uniform vec2  iResolution;
    uniform float iTime;

    // ── Neon palette that cycles through cyber colors ───────
    vec3 neon(float t) {
        vec3 a = vec3(0.5, 0.5, 0.5);
        vec3 b = vec3(0.5, 0.5, 0.5);
        vec3 c = vec3(2.0, 1.0, 0.5);
        vec3 d = vec3(0.50, 0.20, 0.25);
        return a + b * cos(6.28318 * (c * t + d));
    }

    void main() {
        vec2 fragCoord = gl_FragCoord.xy;
        vec2 uv = (fragCoord - 0.5 * iResolution) / iResolution.y;

        // ── Polar coordinates ───────────────────────────────
        float r     = length(uv);
        float angle = atan(uv.y, uv.x);

        // ── Infinite tunnel mapping ─────────────────────────
        // 1/r creates the depth illusion; scrolling iTime = motion
        float depth  = 0.5 / (r + 0.05) + iTime * 1.2;
        float tunnel = angle / 3.14159;

        // ── Wormhole distortion: twist increases with depth ─
        float twist = sin(depth * 0.5 + iTime * 0.3) * 0.4;
        tunnel += twist;

        // ── Ring grid pattern ───────────────────────────────
        float rings   = sin(depth * 8.0) * 0.5 + 0.5;
        float stripes = sin(angle * 6.0 + depth * 2.0 + twist * 6.2832) * 0.5 + 0.5;

        // Combine into a sharp-edged wireframe look
        float wire = pow(rings, 8.0) + pow(stripes, 8.0);
        wire = clamp(wire, 0.0, 1.0);

        // ── Secondary layer (slower, offset) for parallax ───
        float depth2  = 0.3 / (r + 0.08) + iTime * 0.7;
        float rings2  = sin(depth2 * 5.0 + 1.5) * 0.5 + 0.5;
        float stripe2 = sin(angle * 4.0 - depth2 * 3.0) * 0.5 + 0.5;
        float wire2   = pow(rings2, 10.0) + pow(stripe2, 10.0);
        wire2 = clamp(wire2, 0.0, 1.0);

        // ── Color each layer differently ────────────────────
        vec3 col1 = neon(depth * 0.04 + iTime * 0.05) * wire;
        vec3 col2 = neon(depth2 * 0.06 + 0.5 + iTime * 0.03) * wire2 * 0.5;

        vec3 color = col1 + col2;

        // ── Center glow (wormhole core) ─────────────────────
        float core = exp(-r * 3.0);
        vec3 coreColor = neon(iTime * 0.1) * 1.5;
        color += core * coreColor * (0.5 + 0.5 * sin(iTime * 2.0));

        // ── Edge glow rings pulsing outward ─────────────────
        float pulse1 = exp(-abs(r - fract(iTime * 0.4)) * 12.0);
        float pulse2 = exp(-abs(r - fract(iTime * 0.4 + 0.5)) * 12.0);
        color += pulse1 * vec3(0.2, 0.5, 1.0) * 0.6;
        color += pulse2 * vec3(1.0, 0.3, 0.6) * 0.4;

        // ── Depth fog: darken edges for tunnel depth feel ───
        float fog = smoothstep(0.0, 0.4, r);
        color *= mix(1.0, 0.25, fog * fog);

        // ── Subtle scanlines for CRT / retro feel ───────────
        float scan = 0.95 + 0.05 * sin(fragCoord.y * 2.5);
        color *= scan;

        // ── Very light film grain ───────────────────────────
        float grain = fract(sin(dot(fragCoord, vec2(12.9898, 78.233)) + iTime) * 43758.5453);
        color += (grain - 0.5) * 0.02;

        gl_FragColor = vec4(color, 1.0);
    }
)";

void ShaderBackground::ensureShader() {
    if (!m_shaderLoaded) {
        m_shader.loadFromMemory(FRAG_SOURCE, sf::Shader::Fragment);
        m_shaderLoaded = true;
    }
}

void ShaderBackground::ensureRT(unsigned w, unsigned h) {
    if (w == 0u || h == 0u) return;
    sf::Vector2u needed(w, h);
    if (m_rtSize != needed) {
        m_rt.create(w, h);
        m_rtSize = needed;
    }
}

void ShaderBackground::draw(const ImVec2& screenPos,
    const ImVec2& screenSize,
    ImDrawList* dl) {
    if (!dl) return;
    const int w = static_cast<int>(screenSize.x);
    const int h = static_cast<int>(screenSize.y);
    if (w <= 1 || h <= 1) return;

    ensureShader();
    ensureRT(static_cast<unsigned>(w), static_cast<unsigned>(h));

    m_shader.setUniform("iResolution", sf::Glsl::Vec2((float)w, (float)h));
    m_shader.setUniform("iTime", m_clock.getElapsedTime().asSeconds());

    m_rt.clear(sf::Color::Black);

    sf::RectangleShape quad;
    quad.setSize(sf::Vector2f((float)w, (float)h));
    quad.setPosition(0.f, 0.f);

    sf::RenderStates states;
    states.shader = &m_shader;
    m_rt.draw(quad, states);
    m_rt.display();

    const sf::Texture& tex = m_rt.getTexture();
    const ImTextureID texId = (ImTextureID)(intptr_t)tex.getNativeHandle();
    const ImVec2 p1(screenPos.x + screenSize.x, screenPos.y + screenSize.y);
    dl->AddImage(texId, screenPos, p1, ImVec2(0, 0), ImVec2(1, 1),
        IM_COL32(255, 255, 255, 255));
}