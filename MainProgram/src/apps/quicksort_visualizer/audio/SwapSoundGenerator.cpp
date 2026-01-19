// ============================================================================
// File: src/apps/quicksort_visualizer/audio/SwapSoundGenerator.cpp
// ============================================================================

/**
 * @file SwapSoundGenerator.cpp
 * @brief Implementation of the SwapSoundGenerator class.
 */

#include "apps/quicksort_visualizer/audio/SwapSoundGenerator.h"
#include "systems/audio/AudioManager.h"
#include <cmath>
#include <algorithm>
#include <iostream>

namespace quicksort {
    namespace audio {

        // ============================================================================
        // Construction
        // ============================================================================

        SwapSoundGenerator::SwapSoundGenerator()
            : m_audioManager(nullptr)
            , m_soundBuffers()
            , m_sounds()
            , m_nextSoundIndex(0)
            , m_fallbackVolume(0.5f)
            , m_enabled(true)
            , m_elementCount(50)
            , m_initialized(false)
        {
        }

        // ============================================================================
        // Configuration
        // ============================================================================

        void SwapSoundGenerator::setAudioManager(AudioManager* audioManager) {
            m_audioManager = audioManager;

            if (m_audioManager) {
                std::cout << "[SwapSoundGenerator] AudioManager connected, SFX Volume: "
                    << (m_audioManager->getSFXVolume() * 100.0f) << "%\n";
            }
        }

        void SwapSoundGenerator::setElementCount(uint32_t count) {
            m_elementCount = std::max(1u, count);
        }

        void SwapSoundGenerator::setVolume(float volume) {
            m_fallbackVolume = std::clamp(volume, 0.0f, 1.0f);
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

            float frequency = calculateFrequency(swap);

            std::vector<sf::Int16> samples;
            generateTone(frequency, TONE_DURATION, samples);

            // Shorter envelope for rapid sounds
            size_t attackSamples = static_cast<size_t>(SAMPLE_RATE * 0.002f);
            size_t releaseSamples = static_cast<size_t>(SAMPLE_RATE * 0.005f);
            applyEnvelope(samples, attackSamples, releaseSamples);

            playSound(samples);
        }

        void SwapSoundGenerator::playVerificationSound(uint32_t elementIndex, uint32_t totalElements) {
            if (!m_enabled || !m_initialized) {
                return;
            }

            float frequency = calculateVerificationFrequency(elementIndex, totalElements);

            std::vector<sf::Int16> samples;
            generateTone(frequency, VERIFICATION_DURATION, samples);

            // Very short envelope for verification sweep
            size_t attackSamples = static_cast<size_t>(SAMPLE_RATE * 0.001f);
            size_t releaseSamples = static_cast<size_t>(SAMPLE_RATE * 0.003f);
            applyEnvelope(samples, attackSamples, releaseSamples);

            playSound(samples);
        }

        void SwapSoundGenerator::stop() {
            for (auto& sound : m_sounds) {
                sound.stop();
            }
        }

        // ============================================================================
        // Initialization
        // ============================================================================

        bool SwapSoundGenerator::initialize() {
            std::vector<sf::Int16> testSamples;
            generateTone(440.0f, 0.01f, testSamples);

            // Initialize all buffers in the pool with test tone
            bool allSuccess = true;
            for (size_t i = 0; i < SOUND_POOL_SIZE; ++i) {
                if (!m_soundBuffers[i].loadFromSamples(testSamples.data(), testSamples.size(), 1, SAMPLE_RATE)) {
                    allSuccess = false;
                }
                m_sounds[i].setBuffer(m_soundBuffers[i]);
            }

            m_initialized = allSuccess;
            m_nextSoundIndex = 0;

            if (m_initialized) {
                std::cout << "[SwapSoundGenerator] Initialized with " << SOUND_POOL_SIZE << " sound slots\n";
            }
            else {
                std::cerr << "[SwapSoundGenerator] Failed to initialize sound pool\n";
            }

            return m_initialized;
        }

        // ============================================================================
        // Internal Methods
        // ============================================================================

        size_t SwapSoundGenerator::findAvailableSoundSlot() {
            // First, try to find a sound that's not playing
            for (size_t i = 0; i < SOUND_POOL_SIZE; ++i) {
                size_t index = (m_nextSoundIndex + i) % SOUND_POOL_SIZE;
                if (m_sounds[index].getStatus() != sf::Sound::Playing) {
                    m_nextSoundIndex = (index + 1) % SOUND_POOL_SIZE;
                    return index;
                }
            }

            // All sounds are playing - use round-robin to pick the oldest
            size_t index = m_nextSoundIndex;
            m_nextSoundIndex = (m_nextSoundIndex + 1) % SOUND_POOL_SIZE;
            return index;
        }

        void SwapSoundGenerator::playSound(const std::vector<sf::Int16>& samples) {
            size_t slotIndex = findAvailableSoundSlot();

            // Stop the sound if it's playing (smooth transition)
            if (m_sounds[slotIndex].getStatus() == sf::Sound::Playing) {
                m_sounds[slotIndex].stop();
            }

            // Load new samples into this slot's buffer
            if (m_soundBuffers[slotIndex].loadFromSamples(samples.data(), samples.size(), 1, SAMPLE_RATE)) {
                m_sounds[slotIndex].setBuffer(m_soundBuffers[slotIndex]);
                m_sounds[slotIndex].setVolume(getEffectiveVolume() * 100.0f);
                m_sounds[slotIndex].play();
            }
        }

        void SwapSoundGenerator::generateTone(float frequency, float duration,
            std::vector<sf::Int16>& samples) {
            size_t sampleCount = static_cast<size_t>(SAMPLE_RATE * duration);
            samples.resize(sampleCount);

            const float amplitude = 0.20f;  // Slightly lower to prevent clipping when overlapping
            const float twoPi = 2.0f * 3.14159265358979f;

            for (size_t i = 0; i < sampleCount; ++i) {
                float t = static_cast<float>(i) / static_cast<float>(SAMPLE_RATE);

                // Main sine wave
                float sineValue = std::sin(twoPi * frequency * t);

                // Soft harmonic for warmth (reduced)
                float harmonic = std::sin(twoPi * frequency * 2.0f * t) * 0.08f;

                // Third harmonic for brightness (reduced)
                float harmonic3 = std::sin(twoPi * frequency * 3.0f * t) * 0.03f;

                float combined = (sineValue + harmonic + harmonic3) * amplitude;
                samples[i] = static_cast<sf::Int16>(combined * 32767.0f);
            }
        }

        float SwapSoundGenerator::calculateFrequency(const data::SwapOperation& swap) const {
            float progress = swap.getProgressRatio();

            float logMin = std::log(MIN_FREQUENCY);
            float logMax = std::log(MAX_FREQUENCY);
            float logFreq = logMin + progress * (logMax - logMin);

            return std::exp(logFreq);
        }

        float SwapSoundGenerator::calculateVerificationFrequency(uint32_t index, uint32_t total) const {
            if (total == 0) return VERIFICATION_MIN_FREQ;

            float progress = static_cast<float>(index) / static_cast<float>(total);

            float logMin = std::log(VERIFICATION_MIN_FREQ);
            float logMax = std::log(VERIFICATION_MAX_FREQ);
            float logFreq = logMin + progress * (logMax - logMin);

            return std::exp(logFreq);
        }

        void SwapSoundGenerator::applyEnvelope(std::vector<sf::Int16>& samples,
            size_t attackSamples,
            size_t releaseSamples) {
            if (samples.empty()) return;

            size_t totalSamples = samples.size();

            // Attack (fade in)
            for (size_t i = 0; i < attackSamples && i < totalSamples; ++i) {
                float envelope = static_cast<float>(i) / static_cast<float>(attackSamples);
                // Use smooth curve instead of linear
                envelope = envelope * envelope;  // Quadratic for smoother attack
                samples[i] = static_cast<sf::Int16>(samples[i] * envelope);
            }

            // Release (fade out)
            size_t releaseStart = totalSamples > releaseSamples ? totalSamples - releaseSamples : 0;
            for (size_t i = releaseStart; i < totalSamples; ++i) {
                float envelope = static_cast<float>(totalSamples - i) / static_cast<float>(releaseSamples);
                // Use smooth curve for release
                envelope = envelope * envelope;  // Quadratic for smoother release
                samples[i] = static_cast<sf::Int16>(samples[i] * envelope);
            }
        }

        float SwapSoundGenerator::getEffectiveVolume() const {
            if (m_audioManager) {
                return m_audioManager->getSFXVolume();
            }
            return m_fallbackVolume;
        }

    } // namespace audio
} // namespace quicksort