// ============================================================================
// File: src/apps/quicksort_visualizer/ui/ControlPanel.cpp
// ============================================================================

/**
 * @file ControlPanel.cpp
 * @brief Implementation of the ControlPanel UI component.
 */

#include "apps/quicksort_visualizer/ui/ControlPanel.h"
#include "apps/quicksort_visualizer/visualization/GridConfig.h"
#include "imgui.h"

namespace quicksort {
    namespace ui {

        // ============================================================================
        // Construction
        // ============================================================================

        ControlPanel::ControlPanel()
            : m_callbacks{}
            , m_threadState(ThreadState::Uninitialized)
            , m_elementCount(config::GridConfig::DEFAULT_ELEMENT_COUNT)
        {
        }

        // ============================================================================
        // Configuration
        // ============================================================================

        void ControlPanel::setCallbacks(const ControlPanelCallbacks& callbacks) {
            m_callbacks = callbacks;
        }

        void ControlPanel::setThreadState(ThreadState state) {
            m_threadState = state;
        }

        void ControlPanel::setElementCount(uint32_t count) {
            m_elementCount = count;
        }

        // ============================================================================
        // Rendering
        // ============================================================================

        void ControlPanel::render(const ImVec2& position, const ImVec2& size) {
            ImGui::SetNextWindowPos(position, ImGuiCond_Always);
            ImGui::SetNextWindowSize(size, ImGuiCond_Always);

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoScrollbar;

            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.09f, 0.12f, 0.98f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.30f, 0.40f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(PADDING, PADDING));

            if (ImGui::Begin("##ControlPanel", nullptr, flags)) {
                float contentWidth = size.x - (PADDING * 2);
                float contentHeight = size.y - (PADDING * 2);

                // Calculate button dimensions (4 buttons now)
                float buttonCount = 4.0f;
                float totalSpacing = BUTTON_SPACING * (buttonCount - 1);
                float buttonWidth = (contentWidth - totalSpacing) / buttonCount;
                float buttonHeight = contentHeight * 0.6f;

                // Center buttons vertically
                float verticalOffset = (contentHeight - buttonHeight) * 0.4f;
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + verticalOffset);

                // Determine button states based on thread state
                bool threadReady = (m_threadState == ThreadState::Idle);
                bool threadRunning = (m_threadState == ThreadState::Running);

                // Button: Random Values (enabled when not running)
                if (createButton("Random Values", buttonWidth, buttonHeight, !threadRunning)) {
                    if (m_callbacks.onRandomValues) {
                        m_callbacks.onRandomValues();
                    }
                }

                ImGui::SameLine(0, BUTTON_SPACING);

                // Button: Set Amount (enabled when not running)
                if (createButton("Set Amount", buttonWidth, buttonHeight, !threadRunning)) {
                    if (m_callbacks.onSetAmount) {
                        m_callbacks.onSetAmount();
                    }
                }

                ImGui::SameLine(0, BUTTON_SPACING);

                // Button: Start Quicksort (enabled only when thread is ready)
                if (createButton("Start Quicksort", buttonWidth, buttonHeight, threadReady)) {
                    if (m_callbacks.onStartQuicksort) {
                        m_callbacks.onStartQuicksort();
                    }
                }

                ImGui::SameLine(0, BUTTON_SPACING);

                // Button: Reset View (always enabled)
                if (createButton("Reset View", buttonWidth, buttonHeight, true)) {
                    if (m_callbacks.onResetView) {
                        m_callbacks.onResetView();
                    }
                }

                // Thread status and element count on the right
                ImGui::SameLine();
                float statusX = size.x - 250.0f;
                if (ImGui::GetCursorPosX() < statusX) {
                    ImGui::SetCursorPosX(statusX);
                }

                ImGui::BeginGroup();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - verticalOffset + 5.0f);

                // Element count
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.7f, 0.8f, 1.0f));
                ImGui::Text("Elements: %u", m_elementCount);
                ImGui::PopStyleColor();

                // Thread status
                renderThreadStatus();

                ImGui::EndGroup();
            }
            ImGui::End();

            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(2);
        }

        // ============================================================================
        // Rendering Helpers
        // ============================================================================

        bool ControlPanel::createButton(const char* label, float width, float height, bool enabled) {
            if (!enabled) {
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
            }

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.18f, 0.25f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                enabled ? ImVec4(0.25f, 0.30f, 0.40f, 1.0f)
                : ImVec4(0.15f, 0.18f, 0.25f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                enabled ? ImVec4(0.20f, 0.25f, 0.35f, 1.0f)
                : ImVec4(0.15f, 0.18f, 0.25f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.90f, 0.95f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

            bool clicked = ImGui::Button(label, ImVec2(width, height));

            ImGui::PopStyleVar();
            ImGui::PopStyleColor(4);

            if (!enabled) {
                ImGui::PopStyleVar();
                clicked = false;  // Ensure disabled buttons don't register clicks
            }

            return clicked;
        }

        void ControlPanel::renderThreadStatus() {
            ImVec4 statusColor;
            const char* statusText;

            switch (m_threadState) {
            case ThreadState::Idle:
                statusColor = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);  // Green
                statusText = "Ready";
                break;
            case ThreadState::Running:
                statusColor = ImVec4(0.2f, 0.6f, 1.0f, 1.0f);  // Blue
                statusText = "Sorting...";
                break;
            case ThreadState::Paused:
                statusColor = ImVec4(1.0f, 0.8f, 0.2f, 1.0f);  // Yellow
                statusText = "Paused";
                break;
            case ThreadState::Error:
            case ThreadState::Stopped:
                statusColor = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);  // Red
                statusText = "Stopped";
                break;
            default:
                statusColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);  // Gray
                statusText = "Init...";
                break;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, statusColor);
            ImGui::Text("Status: %s", statusText);
            ImGui::PopStyleColor();
        }

    } // namespace ui
} // namespace quicksort