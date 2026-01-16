// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/CpuTLPStateFactory.cpp
// ============================================================================

/**
 * @file CpuTLPStateFactory.cpp
 * @brief Factory function for creating CpuTLPSharedCacheState.
 *
 * Provides a clean factory interface to create the CPU TLP state
 * without exposing implementation details to the main menu.
 */

#include "apps/cpu_tlp_shared_cache/CpuTLPSharedCacheState.h"
#include "core/fsm/State.h"
#include <memory>

 /**
  * @brief Creates a new CpuTLPSharedCacheState instance.
  * @param stateManager Pointer to state manager.
  * @param window Pointer to render window.
  * @return Unique pointer to the created state.
  */
std::unique_ptr<State> CreateCpuTLPSharedCacheState(StateManager* stateManager,
    sf::RenderWindow* window) {
    return std::make_unique<CpuTLPSharedCacheState>(stateManager, window);
}