// ============================================================================
// File: src/apps/quicksort_visualizer/ui/GridPanel.cpp
// ============================================================================

/**
 * @file GridPanel.cpp
 * @brief Implementation of the GridPanel UI component.
 */

#include "apps/quicksort_visualizer/ui/GridPanel.h"
#include "apps/quicksort_visualizer/visualization/GridConfig.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>
#include <algorithm>

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

        void GridPanel::render(const ImVec2& position, const ImVec2& size) {
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size, ImGuiCond_Always);

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse |
                ImGuiWindowFlags_NoBringToFrontOnFocus;

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

                // Check hover state using direct mouse position check
                ImVec2 mousePos = ImGui::GetMousePos();
                m_isHovered = (mousePos.x >= contentMin.x && mousePos.x <= contentMax.x &&
                    mousePos.y >= contentMin.y && mousePos.y <= contentMax.y);

                // Create an invisible button covering the content area to capture input
                // Use NoNav to prevent focus issues
                ImGui::SetCursorScreenPos(contentMin);
                ImGui::InvisibleButton("##GridContent",
                    ImVec2(m_contentSize.x, m_contentSize.y),
                    ImGuiButtonFlags_MouseButtonLeft |
                    ImGuiButtonFlags_MouseButtonRight);

                // Use IsItemHovered but also keep our manual check as backup
                m_isContentHovered = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || m_isHovered;

                // Get draw list for custom rendering
                ImDrawList* drawList = ImGui::GetWindowDrawList();

                // Render layers (back to front)
                renderBackground(drawList, contentMin, contentMax);
                renderGridBounds(drawList, contentMin);

                if (m_showGridLines) {
                    renderGridLines(drawList, contentMin, contentMax);
                }

                renderBorder(drawList, contentMin, contentMax);
                renderPlaceholder(contentMin, contentMax);
            }
            ImGui::End();

            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(2);

            // Render coordinate info AFTER the main window ends
            // This ensures it's not affected by the InvisibleButton focus
            if (m_showCoordinates) {
                renderCoordinateInfo();
            }
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
            // Solid background fill
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
            // Get visible grid region
            Rect visibleRect = m_transform.getVisibleGridRect();
            float zoom = m_transform.getZoom();

            // Determine grid spacing based on zoom level
            float majorSpacing = config::GridConfig::MAJOR_GRID_SPACING;
            float minorSpacing = config::GridConfig::MINOR_GRID_SPACING;

            // Adjust spacing for zoom (hide minor lines when zoomed out)
            bool showMinorLines = (zoom >= 0.5f);

            // Colors
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

            // Calculate starting points (aligned to grid)
            float startX = std::floor(visibleRect.x / majorSpacing) * majorSpacing;
            float startY = std::floor(visibleRect.y / majorSpacing) * majorSpacing;

            // Draw minor grid lines first (if visible)
            if (showMinorLines) {
                float minorStartX = std::floor(visibleRect.x / minorSpacing) * minorSpacing;
                float minorStartY = std::floor(visibleRect.y / minorSpacing) * minorSpacing;

                for (float gx = minorStartX; gx <= visibleRect.right(); gx += minorSpacing) {
                    // Skip if this is a major line
                    if (std::fmod(std::abs(gx), majorSpacing) < 0.01f) continue;

                    Vec2 screenPos = m_transform.gridToScreen(Vec2(gx, 0));
                    float screenX = contentMin.x + (screenPos.x - m_contentPosition.x);

                    if (screenX >= contentMin.x && screenX <= contentMax.x) {
                        drawList->AddLine(
                            ImVec2(screenX, contentMin.y),
                            ImVec2(screenX, contentMax.y),
                            minorColor, 1.0f
                        );
                    }
                }

                for (float gy = minorStartY; gy <= visibleRect.bottom(); gy += minorSpacing) {
                    // Skip if this is a major line
                    if (std::fmod(std::abs(gy), majorSpacing) < 0.01f) continue;

                    Vec2 screenPos = m_transform.gridToScreen(Vec2(0, gy));
                    float screenY = contentMin.y + (screenPos.y - m_contentPosition.y);

                    if (screenY >= contentMin.y && screenY <= contentMax.y) {
                        drawList->AddLine(
                            ImVec2(contentMin.x, screenY),
                            ImVec2(contentMax.x, screenY),
                            minorColor, 1.0f
                        );
                    }
                }
            }

            // Draw major grid lines
            for (float gx = startX; gx <= visibleRect.right(); gx += majorSpacing) {
                Vec2 screenPos = m_transform.gridToScreen(Vec2(gx, 0));
                float screenX = contentMin.x + (screenPos.x - m_contentPosition.x);

                if (screenX >= contentMin.x && screenX <= contentMax.x) {
                    drawList->AddLine(
                        ImVec2(screenX, contentMin.y),
                        ImVec2(screenX, contentMax.y),
                        majorColor, 1.0f
                    );
                }
            }

            for (float gy = startY; gy <= visibleRect.bottom(); gy += majorSpacing) {
                Vec2 screenPos = m_transform.gridToScreen(Vec2(0, gy));
                float screenY = contentMin.y + (screenPos.y - m_contentPosition.y);

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
            // Draw the actual grid content bounds (0,0 to gridWidth,gridHeight)
            Vec2 gridSize = m_transform.getGridSize();

            // Convert grid corners to screen coordinates
            Vec2 topLeftScreen = m_transform.gridToScreen(Vec2(0, 0));
            Vec2 bottomRightScreen = m_transform.gridToScreen(gridSize);

            // Offset by content position
            ImVec2 boundsMin(
                contentMin.x + (topLeftScreen.x - m_contentPosition.x),
                contentMin.y + (topLeftScreen.y - m_contentPosition.y)
            );
            ImVec2 boundsMax(
                contentMin.x + (bottomRightScreen.x - m_contentPosition.x),
                contentMin.y + (bottomRightScreen.y - m_contentPosition.y)
            );

            // Draw grid bounds rectangle (slightly visible)
            ImU32 boundsColor = IM_COL32(60, 80, 120, 100);
            drawList->AddRectFilled(boundsMin, boundsMax, boundsColor);

            // Draw bounds border
            ImU32 boundsBorderColor = IM_COL32(80, 100, 150, 200);
            drawList->AddRect(boundsMin, boundsMax, boundsBorderColor, 0.0f, 0, 2.0f);
        }

        void GridPanel::renderCoordinateInfo() {
            // Display coordinate info in corner
            ImGuiIO& io = ImGui::GetIO();
            ImVec2 mousePos = io.MousePos;

            // Convert to grid coordinates
            Vec2 gridPos(0, 0);
            if (m_isContentHovered) {
                Vec2 screenPos(mousePos.x - m_contentPosition.x, mousePos.y - m_contentPosition.y);
                gridPos = m_transform.screenToGrid(screenPos);
            }

            // Info overlay in top-left - rendered OUTSIDE the main grid window
            ImVec2 overlayPos(m_contentPosition.x + 10, m_contentPosition.y + 10);
            ImGui::SetNextWindowPos(overlayPos, ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(0.75f);

            // Use flags that prevent this window from being affected by other interactions
            ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoInputs;  // Key: ignore all input

            // Push a unique ID to avoid conflicts
            ImGui::PushID("GridInfoOverlay");

            if (ImGui::Begin("##GridInfoOverlay", nullptr, overlayFlags)) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.8f, 0.9f, 1.0f));

                Vec2 gridSize = m_transform.getGridSize();
                ImGui::Text("Grid: %.0fx%.0f", gridSize.x, gridSize.y);
                ImGui::Text("Zoom: %.1f%%", m_transform.getZoom() * 100.0f);

                if (m_isContentHovered) {
                    ImGui::Separator();
                    ImGui::Text("Mouse: (%.1f, %.1f)", gridPos.x, gridPos.y);
                }

                // Show visible area
                Rect visible = m_transform.getVisibleGridRect();
                ImGui::Text("View: (%.0f,%.0f)-(%.0f,%.0f)",
                    visible.x, visible.y, visible.right(), visible.bottom());

                ImGui::PopStyleColor();
            }
            ImGui::End();

            ImGui::PopID();
        }

        void GridPanel::renderPlaceholder(const ImVec2& contentMin, const ImVec2& contentMax) {
            // Center text placeholder - will be removed in Step 4 when elements are added
            const char* text = "Drag to pan | Scroll to zoom | Elements will appear here";
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