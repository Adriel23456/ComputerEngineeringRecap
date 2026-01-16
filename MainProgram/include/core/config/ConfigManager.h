// ============================================================================
// File: include/core/config/ConfigManager.h
// ============================================================================

#pragma once

/**
 * @file ConfigManager.h
 * @brief Application configuration management.
 *
 * Handles loading, saving, and providing access to application
 * settings stored in JSON format.
 *
 * @note Follows:
 *   - SRP: Only manages configuration data
 *   - OCP: New settings can be added without modifying existing code
 */

#include <string>

 /**
  * @class ConfigManager
  * @brief Manages application configuration persistence and access.
  *
  * Provides a centralized interface for reading and writing
  * application settings including video, audio, and display options.
  */
class ConfigManager {
public:
    /**
     * @brief Constructs manager with default settings.
     */
    ConfigManager();

    /**
     * @brief Loads configuration from JSON file.
     * @param filepath Path to configuration file.
     * @return true if loaded successfully, false otherwise.
     */
    bool load(const std::string& filepath);

    /**
     * @brief Saves current configuration to JSON file.
     * @param filepath Path to save configuration.
     * @return true if saved successfully, false otherwise.
     */
    bool save(const std::string& filepath);

    /**
     * @brief Resets all settings to default values.
     */
    void setDefault();

    // ========================================================================
    // Video Settings Getters
    // ========================================================================

    /** @brief Gets current resolution width in pixels. */
    unsigned int getResolutionWidth() const;

    /** @brief Gets current resolution height in pixels. */
    unsigned int getResolutionHeight() const;

    /** @brief Gets target framerate limit. */
    unsigned int getFramerate() const;

    /** @brief Checks if VSync is enabled. */
    bool isVSyncEnabled() const;

    /** @brief Gets SFML window style flags. */
    int getWindowStyle() const;

    /** @brief Gets screen mode as string ("window"/"fullscreen"/"borderless"). */
    std::string getScreenModeString() const;

    // ========================================================================
    // Audio Settings Getters (0-100 range)
    // ========================================================================

    /** @brief Gets background music volume (0-100). */
    int getBGMVolume() const;

    /** @brief Gets sound effects volume (0-100). */
    int getSFXVolume() const;

    // ========================================================================
    // Setters
    // ========================================================================

    /**
     * @brief Sets display resolution.
     * @param width Resolution width in pixels.
     * @param height Resolution height in pixels.
     */
    void setResolution(unsigned int width, unsigned int height);

    /**
     * @brief Sets target framerate.
     * @param fps Frames per second limit.
     */
    void setFramerate(unsigned int fps);

    /**
     * @brief Enables or disables VSync.
     * @param enabled true to enable VSync.
     */
    void setVSyncEnabled(bool enabled);

    /**
     * @brief Sets window display mode.
     * @param mode One of: "window", "fullscreen", "borderless".
     */
    void setWindowStyle(const std::string& mode);

    /**
     * @brief Sets audio volumes.
     * @param bgm Background music volume (0-100).
     * @param sfx Sound effects volume (0-100).
     */
    void setAudio(int bgm, int sfx);

private:
    /**
     * @brief Safely parses string to unsigned int.
     * @param str String to parse.
     * @param defaultValue Value to return on parse failure.
     * @return Parsed value or default.
     */
    static unsigned int toUIntSafe(const std::string& str, unsigned int defaultValue);

    /**
     * @brief Safely parses string to int.
     * @param str String to parse.
     * @param defaultValue Value to return on parse failure.
     * @return Parsed value or default.
     */
    static int toIntSafe(const std::string& str, int defaultValue);

    // Video settings
    unsigned int m_resolutionWidth = 1920;
    unsigned int m_resolutionHeight = 1080;
    unsigned int m_framerate = 60;
    bool         m_vsync = true;
    int          m_windowStyle = 0;
    std::string  m_screenMode = "window";

    // Audio settings (0-100 range)
    int m_bgmVolume = 40;
    int m_sfxVolume = 60;
};