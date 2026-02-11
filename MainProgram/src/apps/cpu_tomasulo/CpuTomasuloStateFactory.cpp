/**
 * @file CpuTomasuloStateFactory.cpp
 * @brief Factory function for creating CpuTomasuloState.
 */

#include "apps/cpu_tomasulo/CpuTomasuloState.h"
#include "core/fsm/State.h"
#include <memory>

class AudioManager;

std::unique_ptr<State> CreateCpuTomasuloState(StateManager* stateManager,
    sf::RenderWindow* window,
    AudioManager* /*audio*/) {
    return std::make_unique<CpuTomasuloState>(stateManager, window);
}