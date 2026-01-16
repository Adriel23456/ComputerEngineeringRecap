// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/widgets/ZoomPanImage.h
// ============================================================================

#pragma once

/**
 * @file ZoomPanImage.h
 * @brief Zoomable and pannable image viewer widget.
 *
 * Provides interactive image viewing with zoom (mouse wheel)
 * and pan (drag) functionality.
 *
 * @note Follows:
 *   - SRP: Only handles image pan/zoom
 *   - OCP: Overlay callback allows extension
 */

#include <SFML/Graphics/Texture.hpp>
#include <imgui.h>
#include <functional>

 /**
  * @class ZoomPanImage
  * @brief Interactive image viewer with zoom and pan.
  *
  * Features:
  * - Mouse wheel zoom (when enabled)
  * - Left-click drag to pan
  * - Double-click to reset view
  * - Configurable minimum visible fraction
  * - Optional overlay callback for custom drawing
  */
class ZoomPanImage {
public:
    /**
     * @brief Renders image with zoom/pan.
     * @param tex Texture to display.
     * @param id ImGui widget ID.
     */
    void render(const sf::Texture& tex, const char* id);

    /**
     * @brief Renders image with custom overlay.
     * @param tex Texture to display.
     * @param id ImGui widget ID.
     * @param overlay Callback receiving origin, scale, and draw list.
     */
    void renderWithOverlay(
        const sf::Texture& tex,
        const char* id,
        const std::function<void(const ImVec2& origin, float scale, ImDrawList* dl)>& overlay
    );

    /**
     * @brief Resets zoom and pan to defaults.
     */
    void reset() {
        m_zoom = 1.0f;
        m_pan = ImVec2(0, 0);
    }

    // ========================================================================
    // Configuration
    // ========================================================================

    /**
     * @brief Enables/disables zoom functionality.
     * @param enabled true to allow zoom.
     */
    void setZoomEnabled(bool enabled) { m_zoomEnabled = enabled; }

    /**
     * @brief Sets visible fraction when zoom is disabled.
     * @param frac Fraction (0-1). E.g., 0.8 shows 80% of image.
     */
    void setLockedVisibleFraction(float frac) { m_lockedVisibleFrac = frac; }

    /**
     * @brief Sets minimum visible fraction during pan.
     * @param frac Minimum fraction that must remain visible (0-1).
     */
    void setMinVisibleFraction(float frac) { m_minVisibleFrac = frac; }

private:
    float m_zoom = 1.0f;               ///< Zoom multiplier
    ImVec2 m_pan = ImVec2(0, 0);       ///< Pan offset in screen pixels

    bool m_zoomEnabled = true;         ///< Zoom enabled flag
    float m_lockedVisibleFrac = 1.0f;  ///< Visible fraction when zoom disabled
    float m_minVisibleFrac = 0.15f;    ///< Minimum visible fraction

    /**
     * @brief Clamps value to range.
     */
    static float clamp(float v, float lo, float hi) {
        return v < lo ? lo : (v > hi ? hi : v);
    }
};