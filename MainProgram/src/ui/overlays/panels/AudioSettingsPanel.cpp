// ============================================================================
// File: src/ui/overlays/panels/AudioSettingsPanel.cpp
// ============================================================================

/**
 * @file AudioSettingsPanel.cpp
 * @brief Implementation of AudioSettingsPanel.
 */

#include "ui/overlays/panels/AudioSettingsPanel.h"
#include "core/config/ConfigManager.h"
#include "systems/audio/AudioManager.h"
#include "imgui.h"
#include <string>

 // ============================================================================
 // Construction
 // ============================================================================

AudioSettingsPanel::AudioSettingsPanel(ConfigManager* config, AudioManager* audio)
    : m_config(config)
    , m_audio(audio)
    , m_pendingBGM(config->getBGMVolume())
    , m_pendingSFX(config->getSFXVolume())
{
}

// ============================================================================
// Rendering
// ============================================================================

void AudioSettingsPanel::render(float fullWidth) {
    ImGui::Text("Audio Settings");
    ImGui::Separator();
    ImGui::Spacing();

    // BGM Volume Slider
    ImGui::SetNextItemWidth(fullWidth);
    int bgm = m_pendingBGM;
    if (ImGui::SliderInt("##BGMSlider", &bgm, 0, 100, "BGM Volume: %d%%")) {
        m_pendingBGM = bgm;
        m_audio->setBGMVolume(m_pendingBGM / 100.f);
        m_config->setAudio(m_pendingBGM, m_pendingSFX);
        m_config->save(std::string(RESOURCES_PATH) + "config.json");
    }

    ImGui::Spacing();

    // SFX Volume Slider
    ImGui::SetNextItemWidth(fullWidth);
    int sfx = m_pendingSFX;
    if (ImGui::SliderInt("##SFXSlider", &sfx, 0, 100, "SFX Volume: %d%%")) {
        m_pendingSFX = sfx;
        m_audio->setSFXVolume(m_pendingSFX / 100.f);
        m_config->setAudio(m_pendingBGM, m_pendingSFX);
        m_config->save(std::string(RESOURCES_PATH) + "config.json");
    }
}