// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/views/GeneralView.h
// ============================================================================

#pragma once

/**
 * @file GeneralView.h
 * @brief General overview panel with global PE controls.
 *
 * Displays system architecture diagram and provides controls
 * that operate on all processing elements simultaneously.
 *
 * @note Follows:
 *   - SRP: Combines overview display with global control
 *   - DIP: Uses callback API for PE control
 */

#include "apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h"
#include <SFML/Graphics/Texture.hpp>

 /**
  * @class GeneralView
  * @brief System overview with global PE control buttons.
  *
  * Features:
  * - Architecture diagram display
  * - Global Reset/Step/Run/Stop for all PEs
  * - Step-until-N functionality
  */
class GeneralView : public ICpuTLPView {
public:
    GeneralView();

    /**
     * @brief Renders the view with image and controls.
     */
    void render() override;

private:
    /**
     * @brief Loads architecture diagram if not already loaded.
     */
    void ensureLoaded();

    sf::Texture m_texture;       ///< Architecture diagram texture
    bool m_loaded = false;       ///< Texture load status
    int m_untilSteps = 1;        ///< Step count for StepUntil (minimum 1)
};