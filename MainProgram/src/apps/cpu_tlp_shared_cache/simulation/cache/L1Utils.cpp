// ============================================================================
// File: src/apps/cpu_tlp_shared_cache/simulation/cache/L1Utils.cpp
// ============================================================================

/**
 * @file L1Utils.cpp
 * @brief Utility functions for L1 cache line access.
 */

#include "apps/cpu_tlp_shared_cache/simulation/cache/L1Utils.h"
#include <cstring>

 /**
  * @brief Reads 64-bit double-word from cache line.
  * @param line Cache line to read from.
  * @param dw Double-word index (0-3 for 32-byte line).
  * @return 64-bit value at specified position.
  */
uint64_t read64_in_line(const CacheLine& line, uint8_t dw) {
    uint64_t v;
    std::memcpy(&v, &line.data[dw * 8], 8);
    return v;
}

/**
 * @brief Reads single byte from cache line.
 * @param line Cache line to read from.
 * @param b Byte index (0-31).
 * @return Byte value at specified position.
 */
uint8_t read8_in_line(const CacheLine& line, uint8_t b) {
    return line.data[b];
}

/**
 * @brief Writes 64-bit double-word to cache line.
 * @param line Cache line to write to.
 * @param dw Double-word index (0-3).
 * @param v Value to write.
 */
void write64_in_line(CacheLine& line, uint8_t dw, uint64_t v) {
    std::memcpy(&line.data[dw * 8], &v, 8);
}

/**
 * @brief Writes single byte to cache line.
 * @param line Cache line to write to.
 * @param b Byte index (0-31).
 * @param v Byte value to write.
 */
void write8_in_line(CacheLine& line, uint8_t b, uint8_t v) {
    line.data[b] = v;
}

/**
 * @brief Splits 64-bit address into tag, set index, and offset.
 * @param addr Full 64-bit address.
 * @return Decomposed address parts.
 */
AddrParts splitAddress(uint64_t addr) {
    constexpr uint64_t OFF_MASK = (1ULL << OFFSET_BITS) - 1ULL;
    constexpr uint64_t IDX_MASK = ((1ULL << INDEX_BITS) - 1ULL) << OFFSET_BITS;

    const uint64_t off = addr & OFF_MASK;
    const uint64_t set = (addr & IDX_MASK) >> OFFSET_BITS;
    const uint64_t tag = addr >> (OFFSET_BITS + INDEX_BITS);

    return AddrParts{
        tag,
        static_cast<uint32_t>(set),
        static_cast<uint32_t>(off),
        static_cast<uint8_t>(off >> 3),         // dw_in_line
        static_cast<uint8_t>(off & 0x1F)        // byte_in_line
    };
}