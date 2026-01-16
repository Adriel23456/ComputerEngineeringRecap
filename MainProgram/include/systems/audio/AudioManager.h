// ============================================================================
// File: include/systems/audio/AudioManager.h
// ============================================================================

#pragma once

/**
 * @file AudioManager.h
 * @brief Audio system management for BGM and SFX.
 *
 * Provides unified interface for background music and sound effects
 * with support for multiple named sound effects.
 *
 * @note Follows:
 *   - SRP: Only manages audio playback
 *   - OCP: New audio types can be added via the keyed SFX system
 */

#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

 /**
  * @class AudioManager
  * @brief Manages background music and sound effects playback.
  *
  * Supports both legacy single-SFX mode and modern keyed multi-SFX system.
  * Volume is controlled via normalized 0.0-1.0 float values.
  */
class AudioManager {
public:
    AudioManager() = default;

    // ========================================================================
    // Background Music
    // ========================================================================

    /**
     * @brief Initializes BGM with specified audio file.
     * @param bgmPath Path relative to RESOURCES_PATH.
     * @return true if loaded successfully.
     */
    bool init(const std::string& bgmPath);

    /**
     * @brief Starts playing background music.
     * @param loop Whether to loop the music.
     */
    void playBGM(bool loop = true);

    /**
     * @brief Stops background music playback.
     */
    void stopBGM();

    // ========================================================================
    // Volume Control
    // ========================================================================

    /**
     * @brief Sets background music volume.
     * @param volume Normalized volume (0.0 to 1.0).
     */
    void setBGMVolume(float volume);

    /**
     * @brief Sets sound effects volume.
     * @param volume Normalized volume (0.0 to 1.0).
     */
    void setSFXVolume(float volume);

    /**
     * @brief Gets current BGM volume.
     * @return Normalized volume (0.0 to 1.0).
     */
    float getBGMVolume() const { return m_bgmVolume; }

    /**
     * @brief Gets current SFX volume.
     * @return Normalized volume (0.0 to 1.0).
     */
    float getSFXVolume() const { return m_sfxVolume; }

    // ========================================================================
    // Legacy Single SFX (for backward compatibility)
    // ========================================================================

    /**
     * @brief Loads a single sound effect (legacy API).
     * @param sfxPath Path relative to RESOURCES_PATH.
     * @return true if loaded successfully.
     */
    bool loadSFX(const std::string& sfxPath);

    /**
     * @brief Plays the loaded single sound effect (legacy API).
     */
    void playSFX();

    // ========================================================================
    // Keyed Multi-SFX System
    // ========================================================================

    /**
     * @brief Loads a sound effect with a unique key.
     * @param key Unique identifier for the sound.
     * @param sfxPath Path relative to RESOURCES_PATH.
     * @return true if loaded successfully.
     */
    bool loadSFX(const std::string& key, const std::string& sfxPath);

    /**
     * @brief Plays a sound effect by key.
     * @param key Identifier of the sound to play.
     */
    void playSFX(const std::string& key);

private:
    // Background music
    sf::Music m_bgm;

    // Legacy single SFX support
    sf::SoundBuffer m_sfxBuffer;
    sf::Sound       m_sfx;

    // Multi-SFX support (keyed)
    std::unordered_map<std::string, sf::SoundBuffer> m_sfxBuffers;
    std::unordered_map<std::string, sf::Sound>       m_sfxMap;

    // Volume levels (normalized 0.0-1.0)
    float m_bgmVolume = 0.4f;
    float m_sfxVolume = 0.6f;
};