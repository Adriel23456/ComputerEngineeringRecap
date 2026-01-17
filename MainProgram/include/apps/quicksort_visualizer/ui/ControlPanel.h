// ============================================================================
// File: include/apps/quicksort_visualizer/ui/ControlPanel.h
// ============================================================================

#pragma once

/**
 * @file ControlPanel.h
 * @brief UI component for the control button panel.
 *
 * Provides buttons for user interaction with the visualizer:
 * - Random Values: Generate random data
 * - Set Amount: Configure element count
 * - Start Quicksort: Begin sorting
 *
 * @note Design Principles:
 *   - SRP: Only handles control panel rendering and button callbacks
 *   - OCP: New buttons can be added without modifying existing code
 *   - DIP: Uses callbacks instead of direct dependencies
 */

#include "apps/quicksort_visualizer/threading/LogicThreadController.h"
#include <functional>
#include <string>
#include <cstdint>

 // Forward declarations
struct ImVec2;

namespace quicksort {
    namespace ui {

        /**
         * @struct ControlPanelCallbacks
         * @brief Callbacks for control panel button actions.
         *
         * Separates the button actions from the panel implementation,
         * allowing the state class to handle the actual logic.
         */
        struct ControlPanelCallbacks {
            std::function<void()> onRandomValues;   ///< Called when "Random Values" clicked
            std::function<void()> onSetAmount;      ///< Called when "Set Amount" clicked
            std::function<void()> onStartQuicksort; ///< Called when "Start Quicksort" clicked
            std::function<void()> onResetView;      ///< Called when "Reset View" clicked
        };

        /**
         * @class ControlPanel
         * @brief Renders the control panel with action buttons.
         *
         * Layout: Horizontal row of evenly-spaced buttons
         * Height: 15% of total window height (configured externally)
         */
        class ControlPanel {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            /**
             * @brief Constructs the control panel.
             */
            ControlPanel();

            /**
             * @brief Destructor.
             */
            ~ControlPanel() = default;

            // ========================================================================
            // Configuration
            // ========================================================================

            /**
             * @brief Sets the callback functions for button actions.
             * @param callbacks Struct containing all callback functions.
             */
            void setCallbacks(const ControlPanelCallbacks& callbacks);

            /**
             * @brief Updates the thread state for UI feedback.
             * @param state Current state of the logic thread.
             */
            void setThreadState(ThreadState state);

            /**
             * @brief Sets the current element count for display.
             * @param count Number of elements.
             */
            void setElementCount(uint32_t count);

            // ========================================================================
            // Rendering
            // ========================================================================

            /**
             * @brief Renders the control panel.
             * @param position Top-left position of the panel.
             * @param size Size of the panel.
             */
            void render(const ImVec2& position, const ImVec2& size);

        private:
            // ========================================================================
            // Rendering Helpers
            // ========================================================================

            /**
             * @brief Creates a styled control button.
             * @param label Button text.
             * @param width Button width.
             * @param height Button height.
             * @param enabled Whether button is clickable.
             * @return true if button was clicked.
             */
            bool createButton(const char* label, float width, float height, bool enabled = true);

            /**
             * @brief Renders thread status indicator.
             */
            void renderThreadStatus();

            // ========================================================================
            // Data Members
            // ========================================================================

            ControlPanelCallbacks m_callbacks;  ///< Button action callbacks
            ThreadState m_threadState;          ///< Current thread state
            uint32_t m_elementCount;            ///< Current element count

            // Layout constants
            static constexpr float BUTTON_SPACING = 20.0f;
            static constexpr float PADDING = 10.0f;
        };

    } // namespace ui
} // namespace quicksort