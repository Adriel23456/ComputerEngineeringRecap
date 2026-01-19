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

            colors.normal = IM_COL32(220, 220, 200, 255);
            colors.comparing = IM_COL32(255, 220, 80, 255);
            colors.swapping = IM_COL32(255, 100, 100, 255);
            colors.pivot = IM_COL32(100, 220, 120, 255);
            colors.sorted = IM_COL32(100, 180, 255, 255);
            colors.active = IM_COL32(255, 160, 80, 255);
            colors.outline = IM_COL32(40, 45, 55, 200);
            colors.verification = IM_COL32(255, 80, 80, 255);  // Bright red for verification

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
            , m_animIndexA(0)
            , m_animIndexB(0)
            , m_animAlpha(1.0f)
            , m_isVerifying(false)
            , m_verificationIndex(0)
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

            for (uint32_t i = 0; i < count; ++i) {
                const data::SortElement& element = collection.getElement(i);

                Vec2 gridPos, gridSize;
                calculateElementRect(i, element.value, maxValue, elementWidth, gridHeight,
                    gridPos, gridSize);

                Vec2 topLeftViewport = m_transform.gridToScreen(gridPos);
                Vec2 bottomRightViewport = m_transform.gridToScreen(
                    Vec2(gridPos.x + gridSize.x, gridPos.y + gridSize.y)
                );

                ImVec2 screenMin(contentMin.x + topLeftViewport.x,
                    contentMin.y + topLeftViewport.y);
                ImVec2 screenMax(contentMin.x + bottomRightViewport.x,
                    contentMin.y + bottomRightViewport.y);

                Vec2 viewportSize = m_transform.getViewportSize();
                if (screenMax.x < contentMin.x || screenMin.x > contentMin.x + viewportSize.x ||
                    screenMax.y < contentMin.y || screenMin.y > contentMin.y + viewportSize.y) {
                    continue;
                }

                // Determine color
                uint32_t fillColor;

                if (m_isVerifying && i == m_verificationIndex) {
                    // Verification sweep - bright red
                    fillColor = m_colors.verification;
                }
                else if (m_isVerifying && i < m_verificationIndex) {
                    // Already verified - show as sorted (blue)
                    fillColor = m_colors.sorted;
                }
                else {
                    fillColor = getColorForState(element.state);
                }

                // Apply animation alpha if this element is being animated (swap)
                if (m_hasAnimation && (i == m_animIndexA || i == m_animIndexB)) {
                    fillColor = applyAlpha(fillColor, m_animAlpha);
                }

                drawList->AddRectFilled(screenMin, screenMax, fillColor);

                if (m_showOutlines && (screenMax.x - screenMin.x) > 3.0f) {
                    uint32_t outlineColor = m_colors.outline;

                    if (m_hasAnimation && (i == m_animIndexA || i == m_animIndexB)) {
                        outlineColor = applyAlpha(outlineColor, m_animAlpha);
                    }

                    drawList->AddRect(screenMin, screenMax, outlineColor, 0.0f, 0, 1.0f);
                }
            }
        }

        // ============================================================================
        // Animation Support
        // ============================================================================

        void ElementRenderer::setAnimationState(uint32_t indexA, uint32_t indexB, float alpha, bool isAnimating) {
            m_hasAnimation = isAnimating;
            m_animIndexA = indexA;
            m_animIndexB = indexB;
            m_animAlpha = alpha;
        }

        void ElementRenderer::setVerificationIndex(uint32_t index, bool isVerifying) {
            m_isVerifying = isVerifying;
            m_verificationIndex = index;
        }

        void ElementRenderer::clearAnimationState() {
            m_hasAnimation = false;
            m_animIndexA = 0;
            m_animIndexB = 0;
            m_animAlpha = 1.0f;
            m_isVerifying = false;
            m_verificationIndex = 0;
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
            uint8_t r = (color >> 0) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = (color >> 16) & 0xFF;
            uint8_t a = (color >> 24) & 0xFF;

            a = static_cast<uint8_t>(a * alpha);

            return IM_COL32(r, g, b, a);
        }

        void ElementRenderer::calculateElementRect(uint32_t index,
            uint32_t value,
            uint32_t maxValue,
            float elementWidth,
            float gridHeight,
            Vec2& outPos,
            Vec2& outSize) const {
            float usableHeight = gridHeight * (1.0f - m_verticalPadding);
            float topPadding = gridHeight * m_verticalPadding;

            float heightRatio = static_cast<float>(value) / static_cast<float>(maxValue);
            float elementHeight = usableHeight * heightRatio;

            float x = config::GridConfig::GRID_LEFT_PADDING +
                static_cast<float>(index) * (elementWidth + m_elementGap);

            float y = topPadding + (usableHeight - elementHeight);

            outPos = Vec2(x, y);
            outSize = Vec2(elementWidth, elementHeight);
        }

    } // namespace ui
} // namespace quicksort