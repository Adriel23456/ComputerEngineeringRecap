// ============================================================================
// File: src/apps/quicksort_visualizer/animation/SwapAnimator.cpp
// ============================================================================

/**
 * @file SwapAnimator.cpp
 * @brief Implementation of the SwapAnimator class.
 */

#include "apps/quicksort_visualizer/animation/SwapAnimator.h"
#include <algorithm>

namespace quicksort {
    namespace animation {

        // ============================================================================
        // Construction
        // ============================================================================

        SwapAnimator::SwapAnimator()
            : m_state()
            , m_currentSwap()
            , m_phaseTimer(0.0f)
            , m_speed(1.0f)
            , m_swapCallback(nullptr)
            , m_soundCallback(nullptr)
        {
        }

        // ============================================================================
        // Configuration
        // ============================================================================

        void SwapAnimator::setSwapCallback(SwapCallback callback) {
            m_swapCallback = std::move(callback);
        }

        void SwapAnimator::setSoundCallback(SoundCallback callback) {
            m_soundCallback = std::move(callback);
        }

        void SwapAnimator::setSpeed(float speed) {
            m_speed = std::max(0.1f, speed);
        }

        // ============================================================================
        // Animation Control
        // ============================================================================

        bool SwapAnimator::startSwap(const data::SwapOperation& swap) {
            // Can't start if already animating
            if (isAnimating()) {
                return false;
            }

            // Store swap data
            m_currentSwap = swap;

            // Initialize state
            m_state.indexA = swap.indexA;
            m_state.indexB = swap.indexB;
            m_state.alpha = 1.0f;
            m_state.phase = AnimationPhase::FadeOut;
            m_phaseTimer = 0.0f;

            // Play sound at start of animation
            if (m_soundCallback) {
                m_soundCallback(swap);
            }

            return true;
        }

        void SwapAnimator::update(float deltaTime) {
            if (!isAnimating()) {
                return;
            }

            // Apply speed multiplier
            float adjustedDelta = deltaTime * m_speed;
            m_phaseTimer += adjustedDelta;

            float phaseDuration = FADE_DURATION / m_speed;
            float progress = std::min(m_phaseTimer / phaseDuration, 1.0f);

            switch (m_state.phase) {
            case AnimationPhase::FadeOut:
                // Fade from 1.0 to 0.0
                m_state.alpha = 1.0f - progress;

                if (progress >= 1.0f) {
                    transitionToNextPhase();
                }
                break;

            case AnimationPhase::FadeIn:
                // Fade from 0.0 to 1.0
                m_state.alpha = progress;

                if (progress >= 1.0f) {
                    transitionToNextPhase();
                }
                break;

            case AnimationPhase::Idle:
            default:
                break;
            }
        }

        bool SwapAnimator::isAnimating() const {
            return m_state.phase != AnimationPhase::Idle;
        }

        const AnimationState& SwapAnimator::getState() const {
            return m_state;
        }

        void SwapAnimator::reset() {
            m_state = AnimationState();
            m_currentSwap = data::SwapOperation();
            m_phaseTimer = 0.0f;
        }

        // ============================================================================
        // Internal Methods
        // ============================================================================

        void SwapAnimator::transitionToNextPhase() {
            m_phaseTimer = 0.0f;

            switch (m_state.phase) {
            case AnimationPhase::FadeOut:
                // Apply the actual swap to data
                if (m_swapCallback) {
                    m_swapCallback(m_currentSwap.indexA, m_currentSwap.indexB);
                }

                // Transition to fade in
                m_state.phase = AnimationPhase::FadeIn;
                m_state.alpha = 0.0f;
                break;

            case AnimationPhase::FadeIn:
                // Animation complete
                m_state.phase = AnimationPhase::Idle;
                m_state.alpha = 1.0f;
                break;

            case AnimationPhase::Idle:
            default:
                break;
            }
        }

    } // namespace animation
} // namespace quicksort