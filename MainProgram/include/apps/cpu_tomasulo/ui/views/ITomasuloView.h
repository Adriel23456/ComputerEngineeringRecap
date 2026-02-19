// ============================================================================
// File: include/apps/cpu_tomasulo/ui/views/ITomasuloView.h
// ============================================================================

#pragma once

/**
 * @file ITomasuloView.h
 * @brief Base interface for all Tomasulo CPU panel views.
 *
 * Every panel rendered inside the CpuTomasuloState sidebar must implement
 * this interface. Default no-op implementations are provided for
 * handleEvent and update so leaf views only override what they need.
 *
 * @note
 *   - ISP: Views that don't process events or animate simply ignore them.
 *   - LSP: All concrete views are drop-in replacements for ITomasuloView.
 */

#include <SFML/Window/Event.hpp>

class ITomasuloView {
public:
    virtual ~ITomasuloView() = default;

    virtual void handleEvent(sf::Event& /*event*/) {}
    virtual void update(float /*deltaTime*/) {}
    virtual void render() = 0;
};