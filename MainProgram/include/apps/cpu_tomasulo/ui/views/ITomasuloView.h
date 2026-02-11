#pragma once

/**
 * @file ITomasuloView.h
 * @brief Interface for all Tomasulo CPU views.
 */

#include <SFML/Window/Event.hpp>

class ITomasuloView {
public:
    virtual ~ITomasuloView() = default;

    virtual void handleEvent(sf::Event& /*event*/) {}
    virtual void update(float /*deltaTime*/) {}
    virtual void render() = 0;
};