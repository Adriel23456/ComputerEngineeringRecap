// ============================================================================
// File: src/apps/quicksort_visualizer/ui/ControlPanel.cpp
// ============================================================================

/**
 * @file ControlPanel.cpp
 * @brief Implementation of the ControlPanel UI component.
 */

#include "apps/quicksort_visualizer/ui/ControlPanel.h"
#include "imgui.h"

namespace quicksort {
    namespace ui {

        // ============================================================================
        // Construction
        // ============================================================================

        ControlPanel::ControlPanel()
            : m_callbacks{}
            , m_elementCount(50)
            , m_threadState(ThreadState::Idle)
            , m_sortingActive(false)
        {
        }

        // ============================================================================
        // Configuration
        // ============================================================================

        void ControlPanel::setCallbacks(const ControlPanelCallbacks& callbacks) {
            m_callbacks = callbacks;
        }

        void ControlPanel::setElementCount(uint32_t count) {
            m_elementCount = count;
        }

        void ControlPanel::setThreadState(ThreadState state) {
            m_threadState = state;
        }

        void ControlPanel::setSortingActive(bool sorting) {
            m_sortingActive = sorting;
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

            // Style
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.12f, 0.15f, 0.95f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.30f, 0.40f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 15));

            if (ImGui::Begin("##ControlPanel", nullptr, flags)) {
                // Calculate button dimensions
                float availableWidth = size.x - 40.0f;  // Account for padding
                float buttonWidth = (availableWidth - BUTTON_SPACING * 3) / 4.0f;
                float buttonHeight = size.y - STATUS_HEIGHT - 45.0f;

                buttonHeight = std::max(buttonHeight, 40.0f);
                buttonWidth = std::max(buttonWidth, 100.0f);

                renderButtons(buttonWidth, buttonHeight);

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                renderStatus();
            }
            ImGui::End();

            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(2);
        }

        void ControlPanel::renderButtons(float buttonWidth, float buttonHeight) {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));

            // Determine if buttons should be disabled
            bool disableDataButtons = m_sortingActive;
            bool disableStartButton = m_sortingActive;

            // Random Values button
            if (disableDataButtons) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.6f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.5f, 0.7f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.18f, 0.35f, 0.55f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            }

            if (ImGui::Button("Random Values", ImVec2(buttonWidth, buttonHeight))) {
                if (!disableDataButtons && m_callbacks.onRandomValues) {
                    m_callbacks.onRandomValues();
                }
            }
            ImGui::PopStyleColor(4);

            ImGui::SameLine(0, BUTTON_SPACING);

            // Set Amount button
            if (disableDataButtons) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.35f, 0.5f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.45f, 0.6f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.3f, 0.45f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            }

            if (ImGui::Button("Set Amount", ImVec2(buttonWidth, buttonHeight))) {
                if (!disableDataButtons && m_callbacks.onSetAmount) {
                    m_callbacks.onSetAmount();
                }
            }
            ImGui::PopStyleColor(4);

            ImGui::SameLine(0, BUTTON_SPACING);

            // Start Quicksort button
            if (disableStartButton) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.55f, 0.35f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.65f, 0.4f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.18f, 0.5f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            }

            if (ImGui::Button("Start Quicksort", ImVec2(buttonWidth, buttonHeight))) {
                if (!disableStartButton && m_callbacks.onStartQuicksort) {
                    m_callbacks.onStartQuicksort();
                }
            }
            ImGui::PopStyleColor(4);

            ImGui::SameLine(0, BUTTON_SPACING);

            // Reset View button (always enabled)
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.35f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.45f, 0.25f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45f, 0.3f, 0.18f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

            if (ImGui::Button("Reset View", ImVec2(buttonWidth, buttonHeight))) {
                if (m_callbacks.onResetView) {
                    m_callbacks.onResetView();
                }
            }
            ImGui::PopStyleColor(4);

            ImGui::PopStyleVar(2);
        }

        void ControlPanel::renderStatus() {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.75f, 0.8f, 1.0f));

            // Element count
            ImGui::Text("Elements: %u", m_elementCount);

            ImGui::SameLine(200);

            // Thread state with color coding
            ImGui::Text("Status: ");
            ImGui::SameLine();

            ImVec4 stateColor;
            switch (m_threadState) {
            case ThreadState::Idle:
                stateColor = ImVec4(0.5f, 0.8f, 0.5f, 1.0f);  // Green
                break;
            case ThreadState::Running:
                stateColor = ImVec4(1.0f, 0.8f, 0.3f, 1.0f);  // Yellow
                break;
            case ThreadState::Completed:
                stateColor = ImVec4(0.4f, 0.7f, 1.0f, 1.0f);  // Blue
                break;
            case ThreadState::Error:
                stateColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);  // Red
                break;
            default:
                stateColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);  // Gray
                break;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, stateColor);
            ImGui::Text("%s", getThreadStateString().c_str());
            ImGui::PopStyleColor();

            // Show sorting indicator if active
            if (m_sortingActive) {
                ImGui::SameLine(400);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.3f, 1.0f));
                ImGui::Text("[SORTING IN PROGRESS]");
                ImGui::PopStyleColor();
            }

            ImGui::PopStyleColor();
        }

        std::string ControlPanel::getThreadStateString() const {
            switch (m_threadState) {
            case ThreadState::Uninitialized: return "Uninitialized";
            case ThreadState::Idle:          return "Ready";
            case ThreadState::Running:       return "Sorting...";
            case ThreadState::Completed:     return "Completed";
            case ThreadState::ShuttingDown:  return "Shutting Down";
            case ThreadState::Error:         return "Error";
            default:                         return "Unknown";
            }
        }

    } // namespace ui
} // namespace quicksort