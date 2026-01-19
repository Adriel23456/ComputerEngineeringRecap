// ============================================================================
// File: include/apps/quicksort_visualizer/ui/ControlPanel.h
// ============================================================================

#pragma once

/**
 * @file ControlPanel.h
 * @brief UI component for control buttons and status display.
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
            std::function<void()> onRandomValues;
            std::function<void()> onSetAmount;
            std::function<void()> onStartQuicksort;
            std::function<void()> onResetView;
        };

        /**
         * @class ControlPanel
         * @brief Renders and handles control panel UI.
         */
        class ControlPanel {
        public:
            ControlPanel();
            ~ControlPanel() = default;

            void setCallbacks(const ControlPanelCallbacks& callbacks);
            void setElementCount(uint32_t count);
            void setThreadState(ThreadState state);
            void setSortingActive(bool sorting);
            void render(const ImVec2& position, const ImVec2& size);

        private:
            void renderButtons(float buttonWidth, float buttonHeight);
            void renderStatus(float availableWidth);
            std::string getThreadStateString() const;

            ControlPanelCallbacks m_callbacks;
            uint32_t m_elementCount;
            ThreadState m_threadState;
            bool m_sortingActive;

            static constexpr float BUTTON_SPACING = 15.0f;
            static constexpr float STATUS_HEIGHT = 30.0f;
        };

    } // namespace ui
} // namespace quicksort