// ============================================================================
// File: include/apps/quicksort_visualizer/visualization/GridTransform.h
// ============================================================================

#pragma once

/**
 * @file GridTransform.h
 * @brief Coordinate transformation logic for the visualization grid.
 *
 * Handles all mathematical operations for pan, zoom, and coordinate
 * conversions. This class contains NO UI code - pure logic only.
 *
 * @note Design Principles:
 *   - SRP: Only handles coordinate transformations
 *   - OCP: Can be extended for different transform modes
 *   - DIP: No dependencies on UI frameworks
 */

namespace quicksort {

    /**
     * @struct Vec2
     * @brief Simple 2D vector for coordinate operations.
     *
     * Using our own struct to avoid UI framework dependencies in logic layer.
     */
    struct Vec2 {
        float x = 0.0f;
        float y = 0.0f;

        Vec2() = default;
        Vec2(float x_, float y_) : x(x_), y(y_) {}

        Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
        Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
        Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
        Vec2 operator/(float scalar) const { return Vec2(x / scalar, y / scalar); }
        Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
        Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
    };

    /**
     * @struct Rect
     * @brief Simple rectangle for bounds operations.
     */
    struct Rect {
        float x = 0.0f;      ///< Left position
        float y = 0.0f;      ///< Top position
        float width = 0.0f;  ///< Width
        float height = 0.0f; ///< Height

        Rect() = default;
        Rect(float x_, float y_, float w_, float h_) : x(x_), y(y_), width(w_), height(h_) {}

        float right() const { return x + width; }
        float bottom() const { return y + height; }
        Vec2 center() const { return Vec2(x + width * 0.5f, y + height * 0.5f); }
        bool contains(const Vec2& point) const {
            return point.x >= x && point.x <= right() &&
                point.y >= y && point.y <= bottom();
        }
    };

    /**
     * @class GridTransform
     * @brief Manages coordinate transformations for the visualization grid.
     *
     * Provides:
     * - Zoom operations (in/out, to point)
     * - Pan operations with bounds clamping
     * - Coordinate conversions (screen <-> grid)
     * - Smooth interpolation for animations (optional)
     *
     * Coordinate Systems:
     * - Screen Space: Pixel coordinates in the viewport
     * - Grid Space: Logical coordinates in the full grid
     */
    class GridTransform {
    public:
        // ========================================================================
        // Construction
        // ========================================================================

        /**
         * @brief Constructs transform with default values.
         */
        GridTransform();

        /**
         * @brief Constructs transform with specific grid size.
         * @param gridWidth Logical width of the grid.
         * @param gridHeight Logical height of the grid.
         */
        GridTransform(float gridWidth, float gridHeight);

        // ========================================================================
        // Grid Size Management
        // ========================================================================

        /**
         * @brief Sets the logical size of the grid.
         * @param width Grid width in logical units.
         * @param height Grid height in logical units.
         */
        void setGridSize(float width, float height);

        /**
         * @brief Gets the current grid size.
         * @return Grid dimensions as Vec2.
         */
        Vec2 getGridSize() const;

        /**
         * @brief Sets the viewport (screen) size.
         * @param width Viewport width in pixels.
         * @param height Viewport height in pixels.
         */
        void setViewportSize(float width, float height);

        /**
         * @brief Gets the current viewport size.
         * @return Viewport dimensions as Vec2.
         */
        Vec2 getViewportSize() const;

        // ========================================================================
        // Zoom Operations
        // ========================================================================

        /**
         * @brief Sets the target zoom level (with smooth animation).
         * @param zoom New zoom level (clamped to valid range).
         */
        void setZoom(float zoom);

        /**
         * @brief Gets the current zoom level.
         * @return Current zoom value.
         */
        float getZoom() const;

        /**
         * @brief Gets the target zoom level (for smooth transitions).
         * @return Target zoom value.
         */
        float getTargetZoom() const;

        /**
         * @brief Zooms in/out by a delta amount.
         * @param delta Zoom change (positive = zoom in).
         */
        void zoomBy(float delta);

        /**
         * @brief Zooms toward a specific screen point.
         * @param delta Zoom change amount.
         * @param screenPoint Point to zoom toward (in screen coordinates).
         */
        void zoomToward(float delta, const Vec2& screenPoint);

        /**
         * @brief Resets zoom to default level.
         */
        void resetZoom();

        // ========================================================================
        // Pan Operations
        // ========================================================================

        /**
         * @brief Sets the pan offset with smooth animation.
         * @param offset New pan offset in grid coordinates.
         */
        void setPan(const Vec2& offset);

        /**
         * @brief Gets the current pan offset.
         * @return Current pan offset in grid coordinates.
         */
        Vec2 getPan() const;

        /**
         * @brief Gets the target pan offset (for smooth transitions).
         * @return Target pan offset.
         */
        Vec2 getTargetPan() const;

        /**
         * @brief Pans by a delta in screen coordinates (with animation).
         * @param screenDelta Movement in screen pixels.
         */
        void panBy(const Vec2& screenDelta);

        /**
         * @brief Pans by a delta IMMEDIATELY (no animation, for dragging).
         * @param screenDelta Movement in screen pixels.
         */
        void panByImmediate(const Vec2& screenDelta);

        /**
         * @brief Resets pan to center the grid.
         */
        void resetPan();

        /**
         * @brief Resets both pan and zoom to defaults.
         */
        void resetView();

        // ========================================================================
        // Coordinate Conversions
        // ========================================================================

        /**
         * @brief Converts screen coordinates to grid coordinates.
         * @param screenPos Position in screen space.
         * @return Position in grid space.
         */
        Vec2 screenToGrid(const Vec2& screenPos) const;

        /**
         * @brief Converts grid coordinates to screen coordinates.
         * @param gridPos Position in grid space.
         * @return Position in screen space.
         */
        Vec2 gridToScreen(const Vec2& gridPos) const;

        /**
         * @brief Gets the visible region of the grid in grid coordinates.
         * @return Rectangle representing visible area.
         */
        Rect getVisibleGridRect() const;

        /**
         * @brief Checks if a grid point is currently visible.
         * @param gridPos Position in grid space.
         * @return true if point is within visible region.
         */
        bool isGridPointVisible(const Vec2& gridPos) const;

        // ========================================================================
        // Animation Update
        // ========================================================================

        /**
         * @brief Updates smooth interpolation for pan and zoom.
         * @param deltaTime Time since last update in seconds.
         */
        void update(float deltaTime);

        /**
         * @brief Checks if transform is currently animating.
         * @return true if pan or zoom is interpolating.
         */
        bool isAnimating() const;

    private:
        // ========================================================================
        // Internal Helpers
        // ========================================================================

        /**
         * @brief Clamps pan offset to keep grid content visible.
         * @param pan Pan offset to clamp.
         * @return Clamped pan offset.
         */
        Vec2 clampPan(const Vec2& pan) const;

        /**
         * @brief Clamps zoom to valid range.
         * @param zoom Zoom value to clamp.
         * @return Clamped zoom value.
         */
        float clampZoom(float zoom) const;

        /**
         * @brief Interpolates toward target value.
         * @param current Current value.
         * @param target Target value.
         * @param smoothing Smoothing factor.
         * @param deltaTime Time step.
         * @return Interpolated value.
         */
        static float lerp(float current, float target, float smoothing, float deltaTime);

        // ========================================================================
        // Data Members
        // ========================================================================

        // Grid dimensions (logical units)
        Vec2 m_gridSize;

        // Viewport dimensions (screen pixels)
        Vec2 m_viewportSize;

        // Current transform state
        float m_zoom;        ///< Current zoom level
        Vec2 m_pan;          ///< Current pan offset (grid coords at viewport center)

        // Target state (for smooth interpolation)
        float m_targetZoom;  ///< Target zoom level
        Vec2 m_targetPan;    ///< Target pan offset

        // Animation threshold
        static constexpr float ANIMATION_EPSILON = 0.001f;
    };

} // namespace quicksort