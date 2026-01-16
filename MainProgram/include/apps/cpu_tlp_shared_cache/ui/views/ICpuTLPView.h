// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/ui/views/ICpuTLPView.h
// ============================================================================

#pragma once

/**
 * @file ICpuTLPView.h
 * @brief Interface for CPU TLP simulation views.
 *
 * Defines the base interface that all UI views must implement.
 * Views are rendered within a parent container that manages layout.
 *
 * @note Follows:
 *   - ISP: Minimal interface with optional overrides
 *   - OCP: New views can be added without modifying existing code
 */

#include <SFML/Graphics.hpp>

 /**
  * @class ICpuTLPView
  * @brief Abstract base interface for simulation views.
  *
  * Views are responsible for:
  * - Handling input events (optional)
  * - Updating state each frame (optional)
  * - Rendering UI content (required)
  *
  * The parent container manages the render area; views render into
  * the current ImGui context.
  */
class ICpuTLPView {
public:
    virtual ~ICpuTLPView() = default;

    /**
     * @brief Handles SFML events.
     * @param event Event to process.
     *
     * Override to handle keyboard, mouse, or other events.
     * Default implementation does nothing.
     */
    virtual void handleEvent(sf::Event& event) {
        (void)event;  // Suppress unused parameter warning
    }

    /**
     * @brief Updates view state.
     * @param deltaTime Time elapsed since last frame (seconds).
     *
     * Override for animations or time-based updates.
     * Default implementation does nothing.
     */
    virtual void update(float deltaTime) {
        (void)deltaTime;  // Suppress unused parameter warning
    }

    /**
     * @brief Renders the view content.
     *
     * Must be implemented by derived classes.
     * Called each frame within an ImGui context.
     */
    virtual void render() = 0;
};