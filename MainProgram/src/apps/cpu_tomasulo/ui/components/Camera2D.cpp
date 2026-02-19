// ============================================================================
// File: src/apps/cpu_tomasulo/ui/components/Camera2D.cpp
// ============================================================================

/**
 * @file Camera2D.cpp
 * @brief Implementation of Camera2D.
 */

#include "apps/cpu_tomasulo/ui/components/Camera2D.h"
#include "imgui.h"

 // ============================================================================
 // Lifecycle
 // ============================================================================

void Camera2D::init(float worldW, float worldH) {
    m_center = { worldW * 0.5f, worldH * 0.5f };
    m_centerTarget = m_center;
    m_zoom = 1.0f;
    m_zoomTarget = 1.0f;
    m_velocity = { 0, 0 };
    m_dragging = false;
    m_dragButton = -1;
    m_initialized = true;
}

void Camera2D::reset() {
    m_zoom = 1.0f;
    m_zoomTarget = 1.0f;
    m_dragging = false;
    m_dragButton = -1;
    m_velocity = { 0, 0 };
    m_initialized = false;   // will re-center on the next init() call
}

// ============================================================================
// Static Helpers
// ============================================================================

float Camera2D::computeFitScale(float canvasW, float canvasH,
    float worldW, float worldH, float fill) {
    return std::min(canvasW / worldW, canvasH / worldH) * fill;
}

ImVec2 Camera2D::worldOrigin(const ImVec2& canvasP0,
    const ImVec2& canvasSize,
    float fitScale) const {
    const float scale = fitScale * m_zoom;
    const float cx = canvasP0.x + canvasSize.x * 0.5f;
    const float cy = canvasP0.y + canvasSize.y * 0.5f;
    return { cx - m_center.x * scale,
             cy - m_center.y * scale };
}

// ============================================================================
// Input Handling
// ============================================================================

void Camera2D::handleInput(const ImVec2& canvasP0,
    const ImVec2& canvasSize,
    float fitScale) {
    ImGuiIO& io = ImGui::GetIO();
    const ImVec2 mouse = io.MousePos;

    const ImVec2 canvasCenter = {
        canvasP0.x + canvasSize.x * 0.5f,
        canvasP0.y + canvasSize.y * 0.5f
    };

    const float oldScale = fitScale * m_zoomTarget;

    // ── Double-right-click -> reset ────────────────────────────
    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right)) {
        reset();
        return;
    }

    // ── Mouse-wheel zoom (cursor-anchored) ────────────────────
    if (io.MouseWheel != 0.0f) {
        // World point currently under the cursor — stays fixed after zoom
        const ImVec2 worldUnderMouse = {
            m_centerTarget.x + (mouse.x - canvasCenter.x) / oldScale,
            m_centerTarget.y + (mouse.y - canvasCenter.y) / oldScale
        };

        const float factor = (io.MouseWheel > 0.0f) ? config.zoomStep : (1.0f / config.zoomStep);
        const float newZoom = clampf(m_zoomTarget * factor, config.zoomMin, config.zoomMax);

        if (newZoom != m_zoomTarget) {
            m_zoomTarget = newZoom;
            const float newScale = fitScale * m_zoomTarget;
            // Recompute center so the world point under the cursor does not shift
            m_centerTarget = {
                worldUnderMouse.x - (mouse.x - canvasCenter.x) / newScale,
                worldUnderMouse.y - (mouse.y - canvasCenter.y) / newScale
            };
        }
    }

    // ── Begin drag ────────────────────────────────────────────
    auto tryBeginDrag = [&](ImGuiMouseButton btn) {
        if (ImGui::IsMouseClicked(btn)) {
            m_dragging = true;
            m_dragButton = static_cast<int>(btn);
            m_dragStartMouse = mouse;
            m_dragStartCenterTarget = m_centerTarget;
            m_velocity = { 0, 0 };
        }
        };
    tryBeginDrag(ImGuiMouseButton_Left);
    tryBeginDrag(ImGuiMouseButton_Right);

    // ── Continue / end drag ───────────────────────────────────
    if (m_dragging) {
        const bool held =
            (m_dragButton == ImGuiMouseButton_Left && ImGui::IsMouseDown(ImGuiMouseButton_Left)) ||
            (m_dragButton == ImGuiMouseButton_Right && ImGui::IsMouseDown(ImGuiMouseButton_Right));

        if (held) {
            const float  scale = fitScale * m_zoomTarget;
            const ImVec2 delta = { mouse.x - m_dragStartMouse.x,
                                      mouse.y - m_dragStartMouse.y };
            const ImVec2 newTarget = {
                m_dragStartCenterTarget.x - delta.x / scale,
                m_dragStartCenterTarget.y - delta.y / scale
            };

            // Record velocity for inertia on release
            if (config.enableInertia && io.DeltaTime > 0.0f) {
                m_velocity = {
                    (newTarget.x - m_centerTarget.x) / io.DeltaTime,
                    (newTarget.y - m_centerTarget.y) / io.DeltaTime
                };
            }
            m_centerTarget = newTarget;
        }
        else {
            m_dragging = false;
            m_dragButton = -1;
        }
    }
}

// ============================================================================
// Animation
// ============================================================================

void Camera2D::animate(float dt) {
    if (dt <= 0.0f) return;

    // Smooth zoom toward target
    {
        const float a = expAlpha(config.zoomSmoothness, dt);
        m_zoom += (m_zoomTarget - m_zoom) * a;
    }
    // Smooth pan toward target
    {
        const float a = expAlpha(config.panSmoothness, dt);
        m_center = lerp(m_center, m_centerTarget, a);
    }
    // Inertial drift after drag release
    if (config.enableInertia && !m_dragging) {
        const float speed = std::sqrt(m_velocity.x * m_velocity.x +
            m_velocity.y * m_velocity.y);
        if (speed > config.inertiaMinSpeed) {
            m_centerTarget.x += m_velocity.x * dt;
            m_centerTarget.y += m_velocity.y * dt;
            const float f = std::exp(-config.inertiaFriction * dt);
            m_velocity.x *= f;
            m_velocity.y *= f;
        }
        else {
            m_velocity = { 0, 0 };
        }
    }
}

// ============================================================================
// Viewport Clamping
// ============================================================================

void Camera2D::clampTarget(float worldW, float worldH,
    float canvasW, float canvasH,
    float fitScale) {
    const float scale = fitScale * m_zoomTarget;
    const float halfViewW = canvasW / (2.0f * scale);
    const float halfViewH = canvasH / (2.0f * scale);

    const float minVis = config.minVisibleFrac;
    const float overlapW = worldW * minVis;
    const float overlapH = worldH * minVis;

    // Derivation: visible X range is [centerTarget.x - halfViewW, centerTarget.x + halfViewW].
    // It must overlap [0, worldW] by at least overlapW.
    const float minCX = overlapW - halfViewW;
    const float maxCX = worldW - overlapW + halfViewW;
    const float minCY = overlapH - halfViewH;
    const float maxCY = worldH - overlapH + halfViewH;

    // If constraints invert (very zoomed out), just center on the world
    m_centerTarget.x = (minCX < maxCX)
        ? clampf(m_centerTarget.x, minCX, maxCX)
        : worldW * 0.5f;
    m_centerTarget.y = (minCY < maxCY)
        ? clampf(m_centerTarget.y, minCY, maxCY)
        : worldH * 0.5f;
}