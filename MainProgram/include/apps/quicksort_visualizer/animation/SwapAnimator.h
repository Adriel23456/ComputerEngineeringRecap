// ============================================================================
// File: include/apps/quicksort_visualizer/animation/SwapAnimator.h
// ============================================================================

#pragma once

/**
 * @file SwapAnimator.h
 * @brief Manages swap animation state machine.
 *
 * Handles the visual animation of element swaps:
 * - Fade out phase
 * - Swap (instant)
 * - Fade in phase
 *
 * @note Design Principles:
 *   - SRP: Only handles animation state
 *   - State machine pattern for clear transitions
 */

#include "apps/quicksort_visualizer/data/SwapOperation.h"
#include <cstdint>
#include <functional>

namespace quicksort {
    namespace animation {

        /**
         * @enum AnimationPhase
         * @brief Current phase of the swap animation.
         */
        enum class AnimationPhase {
            Idle,       ///< No animation in progress
            FadeOut,    ///< Elements fading out
            FadeIn      ///< Elements fading in (after swap)
        };

        /**
         * @struct AnimationState
         * @brief Current state of animated elements.
         */
        struct AnimationState {
            uint32_t indexA;     ///< First element index
            uint32_t indexB;     ///< Second element index
            float alpha;         ///< Current alpha (0.0 - 1.0)
            AnimationPhase phase; ///< Current phase

            AnimationState()
                : indexA(0)
                , indexB(0)
                , alpha(1.0f)
                , phase(AnimationPhase::Idle)
            {
            }
        };

        /**
         * @class SwapAnimator
         * @brief Manages the animation of swap operations.
         */
        class SwapAnimator {
        public:
            // ========================================================================
            // Types
            // ========================================================================

            using SwapCallback = std::function<void(uint32_t, uint32_t)>;
            using SoundCallback = std::function<void(const data::SwapOperation&)>;

            // ========================================================================
            // Construction
            // ========================================================================

            /**
             * @brief Constructs the swap animator.
             */
            SwapAnimator();

            /**
             * @brief Destructor.
             */
            ~SwapAnimator() = default;

            // ========================================================================
            // Configuration
            // ========================================================================

            /**
             * @brief Sets the callback for when swap should be applied to data.
             * @param callback Function to call with indices to swap.
             */
            void setSwapCallback(SwapCallback callback);

            /**
             * @brief Sets the callback for playing swap sound.
             * @param callback Function to call with swap operation.
             */
            void setSoundCallback(SoundCallback callback);

            /**
             * @brief Sets animation speed multiplier.
             * @param speed Multiplier (1.0 = normal, 2.0 = twice as fast).
             */
            void setSpeed(float speed);

            // ========================================================================
            // Animation Control
            // ========================================================================

            /**
             * @brief Starts a new swap animation.
             * @param swap The swap operation to animate.
             * @return true if animation started, false if already animating.
             */
            bool startSwap(const data::SwapOperation& swap);

            /**
             * @brief Updates the animation state.
             * @param deltaTime Time since last update in seconds.
             */
            void update(float deltaTime);

            /**
             * @brief Checks if an animation is currently in progress.
             * @return true if animating.
             */
            bool isAnimating() const;

            /**
             * @brief Gets the current animation state.
             * @return Current state for rendering.
             */
            const AnimationState& getState() const;

            /**
             * @brief Resets the animator to idle state.
             */
            void reset();

        private:
            // ========================================================================
            // Internal Methods
            // ========================================================================

            /**
             * @brief Transitions to the next animation phase.
             */
            void transitionToNextPhase();

            // ========================================================================
            // Data Members
            // ========================================================================

            AnimationState m_state;            ///< Current animation state
            data::SwapOperation m_currentSwap; ///< Current swap being animated
            float m_phaseTimer;                ///< Timer for current phase
            float m_speed;                     ///< Animation speed multiplier

            SwapCallback m_swapCallback;       ///< Callback to apply swap
            SoundCallback m_soundCallback;     ///< Callback to play sound

            // Animation timing constants
            static constexpr float FADE_DURATION = 0.08f;  ///< Duration of fade (fast)
        };

    } // namespace animation
} // namespace quicksort