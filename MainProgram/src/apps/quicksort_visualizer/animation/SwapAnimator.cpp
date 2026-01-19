// ============================================================================
// File: src/apps/quicksort_visualizer/animation/SwapAnimator.cpp
// ============================================================================

/**
 * @file SwapAnimator.cpp
 * @brief Implementation of the SwapAnimator class.
 */

#include "apps/quicksort_visualizer/animation/SwapAnimator.h"
#include <algorithm>
#include <cmath>

namespace quicksort {
    namespace animation {

        // ============================================================================
        // Speed Lookup Table
        // ============================================================================

        namespace {

            /**
             * @brief Speed control point for interpolation.
             */
            struct SpeedPoint {
                uint32_t elements;
                float speed;
            };

            /**
             * @brief Speed curve definition - maps element count to speed multiplier.
             */
            constexpr SpeedPoint SPEED_CURVE[] = {
                {   15,     1.0f },
                {   20,     1.5f },
                {   30,     1.8f },
                {   40,     2.0f },
                {   60,     2.5f },
                {   80,     3.0f },
                {  100,     4.0f },
                {  150,     5.0f },
                {  200,     8.0f },
                {  250,    15.0f },
                {  300,    25.0f },
                {  400,    50.0f },
                {  500,   150.0f },
                {  600,   400.0f },
                {  700,   700.0f },
                {  800,  1000.0f },
                {  900,  2000.0f },
                { 1000,  5000.0f },   // Essentially instant
            };

            constexpr size_t SPEED_CURVE_SIZE = sizeof(SPEED_CURVE) / sizeof(SPEED_CURVE[0]);

            /**
             * @brief Linearly interpolates between two values.
             */
            float lerp(float a, float b, float t) {
                return a + t * (b - a);
            }

            /**
             * @brief Calculates speed multiplier from the curve using linear interpolation.
             */
            float getSpeedFromCurve(uint32_t elementCount) {
                // Below minimum - use minimum speed
                if (elementCount <= SPEED_CURVE[0].elements) {
                    return SPEED_CURVE[0].speed;
                }

                // Above maximum - use maximum speed (instant)
                if (elementCount >= SPEED_CURVE[SPEED_CURVE_SIZE - 1].elements) {
                    return SPEED_CURVE[SPEED_CURVE_SIZE - 1].speed;
                }

                // Find the two points to interpolate between
                for (size_t i = 0; i < SPEED_CURVE_SIZE - 1; ++i) {
                    if (elementCount >= SPEED_CURVE[i].elements &&
                        elementCount < SPEED_CURVE[i + 1].elements) {

                        // Calculate interpolation factor
                        float range = static_cast<float>(SPEED_CURVE[i + 1].elements - SPEED_CURVE[i].elements);
                        float position = static_cast<float>(elementCount - SPEED_CURVE[i].elements);
                        float t = position / range;

                        // Interpolate speed
                        return lerp(SPEED_CURVE[i].speed, SPEED_CURVE[i + 1].speed, t);
                    }
                }

                // Fallback (shouldn't reach here)
                return 1.0f;
            }

        } // anonymous namespace

        // ============================================================================
        // Construction
        // ============================================================================

        SwapAnimator::SwapAnimator()
            : m_state()
            , m_currentSwap()
            , m_phaseTimer(0.0f)
            , m_baseSpeed(1.0f)
            , m_elementCount(50)
            , m_totalElements(0)
            , m_verificationTimer(0.0f)
            , m_swapCallback(nullptr)
            , m_soundCallback(nullptr)
            , m_verificationSoundCallback(nullptr)
            , m_verificationCompleteCallback(nullptr)
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

        void SwapAnimator::setVerificationSoundCallback(VerificationSoundCallback callback) {
            m_verificationSoundCallback = std::move(callback);
        }

        void SwapAnimator::setVerificationCompleteCallback(VerificationCompleteCallback callback) {
            m_verificationCompleteCallback = std::move(callback);
        }

        void SwapAnimator::setBaseSpeed(float speed) {
            m_baseSpeed = std::max(0.1f, speed);
        }

        void SwapAnimator::setElementCount(uint32_t count) {
            m_elementCount = std::max(1u, count);
        }

        // ============================================================================
        // Animation Control
        // ============================================================================

        bool SwapAnimator::startSwap(const data::SwapOperation& swap) {
            if (isAnimating()) {
                return false;
            }

            m_currentSwap = swap;

            m_state.indexA = swap.indexA;
            m_state.indexB = swap.indexB;
            m_state.alpha = 1.0f;
            m_state.phase = AnimationPhase::FadeOut;
            m_phaseTimer = 0.0f;

            if (m_soundCallback) {
                m_soundCallback(swap);
            }

            return true;
        }

        void SwapAnimator::startVerification(uint32_t totalElements) {
            m_state.phase = AnimationPhase::Verification;
            m_state.verificationIndex = 0;
            m_state.alpha = 1.0f;
            m_totalElements = totalElements;
            m_verificationTimer = 0.0f;

            // Play first sound
            if (m_verificationSoundCallback && totalElements > 0) {
                m_verificationSoundCallback(0, totalElements);
            }
        }

        void SwapAnimator::update(float deltaTime) {
            if (m_state.phase == AnimationPhase::Verification) {
                updateVerification(deltaTime);
                return;
            }

            if (m_state.phase == AnimationPhase::Idle) {
                return;
            }

            float dynamicSpeed = calculateDynamicSpeed();
            float adjustedDelta = deltaTime * dynamicSpeed;
            m_phaseTimer += adjustedDelta;

            float phaseDuration = BASE_FADE_DURATION;
            float progress = std::min(m_phaseTimer / phaseDuration, 1.0f);

            switch (m_state.phase) {
            case AnimationPhase::FadeOut:
                m_state.alpha = 1.0f - progress;
                if (progress >= 1.0f) {
                    transitionToNextPhase();
                }
                break;

            case AnimationPhase::FadeIn:
                m_state.alpha = progress;
                if (progress >= 1.0f) {
                    transitionToNextPhase();
                }
                break;

            default:
                break;
            }
        }

        void SwapAnimator::updateVerification(float deltaTime) {
            // Verification also uses dynamic speed, but even faster
            float speedMultiplier = calculateDynamicSpeed() * 2.0f;
            m_verificationTimer += deltaTime * speedMultiplier;

            float interval = BASE_VERIFICATION_INTERVAL;

            if (m_verificationTimer >= interval) {
                m_verificationTimer = 0.0f;

                // Move to next element
                m_state.verificationIndex++;

                if (m_state.verificationIndex >= m_totalElements) {
                    // Verification complete
                    m_state.phase = AnimationPhase::Idle;
                    m_state.verificationIndex = 0;

                    if (m_verificationCompleteCallback) {
                        m_verificationCompleteCallback();
                    }
                }
                else {
                    // Play sound for current element
                    if (m_verificationSoundCallback) {
                        m_verificationSoundCallback(m_state.verificationIndex, m_totalElements);
                    }
                }
            }
        }

        bool SwapAnimator::isAnimating() const {
            return m_state.phase == AnimationPhase::FadeOut ||
                m_state.phase == AnimationPhase::FadeIn;
        }

        bool SwapAnimator::isVerifying() const {
            return m_state.phase == AnimationPhase::Verification;
        }

        const AnimationState& SwapAnimator::getState() const {
            return m_state;
        }

        void SwapAnimator::reset() {
            m_state = AnimationState();
            m_currentSwap = data::SwapOperation();
            m_phaseTimer = 0.0f;
            m_verificationTimer = 0.0f;
            m_totalElements = 0;
        }

        // ============================================================================
        // Internal Methods
        // ============================================================================

        void SwapAnimator::transitionToNextPhase() {
            m_phaseTimer = 0.0f;

            switch (m_state.phase) {
            case AnimationPhase::FadeOut:
                if (m_swapCallback) {
                    m_swapCallback(m_currentSwap.indexA, m_currentSwap.indexB);
                }
                m_state.phase = AnimationPhase::FadeIn;
                m_state.alpha = 0.0f;
                break;

            case AnimationPhase::FadeIn:
                m_state.phase = AnimationPhase::Idle;
                m_state.alpha = 1.0f;
                break;

            default:
                break;
            }
        }

        float SwapAnimator::calculateDynamicSpeed() const {
            return m_baseSpeed * getSpeedFromCurve(m_elementCount);
        }

    } // namespace animation
} // namespace quicksort