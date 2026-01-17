// ============================================================================
// File: include/apps/quicksort_visualizer/input/GridInputHandler.h
// ============================================================================

#pragma once

/**
 * @file GridInputHandler.h
 * @brief Input handling for grid pan and zoom operations.
 *
 * Processes mouse input to control the grid view:
 * - Left click + drag: Pan the view
 * - Scroll wheel: Zoom in/out
 *
 * Uses ImGui's mouse state instead of SFML events because the grid
 * is rendered as an ImGui window which captures SFML mouse events.
 *
 * @note Design Principles:
 *   - SRP: Only handles input processing
 *   - DIP: Depends on GridTransform abstraction
 */

#include "apps/quicksort_visualizer/visualization/GridTransform.h"

namespace quicksort {
    namespace input {

        /**
         * @class GridInputHandler
         * @brief Processes input for grid navigation using ImGui mouse state.
         *
         * This handler uses ImGui's IO system to read mouse state because
         * the grid is rendered as an ImGui window, which captures SFML events.
         */
        class GridInputHandler {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            /**
             * @brief Constructs the input handler.
             * @param transform Reference to the grid transform to manipulate.
             */
            explicit GridInputHandler(GridTransform& transform);

            /**
             * @brief Destructor.
             */
            ~GridInputHandler() = default;

            // Non-copyable
            GridInputHandler(const GridInputHandler&) = delete;
            GridInputHandler& operator=(const GridInputHandler&) = delete;

            // ========================================================================
            // Input Processing
            // ========================================================================

            /**
             * @brief Processes input using ImGui mouse state.
             *
             * Should be called every frame during update.
             *
             * @param isGridHovered Whether the mouse is over the grid content area.
             * @param contentPosition Top-left position of the grid content area.
             * @param contentSize Size of the grid content area.
             */
            void processInput(bool isGridHovered, const Vec2& contentPosition, const Vec2& contentSize);

            // ========================================================================
            // State Queries
            // ========================================================================

            /**
             * @brief Checks if currently dragging to pan.
             * @return true if drag operation is active.
             */
            bool isDragging() const;

        private:
            // ========================================================================
            // Internal Helpers
            // ========================================================================

            /**
             * @brief Handles drag (pan) input.
             * @param isGridHovered Whether mouse is over grid.
             */
            void handleDrag(bool isGridHovered);

            /**
             * @brief Handles scroll (zoom) input.
             * @param isGridHovered Whether mouse is over grid.
             * @param contentPosition Grid content position.
             */
            void handleScroll(bool isGridHovered, const Vec2& contentPosition);

            // ========================================================================
            // Data Members
            // ========================================================================

            GridTransform& m_transform;  ///< Reference to grid transform

            bool m_isDragging;           ///< Whether drag is active
            Vec2 m_lastMousePos;         ///< Last mouse position for drag delta
        };

    } // namespace input
} // namespace quicksort