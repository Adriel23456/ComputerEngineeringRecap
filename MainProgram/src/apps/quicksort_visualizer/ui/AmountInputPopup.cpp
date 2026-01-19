// ============================================================================
// File: src/apps/quicksort_visualizer/ui/AmountInputPopup.cpp
// ============================================================================

/**
 * @file AmountInputPopup.cpp
 * @brief Implementation of the AmountInputPopup class.
 */

#include "apps/quicksort_visualizer/ui/AmountInputPopup.h"
#include "apps/quicksort_visualizer/visualization/GridConfig.h"
#include "imgui.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace quicksort {
    namespace ui {

        // ============================================================================
        // Construction
        // ============================================================================

        AmountInputPopup::AmountInputPopup()
            : m_callbacks{}
            , m_isOpen(false)
            , m_shouldOpen(false)
            , m_focusInput(false)
            , m_inputBuffer{}
            , m_minValue(config::GridConfig::MIN_ELEMENT_COUNT)
            , m_maxValue(config::GridConfig::MAX_ELEMENT_COUNT)
            , m_errorMessage()
        {
            std::memset(m_inputBuffer, 0, sizeof(m_inputBuffer));
        }

        // ============================================================================
        // Configuration
        // ============================================================================

        void AmountInputPopup::setCallbacks(const AmountInputCallbacks& callbacks) {
            m_callbacks = callbacks;
        }

        void AmountInputPopup::setRange(uint32_t minValue, uint32_t maxValue) {
            m_minValue = minValue;
            m_maxValue = maxValue;
        }

        // ============================================================================
        // Popup Control
        // ============================================================================

        void AmountInputPopup::open(uint32_t currentValue) {
            // Format current value into buffer
            snprintf(m_inputBuffer, sizeof(m_inputBuffer), "%u", currentValue);
            m_errorMessage.clear();
            m_shouldOpen = true;
            m_focusInput = true;
        }

        void AmountInputPopup::close() {
            m_isOpen = false;
            m_shouldOpen = false;
        }

        bool AmountInputPopup::isOpen() const {
            return m_isOpen;
        }

        // ============================================================================
        // Rendering
        // ============================================================================

        void AmountInputPopup::render() {
            // Handle deferred open
            if (m_shouldOpen) {
                ImGui::OpenPopup("Set Element Count");
                m_shouldOpen = false;
                m_isOpen = true;
            }

            // Center the popup
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(POPUP_WIDTH, POPUP_HEIGHT));

            // Popup style
            ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.12f, 0.14f, 0.18f, 0.98f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.35f, 0.45f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.15f, 0.18f, 0.22f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.18f, 0.22f, 0.28f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(25.0f, 20.0f));

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoScrollbar;

            if (ImGui::BeginPopupModal("Set Element Count", nullptr, flags)) {
                // Title
                ImGui::PushFont(nullptr);  // Use default font
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.92f, 0.95f, 1.0f));

                // Center the title
                const char* title = "Enter Element Count";
                float titleWidth = ImGui::CalcTextSize(title).x;
                ImGui::SetCursorPosX((POPUP_WIDTH - 50.0f - titleWidth) * 0.5f);
                ImGui::Text("%s", title);
                ImGui::PopStyleColor();
                ImGui::PopFont();

                ImGui::Spacing();
                ImGui::Spacing();

                // Description text
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.75f, 0.8f, 1.0f));
                ImGui::TextWrapped("Specify how many elements to generate for the quicksort visualization:");
                ImGui::PopStyleColor();

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();

                // Input field
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.09f, 0.12f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.12f, 0.14f, 0.18f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.15f, 0.18f, 0.22f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.25f, 0.35f, 0.5f, 0.8f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15.0f, 12.0f));

                // Label
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.65f, 0.7f, 1.0f));
                ImGui::Text("Number of elements:");
                ImGui::PopStyleColor();

                ImGui::Spacing();

                ImGui::SetNextItemWidth(-1);  // Full width

                // Focus input on first frame
                if (m_focusInput) {
                    ImGui::SetKeyboardFocusHere();
                    m_focusInput = false;
                }

                // Input with Enter key handling
                ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_CharsDecimal |
                    ImGuiInputTextFlags_EnterReturnsTrue;

                if (ImGui::InputText("##AmountInput", m_inputBuffer, sizeof(m_inputBuffer), inputFlags)) {
                    // Enter pressed
                    if (validateInput()) {
                        accept();
                    }
                }

                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(4);

                ImGui::Spacing();

                // Range hint
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.55f, 0.6f, 1.0f));
                ImGui::Text("Valid range: %u - %u elements", m_minValue, m_maxValue);
                ImGui::PopStyleColor();

                // Error message
                if (!m_errorMessage.empty()) {
                    ImGui::Spacing();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                    ImGui::TextWrapped("%s", m_errorMessage.c_str());
                    ImGui::PopStyleColor();
                }

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();

                // Buttons - centered
                float buttonAreaWidth = BUTTON_WIDTH * 2 + 30.0f;  // Two buttons + spacing
                float startX = (POPUP_WIDTH - 50.0f - buttonAreaWidth) * 0.5f;
                ImGui::SetCursorPosX(startX);

                // Button styles
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

                // Accept button (green)
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.55f, 0.35f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.65f, 0.4f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.18f, 0.5f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

                if (ImGui::Button("Accept", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT))) {
                    if (validateInput()) {
                        accept();
                    }
                }

                ImGui::PopStyleColor(4);

                ImGui::SameLine(0, 30.0f);

                // Cancel button (red)
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.2f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65f, 0.25f, 0.25f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.18f, 0.18f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

                if (ImGui::Button("Cancel", ImVec2(BUTTON_WIDTH, BUTTON_HEIGHT))) {
                    cancel();
                }

                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar(2);

                // Handle Escape key
                if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                    cancel();
                }

                ImGui::EndPopup();
            }
            else {
                // Popup was closed externally
                m_isOpen = false;
            }

            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(4);
        }

        // ============================================================================
        // Internal Helpers
        // ============================================================================

        bool AmountInputPopup::validateInput() {
            // Check for empty input
            if (m_inputBuffer[0] == '\0') {
                m_errorMessage = "Please enter a value.";
                return false;
            }

            // Parse the input
            char* endPtr = nullptr;
            long value = std::strtol(m_inputBuffer, &endPtr, 10);

            // Check for invalid characters
            if (*endPtr != '\0') {
                m_errorMessage = "Invalid number format. Please enter digits only.";
                return false;
            }

            // Check for negative values
            if (value < 0) {
                m_errorMessage = "Value must be positive.";
                return false;
            }

            // Check range
            if (value < static_cast<long>(m_minValue)) {
                char buf[64];
                snprintf(buf, sizeof(buf), "Value must be at least %u.", m_minValue);
                m_errorMessage = buf;
                return false;
            }

            if (value > static_cast<long>(m_maxValue)) {
                char buf[64];
                snprintf(buf, sizeof(buf), "Value cannot exceed %u.", m_maxValue);
                m_errorMessage = buf;
                return false;
            }

            m_errorMessage.clear();
            return true;
        }

        void AmountInputPopup::accept() {
            uint32_t value = static_cast<uint32_t>(std::strtoul(m_inputBuffer, nullptr, 10));

            ImGui::CloseCurrentPopup();
            m_isOpen = false;

            if (m_callbacks.onAccept) {
                m_callbacks.onAccept(value);
            }
        }

        void AmountInputPopup::cancel() {
            ImGui::CloseCurrentPopup();
            m_isOpen = false;

            if (m_callbacks.onCancel) {
                m_callbacks.onCancel();
            }
        }

    } // namespace ui
} // namespace quicksort