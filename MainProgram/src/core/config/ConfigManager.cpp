// ============================================================================
// File: src/core/config/ConfigManager.cpp
// ============================================================================

/**
 * @file ConfigManager.cpp
 * @brief Implementation of ConfigManager class.
 *
 * Handles JSON parsing with robust error handling and type coercion
 * for maximum compatibility with various configuration file formats.
 */

#include "core/config/ConfigManager.h"
#include <SFML/Window.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

// ============================================================================
// Helper Functions
// ============================================================================

namespace {

    /**
     * @brief Clamps integer value to 0-100 range.
     * @param value Value to clamp.
     * @return Clamped value.
     */
    int clampVolume(int value) {
        return std::max(0, std::min(100, value));
    }

} // anonymous namespace

// ============================================================================
// Construction
// ============================================================================

ConfigManager::ConfigManager() {
    setDefault();
}

// ============================================================================
// Default Settings
// ============================================================================

void ConfigManager::setDefault() {
    m_resolutionWidth = 1920;
    m_resolutionHeight = 1080;
    m_framerate = 60;
    m_vsync = true;
    m_screenMode = "window";
    m_windowStyle = sf::Style::Titlebar | sf::Style::Close;
    m_bgmVolume = 40;
    m_sfxVolume = 60;
}

// ============================================================================
// Safe Type Conversion
// ============================================================================

unsigned int ConfigManager::toUIntSafe(const std::string& str, unsigned int defaultValue) {
    try {
        return static_cast<unsigned int>(std::stoul(str));
    }
    catch (...) {
        return defaultValue;
    }
}

int ConfigManager::toIntSafe(const std::string& str, int defaultValue) {
    try {
        return static_cast<int>(std::stol(str));
    }
    catch (...) {
        return defaultValue;
    }
}

// ============================================================================
// File I/O
// ============================================================================

bool ConfigManager::load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "[Config] Could not open " << filepath << ", using defaults.\n";
        return false;
    }

    json data;
    try {
        file >> data;
    }
    catch (const std::exception& e) {
        std::cout << "[Config] Invalid JSON: " << e.what() << " (using defaults)\n";
        return false;
    }

    // Parse resolution object
    if (data.contains("resolution") && data["resolution"].is_object()) {
        auto& res = data["resolution"];
        if (res.contains("width")) {
            if (res["width"].is_number_unsigned()) {
                m_resolutionWidth = res["width"].get<unsigned>();
            }
            else if (res["width"].is_string()) {
                m_resolutionWidth = toUIntSafe(res["width"].get<std::string>(), m_resolutionWidth);
            }
        }
        if (res.contains("height")) {
            if (res["height"].is_number_unsigned()) {
                m_resolutionHeight = res["height"].get<unsigned>();
            }
            else if (res["height"].is_string()) {
                m_resolutionHeight = toUIntSafe(res["height"].get<std::string>(), m_resolutionHeight);
            }
        }
    }

    // Parse framerate
    if (data.contains("framerate")) {
        if (data["framerate"].is_number_unsigned()) {
            m_framerate = data["framerate"].get<unsigned>();
        }
        else if (data["framerate"].is_number_integer()) {
            m_framerate = static_cast<unsigned>(data["framerate"].get<int>());
        }
        else if (data["framerate"].is_string()) {
            m_framerate = toUIntSafe(data["framerate"].get<std::string>(), m_framerate);
        }
    }

    // Parse VSync
    if (data.contains("vsync")) {
        if (data["vsync"].is_boolean()) {
            m_vsync = data["vsync"].get<bool>();
        }
        else if (data["vsync"].is_string()) {
            std::string val = data["vsync"].get<std::string>();
            std::transform(val.begin(), val.end(), val.begin(), ::tolower);
            m_vsync = (val == "true" || val == "1" || val == "on");
        }
    }

    // Parse screen mode
    if (data.contains("screenMode") && data["screenMode"].is_string()) {
        setWindowStyle(data["screenMode"].get<std::string>());
    }

    // Parse audio settings
    if (data.contains("audio") && data["audio"].is_object()) {
        auto& audio = data["audio"];
        if (audio.contains("bgm")) {
            if (audio["bgm"].is_number_integer()) {
                m_bgmVolume = clampVolume(audio["bgm"].get<int>());
            }
            else if (audio["bgm"].is_string()) {
                m_bgmVolume = clampVolume(toIntSafe(audio["bgm"].get<std::string>(), m_bgmVolume));
            }
        }
        if (audio.contains("sfx")) {
            if (audio["sfx"].is_number_integer()) {
                m_sfxVolume = clampVolume(audio["sfx"].get<int>());
            }
            else if (audio["sfx"].is_string()) {
                m_sfxVolume = clampVolume(toIntSafe(audio["sfx"].get<std::string>(), m_sfxVolume));
            }
        }
    }

    return true;
}

bool ConfigManager::save(const std::string& filepath) {
    json data;
    data["resolution"]["width"] = m_resolutionWidth;
    data["resolution"]["height"] = m_resolutionHeight;
    data["framerate"] = m_framerate;
    data["vsync"] = m_vsync;
    data["screenMode"] = m_screenMode;
    data["audio"]["bgm"] = m_bgmVolume;
    data["audio"]["sfx"] = m_sfxVolume;

    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    file << data.dump(4);
    return true;
}

// ============================================================================
// Getters
// ============================================================================

unsigned int ConfigManager::getResolutionWidth()  const { return m_resolutionWidth; }
unsigned int ConfigManager::getResolutionHeight() const { return m_resolutionHeight; }
unsigned int ConfigManager::getFramerate()        const { return m_framerate; }
bool         ConfigManager::isVSyncEnabled()      const { return m_vsync; }
int          ConfigManager::getWindowStyle()      const { return m_windowStyle; }
std::string  ConfigManager::getScreenModeString() const { return m_screenMode; }
int          ConfigManager::getBGMVolume()        const { return m_bgmVolume; }
int          ConfigManager::getSFXVolume()        const { return m_sfxVolume; }

// ============================================================================
// Setters
// ============================================================================

void ConfigManager::setResolution(unsigned int width, unsigned int height) {
    m_resolutionWidth = width;
    m_resolutionHeight = height;
}

void ConfigManager::setFramerate(unsigned int fps) {
    m_framerate = fps;
}

void ConfigManager::setVSyncEnabled(bool enabled) {
    m_vsync = enabled;
}

void ConfigManager::setAudio(int bgm, int sfx) {
    m_bgmVolume = clampVolume(bgm);
    m_sfxVolume = clampVolume(sfx);
}

void ConfigManager::setWindowStyle(const std::string& mode) {
    m_screenMode = mode;

    if (mode == "window") {
        m_windowStyle = sf::Style::Titlebar | sf::Style::Close;
    }
    else if (mode == "fullscreen") {
        m_windowStyle = sf::Style::Fullscreen;
    }
    else if (mode == "borderless") {
        m_windowStyle = sf::Style::None;
    }
    else {
        // Invalid mode - default to windowed
        m_screenMode = "window";
        m_windowStyle = sf::Style::Titlebar | sf::Style::Close;
    }
}