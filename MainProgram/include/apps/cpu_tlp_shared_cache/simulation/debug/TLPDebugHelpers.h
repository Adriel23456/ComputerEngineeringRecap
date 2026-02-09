// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/debug/TLPDebugHelpers.h
// ============================================================================

#pragma once

/**
 * @file TLPDebugHelpers.h
 * @brief Helper functions for debug output formatting.
 *
 * Provides human-readable string conversion for MESI states and bus commands.
 *
 * @note Separated from tlp_debug.h to avoid circular dependencies with
 *       l1_cache.h and l1_snoop.h.
 */

#include "apps/cpu_tlp_shared_cache/simulation/cache/l1_cache.h"
#include "apps/cpu_tlp_shared_cache/simulation/cache/l1_snoop.h"

 /**
  * @brief Converts MESI state to human-readable string.
  * @param state MESI state enum value.
  * @return Single-character string: "I", "S", "E", "M", or "?".
  */
inline const char* mesiName(Mesi state) {
    switch (state) {
    case Mesi::I: return "I";
    case Mesi::S: return "S";
    case Mesi::E: return "E";
    case Mesi::M: return "M";
    }
    return "?";
}

/**
 * @brief Converts bus command to human-readable string.
 * @param cmd Bus command enum value.
 * @return Command name string.
 */
inline const char* cmdName(BusCmd cmd) {
    switch (cmd) {
    case BusCmd::BusRd:     return "BusRd";
    case BusCmd::BusRdX:    return "BusRdX";
    case BusCmd::BusUpgr:   return "BusUpgr";
    case BusCmd::WriteBack: return "WriteBack";
    }
    return "?";
}