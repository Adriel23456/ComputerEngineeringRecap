// ============================================================================
// File: include/apps/quicksort_visualizer/audio/SwapSoundGenerator.h
// ============================================================================

#pragma once

/**
 * @file SwapSoundGenerator.h
 * @brief Generates and plays swap sounds for visualization.
 *
 * Uses a sound pool to prevent audio collision artifacts when
 * sounds are played in rapid succession.
 */

#include "apps/quicksort_visualizer/data/SwapOperation.h"
#include <SFML/Audio.hpp>
#include <vector>
#include <array>
#include <memory>
#include <cstdint>

 // Forward declaration
class AudioManager;

namespace quicksort {
    namespace audio {

        /**
         * @class SwapSoundGenerator
         * @brief Generates and plays procedural sounds for swap operations.
         *
         * Uses a pool of sound objects to allow overlapping sounds without
         * interruption artifacts.
         */
        class SwapSoundGenerator {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            SwapSoundGenerator();
            ~SwapSoundGenerator() = default;

            // Non-copyable
            SwapSoundGenerator(const SwapSoundGenerator&) = delete;
            SwapSoundGenerator& operator=(const SwapSoundGenerator&) = delete;

            // ========================================================================
            // Configuration
            // ========================================================================

            void setAudioManager(AudioManager* audioManager);
            void setElementCount(uint32_t count);
            void setVolume(float volume);
            void setEnabled(bool enabled);

            // ========================================================================
            // Sound Playback
            // ========================================================================

            void playSwapSound(const data::SwapOperation& swap);
            void playVerificationSound(uint32_t elementIndex, uint32_t totalElements);
            void stop();

            // ========================================================================
            // Initialization
            // ========================================================================

            bool initialize();

        private:
            // ========================================================================
            // Internal Methods
            // ========================================================================

            void generateTone(float frequency, float duration, std::vector<sf::Int16>& samples);
            float calculateFrequency(const data::SwapOperation& swap) const;
            float calculateVerificationFrequency(uint32_t index, uint32_t total) const;
            void applyEnvelope(std::vector<sf::Int16>& samples,
                size_t attackSamples,
                size_t releaseSamples);
            float getEffectiveVolume() const;

            /**
             * @brief Finds an available sound slot in the pool.
             * @return Index of available slot, or reuses oldest if all busy.
             */
            size_t findAvailableSoundSlot();

            /**
             * @brief Plays a sound with the given samples.
             * @param samples Audio samples to play.
             */
            void playSound(const std::vector<sf::Int16>& samples);

            // ========================================================================
            // Data Members
            // ========================================================================

            AudioManager* m_audioManager;

            // Sound pool to prevent audio collisions
            static constexpr size_t SOUND_POOL_SIZE = 8;  ///< Number of concurrent sounds
            std::array<sf::SoundBuffer, SOUND_POOL_SIZE> m_soundBuffers;
            std::array<sf::Sound, SOUND_POOL_SIZE> m_sounds;
            size_t m_nextSoundIndex;  ///< Round-robin index for sound pool

            float m_fallbackVolume;
            bool m_enabled;
            uint32_t m_elementCount;
            bool m_initialized;

            // Sound parameters
            static constexpr float MIN_FREQUENCY = 220.0f;
            static constexpr float MAX_FREQUENCY = 880.0f;
            static constexpr float VERIFICATION_MIN_FREQ = 330.0f;
            static constexpr float VERIFICATION_MAX_FREQ = 1320.0f;
            static constexpr float TONE_DURATION = 0.035f;        ///< Shorter duration
            static constexpr float VERIFICATION_DURATION = 0.015f; ///< Very short for sweep
            static constexpr unsigned int SAMPLE_RATE = 44100;
        };

    } // namespace audio
} // namespace quicksort