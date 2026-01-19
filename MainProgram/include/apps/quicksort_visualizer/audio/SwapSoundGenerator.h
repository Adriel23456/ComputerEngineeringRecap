// ============================================================================
// File: include/apps/quicksort_visualizer/audio/SwapSoundGenerator.h
// ============================================================================

#pragma once

/**
 * @file SwapSoundGenerator.h
 * @brief Generates and plays swap sounds for visualization.
 *
 * Creates procedural audio tones that increase in pitch as sorting progresses.
 * Uses SFML audio for playback.
 *
 * @note Design Principles:
 *   - SRP: Only handles sound generation and playback
 *   - Integrates with existing SFX volume system
 */

#include "apps/quicksort_visualizer/data/SwapOperation.h"
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <cstdint>

namespace quicksort {
    namespace audio {

        /**
         * @class SwapSoundGenerator
         * @brief Generates and plays procedural sounds for swap operations.
         */
        class SwapSoundGenerator {
        public:
            // ========================================================================
            // Construction
            // ========================================================================

            /**
             * @brief Constructs the sound generator.
             */
            SwapSoundGenerator();

            /**
             * @brief Destructor.
             */
            ~SwapSoundGenerator() = default;

            // Non-copyable (owns SFML resources)
            SwapSoundGenerator(const SwapSoundGenerator&) = delete;
            SwapSoundGenerator& operator=(const SwapSoundGenerator&) = delete;

            // ========================================================================
            // Configuration
            // ========================================================================

            /**
             * @brief Sets the base element count for pitch scaling.
             * @param count Number of elements being sorted.
             */
            void setElementCount(uint32_t count);

            /**
             * @brief Sets the volume (0.0 to 1.0).
             * @param volume Volume level.
             */
            void setVolume(float volume);

            /**
             * @brief Enables or disables sound.
             * @param enabled Whether to play sounds.
             */
            void setEnabled(bool enabled);

            // ========================================================================
            // Sound Playback
            // ========================================================================

            /**
             * @brief Plays a sound for a swap operation.
             * @param swap The swap operation (used for pitch calculation).
             */
            void playSwapSound(const data::SwapOperation& swap);

            /**
             * @brief Stops any playing sounds.
             */
            void stop();

            // ========================================================================
            // Initialization
            // ========================================================================

            /**
             * @brief Initializes audio resources.
             * @return true if successful.
             */
            bool initialize();

        private:
            // ========================================================================
            // Internal Methods
            // ========================================================================

            /**
             * @brief Generates a sine wave tone.
             * @param frequency Frequency in Hz.
             * @param duration Duration in seconds.
             * @param samples Output sample buffer.
             */
            void generateTone(float frequency, float duration, std::vector<sf::Int16>& samples);

            /**
             * @brief Calculates frequency for a swap based on progress.
             * @param swap The swap operation.
             * @return Frequency in Hz.
             */
            float calculateFrequency(const data::SwapOperation& swap) const;

            /**
             * @brief Applies an envelope to prevent clicking.
             * @param samples The sample buffer to modify.
             * @param attackSamples Number of attack samples.
             * @param releaseSamples Number of release samples.
             */
            void applyEnvelope(std::vector<sf::Int16>& samples,
                size_t attackSamples,
                size_t releaseSamples);

            // ========================================================================
            // Data Members
            // ========================================================================

            sf::SoundBuffer m_soundBuffer;      ///< Current sound buffer
            sf::Sound m_sound;                   ///< Sound player

            float m_volume;                      ///< Volume (0.0 - 1.0)
            bool m_enabled;                      ///< Whether sound is enabled
            uint32_t m_elementCount;             ///< Element count for scaling
            bool m_initialized;                  ///< Whether audio is initialized

            // Sound parameters
            static constexpr float MIN_FREQUENCY = 220.0f;   ///< Starting frequency (A3)
            static constexpr float MAX_FREQUENCY = 880.0f;   ///< Ending frequency (A5)
            static constexpr float TONE_DURATION = 0.05f;    ///< Duration in seconds
            static constexpr unsigned int SAMPLE_RATE = 44100; ///< Audio sample rate
        };

    } // namespace audio
} // namespace quicksort