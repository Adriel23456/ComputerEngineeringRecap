// ============================================================================
// File: src/apps/quicksort_visualizer/ui/ElementRenderer.cpp
// ============================================================================

/**
 * @file ElementRenderer.cpp
 * @brief Implementation of the ElementRenderer class.
 */

#include "apps/quicksort_visualizer/ui/ElementRenderer.h"
#include "apps/quicksort_visualizer/data/ElementCollection.h"
#include "apps/quicksort_visualizer/visualization/GridConfig.h"
#include "imgui.h"

namespace quicksort {
    namespace ui {

        // ============================================================================
        // ElementColors
        // ============================================================================

        ElementColors ElementColors::createDefault() {
            ElementColors colors;

            // Chalk/cream color for normal state
            colors.normal = IM_COL32(220, 220, 200, 255);

            // Yellow for comparing
            colors.comparing = IM_COL32(255, 220, 80, 255);

            // Red for swapping
            colors.swapping = IM_COL32(255, 100, 100, 255);

            // Green for pivot
            colors.pivot = IM_COL32(100, 220, 120, 255);

            // Blue for sorted
            colors.sorted = IM_COL32(100, 180, 255, 255);

            // Orange for active partition
            colors.active = IM_COL32(255, 160, 80, 255);

            // Dark outline
            colors.outline = IM_COL32(40, 45, 55, 200);

            return colors;
        }

        // ============================================================================
        // Construction
        // ============================================================================

        ElementRenderer::ElementRenderer(const GridTransform& transform)
            : m_transform(transform)
            , m_colors(ElementColors::createDefault())
            , m_showOutlines(true)
            , m_elementGap(DEFAULT_GAP)
            , m_verticalPadding(DEFAULT_VERTICAL_PADDING)
            , m_hasAnimation(false)
            , m_animState()
        {
        }

        // ============================================================================
        // Rendering
        // ============================================================================

        void ElementRenderer::render(ImDrawList* drawList,
            const data::ElementCollection& collection,
            const ImVec2& contentMin,
            float elementWidth,
            float gridHeight) {
            if (collection.isEmpty()) {
                return;
            }

            uint32_t count = collection.getCount();
            uint32_t maxValue = collection.getMaxValue();

            // Render each element
            for (uint32_t i = 0; i < count; ++i) {
                const data::SortElement& element = collection.getElement(i);

                // Calculate position and size in grid coordinates
                Vec2 gridPos, gridSize;
                calculateElementRect(i, element.value, maxValue, elementWidth, gridHeight,
                    gridPos, gridSize);

                // Convert grid coordinates to screen coordinates
                Vec2 topLeftViewport = m_transform.gridToScreen(gridPos);
                Vec2 bottomRightViewport = m_transform.gridToScreen(
                    Vec2(gridPos.x + gridSize.x, gridPos.y + gridSize.y)
                );

                // Convert viewport coords to screen coords
                ImVec2 screenMin(contentMin.x + topLeftViewport.x,
                    contentMin.y + topLeftViewport.y);
                ImVec2 screenMax(contentMin.x + bottomRightViewport.x,
                    contentMin.y + bottomRightViewport.y);

                // Skip if completely outside viewport
                Vec2 viewportSize = m_transform.getViewportSize();
                if (screenMax.x < contentMin.x || screenMin.x > contentMin.x + viewportSize.x ||
                    screenMax.y < contentMin.y || screenMin.y > contentMin.y + viewportSize.y) {
                    continue;
                }

                // Get color based on state
                uint32_t fillColor = getColorForState(element.state);

                // Apply animation alpha if this element is being animated
                if (m_hasAnimation && (i == m_animState.indexA || i == m_animState.indexB)) {
                    fillColor = applyAlpha(fillColor, m_animState.alpha);
                }

                // Draw filled rectangle
                drawList->AddRectFilled(screenMin, screenMax, fillColor);

                // Draw outline if enabled (only if element is wide enough and fully visible)
                if (m_showOutlines && (screenMax.x - screenMin.x) > 3.0f) {
                    uint32_t outlineColor = m_colors.outline;

                    // Apply animation alpha to outline too
                    if (m_hasAnimation && (i == m_animState.indexA || i == m_animState.indexB)) {
                        outlineColor = applyAlpha(outlineColor, m_animState.alpha);
                    }

                    drawList->AddRect(screenMin, screenMax, outlineColor, 0.0f, 0, 1.0f);
                }
            }
        }

        // ============================================================================
        // Animation Support
        // ============================================================================

        void ElementRenderer::setAnimationState(const animation::AnimationState& state) {
            m_hasAnimation = (state.phase != animation::AnimationPhase::Idle);
            m_animState = state;
        }

        void ElementRenderer::clearAnimationState() {
            m_hasAnimation = false;
            m_animState = animation::AnimationState();
        }

        // ============================================================================
        // Configuration
        // ============================================================================

        void ElementRenderer::setColors(const ElementColors& colors) {
            m_colors = colors;
        }

        void ElementRenderer::setShowOutlines(bool show) {
            m_showOutlines = show;
        }

        void ElementRenderer::setElementGap(float gap) {
            m_elementGap = gap;
        }

        void ElementRenderer::setVerticalPadding(float ratio) {
            m_verticalPadding = std::clamp(ratio, 0.0f, 0.5f);
        }

        // ============================================================================
        // Internal Helpers
        // ============================================================================

        uint32_t ElementRenderer::getColorForState(data::ElementState state) const {
            switch (state) {
            case data::ElementState::Normal:    return m_colors.normal;
            case data::ElementState::Comparing: return m_colors.comparing;
            case data::ElementState::Swapping:  return m_colors.swapping;
            case data::ElementState::Pivot:     return m_colors.pivot;
            case data::ElementState::Sorted:    return m_colors.sorted;
            case data::ElementState::Active:    return m_colors.active;
            default:                            return m_colors.normal;
            }
        }

        uint32_t ElementRenderer::applyAlpha(uint32_t color, float alpha) const {
            // Extract RGBA components
            uint8_t r = (color >> 0) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = (color >> 16) & 0xFF;
            uint8_t a = (color >> 24) & 0xFF;

            // Apply alpha
            a = static_cast<uint8_t>(a * alpha);

            // Reconstruct color
            return IM_COL32(r, g, b, a);
        }

        void ElementRenderer::calculateElementRect(uint32_t index,
            uint32_t value,
            uint32_t maxValue,
            float elementWidth,
            float gridHeight,
            Vec2& outPos,
            Vec2& outSize) const {
            // Calculate usable height (with padding at top)
            float usableHeight = gridHeight * (1.0f - m_verticalPadding);
            float topPadding = gridHeight * m_verticalPadding;

            // Calculate element height proportional to value
            float heightRatio = static_cast<float>(value) / static_cast<float>(maxValue);
            float elementHeight = usableHeight * heightRatio;

            // Calculate x position using the configured left padding
            float x = config::GridConfig::GRID_LEFT_PADDING +
                static_cast<float>(index) * (elementWidth + m_elementGap);

            // Calculate y position (elements grow upward from bottom)
            float y = topPadding + (usableHeight - elementHeight);

            outPos = Vec2(x, y);
            outSize = Vec2(elementWidth, elementHeight);
        }

    } // namespace ui
} // namespace quicksort