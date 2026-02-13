#pragma once

/**
 * @file TomasuloRegisterFile.h
 * @brief Simulation-side register file for Tomasulo CPU.
 *
 * 16 x 64-bit general-purpose registers.
 * No UI logic — the UI widget reads from this.
 *
 * @note SRP: Only manages register storage and named access.
 */

#include <array>
#include <string>
#include <cstdint>

 /**
  * @class TomasuloRegisterFile
  * @brief 16 x 64-bit register file for Tomasulo simulation.
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

    uint64_t get(int idx) const;
    void     set(int idx, uint64_t value);

    uint64_t getByName(const std::string& name) const;
    void     setByName(const std::string& name, uint64_t value);

    /** @brief Reset all registers to power-on defaults. */
    void reset();

    const uint64_t* rawData() const { return m_regs.data(); }

private:
    std::array<uint64_t, kCount> m_regs{};

    static int nameToIndex(const std::string& name);
};