// ============================================================================
// File: include/apps/cpu_tomasulo/ui/components/Camera2D.h
// ============================================================================

#pragma once

/**
 * @file Camera2D.h
 * @brief 2D camera with smooth pan, zoom, inertia, and viewport clamping.
 *
 * Responsibility: Owns all camera state and world-to-screen transforms.
 *
 * Features:
 *   - Mouse-wheel zoom anchored to the cursor position
 *   - Left or right button drag pan
 *   - Exponential smoothing toward target state (separate zoom / pan rates)
 *   - Optional inertial drift on drag release
 *   - Viewport clamping: the image always remains partially visible
 *   - Double-right-click resets to default
 */

#include "imgui.h"
#include <cmath>
#include <algorithm>

class Camera2D {
public:
    // ── Configuration (public so the owner can tweak at runtime) ──
    struct Config {
        float zoomMin = 0.35f;
        float zoomMax = 3.00f;
        float zoomStep = 1.12f;   ///< Multiplier applied per scroll tick.
        float zoomSmoothness = 20.0f;   ///< Higher = snappier zoom animation.
        float panSmoothness = 24.0f;   ///< Higher = snappier pan animation.
        bool  enableInertia = true;
        float inertiaFriction = 10.0f;
        float inertiaMinSpeed = 1.5f; ///< Velocity (world units/s) below which inertia stops.
        float fitFill = 0.90f;///< Fraction of canvas used when fitting.
        float minVisibleFrac = 0.20f;///< Fraction of world that must stay inside viewport.
    };

    Config config;

    // ── Lifecycle ──────────────────────────────────────────────────
    Camera2D() = default;

    /** @brief Call once when the world (image) size is first known. */
    void init(float worldW, float worldH);

    /** @brief Reset camera to centered position at zoom = 1. */
    void reset();

    bool isInitialized() const { return m_initialized; }

    // ── Per-frame Pipeline ────────────────────────────────────────
    /** @brief Process mouse input. Call only when the canvas is hovered. */
    void handleInput(const ImVec2& canvasP0,
        const ImVec2& canvasSize,
        float fitScale);

    /** @brief Animate current state toward target. Call every frame. */
    void animate(float dt);

    /** @brief Clamp target so at least minVisibleFrac of the image stays on screen. */
    void clampTarget(float worldW, float worldH,
        float canvasW, float canvasH,
        float fitScale);

    // ── Queries ───────────────────────────────────────────────────
    float zoom()       const { return m_zoom; }
    float zoomTarget() const { return m_zoomTarget; }

    /** @brief Compute fit scale: how large one world-pixel is on screen at zoom = 1. */
    static float computeFitScale(float canvasW, float canvasH,
        float worldW, float worldH, float fill);

    /** @brief Returns the screen-space top-left corner of the world image. */
    ImVec2 worldOrigin(const ImVec2& canvasP0,
        const ImVec2& canvasSize,
        float fitScale) const;

    float currentScale(float fitScale) const { return fitScale * m_zoom; }

private:
    static float  clampf(float v, float lo, float hi) { return std::max(lo, std::min(v, hi)); }
    static float  expAlpha(float k, float dt) { return 1.0f - std::exp(-k * dt); }
    static ImVec2 lerp(const ImVec2& a, const ImVec2& b, float t) {
        return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
    }

    bool   m_initialized = false;

    // Current (animated)
    ImVec2 m_center = { 0, 0 };
    float  m_zoom = 1.0f;

    // Target (updated by input)
    ImVec2 m_centerTarget = { 0, 0 };
    float  m_zoomTarget = 1.0f;

    // Drag state
    bool   m_dragging = false;
    int    m_dragButton = -1;
    ImVec2 m_dragStartMouse = { 0, 0 };
    ImVec2 m_dragStartCenterTarget = { 0, 0 };

    // Inertia
    ImVec2 m_velocity = { 0, 0 };  ///< World units / second.
};