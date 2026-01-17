// ============================================================================
// File: src/apps/quicksort_visualizer/input/GridInputHandler.cpp
// ============================================================================

/**
 * @file GridInputHandler.cpp
 * @brief Implementation of grid input handling using ImGui mouse state.
 */

#include "apps/quicksort_visualizer/input/GridInputHandler.h"
#include "apps/quicksort_visualizer/visualization/GridConfig.h"
#include "imgui.h"

namespace quicksort {
    namespace input {

        // ============================================================================
        // Construction
        // ============================================================================

        GridInputHandler::GridInputHandler(GridTransform& transform)
            : m_transform(transform)
            , m_isDragging(false)
            , m_lastMousePos(0.0f, 0.0f)
        {
        }

        // ============================================================================
        // Input Processing
        // ============================================================================

        void GridInputHandler::processInput(bool isGridHovered,
            const Vec2& contentPosition,
            const Vec2& contentSize) {
            (void)contentSize; // Unused but kept for potential future use

            // Handle drag (pan)
            handleDrag(isGridHovered);

            // Handle scroll (zoom)
            handleScroll(isGridHovered, contentPosition);
        }

        void GridInputHandler::handleDrag(bool isGridHovered) {
            ImGuiIO& io = ImGui::GetIO();

            // Get current mouse position
            Vec2 currentMousePos(io.MousePos.x, io.MousePos.y);

            // Check for drag start
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isGridHovered) {
                m_isDragging = true;
                m_lastMousePos = currentMousePos;
            }

            // Check for drag end
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                m_isDragging = false;
            }

            // Process drag movement - use IMMEDIATE panning (no animation delay)
            if (m_isDragging) {
                Vec2 delta = currentMousePos - m_lastMousePos;

                // Only pan if there's actual movement
                if (delta.x != 0.0f || delta.y != 0.0f) {
                    // Use panByImmediate for instant response during drag
                    m_transform.panByImmediate(delta);
                }

                m_lastMousePos = currentMousePos;
            }
        }

        void GridInputHandler::handleScroll(bool isGridHovered, const Vec2& contentPosition) {
            if (!isGridHovered) {
                return;
            }

            ImGuiIO& io = ImGui::GetIO();
            float scrollDelta = io.MouseWheel;

            if (scrollDelta != 0.0f) {
                float zoomDelta = scrollDelta * config::GridConfig::ZOOM_SPEED;

                // Get mouse position relative to grid content area
                Vec2 mousePos(
                    io.MousePos.x - contentPosition.x,
                    io.MousePos.y - contentPosition.y
                );

                m_transform.zoomToward(zoomDelta, mousePos);
            }
        }

        // ============================================================================
        // State Queries
        // ============================================================================

        bool GridInputHandler::isDragging() const {
            return m_isDragging;
        }

    } // namespace input
} // namespace quicksort