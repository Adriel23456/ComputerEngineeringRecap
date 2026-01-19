// ============================================================================
// File: src/apps/quicksort_visualizer/ui/GridPanel.cpp
// ============================================================================

/**
 * @file GridPanel.cpp
 * @brief Implementation of the GridPanel UI component.
 */

#include "apps/quicksort_visualizer/ui/GridPanel.h"
#include "apps/quicksort_visualizer/ui/ElementRenderer.h"
#include "apps/quicksort_visualizer/data/ElementCollection.h"
#include "apps/quicksort_visualizer/visualization/GridConfig.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>
#include <algorithm>
#include <cstdio>

namespace quicksort {
    namespace ui {

        // ============================================================================
        // Construction
        // ============================================================================

        GridPanel::GridPanel(GridTransform& transform)
            : m_transform(transform)
            , m_isHovered(false)
            , m_isContentHovered(false)
            , m_contentPosition(0.0f, 0.0f)
            , m_contentSize(0.0f, 0.0f)
            , m_showGridLines(config::GridConfig::SHOW_GRID_LINES)
            , m_showCoordinates(true)
        {
        }

        // ============================================================================
        // Rendering
        // ============================================================================

        void GridPanel::render(const ImVec2& position, const ImVec2& size,
            const data::ElementCollection* elements,
            ElementRenderer* renderer,
            float elementWidth) {
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size, ImGuiCond_Always);

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse;

            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(
                config::GridConfig::GRID_BG_COLOR[0],
                config::GridConfig::GRID_BG_COLOR[1],
                config::GridConfig::GRID_BG_COLOR[2],
                config::GridConfig::GRID_BG_COLOR[3]
            ));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(
                config::GridConfig::BORDER_COLOR[0],
                config::GridConfig::BORDER_COLOR[1],
                config::GridConfig::BORDER_COLOR[2],
                config::GridConfig::BORDER_COLOR[3]
            ));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, config::GridConfig::BORDER_THICKNESS);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

            if (ImGui::Begin("##GridPanel", nullptr, flags)) {
                // Get content region
                ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
                ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
                ImVec2 windowPos = ImGui::GetWindowPos();

                contentMin.x += windowPos.x;
                contentMin.y += windowPos.y;
                contentMax.x += windowPos.x;
                contentMax.y += windowPos.y;

                // Cache content area info
                m_contentPosition = Vec2(contentMin.x, contentMin.y);
                m_contentSize = Vec2(contentMax.x - contentMin.x, contentMax.y - contentMin.y);

                // Update transform viewport size
                m_transform.setViewportSize(m_contentSize.x, m_contentSize.y);

                // Check hover state
                ImVec2 mousePos = ImGui::GetMousePos();
                m_isHovered = (mousePos.x >= contentMin.x && mousePos.x <= contentMax.x &&
                    mousePos.y >= contentMin.y && mousePos.y <= contentMax.y);

                // Create invisible button to capture input
                ImGui::SetCursorScreenPos(contentMin);
                ImGui::InvisibleButton("##GridContent",
                    ImVec2(m_contentSize.x, m_contentSize.y),
                    ImGuiButtonFlags_MouseButtonLeft |
                    ImGuiButtonFlags_MouseButtonRight);

                m_isContentHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || m_isHovered;

                // Get draw list for custom rendering
                ImDrawList* drawList = ImGui::GetWindowDrawList();

                // Render layers (back to front)
                renderBackground(drawList, contentMin, contentMax);
                renderGridBounds(drawList, contentMin);

                if (m_showGridLines) {
                    renderGridLines(drawList, contentMin, contentMax);
                }

                // Render elements if provided
                if (elements && renderer && !elements->isEmpty()) {
                    Vec2 gridSize = m_transform.getGridSize();
                    renderer->render(drawList, *elements, contentMin, elementWidth, gridSize.y);
                }
                else {
                    // Show placeholder if no elements
                    renderPlaceholder(contentMin, contentMax);
                }

                renderBorder(drawList, contentMin, contentMax);

                // Render coordinate info
                if (m_showCoordinates) {
                    renderCoordinateInfo(drawList, contentMin);
                }
            }
            ImGui::End();

            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(2);
        }

        // ============================================================================
        // State Queries
        // ============================================================================

        bool GridPanel::isHovered() const {
            return m_isHovered;
        }

        bool GridPanel::isContentHovered() const {
            return m_isContentHovered;
        }

        Vec2 GridPanel::getContentPosition() const {
            return m_contentPosition;
        }

        Vec2 GridPanel::getContentSize() const {
            return m_contentSize;
        }

        // ============================================================================
        // Visual Options
        // ============================================================================

        void GridPanel::setShowGridLines(bool show) {
            m_showGridLines = show;
        }

        void GridPanel::setShowCoordinates(bool show) {
            m_showCoordinates = show;
        }

        // ============================================================================
        // Rendering Helpers
        // ============================================================================

        void GridPanel::renderBackground(ImDrawList* drawList,
            const ImVec2& contentMin,
            const ImVec2& contentMax) {
            drawList->AddRectFilled(
                contentMin, contentMax,
                IM_COL32(
                    static_cast<int>(config::GridConfig::GRID_BG_COLOR[0] * 255),
                    static_cast<int>(config::GridConfig::GRID_BG_COLOR[1] * 255),
                    static_cast<int>(config::GridConfig::GRID_BG_COLOR[2] * 255),
                    static_cast<int>(config::GridConfig::GRID_BG_COLOR[3] * 255)
                )
            );
        }

        void GridPanel::renderGridLines(ImDrawList* drawList,
            const ImVec2& contentMin,
            const ImVec2& contentMax) {
            Rect visibleRect = m_transform.getVisibleGridRect();
            float zoom = m_transform.getZoom();

            float majorSpacing = config::GridConfig::MAJOR_GRID_SPACING;
            float minorSpacing = config::GridConfig::MINOR_GRID_SPACING;

            bool showMinorLines = (zoom >= 0.5f);

            ImU32 majorColor = IM_COL32(
                static_cast<int>(config::GridConfig::GRID_LINE_COLOR[0] * 255),
                static_cast<int>(config::GridConfig::GRID_LINE_COLOR[1] * 255),
                static_cast<int>(config::GridConfig::GRID_LINE_COLOR[2] * 255),
                static_cast<int>(config::GridConfig::GRID_LINE_COLOR[3] * 255 * 0.8f)
            );
            ImU32 minorColor = IM_COL32(
                static_cast<int>(config::GridConfig::GRID_LINE_COLOR[0] * 255),
                static_cast<int>(config::GridConfig::GRID_LINE_COLOR[1] * 255),
                static_cast<int>(config::GridConfig::GRID_LINE_COLOR[2] * 255),
                static_cast<int>(config::GridConfig::GRID_LINE_COLOR[3] * 255 * 0.3f)
            );

            float startX = std::floor(visibleRect.x / majorSpacing) * majorSpacing;
            float startY = std::floor(visibleRect.y / majorSpacing) * majorSpacing;

            if (showMinorLines) {
                float minorStartX = std::floor(visibleRect.x / minorSpacing) * minorSpacing;
                float minorStartY = std::floor(visibleRect.y / minorSpacing) * minorSpacing;

                for (float gx = minorStartX; gx <= visibleRect.right(); gx += minorSpacing) {
                    if (std::fmod(std::abs(gx), majorSpacing) < 0.01f) continue;

                    Vec2 viewportPos = m_transform.gridToScreen(Vec2(gx, 0));
                    float screenX = contentMin.x + viewportPos.x;

                    if (screenX >= contentMin.x && screenX <= contentMax.x) {
                        drawList->AddLine(
                            ImVec2(screenX, contentMin.y),
                            ImVec2(screenX, contentMax.y),
                            minorColor, 1.0f
                        );
                    }
                }

                for (float gy = minorStartY; gy <= visibleRect.bottom(); gy += minorSpacing) {
                    if (std::fmod(std::abs(gy), majorSpacing) < 0.01f) continue;

                    Vec2 viewportPos = m_transform.gridToScreen(Vec2(0, gy));
                    float screenY = contentMin.y + viewportPos.y;

                    if (screenY >= contentMin.y && screenY <= contentMax.y) {
                        drawList->AddLine(
                            ImVec2(contentMin.x, screenY),
                            ImVec2(contentMax.x, screenY),
                            minorColor, 1.0f
                        );
                    }
                }
            }

            for (float gx = startX; gx <= visibleRect.right(); gx += majorSpacing) {
                Vec2 viewportPos = m_transform.gridToScreen(Vec2(gx, 0));
                float screenX = contentMin.x + viewportPos.x;

                if (screenX >= contentMin.x && screenX <= contentMax.x) {
                    drawList->AddLine(
                        ImVec2(screenX, contentMin.y),
                        ImVec2(screenX, contentMax.y),
                        majorColor, 1.0f
                    );
                }
            }

            for (float gy = startY; gy <= visibleRect.bottom(); gy += majorSpacing) {
                Vec2 viewportPos = m_transform.gridToScreen(Vec2(0, gy));
                float screenY = contentMin.y + viewportPos.y;

                if (screenY >= contentMin.y && screenY <= contentMax.y) {
                    drawList->AddLine(
                        ImVec2(contentMin.x, screenY),
                        ImVec2(contentMax.x, screenY),
                        majorColor, 1.0f
                    );
                }
            }
        }

        void GridPanel::renderBorder(ImDrawList* drawList,
            const ImVec2& contentMin,
            const ImVec2& contentMax) {
            ImU32 borderColor = IM_COL32(
                static_cast<int>(config::GridConfig::BORDER_COLOR[0] * 255),
                static_cast<int>(config::GridConfig::BORDER_COLOR[1] * 255),
                static_cast<int>(config::GridConfig::BORDER_COLOR[2] * 255),
                static_cast<int>(config::GridConfig::BORDER_COLOR[3] * 255)
            );

            drawList->AddRect(contentMin, contentMax, borderColor,
                8.0f, 0, config::GridConfig::BORDER_THICKNESS);
        }

        void GridPanel::renderGridBounds(ImDrawList* drawList, const ImVec2& contentMin) {
            Vec2 gridSize = m_transform.getGridSize();

            Vec2 topLeftViewport = m_transform.gridToScreen(Vec2(0, 0));
            Vec2 bottomRightViewport = m_transform.gridToScreen(gridSize);

            ImVec2 boundsMin(
                contentMin.x + topLeftViewport.x,
                contentMin.y + topLeftViewport.y
            );
            ImVec2 boundsMax(
                contentMin.x + bottomRightViewport.x,
                contentMin.y + bottomRightViewport.y
            );

            ImU32 boundsColor = IM_COL32(60, 80, 120, 100);
            drawList->AddRectFilled(boundsMin, boundsMax, boundsColor);

            ImU32 boundsBorderColor = IM_COL32(80, 100, 150, 200);
            drawList->AddRect(boundsMin, boundsMax, boundsBorderColor, 0.0f, 0, 2.0f);
        }

        void GridPanel::renderCoordinateInfo(ImDrawList* drawList, const ImVec2& contentMin) {
            ImGuiIO& io = ImGui::GetIO();
            ImVec2 mousePos = io.MousePos;

            const float padding = 8.0f;
            const float panelX = contentMin.x + 10.0f;
            const float panelY = contentMin.y + 10.0f;

            char line1[64], line2[64], line3[64], line4[64];
            Vec2 gridSize = m_transform.getGridSize();
            Rect visible = m_transform.getVisibleGridRect();

            snprintf(line1, sizeof(line1), "Grid: %.0fx%.0f", gridSize.x, gridSize.y);
            snprintf(line2, sizeof(line2), "Zoom: %.1f%%", m_transform.getZoom() * 100.0f);

            bool showMouse = m_isContentHovered;
            if (showMouse) {
                Vec2 viewportPos(mousePos.x - m_contentPosition.x, mousePos.y - m_contentPosition.y);
                Vec2 gridPos = m_transform.screenToGrid(viewportPos);
                snprintf(line3, sizeof(line3), "Mouse: (%.1f, %.1f)", gridPos.x, gridPos.y);
            }

            snprintf(line4, sizeof(line4), "View: (%.0f,%.0f)-(%.0f,%.0f)",
                visible.x, visible.y, visible.right(), visible.bottom());

            ImVec2 size1 = ImGui::CalcTextSize(line1);
            ImVec2 size2 = ImGui::CalcTextSize(line2);
            ImVec2 size3 = showMouse ? ImGui::CalcTextSize(line3) : ImVec2(0, 0);
            ImVec2 size4 = ImGui::CalcTextSize(line4);

            float maxWidth = std::max({ size1.x, size2.x, size3.x, size4.x });
            float lineHeight = ImGui::GetTextLineHeightWithSpacing();
            float separatorHeight = showMouse ? 4.0f : 0.0f;
            int numLines = showMouse ? 4 : 3;
            float totalHeight = lineHeight * numLines + separatorHeight;

            float panelWidth = maxWidth + padding * 2;
            float panelHeight = totalHeight + padding * 2;

            ImVec2 panelMin(panelX, panelY);
            ImVec2 panelMax(panelX + panelWidth, panelY + panelHeight);

            ImU32 bgColor = IM_COL32(20, 25, 35, 200);
            ImU32 borderColor = IM_COL32(60, 70, 90, 255);
            ImU32 textColor = IM_COL32(180, 200, 230, 255);
            ImU32 separatorColor = IM_COL32(60, 70, 90, 200);

            drawList->AddRectFilled(panelMin, panelMax, bgColor, 4.0f);
            drawList->AddRect(panelMin, panelMax, borderColor, 4.0f, 0, 1.0f);

            float textX = panelX + padding;
            float textY = panelY + padding;

            drawList->AddText(ImVec2(textX, textY), textColor, line1);
            textY += lineHeight;

            drawList->AddText(ImVec2(textX, textY), textColor, line2);
            textY += lineHeight;

            if (showMouse) {
                drawList->AddLine(
                    ImVec2(panelX + padding, textY),
                    ImVec2(panelX + panelWidth - padding, textY),
                    separatorColor, 1.0f
                );
                textY += separatorHeight;

                drawList->AddText(ImVec2(textX, textY), textColor, line3);
                textY += lineHeight;
            }

            drawList->AddText(ImVec2(textX, textY), textColor, line4);
        }

        void GridPanel::renderPlaceholder(const ImVec2& contentMin, const ImVec2& contentMax) {
            const char* text = "Click 'Random Values' to generate elements";
            ImVec2 textSize = ImGui::CalcTextSize(text);

            ImVec2 center(
                (contentMin.x + contentMax.x - textSize.x) * 0.5f,
                (contentMin.y + contentMax.y - textSize.y) * 0.5f
            );

            ImGui::GetWindowDrawList()->AddText(
                center,
                IM_COL32(100, 110, 130, 150),
                text
            );
        }

    } // namespace ui
} // namespace quicksort