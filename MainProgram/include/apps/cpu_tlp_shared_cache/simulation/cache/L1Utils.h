// ============================================================================
// File: include/apps/cpu_tlp_shared_cache/simulation/cache/L1Utils.h
// ============================================================================

#pragma once

/**
 * @file L1Utils.h
 * @brief Utility functions for L1 cache operations.
 *
 * Provides helper functions for reading/writing cache line data
 * and address decomposition.
 */

#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Cache.h"
#include <cstdint>

 /**
  * @brief Reads 64-bit double-word from cache line.
  * @param line Cache line to read from.
  * @param dw Double-word index (0-3 for 32-byte line).
  * @return 64-bit value.
  */
uint64_t read64_in_line(const CacheLine& line, uint8_t dw);

/**
 * @brief Reads single byte from cache line.
 * @param line Cache line to read from.
 * @param b Byte index (0-31).
 * @return Byte value.
 */
uint8_t read8_in_line(const CacheLine& line, uint8_t b);

/**
 * @brief Writes 64-bit double-word to cache line.
 * @param line Cache line to write to.
 * @param dw Double-word index (0-3).
 * @param v Value to write.
 */
void write64_in_line(CacheLine& line, uint8_t dw, uint64_t v);

/**
 * @brief Writes single byte to cache line.
 * @param line Cache line to write to.
 * @param b Byte index (0-31).
 * @param v Value to write.
 */
void write8_in_line(CacheLine& line, uint8_t b, uint8_t v);

/**
 * @brief Splits address into tag, set index, and offset components.
 * @param addr Full 64-bit address.
 * @return Decomposed address parts.
 */
AddrParts splitAddress(uint64_t addr);