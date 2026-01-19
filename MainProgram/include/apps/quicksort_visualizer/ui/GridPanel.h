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
 * - Element rendering (delegated to ElementRenderer)
 *
 * @note Design Principles:
 *   - SRP: Only handles grid panel rendering
 *   - DIP: Depends on GridTransform abstraction for coordinates
 *   - OCP: Can be extended with new visual elements
 */

#include "apps/quicksort_visualizer/visualization/GridTransform.h"
#include <SFML/Graphics.hpp>
#include <string>

 // Forward declarations
struct ImDrawList;
struct ImVec2;

namespace quicksort {

    namespace data {
        class ElementCollection;
    }

    namespace ui {

        class ElementRenderer;

        /**
         * @class GridPanel
         * @brief Renders the visualization grid with pan/zoom support.
         */
        class GridPanel {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            explicit GridPanel(GridTransform& transform);
            ~GridPanel() = default;

            // Non-copyable
            GridPanel(const GridPanel&) = delete;
            GridPanel& operator=(const GridPanel&) = delete;

            // ========================================================================
            // Rendering
            // ========================================================================

            /**
             * @brief Renders the grid panel with elements.
             * @param position Top-left position of the panel.
             * @param size Size of the panel.
             * @param elements Element collection to render (can be nullptr).
             * @param renderer Element renderer to use (can be nullptr).
             * @param elementWidth Width of each element bar.
             */
            void render(const ImVec2& position, const ImVec2& size,
                const data::ElementCollection* elements = nullptr,
                ElementRenderer* renderer = nullptr,
                float elementWidth = 20.0f);

            // ========================================================================
            // State Queries
            // ========================================================================

            bool isHovered() const;
            bool isContentHovered() const;
            Vec2 getContentPosition() const;
            Vec2 getContentSize() const;

            // ========================================================================
            // Visual Options
            // ========================================================================

            void setShowGridLines(bool show);
            void setShowCoordinates(bool show);

        private:
            // ========================================================================
            // Rendering Helpers
            // ========================================================================

            void renderBackground(ImDrawList* drawList,
                const ImVec2& contentMin,
                const ImVec2& contentMax);

            void renderGridLines(ImDrawList* drawList,
                const ImVec2& contentMin,
                const ImVec2& contentMax);

            void renderBorder(ImDrawList* drawList,
                const ImVec2& contentMin,
                const ImVec2& contentMax);

            void renderGridBounds(ImDrawList* drawList,
                const ImVec2& contentMin);

            void renderCoordinateInfo(ImDrawList* drawList, const ImVec2& contentMin);

            void renderPlaceholder(const ImVec2& contentMin, const ImVec2& contentMax);

            // ========================================================================
            // Data Members
            // ========================================================================

            GridTransform& m_transform;
            bool m_isHovered;
            bool m_isContentHovered;
            Vec2 m_contentPosition;
            Vec2 m_contentSize;

            bool m_showGridLines;
            bool m_showCoordinates;
        };

    } // namespace ui
} // namespace quicksort