// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/memory/TomasuloRAM.cpp
// ============================================================================

/**
 * @file TomasuloRAM.cpp
 * @brief Implementation of TomasuloRAM.
 *
 * All out-of-range row accesses are silently ignored (read returns 0,
 * write is a no-op) to keep pipeline components safe during transient
 * states. Address-based accessors throw std::out_of_range on misaligned
 * or out-of-bound addresses to catch bugs in the pipeline components.
 */

#include "apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h"
#include <algorithm>
#include <stdexcept>

 // ============================================================================
 // Address → Row Conversion
 // ============================================================================

size_t TomasuloRAM::addressToRow(uint64_t address) {
    if (address % kStep != 0)
        throw std::out_of_range("TomasuloRAM: address not 8-byte aligned.");
    size_t row = static_cast<size_t>(address / kStep);
    if (row >= kTotalRows)
        throw std::out_of_range("TomasuloRAM: address out of range.");
    return row;
}

// ============================================================================
// Row-Index Access
// ============================================================================

uint64_t TomasuloRAM::read(size_t rowIndex) const {
    if (rowIndex >= kTotalRows) return 0;
    return m_data[rowIndex];
}

void TomasuloRAM::write(size_t rowIndex, uint64_t value) {
    if (rowIndex < kTotalRows)
        m_data[rowIndex] = value;
}

// ============================================================================
// Byte-Address Access
// ============================================================================

uint64_t TomasuloRAM::readAddress(uint64_t address) const {
    return m_data[addressToRow(address)];
}

void TomasuloRAM::writeAddress(uint64_t address, uint64_t value) {
    m_data[addressToRow(address)] = value;
}

// ============================================================================
// Bulk Operations
// ============================================================================

size_t TomasuloRAM::loadBlock(const std::vector<uint64_t>& words) {
    size_t count = std::min(words.size(), kTotalRows);
    for (size_t i = 0; i < count; ++i)
        m_data[i] = words[i];
    return count;
}

void TomasuloRAM::clear() {
    m_data.fill(0);
}