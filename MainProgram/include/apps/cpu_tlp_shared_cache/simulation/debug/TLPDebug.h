// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/debug/TLPDebug.h
// ============================================================================

#pragma once

/**
 * @file TLPDebug.h
 * @brief Debug logging macros and helper functions for TLP simulation.
 *
 * Provides conditional debug output for various simulation components.
 * Debug output can be enabled/disabled via the TLP_DEBUG preprocessor flag.
 *
 * @note Enable debug output by defining TLP_DEBUG=1 before including this file
 *       or via compiler flag: -DTLP_DEBUG=1
 */

#include <iostream>
#include <iomanip>

 // ============================================================================
 // Debug Enable/Disable
 // ============================================================================

#ifndef TLP_DEBUG
#define TLP_DEBUG 0  // Disabled by default for release builds
#endif

// ============================================================================
// Debug Macros
// ============================================================================

#if TLP_DEBUG

/**
 * @brief Debug output for Processing Element operations.
 * @param pe PE index (0-3).
 * @param msg Message to output.
 */
#define DBG_PE(pe, msg) \
    do { std::cout << "[PE" << (pe) << "] " << msg << std::endl; } while(0)

 /**
  * @brief Debug output for L1 Cache operations.
  * @param id Cache ID (0-3).
  * @param msg Message to output.
  */
#define DBG_L1(id, msg) \
    do { std::cout << "[L1" << (id) << "] " << msg << std::endl; } while(0)

  /**
   * @brief Debug output for Bus/Interconnect operations.
   * @param msg Message to output.
   */
#define DBG_BUS(msg) \
    do { std::cout << "[BUS] " << msg << std::endl; } while(0)

   /**
    * @brief Debug output for Shared Memory (DRAM) operations.
    * @param msg Message to output.
    */
#define DBG_SM(msg) \
    do { std::cout << "[DRAM] " << msg << std::endl; } while(0)

#else

// No-op versions when debug is disabled
#define DBG_PE(pe, msg)  do {} while(0)
#define DBG_L1(id, msg)  do {} while(0)
#define DBG_BUS(msg)     do {} while(0)
#define DBG_SM(msg)      do {} while(0)

#endif