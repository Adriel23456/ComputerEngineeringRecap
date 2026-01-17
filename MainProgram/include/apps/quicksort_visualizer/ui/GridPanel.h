// ============================================================================
// File: include/apps/quicksort_visualizer/ui/GridPanel.h
// ============================================================================

#pragma once

/**
 * @file GridPanel.h
 * @brief UI component for rendering the visualization grid.
 *
 * Handles all rendering operations for the grid, including:
 * - Background rendering
 * - Grid lines
 * - Coordinate display
 * - Future: Element bars
 *
 * @note Design Principles:
 *   - SRP: Only handles grid rendering
 *   - DIP: Depends on GridTransform abstraction for coordinates
 *   - OCP: Can be extended with new visual elements
 */

#include "apps/quicksort_visualizer/visualization/GridTransform.h"
#include <SFML/Graphics.hpp>
#include <string>

 // Forward declarations - ImDrawList is in global namespace from imgui
struct ImDrawList;
struct ImVec2;

namespace quicksort {
    namespace ui {

        /**
         * @class GridPanel
         * @brief Renders the visualization grid with pan/zoom support.
         *
         * This class handles:
         * - ImGui window setup and positioning
         * - Grid background and border rendering
         * - Grid line drawing (major/minor)
         * - Coordinate info display
         * - Mouse interaction feedback
         */
        class GridPanel {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            /**
             * @brief Constructs the grid panel.
             * @param transform Reference to the coordinate transform (shared with input handler).
             */
            explicit GridPanel(GridTransform& transform);

            /**
             * @brief Destructor.
             */
            ~GridPanel() = default;

            // Non-copyable (holds reference)
            GridPanel(const GridPanel&) = delete;
            GridPanel& operator=(const GridPanel&) = delete;

            // ========================================================================
            // Rendering
            // ========================================================================

            /**
             * @brief Renders the grid panel.
             * @param position Top-left position of the panel.
             * @param size Size of the panel.
             */
            void render(const ImVec2& position, const ImVec2& size);

            // ========================================================================
            // State Queries
            // ========================================================================

            /**
             * @brief Checks if mouse is over the grid content area.
             * @return true if mouse is hovering over grid.
             */
            bool isHovered() const;

            /**
             * @brief Checks if mouse is over the grid content (updated each frame).
             *
             * This uses ImGui's current frame hover state, making it reliable
             * for input handling within the same frame.
             *
             * @return true if mouse is currently over content area.
             */
            bool isContentHovered() const;

            /**
             * @brief Gets the content region position (inside padding/borders).
             * @return Content area top-left in screen coordinates.
             */
            Vec2 getContentPosition() const;

            /**
             * @brief Gets the content region size.
             * @return Content area dimensions.
             */
            Vec2 getContentSize() const;

            // ========================================================================
            // Visual Options
            // ========================================================================

            /**
             * @brief Enables or disables grid line rendering.
             * @param show Whether to show grid lines.
             */
            void setShowGridLines(bool show);

            /**
             * @brief Enables or disables coordinate info overlay.
             * @param show Whether to show coordinates.
             */
            void setShowCoordinates(bool show);

        private:
            // ========================================================================
            // Rendering Helpers
            // ========================================================================

            /**
             * @brief Renders the grid background.
             * @param drawList ImGui draw list for rendering.
             * @param contentMin Top-left of content area.
             * @param contentMax Bottom-right of content area.
             */
            void renderBackground(ImDrawList* drawList,
                const ImVec2& contentMin,
                const ImVec2& contentMax);

            /**
             * @brief Renders grid lines.
             * @param drawList ImGui draw list for rendering.
             * @param contentMin Top-left of content area.
             * @param contentMax Bottom-right of content area.
             */
            void renderGridLines(ImDrawList* drawList,
                const ImVec2& contentMin,
                const ImVec2& contentMax);

            /**
             * @brief Renders the grid border.
             * @param drawList ImGui draw list for rendering.
             * @param contentMin Top-left of content area.
             * @param contentMax Bottom-right of content area.
             */
            void renderBorder(ImDrawList* drawList,
                const ImVec2& contentMin,
                const ImVec2& contentMax);

            /**
             * @brief Renders the grid content bounds indicator.
             * @param drawList ImGui draw list for rendering.
             * @param contentMin Top-left of content area.
             */
            void renderGridBounds(ImDrawList* drawList,
                const ImVec2& contentMin);

            /**
             * @brief Renders coordinate information overlay.
             */
            void renderCoordinateInfo();

            /**
             * @brief Renders the placeholder text (for Step 3).
             * @param contentMin Top-left of content area.
             * @param contentMax Bottom-right of content area.
             */
            void renderPlaceholder(const ImVec2& contentMin, const ImVec2& contentMax);

            // ========================================================================
            // Data Members
            // ========================================================================

            GridTransform& m_transform;     ///< Reference to coordinate transform
            bool m_isHovered;               ///< Whether mouse is over content (legacy)
            bool m_isContentHovered;        ///< Whether mouse is over content (current frame)
            Vec2 m_contentPosition;         ///< Cached content position
            Vec2 m_contentSize;             ///< Cached content size

            // Visual options
            bool m_showGridLines;           ///< Whether to render grid lines
            bool m_showCoordinates;         ///< Whether to show coordinate overlay
        };

    } // namespace ui
} // namespace quicksort