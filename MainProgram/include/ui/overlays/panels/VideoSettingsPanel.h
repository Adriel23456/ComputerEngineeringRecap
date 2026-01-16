// ============================================================================
// File: include/ui/overlays/panels/VideoSettingsPanel.h
// ============================================================================

#pragma once

/**
 * @file VideoSettingsPanel.h
 * @brief Video settings UI panel.
 *
 * Handles video-related settings including resolution,
 * screen mode, and VSync.
 *
 * @note Follows SRP: Only handles video settings UI
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ConfigManager;

/**
 * @struct ResolutionItem
 * @brief Represents a display resolution option.
 */
struct ResolutionItem {
    unsigned int width;
    unsigned int height;
};

/**
 * @class VideoSettingsPanel
 * @brief UI panel for video settings.
 */
class VideoSettingsPanel {
public:
    /**
     * @brief Constructs the video settings panel.
     * @param window Pointer to render window.
     * @param config Pointer to configuration manager.
     */
    VideoSettingsPanel(sf::RenderWindow* window, ConfigManager* config);

    /**
     * @brief Renders the video settings UI.
     * @param columnWidth Width of each column.
     * @param buttonHeight Height of buttons.
     * @return true if Apply was clicked and changes need to be applied.
     */
    bool render(float columnWidth, float buttonHeight);

    /**
     * @brief Rebuilds resolution list after window recreation.
     */
    void rebuildResolutions();

    /**
     * @brief Checks if there are pending display changes.
     * @return true if settings differ from current config.
     */
    bool hasDisplayChangesPending() const;

    /**
     * @brief Applies pending changes to configuration.
     */
    void applyChanges();

private:
    /** @brief Builds the list of available resolutions. */
    void buildResolutionList();

    /**
     * @brief Calculates aspect ratio string for resolution.
     * @param width Resolution width.
     * @param height Resolution height.
     * @return Aspect ratio string (e.g., "16:9").
     */
    std::string getAspectRatioString(unsigned int width, unsigned int height) const;

    /**
     * @brief Compares two resolutions for equality.
     */
    static bool sameResolution(unsigned int w1, unsigned int h1,
        unsigned int w2, unsigned int h2);

    // Dependencies
    sf::RenderWindow* m_window;
    ConfigManager* m_config;

    // Resolution selection
    std::vector<ResolutionItem> m_resolutionList;
    int m_resolutionIndex = 0;

    // Pending settings
    bool m_pendingVSync = true;
    int  m_screenModeIndex = 0;

    // Screen mode options
    static constexpr const char* SCREEN_MODES[] = { "window", "fullscreen", "borderless" };
};