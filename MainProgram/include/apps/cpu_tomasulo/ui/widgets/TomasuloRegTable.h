// ============================================================================
// File: include/apps/cpu_tomasulo/ui/widgets/TomasuloRegTable.h
// ============================================================================

#pragma once

/**
 * @file TomasuloRegTable.h
 * @brief Register file visualization widget for the Tomasulo CPU.
 *
 * Pure renderer — owns no register data. Reads Value, Qi, and Qi_valid
 * directly from a bound TomasuloRegisterFile*.
 *
 * Columns:
 *   REG Number | Qi (ROB#) | Qi Valid | Value (Hex) | Decimal (int64) | Double
 *
 * - Qi_valid == true  → register is waiting for a ROB result ("Waiting").
 * - Qi_valid == false → register holds an architecturally valid value ("Ready").
 *
 * Thread safety:
 *   The caller MUST hold the simulation mutex while calling render().
 *
 * @note
 *   - SRP: Only handles register table display. No data ownership.
 *   - DIP: Depends on const TomasuloRegisterFile*, not concrete storage.
 */

#include <array>
#include <cstdint>
#include <string>

class TomasuloRegisterFile;

class TomasuloRegTable {
public:
    static constexpr int kRegCount = 16;

    /** @brief Named indices matching the ISA register encoding. */
    enum RegIndex : int {
        REG0 = 0, REG1 = 1, REG2 = 2, REG3 = 3,
        REG4 = 4, REG5 = 5, REG6 = 6, REG7 = 7,
        REG8 = 8, REG9 = 9, REG10 = 10, REG11 = 11,
        REG12 = 12,
        UPPER = 13,  ///< High 64 bits of a 128-bit multiply result.
        LOWER = 14,  ///< Low  64 bits of a 128-bit multiply result.
        PEID = 15,  ///< Processing element ID (read-only).
    };

    /** @brief Initialises display name strings for all 16 registers. */
    TomasuloRegTable();

    // ── Data Source Binding ──────────────────────────────────────

    /**
     * @brief Binds the simulation register file as the read-only data source.
     *
     * The pointer must remain valid for the lifetime of this widget.
     * Typically set once by CpuTomasuloState::bindDataSources().
     *
     * @param regs  Pointer to the simulation register file.
     */
    void bindDataSource(const TomasuloRegisterFile* regs);

    // ── Rendering ────────────────────────────────────────────────

    /**
     * @brief Renders the register file table.
     *
     * @pre   The caller holds the simulation mutex.
     * @param id  ImGui widget ID string.
     */
    void render(const char* id);

private:
    const TomasuloRegisterFile* m_dataSource = nullptr;
    std::array<std::string, kRegCount> m_names{};  ///< Pre-formatted register name strings.

    /** @brief Formats a uint64 as a zero-padded 16-digit hex string. */
    static std::string formatHex(uint64_t v);
};