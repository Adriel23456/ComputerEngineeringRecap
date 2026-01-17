// ============================================================================
// File: include/apps/quicksort_visualizer/visualization/GridConfig.h
// ============================================================================

#pragma once

/**
 * @file GridConfig.h
 * @brief Configuration constants for the visualization grid.
 *
 * Contains all configurable parameters for the grid system.
 * Centralized configuration follows SRP and makes tuning easy.
 *
 * @note Design Principles:
 *   - SRP: Only holds configuration data
 *   - OCP: Can be extended with new config without modifying existing code
 */

#include <cstdint>

namespace quicksort {
    namespace config {

        /**
         * @struct GridConfig
         * @brief Static configuration for the visualization grid.
         */
        struct GridConfig {
            // ========================================================================
            // Grid Resolution
            // ========================================================================

            /** @brief Default grid width in logical pixels. */
            static constexpr float DEFAULT_GRID_WIDTH = 1920.0f;

            /** @brief Default grid height in logical pixels. */
            static constexpr float DEFAULT_GRID_HEIGHT = 1080.0f;

            /** @brief Minimum grid width allowed. */
            static constexpr float MIN_GRID_WIDTH = 480.0f;

            /** @brief Minimum grid height allowed. */
            static constexpr float MIN_GRID_HEIGHT = 270.0f;

            // ========================================================================
            // Zoom Configuration
            // ========================================================================

            /** @brief Default zoom level (1.0 = 100%). */
            static constexpr float DEFAULT_ZOOM = 1.0f;

            /** @brief Minimum zoom level (zoomed out). */
            static constexpr float MIN_ZOOM = 0.1f;

            /** @brief Maximum zoom level (zoomed in). */
            static constexpr float MAX_ZOOM = 5.0f;

            /** @brief Zoom change per scroll wheel tick. */
            static constexpr float ZOOM_SPEED = 0.1f;

            /** @brief Smoothing factor for zoom transitions (0-1, higher = faster). */
            static constexpr float ZOOM_SMOOTHING = 0.15f;

            // ========================================================================
            // Pan Configuration
            // ========================================================================

            /** @brief Margin around grid content (percentage of viewport). */
            static constexpr float PAN_MARGIN_PERCENT = 0.1f;

            /** @brief Smoothing factor for pan transitions (0-1, higher = faster). */
            static constexpr float PAN_SMOOTHING = 0.2f;

            // ========================================================================
            // Visual Configuration
            // ========================================================================

            /** @brief Grid background color (RGBA normalized). */
            static constexpr float GRID_BG_COLOR[4] = { 0.02f, 0.03f, 0.05f, 1.0f };

            /** @brief Grid line color (RGBA normalized). */
            static constexpr float GRID_LINE_COLOR[4] = { 0.15f, 0.18f, 0.25f, 0.5f };

            /** @brief Major grid line spacing in logical pixels. */
            static constexpr float MAJOR_GRID_SPACING = 100.0f;

            /** @brief Minor grid line spacing in logical pixels. */
            static constexpr float MINOR_GRID_SPACING = 20.0f;

            /** @brief Whether to show grid lines. */
            static constexpr bool SHOW_GRID_LINES = true;

            /** @brief Border color around the grid viewport. */
            static constexpr float BORDER_COLOR[4] = { 0.3f, 0.35f, 0.45f, 1.0f };

            /** @brief Border thickness in pixels. */
            static constexpr float BORDER_THICKNESS = 2.0f;

            // ========================================================================
            // Element Configuration (for future steps)
            // ========================================================================

            /** @brief Default number of elements to sort. */
            static constexpr uint32_t DEFAULT_ELEMENT_COUNT = 50;

            /** @brief Minimum number of elements. */
            static constexpr uint32_t MIN_ELEMENT_COUNT = 5;

            /** @brief Maximum number of elements. */
            static constexpr uint32_t MAX_ELEMENT_COUNT = 500;

            /** @brief Horizontal padding ratio for elements (10% extra). */
            static constexpr float ELEMENT_PADDING_RATIO = 1.1f;
        };

    } // namespace config
} // namespace quicksort