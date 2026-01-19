// ============================================================================
// File: include/apps/quicksort_visualizer/ui/ControlPanel.h
// ============================================================================

#pragma once

/**
 * @file ControlPanel.h
 * @brief UI component for control buttons and status display.
 *
 * Provides buttons for:
 * - Random Values generation
 * - Set Amount (element count)
 * - Start Quicksort
 * - Reset View
 *
 * @note Design Principles:
 *   - SRP: Only handles control panel UI
 *   - OCP: New buttons can be added without modifying existing code
 *   - DIP: Uses callbacks for actions (no direct dependencies)
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
         */
        struct ControlPanelCallbacks {
            std::function<void()> onRandomValues;    ///< Generate random values
            std::function<void()> onSetAmount;       ///< Open amount dialog
            std::function<void()> onStartQuicksort;  ///< Start sorting
            std::function<void()> onResetView;       ///< Reset view
        };

        /**
         * @class ControlPanel
         * @brief Renders and handles control panel UI.
         */
        class ControlPanel {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            ControlPanel();
            ~ControlPanel() = default;

            // ========================================================================
            // Configuration
            // ========================================================================

            /**
             * @brief Sets callback functions for button actions.
             * @param callbacks Struct containing all callbacks.
             */
            void setCallbacks(const ControlPanelCallbacks& callbacks);

            /**
             * @brief Updates the displayed element count.
             * @param count Current element count.
             */
            void setElementCount(uint32_t count);

            /**
             * @brief Updates the thread state display.
             * @param state Current thread state.
             */
            void setThreadState(ThreadState state);

            /**
             * @brief Sets whether sorting is in progress (disables most buttons).
             * @param sorting Whether sorting is active.
             */
            void setSortingActive(bool sorting);

            // ========================================================================
            // Rendering
            // ========================================================================

            /**
             * @brief Renders the control panel.
             * @param position Top-left position.
             * @param size Panel size.
             */
            void render(const ImVec2& position, const ImVec2& size);

        private:
            // ========================================================================
            // Internal Rendering
            // ========================================================================

            void renderButtons(float buttonWidth, float buttonHeight);
            void renderStatus();
            std::string getThreadStateString() const;

            // ========================================================================
            // Data Members
            // ========================================================================

            ControlPanelCallbacks m_callbacks;
            uint32_t m_elementCount;
            ThreadState m_threadState;
            bool m_sortingActive;

            // Layout constants
            static constexpr float BUTTON_SPACING = 15.0f;
            static constexpr float STATUS_HEIGHT = 30.0f;
        };

    } // namespace ui
} // namespace quicksort