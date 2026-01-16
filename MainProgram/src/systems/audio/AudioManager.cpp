// ============================================================================
// File: src/systems/audio/AudioManager.cpp
// ============================================================================

/**
 * @file AudioManager.cpp
 * @brief Implementation of AudioManager class.
 *
 * Provides audio playback functionality using SFML audio system.
 */

#include "systems/audio/AudioManager.h"
#include <iostream>
#include <algorithm>

 // ============================================================================
 // Helper Functions
 // ============================================================================

namespace {

    /**
     * @brief Clamps volume to valid 0.0-1.0 range.
     * @param volume Input volume.
     * @return Clamped volume.
     */
    float clampVolume(float volume) {
        return std::max(0.f, std::min(1.f, volume));
    }

    /**
     * @brief Converts normalized volume to SFML volume (0-100).
     * @param normalized Volume in 0.0-1.0 range.
     * @return SFML volume (0-100).
     */
    float toSFMLVolume(float normalized) {
        return normalized * 100.f;
    }

    /**
     * @brief Constructs full resource path.
     * @param relativePath Path relative to resources directory.
     * @return Complete file path.
     */
    std::string getFullPath(const std::string& relativePath) {
        return std::string(RESOURCES_PATH) + relativePath;
    }

} // anonymous namespace

// ============================================================================
// Background Music
// ============================================================================

bool AudioManager::init(const std::string& bgmPath) {
    if (!m_bgm.openFromFile(getFullPath(bgmPath))) {
        std::cout << "[Audio] Could not load BGM: " << bgmPath << "\n";
        return false;
    }

    m_bgm.setVolume(toSFMLVolume(m_bgmVolume));
    m_bgm.setLoop(true);
    return true;
}

void AudioManager::playBGM(bool loop) {
    m_bgm.setLoop(loop);
    m_bgm.play();
}

void AudioManager::stopBGM() {
    m_bgm.stop();
}

// ============================================================================
// Volume Control
// ============================================================================

void AudioManager::setBGMVolume(float volume) {
    m_bgmVolume = clampVolume(volume);
    m_bgm.setVolume(toSFMLVolume(m_bgmVolume));
}

void AudioManager::setSFXVolume(float volume) {
    m_sfxVolume = clampVolume(volume);

    // Update legacy SFX volume
    m_sfx.setVolume(toSFMLVolume(m_sfxVolume));

    // Update all keyed SFX volumes
    for (auto& [key, sound] : m_sfxMap) {
        sound.setVolume(toSFMLVolume(m_sfxVolume));
    }
}

// ============================================================================
// Legacy Single SFX
// ============================================================================

bool AudioManager::loadSFX(const std::string& sfxPath) {
    if (!m_sfxBuffer.loadFromFile(getFullPath(sfxPath))) {
        std::cout << "[Audio] Could not load SFX: " << sfxPath << "\n";
        return false;
    }

    m_sfx.setBuffer(m_sfxBuffer);
    m_sfx.setVolume(toSFMLVolume(m_sfxVolume));
    return true;
}

void AudioManager::playSFX() {
    if (m_sfx.getBuffer()) {
        m_sfx.play();
    }
}

// ============================================================================
// Keyed Multi-SFX System
// ============================================================================

bool AudioManager::loadSFX(const std::string& key, const std::string& sfxPath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(getFullPath(sfxPath))) {
        std::cout << "[Audio] Could not load SFX (" << key << "): " << sfxPath << "\n";
        return false;
    }

    // Store buffer (must persist for sound to work)
    m_sfxBuffers[key] = std::move(buffer);

    // Create and configure sound
    sf::Sound sound;
    sound.setBuffer(m_sfxBuffers[key]);
    sound.setVolume(toSFMLVolume(m_sfxVolume));
    m_sfxMap[key] = std::move(sound);

    return true;
}

void AudioManager::playSFX(const std::string& key) {
    auto it = m_sfxMap.find(key);
    if (it == m_sfxMap.end()) {
        return;
    }

    // Restart if already playing for snappy response
    if (it->second.getStatus() == sf::Sound::Playing) {
        it->second.stop();
    }

    it->second.play();
}