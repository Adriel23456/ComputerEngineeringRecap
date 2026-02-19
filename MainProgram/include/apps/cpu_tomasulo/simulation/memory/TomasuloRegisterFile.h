// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/memory/TomasuloRegisterFile.h
// ============================================================================

#pragma once

/**
 * @file TomasuloRegisterFile.h
 * @brief 16 × 64-bit register file with Tomasulo rename tags.
 *
 * Each entry holds:
 *   - value     : the 64-bit architectural register value
 *   - qi        : 5-bit ROB index of the in-flight instruction that will
 *                 produce the next value (only valid when qi_valid is true)
 *   - qi_valid  : true  → register is stale, value is from ROB[qi]
 *                 false → register holds a committed architectural value
 *
 * Register map (4-bit index):
 *   0x0–0xC  REG0–REG12   (general purpose)
 *   0xD      UPPER         (byte address one past the loaded program)
 *   0xE      LOWER         (lower bound address, reset to 0xFFFF…FFFF)
 *   0xF      PEID          (processor element ID, read-only during execution)
 *
 * Reset state:
 *   All values → 0, except LOWER → 0xFFFFFFFFFFFFFFFF.
 *   All Qi → 0, all Qi_valid → false.
 *
 * @note
 *   - SRP: Only manages register storage and named/indexed access.
 *   - Thread safety: callers must hold the sim mutex during access.
 */

#include <array>
#include <string>
#include <cstdint>

 /** @brief Single register entry with its Tomasulo rename tag. */
struct TomasuloRegEntry {
    uint64_t value = 0;      ///< 64-bit architectural value.
    uint8_t  qi = 0;      ///< ROB slot index (5-bit, used when qi_valid).
    bool     qi_valid = false;  ///< True = value is stale, waiting on ROB[qi].
};

class TomasuloRegisterFile {
public:
    static constexpr int kCount = 16;

    /** @brief Named register indices (matches the ISA 4-bit encoding). */
    enum Reg : int {
        REG0 = 0, REG1 = 1, REG2 = 2, REG3 = 3,
        REG4 = 4, REG5 = 5, REG6 = 6, REG7 = 7,
        REG8 = 8, REG9 = 9, REG10 = 10, REG11 = 11,
        REG12 = 12,
        UPPER = 13,
        LOWER = 14,
        PEID = 15,
    };

    TomasuloRegisterFile();

    // ── Value Access ─────────────────────────────────────────────

    /** @brief Returns the architectural value of register idx. Returns 0 on out-of-range. */
    uint64_t get(int idx) const;

    /** @brief Sets the architectural value of register idx. Silently ignores out-of-range. */
    void set(int idx, uint64_t value);

    /** @brief Returns the value of a register by name (e.g. "REG5", "UPPER"). */
    uint64_t getByName(const std::string& name) const;

    /** @brief Sets the value of a register by name. */
    void setByName(const std::string& name, uint64_t value);

    // ── Full Entry Access ─────────────────────────────────────────

    const TomasuloRegEntry& getEntry(int idx) const;
    TomasuloRegEntry& getEntry(int idx);

    // ── Rename Tag Access ─────────────────────────────────────────

    uint8_t getQi(int idx)        const;
    bool    getQiValid(int idx)   const;
    void    setQi(int idx, uint8_t qi, bool valid);

    /** @brief Resets all registers to power-on state. */
    void reset();

    /** @brief Raw pointer to the entry array (used by TomasuloRegTable). */
    const TomasuloRegEntry* rawEntries() const { return m_regs.data(); }

private:
    std::array<TomasuloRegEntry, kCount> m_regs{};

    /** @brief Converts a register name string to its 0-based index, or -1 on failure. */
    static int nameToIndex(const std::string& name);
};