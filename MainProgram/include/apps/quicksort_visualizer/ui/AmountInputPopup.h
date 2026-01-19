// ============================================================================
// File: include/apps/quicksort_visualizer/ui/AmountInputPopup.h
// ============================================================================

#pragma once

/**
 * @file AmountInputPopup.h
 * @brief Popup dialog for setting the element count.
 *
 * Provides a modal popup with:
 * - Text input for element count
 * - Accept and Cancel buttons
 * - Input validation
 *
 * @note Design Principles:
 *   - SRP: Only handles amount input UI
 *   - OCP: Can be extended for different input types
 *   - DIP: Uses callbacks for communication with parent
 */

#include <functional>
#include <cstdint>
#include <string>

namespace quicksort {
    namespace ui {

        /**
         * @struct AmountInputCallbacks
         * @brief Callbacks for popup actions.
         */
        struct AmountInputCallbacks {
            std::function<void(uint32_t)> onAccept;  ///< Called with new amount when accepted
            std::function<void()> onCancel;          ///< Called when cancelled
        };

        /**
         * @class AmountInputPopup
         * @brief Modal popup for entering element count.
         */
        class AmountInputPopup {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            /**
             * @brief Constructs the popup.
             */
            AmountInputPopup();

            /**
             * @brief Destructor.
             */
            ~AmountInputPopup() = default;

            // ========================================================================
            // Configuration
            // ========================================================================

            /**
             * @brief Sets the callback functions.
             * @param callbacks Struct containing callback functions.
             */
            void setCallbacks(const AmountInputCallbacks& callbacks);

            /**
             * @brief Sets the valid range for element count.
             * @param minValue Minimum allowed value.
             * @param maxValue Maximum allowed value.
             */
            void setRange(uint32_t minValue, uint32_t maxValue);

            // ========================================================================
            // Popup Control
            // ========================================================================

            /**
             * @brief Opens the popup with a default value.
             * @param currentValue Current element count to display.
             */
            void open(uint32_t currentValue);

            /**
             * @brief Closes the popup without accepting.
             */
            void close();

            /**
             * @brief Checks if the popup is currently open.
             * @return true if popup is visible.
             */
            bool isOpen() const;

            // ========================================================================
            // Rendering
            // ========================================================================

            /**
             * @brief Renders the popup if open.
             *
             * Should be called every frame during render.
             */
            void render();

        private:
            // ========================================================================
            // Internal Helpers
            // ========================================================================

            /**
             * @brief Validates the current input.
             * @return true if input is valid.
             */
            bool validateInput();

            /**
             * @brief Accepts the current input and closes.
             */
            void accept();

            /**
             * @brief Cancels and closes the popup.
             */
            void cancel();

            // ========================================================================
            // Data Members
            // ========================================================================

            AmountInputCallbacks m_callbacks;  ///< Callback functions

            bool m_isOpen;                     ///< Whether popup is visible
            bool m_shouldOpen;                 ///< Flag to open on next frame
            bool m_focusInput;                 ///< Flag to focus input field

            char m_inputBuffer[16];            ///< Text input buffer
            uint32_t m_minValue;               ///< Minimum valid value
            uint32_t m_maxValue;               ///< Maximum valid value

            std::string m_errorMessage;        ///< Current error message (empty if valid)

            // Layout constants - INCREASED SIZE
            static constexpr float POPUP_WIDTH = 470.0f;
            static constexpr float POPUP_HEIGHT = 450.0f;
            static constexpr float BUTTON_WIDTH = 120.0f;
            static constexpr float BUTTON_HEIGHT = 40.0f;
        };

    } // namespace ui
} // namespace quicksort