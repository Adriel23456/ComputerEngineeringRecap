#pragma once

/**
 * @file TomasuloRegisterFile.h
 * @brief Simulation-side register file for Tomasulo CPU.
 *
 * 16 x 64-bit general-purpose registers, each with Tomasulo
 * rename tags (Qi, Qi_valid) for out-of-order execution.
 *
 * @note SRP: Only manages register storage and named access.
 */

#include <array>
#include <string>
#include <cstdint>

 /**
  * @struct TomasuloRegEntry
  * @brief Single register entry with value and rename tag.
  */
struct TomasuloRegEntry {
    uint64_t value = 0;     ///< 64-bit architectural value
    uint8_t  qi = 0;     ///< ROB index that will produce the next value (5-bit)
    bool     qi_valid = false;  ///< True = value is stale, waiting on ROB[qi]
};

/**
 * @class TomasuloRegisterFile
 * @brief 16 x 64-bit register file with Tomasulo rename tags.
 *
 * Register map (4-bit index):
 *   0x0–0xC  REG0 – REG12  (general purpose)
 *   0xD      UPPER          (upper program bound)
 *   0xE      LOWER          (lower program bound, default 0xFFFF…)
 *   0xF      PEID           (processor element ID)
 */
class TomasuloRegisterFile {
public:
    static constexpr int kCount = 16;

    // Named indices
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

    // ── Value access (backward-compatible) ──────────────────────

    uint64_t get(int idx) const;
    void     set(int idx, uint64_t value);

    uint64_t getByName(const std::string& name) const;
    void     setByName(const std::string& name, uint64_t value);

    // ── Full entry access ───────────────────────────────────────

    const TomasuloRegEntry& getEntry(int idx) const;
    TomasuloRegEntry& getEntry(int idx);

    // ── Tag access ──────────────────────────────────────────────

    uint8_t getQi(int idx) const;
    bool    getQiValid(int idx) const;
    void    setQi(int idx, uint8_t qi, bool valid);

    /** @brief Reset all registers to power-on defaults. */
    void reset();

    const TomasuloRegEntry* rawEntries() const { return m_regs.data(); }

private:
    std::array<TomasuloRegEntry, kCount> m_regs{};

    static int nameToIndex(const std::string& name);
};