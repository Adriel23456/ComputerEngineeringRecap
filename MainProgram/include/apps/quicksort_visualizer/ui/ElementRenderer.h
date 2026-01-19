// ============================================================================
// File: include/apps/quicksort_visualizer/ui/ElementRenderer.h
// ============================================================================

#pragma once

/**
 * @file ElementRenderer.h
 * @brief Renders sorting elements on the grid.
 *
 * Handles all visual rendering of sort elements:
 * - Rectangle drawing with height based on value
 * - Color based on element state
 * - Positioning within the grid
 * - Fade animation support
 *
 * @note Design Principles:
 *   - SRP: Only handles element rendering
 *   - DIP: Depends on data abstractions, not concrete implementations
 *   - OCP: New visual styles can be added without modification
 */

#include "apps/quicksort_visualizer/data/SortElement.h"
#include "apps/quicksort_visualizer/visualization/GridTransform.h"
#include "apps/quicksort_visualizer/animation/SwapAnimator.h"
#include <vector>
#include <cstdint>

 // Forward declarations
struct ImDrawList;
struct ImVec2;

namespace quicksort {

    namespace data {
        class ElementCollection;
    }

    namespace ui {

        /**
         * @struct ElementColors
         * @brief Color definitions for element states.
         */
        struct ElementColors {
            uint32_t normal;     ///< Default chalk color
            uint32_t comparing;  ///< Yellow for comparison
            uint32_t swapping;   ///< Red for swap
            uint32_t pivot;      ///< Green for pivot
            uint32_t sorted;     ///< Blue for sorted
            uint32_t active;     ///< Orange for active partition
            uint32_t outline;    ///< Outline color for elements

            /**
             * @brief Creates default color scheme.
             */
            static ElementColors createDefault();
        };

        /**
         * @class ElementRenderer
         * @brief Renders elements from an ElementCollection onto the grid.
         */
        class ElementRenderer {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            /**
             * @brief Constructs the element renderer.
             * @param transform Reference to grid transform for coordinate conversion.
             */
            explicit ElementRenderer(const GridTransform& transform);

            /**
             * @brief Destructor.
             */
            ~ElementRenderer() = default;

            // Non-copyable
            ElementRenderer(const ElementRenderer&) = delete;
            ElementRenderer& operator=(const ElementRenderer&) = delete;

            // ========================================================================
            // Rendering
            // ========================================================================

            /**
             * @brief Renders all elements from a collection.
             * @param drawList ImGui draw list for rendering.
             * @param collection The element collection to render.
             * @param contentMin Top-left of the viewport content area.
             * @param elementWidth Width of each element bar.
             * @param gridHeight Height of the grid (for scaling).
             */
            void render(ImDrawList* drawList,
                const data::ElementCollection& collection,
                const ImVec2& contentMin,
                float elementWidth,
                float gridHeight);

            // ========================================================================
            // Animation Support
            // ========================================================================

            /**
             * @brief Sets the current animation state for rendering.
             * @param state Animation state from SwapAnimator.
             */
            void setAnimationState(const animation::AnimationState& state);

            /**
             * @brief Clears the animation state.
             */
            void clearAnimationState();

            // ========================================================================
            // Configuration
            // ========================================================================

            /**
             * @brief Sets the color scheme for rendering.
             * @param colors New color scheme.
             */
            void setColors(const ElementColors& colors);

            /**
             * @brief Enables or disables element outlines.
             * @param show Whether to show outlines.
             */
            void setShowOutlines(bool show);

            /**
             * @brief Sets the gap between elements.
             * @param gap Gap in grid units.
             */
            void setElementGap(float gap);

            /**
             * @brief Sets the vertical padding ratio (space above tallest element).
             * @param ratio Padding as ratio of grid height (0.0 - 0.5).
             */
            void setVerticalPadding(float ratio);

        private:
            // ========================================================================
            // Internal Helpers
            // ========================================================================

            /**
             * @brief Gets the color for an element state.
             * @param state The element state.
             * @return Color as ImU32.
             */
            uint32_t getColorForState(data::ElementState state) const;

            /**
             * @brief Applies alpha to a color.
             * @param color Original color.
             * @param alpha Alpha value (0.0 - 1.0).
             * @return Color with modified alpha.
             */
            uint32_t applyAlpha(uint32_t color, float alpha) const;

            /**
             * @brief Calculates element position and size in grid coordinates.
             * @param index Element index.
             * @param value Element value.
             * @param maxValue Maximum value in collection.
             * @param elementWidth Width of each element.
             * @param gridHeight Height of the grid.
             * @param outPos Output: top-left position in grid coords.
             * @param outSize Output: size in grid coords.
             */
            void calculateElementRect(uint32_t index,
                uint32_t value,
                uint32_t maxValue,
                float elementWidth,
                float gridHeight,
                Vec2& outPos,
                Vec2& outSize) const;

            // ========================================================================
            // Data Members
            // ========================================================================

            const GridTransform& m_transform;  ///< Reference to grid transform
            ElementColors m_colors;            ///< Color scheme
            bool m_showOutlines;               ///< Whether to draw outlines
            float m_elementGap;                ///< Gap between elements
            float m_verticalPadding;           ///< Vertical padding ratio

            // Animation state
            bool m_hasAnimation;               ///< Whether animation is active
            animation::AnimationState m_animState; ///< Current animation state

            // Constants
            static constexpr float DEFAULT_GAP = 1.0f;
            static constexpr float DEFAULT_VERTICAL_PADDING = 0.05f;
        };

    } // namespace ui
} // namespace quicksort