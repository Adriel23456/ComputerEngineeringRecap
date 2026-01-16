// ============================================================================
// File: include/ui/overlays/panels/CreditsPanel.h
// ============================================================================

#pragma once

/**
 * @file CreditsPanel.h
 * @brief Credits display panel.
 *
 * Shows project credits and team information.
 *
 * @note Follows SRP: Only handles credits display
 */

#include <string>

 /**
  * @class CreditsPanel
  * @brief UI panel for displaying credits.
  */
class CreditsPanel {
public:
    /**
     * @brief Constructs the credits panel with default text.
     */
    CreditsPanel();

    /**
     * @brief Renders the credits view.
     * @param fullWidth Available width.
     * @param buttonHeight Height for back button.
     * @return true if Back button was clicked.
     */
    bool render(float fullWidth, float buttonHeight);

private:
    std::string m_creditsText;
};