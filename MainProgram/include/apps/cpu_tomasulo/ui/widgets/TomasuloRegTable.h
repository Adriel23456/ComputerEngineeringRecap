#pragma once

/**
 * @file TomasuloRegTable.h
 * @brief Register file visualization table widget for Tomasulo CPU.
 *
 * Pure renderer — reads from a bound TomasuloRegisterFile data source.
 * Displays Value, Qi tag, Qi_valid status, plus Hex/Decimal/Double views.
 *
 * The caller MUST hold the simulation mutex while calling render().
 *
 * @note
 *   - SRP: Only handles register table display (no data ownership)
 *   - DIP: Depends on const TomasuloRegisterFile*, not on concrete storage
 */

#include <array>
#include <cstdint>
#include <string>

class TomasuloRegisterFile;

/**
 * @class TomasuloRegTable
 * @brief Table widget for rendering Tomasulo CPU registers with rename tags.
 *
 * Columns: Register | Qi | Qi Valid | Hex Value | Decimal | Double
 */
class TomasuloRegTable {
public:
    static constexpr int kRegCount = 16;

    enum RegIndex : int {
        REG0 = 0, REG1 = 1, REG2 = 2, REG3 = 3,
        REG4 = 4, REG5 = 5, REG6 = 6, REG7 = 7,
        REG8 = 8, REG9 = 9, REG10 = 10, REG11 = 11,
        REG12 = 12,
        UPPER = 13,
        LOWER = 14,
        PEID = 15,
    };

    TomasuloRegTable();

    void bindDataSource(const TomasuloRegisterFile* regs);
    void render(const char* id);

private:
    const TomasuloRegisterFile* m_dataSource = nullptr;
    std::array<std::string, kRegCount> m_names{};

    static std::string formatHex(uint64_t v);
};