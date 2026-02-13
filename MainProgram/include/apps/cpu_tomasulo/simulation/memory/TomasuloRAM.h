#pragma once

/**
 * @file TomasuloRAM.h
 * @brief Simulation-side RAM for Tomasulo CPU.
 *
 * Pure data store — 50 KB of 64-bit-word-addressable memory.
 * No UI or rendering logic. The UI widgets read from this.
 *
 * @note SRP: Only manages raw memory storage and access.
 */

#include <array>
#include <vector>
#include <cstdint>
#include <cstddef>

 /**
  * @class TomasuloRAM
  * @brief 50 KB word-addressable RAM for the Tomasulo simulation.
  *
  * Memory layout:
  * - 6400 rows of 8 bytes each  (50 KB total)
  * - Addresses: 0x0, 0x8, 0x10, … , 0xC7F8
  * - All values default to 0
  */
class TomasuloRAM {
public:
    static constexpr uint64_t kSizeBytes = 50ULL * 1024ULL;
    static constexpr size_t   kBytesPerRow = 8;
    static constexpr size_t   kTotalRows = kSizeBytes / kBytesPerRow; // 6400
    static constexpr uint64_t kStep = 0x8ULL;

    TomasuloRAM() { m_data.fill(0); }

    // ── Word-level access ───────────────────────────────────────

    /** @brief Read word at row index. */
    uint64_t read(size_t rowIndex) const;

    /** @brief Write word at row index. */
    void write(size_t rowIndex, uint64_t value);

    /** @brief Read word by byte-aligned address (must be multiple of 8). */
    uint64_t readAddress(uint64_t address) const;

    /** @brief Write word by byte-aligned address. */
    void writeAddress(uint64_t address, uint64_t value);

    // ── Bulk operations ─────────────────────────────────────────

    /**
     * @brief Loads a vector of 64-bit words starting at row 0.
     * @param words Instruction / data words.
     * @return Number of words actually written (clamped to capacity).
     */
    size_t loadBlock(const std::vector<uint64_t>& words);

    /** @brief Zero-fill all memory. */
    void clear();

    // ── Info ─────────────────────────────────────────────────────

    size_t              totalRows()  const { return kTotalRows; }
    const uint64_t* rawData()    const { return m_data.data(); }

private:
    std::array<uint64_t, kTotalRows> m_data{};

    static size_t addressToRow(uint64_t address);
};