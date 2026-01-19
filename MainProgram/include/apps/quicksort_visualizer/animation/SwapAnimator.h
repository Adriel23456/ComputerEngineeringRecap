// ============================================================================
// File: include/apps/quicksort_visualizer/animation/SwapAnimator.h
// ============================================================================

#pragma once

/**
 * @file SwapAnimator.h
 * @brief Manages swap animation state machine with verification sweep.
 */

#include "apps/quicksort_visualizer/data/SwapOperation.h"
#include <cstdint>
#include <functional>

namespace quicksort {
    namespace animation {

        /**
         * @enum AnimationPhase
         * @brief Current phase of the animation.
         */
        enum class AnimationPhase {
            Idle,
            FadeOut,
            FadeIn,
            Verification    ///< Final sweep verification phase
        };

        /**
         * @struct AnimationState
         * @brief Current state of animated elements.
         */
        struct AnimationState {
            uint32_t indexA;
            uint32_t indexB;
            float alpha;
            AnimationPhase phase;
            uint32_t verificationIndex;  ///< Current index during verification sweep

            AnimationState()
                : indexA(0)
                , indexB(0)
                , alpha(1.0f)
                , phase(AnimationPhase::Idle)
                , verificationIndex(0)
            {
            }
        };

        /**
         * @class SwapAnimator
         * @brief Manages the animation of swap operations with dynamic speed.
         */
        class SwapAnimator {
        public:
            // ========================================================================
            // Types
            // ========================================================================

            using SwapCallback = std::function<void(uint32_t, uint32_t)>;
            using SoundCallback = std::function<void(const data::SwapOperation&)>;
            using VerificationSoundCallback = std::function<void(uint32_t, uint32_t)>;
            using VerificationCompleteCallback = std::function<void()>;

            // ========================================================================
            // Construction
            // ========================================================================

            SwapAnimator();
            ~SwapAnimator() = default;

            // ========================================================================
            // Configuration
            // ========================================================================

            void setSwapCallback(SwapCallback callback);
            void setSoundCallback(SoundCallback callback);
            void setVerificationSoundCallback(VerificationSoundCallback callback);
            void setVerificationCompleteCallback(VerificationCompleteCallback callback);

            /**
             * @brief Sets the base animation speed.
             * @param speed Base speed multiplier.
             */
            void setBaseSpeed(float speed);

            /**
             * @brief Sets element count for dynamic speed calculation.
             * @param count Number of elements.
             */
            void setElementCount(uint32_t count);

            // ========================================================================
            // Animation Control
            // ========================================================================

            bool startSwap(const data::SwapOperation& swap);

            /**
             * @brief Starts the verification sweep animation.
             * @param totalElements Total number of elements to verify.
             */
            void startVerification(uint32_t totalElements);

            void update(float deltaTime);
            bool isAnimating() const;
            bool isVerifying() const;
            const AnimationState& getState() const;
            void reset();

        private:
            // ========================================================================
            // Internal Methods
            // ========================================================================

            void transitionToNextPhase();
            void updateVerification(float deltaTime);
            float calculateDynamicSpeed() const;

            // ========================================================================
            // Data Members
            // ========================================================================

            AnimationState m_state;
            data::SwapOperation m_currentSwap;
            float m_phaseTimer;
            float m_baseSpeed;
            uint32_t m_elementCount;
            uint32_t m_totalElements;      ///< Total elements for verification
            float m_verificationTimer;

            SwapCallback m_swapCallback;
            SoundCallback m_soundCallback;
            VerificationSoundCallback m_verificationSoundCallback;
            VerificationCompleteCallback m_verificationCompleteCallback;

            // Animation timing
            static constexpr float BASE_FADE_DURATION = 0.06f;
            static constexpr float BASE_VERIFICATION_INTERVAL = 0.008f;  ///< Very fast sweep
        };

    } // namespace animation
} // namespace quicksort