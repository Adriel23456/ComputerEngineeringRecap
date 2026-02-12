#pragma once
/**
 * @file Camera2D.h
 * @brief 2D camera with smooth pan, zoom, inertia, and clamping.
 *
 * Responsibility: Owns all camera state and transforms.
 *   - Mouse-wheel zoom (cursor-anchored)
 *   - Drag pan (left or right button)
 *   - Exponential smoothing toward target state
 *   - Optional inertial drift on release
 *   - Pan clamping so the image never fully leaves the viewport
 */

#include "imgui.h"
#include <cmath>
#include <algorithm>

class Camera2D {
public:
    // ── Configuration (public so the owner can tweak) ──────────────
    struct Config {
        float zoomMin = 0.35f;
        float zoomMax = 3.00f;
        float zoomStep = 1.12f;
        float zoomSmoothness = 20.0f;   // higher = snappier
        float panSmoothness = 24.0f;
        bool  enableInertia = true;
        float inertiaFriction = 10.0f;
        float inertiaMinSpeed = 1.5f;
        float fitFill = 0.90f;
        float minVisibleFrac = 0.20f;   // fraction of image that must stay visible
    };

    Config config;

    // ── Lifecycle ──────────────────────────────────────────────────
    Camera2D() = default;

    /// Call once when the world (image) size is first known.
    void init(float worldW, float worldH);

    /// Reset camera to centered, zoom 1.
    void reset();

    bool isInitialized() const { return m_initialized; }

    // ── Per-frame pipeline ────────────────────────────────────────
    /// Process input (call only when canvas is hovered).
    void handleInput(const ImVec2& canvasP0,
        const ImVec2& canvasSize,
        float fitScale);

    /// Animate current state toward target (call every frame).
    void animate(float dt);

    /// Clamp target so image is always partially visible.
    void clampTarget(float worldW, float worldH,
        float canvasW, float canvasH,
        float fitScale);

    // ── Queries ───────────────────────────────────────────────────
    float zoom()       const { return m_zoom; }
    float zoomTarget() const { return m_zoomTarget; }

    /// Compute fit scale (how big 1 world-pixel is on screen at zoom=1).
    static float computeFitScale(float canvasW, float canvasH,
        float worldW, float worldH, float fill);

    /// Screen-space top-left of the world image.
    ImVec2 worldOrigin(const ImVec2& canvasP0,
        const ImVec2& canvasSize,
        float fitScale) const;

    float currentScale(float fitScale) const { return fitScale * m_zoom; }

private:
    static float clampf(float v, float lo, float hi) {
        return std::max(lo, std::min(v, hi));
    }
    static float expAlpha(float k, float dt) {
        return 1.0f - std::exp(-k * dt);
    }
    static ImVec2 lerp(const ImVec2& a, const ImVec2& b, float t) {
        return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
    }

    bool   m_initialized = false;

    // Current (animated)
    ImVec2 m_center = { 0, 0 };
    float  m_zoom = 1.0f;

    // Target (what input updates)
    ImVec2 m_centerTarget = { 0, 0 };
    float  m_zoomTarget = 1.0f;

    // Drag state
    bool   m_dragging = false;
    int    m_dragButton = -1;
    ImVec2 m_dragStartMouse = { 0, 0 };
    ImVec2 m_dragStartCenterTarget = { 0, 0 };

    // Inertia
    ImVec2 m_velocity = { 0, 0 };   // world units/sec
};