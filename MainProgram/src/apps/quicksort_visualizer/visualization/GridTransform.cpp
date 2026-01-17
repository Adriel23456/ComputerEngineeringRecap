// ============================================================================
// File: src/apps/quicksort_visualizer/visualization/GridTransform.cpp
// ============================================================================

/**
 * @file GridTransform.cpp
 * @brief Implementation of GridTransform coordinate transformation logic.
 *
 * All coordinate math is handled here with no UI dependencies.
 */

#include "apps/quicksort_visualizer/visualization/GridTransform.h"
#include "apps/quicksort_visualizer/visualization/GridConfig.h"
#include <algorithm>
#include <cmath>

namespace quicksort {

    // ============================================================================
    // Construction
    // ============================================================================

    GridTransform::GridTransform()
        : m_gridSize(config::GridConfig::DEFAULT_GRID_WIDTH,
            config::GridConfig::DEFAULT_GRID_HEIGHT)
        , m_viewportSize(800.0f, 600.0f)  // Default, will be set by UI
        , m_zoom(config::GridConfig::DEFAULT_ZOOM)
        , m_pan(0.0f, 0.0f)
        , m_targetZoom(config::GridConfig::DEFAULT_ZOOM)
        , m_targetPan(0.0f, 0.0f)
    {
        resetPan();
    }

    GridTransform::GridTransform(float gridWidth, float gridHeight)
        : m_gridSize(gridWidth, gridHeight)
        , m_viewportSize(800.0f, 600.0f)
        , m_zoom(config::GridConfig::DEFAULT_ZOOM)
        , m_pan(0.0f, 0.0f)
        , m_targetZoom(config::GridConfig::DEFAULT_ZOOM)
        , m_targetPan(0.0f, 0.0f)
    {
        resetPan();
    }

    // ============================================================================
    // Grid Size Management
    // ============================================================================

    void GridTransform::setGridSize(float width, float height) {
        m_gridSize.x = std::max(width, config::GridConfig::MIN_GRID_WIDTH);
        m_gridSize.y = std::max(height, config::GridConfig::MIN_GRID_HEIGHT);

        // Re-clamp pan after size change
        m_targetPan = clampPan(m_targetPan);
        m_pan = clampPan(m_pan);
    }

    Vec2 GridTransform::getGridSize() const {
        return m_gridSize;
    }

    void GridTransform::setViewportSize(float width, float height) {
        m_viewportSize.x = std::max(width, 1.0f);
        m_viewportSize.y = std::max(height, 1.0f);

        // Re-clamp pan after viewport change
        m_targetPan = clampPan(m_targetPan);
        m_pan = clampPan(m_pan);
    }

    Vec2 GridTransform::getViewportSize() const {
        return m_viewportSize;
    }

    // ============================================================================
    // Zoom Operations
    // ============================================================================

    void GridTransform::setZoom(float zoom) {
        m_targetZoom = clampZoom(zoom);
    }

    float GridTransform::getZoom() const {
        return m_zoom;
    }

    float GridTransform::getTargetZoom() const {
        return m_targetZoom;
    }

    void GridTransform::zoomBy(float delta) {
        setZoom(m_targetZoom + delta);
    }

    void GridTransform::zoomToward(float delta, const Vec2& screenPoint) {
        // Store old target zoom to check if zoom actually changes
        float oldTargetZoom = m_targetZoom;

        // Calculate new target zoom
        float newTargetZoom = clampZoom(m_targetZoom + delta);

        // Only proceed if zoom actually changed
        if (std::abs(newTargetZoom - oldTargetZoom) < ANIMATION_EPSILON) {
            return;
        }

        // Get the grid position under the mouse USING CURRENT values (what user sees now)
        Vec2 gridPointUnderMouse = screenToGrid(screenPoint);

        // Update target zoom
        m_targetZoom = newTargetZoom;

        // Calculate what pan offset would keep gridPointUnderMouse at screenPoint
        // after the zoom completes
        //
        // Formula: screenPoint = viewportCenter + (gridPoint - pan) * zoom
        // Solving for pan: pan = gridPoint - (screenPoint - viewportCenter) / zoom
        //
        Vec2 viewportCenter(m_viewportSize.x * 0.5f, m_viewportSize.y * 0.5f);
        Vec2 screenOffset = screenPoint - viewportCenter;
        Vec2 gridOffset = screenOffset / m_targetZoom;

        m_targetPan = clampPan(gridPointUnderMouse - gridOffset);

        // DO NOT update m_pan or m_zoom here!
        // Let the animation system smoothly interpolate both together.
        // This prevents the visual "jump" where pan is set for a different zoom level.
    }

    void GridTransform::resetZoom() {
        m_targetZoom = config::GridConfig::DEFAULT_ZOOM;
    }

    // ============================================================================
    // Pan Operations
    // ============================================================================

    void GridTransform::setPan(const Vec2& offset) {
        m_targetPan = clampPan(offset);
    }

    Vec2 GridTransform::getPan() const {
        return m_pan;
    }

    Vec2 GridTransform::getTargetPan() const {
        return m_targetPan;
    }

    void GridTransform::panBy(const Vec2& screenDelta) {
        // Convert screen movement to grid movement (inverse of zoom)
        Vec2 gridDelta = screenDelta / m_zoom;
        m_targetPan = clampPan(m_targetPan - gridDelta);  // Subtract because dragging moves content opposite
    }

    void GridTransform::panByImmediate(const Vec2& screenDelta) {
        // Convert screen movement to grid movement (inverse of zoom)
        Vec2 gridDelta = screenDelta / m_zoom;

        // Update BOTH current and target immediately - NO animation delay
        Vec2 newPan = clampPan(m_pan - gridDelta);
        m_pan = newPan;
        m_targetPan = newPan;
    }

    void GridTransform::resetPan() {
        // Center the grid in the viewport
        m_targetPan = Vec2(m_gridSize.x * 0.5f, m_gridSize.y * 0.5f);
        m_pan = m_targetPan;
    }

    void GridTransform::resetView() {
        // Reset to defaults
        m_targetZoom = config::GridConfig::DEFAULT_ZOOM;
        m_targetPan = Vec2(m_gridSize.x * 0.5f, m_gridSize.y * 0.5f);

        // Also update current values immediately for instant reset
        m_zoom = m_targetZoom;
        m_pan = m_targetPan;
    }

    // ============================================================================
    // Coordinate Conversions
    // ============================================================================

    Vec2 GridTransform::screenToGrid(const Vec2& screenPos) const {
        // Screen center
        Vec2 viewportCenter(m_viewportSize.x * 0.5f, m_viewportSize.y * 0.5f);

        // Offset from center in screen coords
        Vec2 screenOffset = screenPos - viewportCenter;

        // Convert to grid coords (divide by zoom)
        Vec2 gridOffset = screenOffset / m_zoom;

        // Add pan (which is the grid position at viewport center)
        return m_pan + gridOffset;
    }

    Vec2 GridTransform::gridToScreen(const Vec2& gridPos) const {
        // Offset from pan center in grid coords
        Vec2 gridOffset = gridPos - m_pan;

        // Convert to screen coords (multiply by zoom)
        Vec2 screenOffset = gridOffset * m_zoom;

        // Add viewport center
        Vec2 viewportCenter(m_viewportSize.x * 0.5f, m_viewportSize.y * 0.5f);
        return viewportCenter + screenOffset;
    }

    Rect GridTransform::getVisibleGridRect() const {
        // Top-left of viewport in grid coords
        Vec2 topLeft = screenToGrid(Vec2(0.0f, 0.0f));

        // Bottom-right of viewport in grid coords
        Vec2 bottomRight = screenToGrid(m_viewportSize);

        return Rect(topLeft.x, topLeft.y,
            bottomRight.x - topLeft.x,
            bottomRight.y - topLeft.y);
    }

    bool GridTransform::isGridPointVisible(const Vec2& gridPos) const {
        Rect visible = getVisibleGridRect();
        return visible.contains(gridPos);
    }

    // ============================================================================
    // Animation Update
    // ============================================================================

    void GridTransform::update(float deltaTime) {
        // Smooth interpolation for zoom
        m_zoom = lerp(m_zoom, m_targetZoom, config::GridConfig::ZOOM_SMOOTHING, deltaTime);

        // Smooth interpolation for pan
        m_pan.x = lerp(m_pan.x, m_targetPan.x, config::GridConfig::PAN_SMOOTHING, deltaTime);
        m_pan.y = lerp(m_pan.y, m_targetPan.y, config::GridConfig::PAN_SMOOTHING, deltaTime);

        // Snap to target if very close (avoid endless tiny movements)
        if (std::abs(m_zoom - m_targetZoom) < ANIMATION_EPSILON) {
            m_zoom = m_targetZoom;
        }
        if (std::abs(m_pan.x - m_targetPan.x) < ANIMATION_EPSILON) {
            m_pan.x = m_targetPan.x;
        }
        if (std::abs(m_pan.y - m_targetPan.y) < ANIMATION_EPSILON) {
            m_pan.y = m_targetPan.y;
        }
    }

    bool GridTransform::isAnimating() const {
        return std::abs(m_zoom - m_targetZoom) > ANIMATION_EPSILON ||
            std::abs(m_pan.x - m_targetPan.x) > ANIMATION_EPSILON ||
            std::abs(m_pan.y - m_targetPan.y) > ANIMATION_EPSILON;
    }

    // ============================================================================
    // Internal Helpers
    // ============================================================================

    Vec2 GridTransform::clampPan(const Vec2& pan) const {
        // Calculate visible size at current target zoom
        float visibleWidth = m_viewportSize.x / m_targetZoom;
        float visibleHeight = m_viewportSize.y / m_targetZoom;

        // Add margin
        float marginX = visibleWidth * config::GridConfig::PAN_MARGIN_PERCENT;
        float marginY = visibleHeight * config::GridConfig::PAN_MARGIN_PERCENT;

        // Calculate bounds
        // Pan represents the grid point at viewport center
        // Allow panning so that at least some content is always visible
        float minX = -marginX + visibleWidth * 0.5f;
        float maxX = m_gridSize.x + marginX - visibleWidth * 0.5f;
        float minY = -marginY + visibleHeight * 0.5f;
        float maxY = m_gridSize.y + marginY - visibleHeight * 0.5f;

        // Handle case where grid is smaller than viewport
        if (minX > maxX) {
            float centerX = m_gridSize.x * 0.5f;
            minX = maxX = centerX;
        }
        if (minY > maxY) {
            float centerY = m_gridSize.y * 0.5f;
            minY = maxY = centerY;
        }

        return Vec2(
            std::clamp(pan.x, minX, maxX),
            std::clamp(pan.y, minY, maxY)
        );
    }

    float GridTransform::clampZoom(float zoom) const {
        return std::clamp(zoom,
            config::GridConfig::MIN_ZOOM,
            config::GridConfig::MAX_ZOOM);
    }

    float GridTransform::lerp(float current, float target, float smoothing, float deltaTime) {
        // Frame-rate independent lerp
        float t = 1.0f - std::pow(1.0f - smoothing, deltaTime * 60.0f);
        return current + (target - current) * t;
    }

} // namespace quicksort