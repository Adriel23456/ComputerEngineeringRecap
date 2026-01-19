// ============================================================================
// File: include/apps/quicksort_visualizer/ui/ElementRenderer.h
// ============================================================================

#pragma once

/**
 * @file ElementRenderer.h
 * @brief Renders sorting elements on the grid with animation support.
 */

#include "apps/quicksort_visualizer/data/SortElement.h"
#include "apps/quicksort_visualizer/visualization/GridTransform.h"
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
            uint32_t normal;
            uint32_t comparing;
            uint32_t swapping;
            uint32_t pivot;
            uint32_t sorted;
            uint32_t active;
            uint32_t outline;
            uint32_t verification;   ///< Color for verification sweep

            static ElementColors createDefault();
        };

        /**
         * @class ElementRenderer
         * @brief Renders elements with swap and verification animation support.
         */
        class ElementRenderer {
        public:
            explicit ElementRenderer(const GridTransform& transform);
            ~ElementRenderer() = default;

            // Non-copyable
            ElementRenderer(const ElementRenderer&) = delete;
            ElementRenderer& operator=(const ElementRenderer&) = delete;

            // Rendering
            void render(ImDrawList* drawList,
                const data::ElementCollection& collection,
                const ImVec2& contentMin,
                float elementWidth,
                float gridHeight);

            // Animation Support
            void setAnimationState(uint32_t indexA, uint32_t indexB, float alpha, bool isAnimating);
            void setVerificationIndex(uint32_t index, bool isVerifying);
            void clearAnimationState();

            // Configuration
            void setColors(const ElementColors& colors);
            void setShowOutlines(bool show);
            void setElementGap(float gap);
            void setVerticalPadding(float ratio);

        private:
            uint32_t getColorForState(data::ElementState state) const;
            uint32_t applyAlpha(uint32_t color, float alpha) const;

            void calculateElementRect(uint32_t index,
                uint32_t value,
                uint32_t maxValue,
                float elementWidth,
                float gridHeight,
                Vec2& outPos,
                Vec2& outSize) const;

            const GridTransform& m_transform;
            ElementColors m_colors;
            bool m_showOutlines;
            float m_elementGap;
            float m_verticalPadding;

            // Swap animation state
            bool m_hasAnimation;
            uint32_t m_animIndexA;
            uint32_t m_animIndexB;
            float m_animAlpha;

            // Verification animation state
            bool m_isVerifying;
            uint32_t m_verificationIndex;

            static constexpr float DEFAULT_GAP = 1.0f;
            static constexpr float DEFAULT_VERTICAL_PADDING = 0.05f;
        };

    } // namespace ui
} // namespace quicksort