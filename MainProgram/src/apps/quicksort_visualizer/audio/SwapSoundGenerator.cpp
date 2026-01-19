// ============================================================================
// File: src/apps/quicksort_visualizer/audio/SwapSoundGenerator.cpp
// ============================================================================

/**
 * @file SwapSoundGenerator.cpp
 * @brief Implementation of the SwapSoundGenerator class.
 */

#include "apps/quicksort_visualizer/audio/SwapSoundGenerator.h"
#include <cmath>
#include <algorithm>

namespace quicksort {
    namespace audio {

        // ============================================================================
        // Construction
        // ============================================================================

        SwapSoundGenerator::SwapSoundGenerator()
            : m_soundBuffer()
            , m_sound()
            , m_volume(0.5f)
            , m_enabled(true)
            , m_elementCount(50)
            , m_initialized(false)
        {
        }

        // ============================================================================
        // Configuration
        // ============================================================================

        void SwapSoundGenerator::setElementCount(uint32_t count) {
            m_elementCount = std::max(1u, count);
        }

        void SwapSoundGenerator::setVolume(float volume) {
            m_volume = std::clamp(volume, 0.0f, 1.0f);
            m_sound.setVolume(m_volume * 100.0f);
        }

        void SwapSoundGenerator::setEnabled(bool enabled) {
            m_enabled = enabled;
            if (!enabled) {
                stop();
            }
        }

        // ============================================================================
        // Sound Playback
        // ============================================================================

        void SwapSoundGenerator::playSwapSound(const data::SwapOperation& swap) {
            if (!m_enabled || !m_initialized) {
                return;
            }

            // Calculate frequency based on swap progress
            float frequency = calculateFrequency(swap);

            // Generate the tone
            std::vector<sf::Int16> samples;
            generateTone(frequency, TONE_DURATION, samples);

            // Apply envelope to prevent clicking
            size_t attackSamples = static_cast<size_t>(SAMPLE_RATE * 0.005f);  // 5ms attack
            size_t releaseSamples = static_cast<size_t>(SAMPLE_RATE * 0.01f); // 10ms release
            applyEnvelope(samples, attackSamples, releaseSamples);

            // Load into buffer and play
            if (m_soundBuffer.loadFromSamples(samples.data(), samples.size(), 1, SAMPLE_RATE)) {
                m_sound.setBuffer(m_soundBuffer);
                m_sound.setVolume(m_volume * 100.0f);
                m_sound.play();
            }
        }

        void SwapSoundGenerator::stop() {
            m_sound.stop();
        }

        // ============================================================================
        // Initialization
        // ============================================================================

        bool SwapSoundGenerator::initialize() {
            // Generate a test tone to verify audio works
            std::vector<sf::Int16> testSamples;
            generateTone(440.0f, 0.01f, testSamples);

            m_initialized = m_soundBuffer.loadFromSamples(
                testSamples.data(),
                testSamples.size(),
                1,
                SAMPLE_RATE
            );

            return m_initialized;
        }

        // ============================================================================
        // Internal Methods
        // ============================================================================

        void SwapSoundGenerator::generateTone(float frequency, float duration,
            std::vector<sf::Int16>& samples) {
            size_t sampleCount = static_cast<size_t>(SAMPLE_RATE * duration);
            samples.resize(sampleCount);

            const float amplitude = 0.3f;  // Keep volume moderate for comfort
            const float twoPi = 2.0f * 3.14159265358979f;

            for (size_t i = 0; i < sampleCount; ++i) {
                float t = static_cast<float>(i) / static_cast<float>(SAMPLE_RATE);

                // Generate sine wave
                float sineValue = std::sin(twoPi * frequency * t);

                // Add a softer harmonic for a more pleasant sound
                float harmonic = std::sin(twoPi * frequency * 2.0f * t) * 0.15f;

                // Combine and scale to 16-bit range
                float combined = (sineValue + harmonic) * amplitude;
                samples[i] = static_cast<sf::Int16>(combined * 32767.0f);
            }
        }

        float SwapSoundGenerator::calculateFrequency(const data::SwapOperation& swap) const {
            // Get progress ratio (0.0 to 1.0)
            float progress = swap.getProgressRatio();

            // Use logarithmic scaling for more natural pitch progression
            // This sounds more musical and pleasing to the ear
            float logMin = std::log(MIN_FREQUENCY);
            float logMax = std::log(MAX_FREQUENCY);
            float logFreq = logMin + progress * (logMax - logMin);

            return std::exp(logFreq);
        }

        void SwapSoundGenerator::applyEnvelope(std::vector<sf::Int16>& samples,
            size_t attackSamples,
            size_t releaseSamples) {
            if (samples.empty()) return;

            size_t totalSamples = samples.size();

            // Apply attack (fade in)
            for (size_t i = 0; i < attackSamples && i < totalSamples; ++i) {
                float envelope = static_cast<float>(i) / static_cast<float>(attackSamples);
                samples[i] = static_cast<sf::Int16>(samples[i] * envelope);
            }

            // Apply release (fade out)
            size_t releaseStart = totalSamples > releaseSamples ? totalSamples - releaseSamples : 0;
            for (size_t i = releaseStart; i < totalSamples; ++i) {
                float envelope = static_cast<float>(totalSamples - i) / static_cast<float>(releaseSamples);
                samples[i] = static_cast<sf::Int16>(samples[i] * envelope);
            }
        }

    } // namespace audio
} // namespace quicksort