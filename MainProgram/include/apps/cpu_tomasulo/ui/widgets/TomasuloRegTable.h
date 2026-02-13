#pragma once

/**
 * @file TomasuloRegTable.h
 * @brief Register file visualization table widget for Tomasulo CPU.
 *
 * Pure renderer — reads from a bound TomasuloRegisterFile data source.
 * Does NOT store its own copy of register values.
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

class TomasuloRegisterFile;   // forward declaration — simulation layer

/**
 * @class TomasuloRegTable
 * @brief Table widget for rendering Tomasulo CPU registers.
 *
 * Features:
 * - 16 registers with 4-bit binary codes
 * - Reads directly from TomasuloRegisterFile (single source of truth)
 * - Hex, Decimal, Double columns
 * - Context menu to copy values
 */
class TomasuloRegTable {
public:
    static constexpr int kRegCount = 16;

    /**
     * @enum RegIndex
     * @brief Register indices matching 4-bit binary encoding.
     */
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

    // ── Data source binding ─────────────────────────────────────

    /**
     * @brief Binds the register data source for rendering.
     * @param regs  Pointer to simulation register file.
     */
    void bindDataSource(const TomasuloRegisterFile* regs);

    // ── Rendering ───────────────────────────────────────────────

    /**
     * @brief Renders the register table.
     *
     * PRECONDITION: The caller holds the simulation mutex.
     *
     * @param id  ImGui widget ID.
     */
    void render(const char* id);

private:
    const TomasuloRegisterFile* m_dataSource = nullptr;
    std::array<std::string, kRegCount> m_names{};   ///< Display names (constant)

    static std::string formatHex(uint64_t v);
};