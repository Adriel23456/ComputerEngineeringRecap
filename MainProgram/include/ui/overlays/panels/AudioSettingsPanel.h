// ============================================================================
// File: include/ui/overlays/panels/AudioSettingsPanel.h
// ============================================================================

#pragma once

/**
 * @file AudioSettingsPanel.h
 * @brief Audio settings UI panel.
 *
 * Handles audio volume settings for BGM and SFX.
 *
 * @note Follows SRP: Only handles audio settings UI
 */

class ConfigManager;
class AudioManager;

/**
 * @class AudioSettingsPanel
 * @brief UI panel for audio settings.
 */
class AudioSettingsPanel {
public:
    /**
     * @brief Constructs the audio settings panel.
     * @param config Pointer to configuration manager.
     * @param audio Pointer to audio manager.
     */
    AudioSettingsPanel(ConfigManager* config, AudioManager* audio);

    /**
     * @brief Renders the audio settings UI.
     * @param fullWidth Full available width for sliders.
     */
    void render(float fullWidth);

private:
    ConfigManager* m_config;
    AudioManager* m_audio;

    int m_pendingBGM;
    int m_pendingSFX;
};