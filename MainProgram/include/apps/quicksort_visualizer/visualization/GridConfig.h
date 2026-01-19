// ============================================================================
// File: include/apps/quicksort_visualizer/visualization/GridConfig.h
// ============================================================================

#pragma once

/**
 * @file GridConfig.h
 * @brief Configuration constants for the visualization grid.
 *
 * Contains all configurable parameters for the grid visualization,
 * centralized in one location for easy adjustment.
 *
 * @note Design Principles:
 *   - SRP: Only contains configuration constants
 *   - OCP: Values can be adjusted without changing other code
 */

#include <cstdint>

namespace quicksort {
    namespace config {

        /**
         * @struct GridConfig
         * @brief Static configuration values for the grid.
         */
        struct GridConfig {
            // ========================================================================
            // Grid Dimensions
            // ========================================================================

            static constexpr float DEFAULT_GRID_WIDTH = 1920.0f;
            static constexpr float DEFAULT_GRID_HEIGHT = 1080.0f;
            static constexpr float MIN_GRID_WIDTH = 200.0f;
            static constexpr float MIN_GRID_HEIGHT = 200.0f;

            // ========================================================================
            // Element Configuration
            // ========================================================================

            static constexpr uint32_t DEFAULT_ELEMENT_COUNT = 50;
            static constexpr uint32_t MIN_ELEMENT_COUNT = 5;
            static constexpr uint32_t MAX_ELEMENT_COUNT = 10000;

            // ========================================================================
            // Grid Padding (space on left and right sides)
            // ========================================================================

            static constexpr float GRID_LEFT_PADDING = 25.0f;   ///< Space before first element
            static constexpr float GRID_RIGHT_PADDING = 25.0f;  ///< Space after last element

            // ========================================================================
            // Zoom Configuration
            // ========================================================================

            static constexpr float DEFAULT_ZOOM = 1.0f;
            static constexpr float MIN_ZOOM = 0.1f;
            static constexpr float MAX_ZOOM = 5.0f;
            static constexpr float ZOOM_SPEED = 0.1f;
            static constexpr float ZOOM_SMOOTHING = 0.15f;

            // ========================================================================
            // Pan Configuration
            // ========================================================================

            static constexpr float PAN_SMOOTHING = 0.20f;
            static constexpr float PAN_MARGIN_PERCENT = 0.1f;

            // ========================================================================
            // Visual Configuration
            // ========================================================================

            static constexpr float MAJOR_GRID_SPACING = 100.0f;
            static constexpr float MINOR_GRID_SPACING = 20.0f;
            static constexpr bool SHOW_GRID_LINES = true;
            static constexpr float BORDER_THICKNESS = 2.0f;

            // Colors (RGBA normalized)
            static constexpr float GRID_BG_COLOR[4] = { 0.08f, 0.09f, 0.11f, 1.0f };
            static constexpr float GRID_LINE_COLOR[4] = { 0.2f, 0.22f, 0.28f, 1.0f };
            static constexpr float BORDER_COLOR[4] = { 0.25f, 0.30f, 0.40f, 1.0f };
        };

    } // namespace config
} // namespace quicksort