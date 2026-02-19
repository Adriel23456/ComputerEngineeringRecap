// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/memory/TomasuloRAM.h
// ============================================================================

#pragma once

/**
 * @file TomasuloRAM.h
 * @brief Word-addressable RAM for the Tomasulo CPU simulation.
 *
 * Stores 50 KB as 6400 × 8-byte rows. Rows are addressed either by
 * row index (0–6399) or by byte-aligned address (0x0, 0x8, …, 0xC7F8).
 *
 * All values default to zero. No pipeline or rendering logic.
 *
 * Usage:
 *   - The assembler loads programs via loadBlock().
 *   - I_Cache and D_Cache read via readAddress() during simulation.
 *   - The binary loader writes via write() under the sim mutex.
 *   - TomasuloRamTable reads via read() for UI display.
 *
 * @note
 *   - SRP: Only manages raw memory storage and access.
 *   - Thread safety: callers must hold the sim mutex during access.
 */

#include <array>
#include <vector>
#include <cstdint>
#include <cstddef>

class TomasuloRAM {
public:
    static constexpr uint64_t kSizeBytes = 50ULL * 1024ULL; ///< 50 KB total.
    static constexpr size_t   kBytesPerRow = 8;               ///< 8 bytes per word.
    static constexpr size_t   kTotalRows = kSizeBytes / kBytesPerRow; ///< 6400 rows.
    static constexpr uint64_t kStep = 0x8ULL;           ///< Address increment.

    TomasuloRAM() { m_data.fill(0); }

    // ── Row-Index Access ─────────────────────────────────────────

    /** @brief Reads the word at a row index. Returns 0 for out-of-range. */
    uint64_t read(size_t rowIndex) const;

    /** @brief Writes a word at a row index. Silently ignores out-of-range. */
    void write(size_t rowIndex, uint64_t value);

    // ── Byte-Address Access ───────────────────────────────────────

    /**
     * @brief Reads the word at a byte-aligned address.
     * @throws std::out_of_range if address is not 8-byte aligned or out of range.
     */
    uint64_t readAddress(uint64_t address) const;

    /**
     * @brief Writes a word at a byte-aligned address.
     * @throws std::out_of_range if address is not 8-byte aligned or out of range.
     */
    void writeAddress(uint64_t address, uint64_t value);

    // ── Bulk Operations ───────────────────────────────────────────

    /**
     * @brief Loads instruction/data words starting at row 0.
     * @param words  Source words (clamped to kTotalRows capacity).
     * @return       Number of words actually written.
     */
    size_t loadBlock(const std::vector<uint64_t>& words);

    /** @brief Zero-fills all 6400 rows. */
    void clear();

    // ── Info ──────────────────────────────────────────────────────
    size_t          totalRows() const { return kTotalRows; }
    const uint64_t* rawData()   const { return m_data.data(); }

private:
    std::array<uint64_t, kTotalRows> m_data{};

    /** @brief Converts a byte address to a row index, throwing on misalignment or overflow. */
    static size_t addressToRow(uint64_t address);
};