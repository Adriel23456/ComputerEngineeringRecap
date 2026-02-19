// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/issue/RegisterFilePipeline.h
// ============================================================================

#pragma once

/**
 * @file RegisterFilePipeline.h
 * @brief Component 9: Register File pipeline interface.
 *
 * Wraps the architectural TomasuloRegisterFile (which stores committed values)
 * and adds per-register Qi rename tags for Tomasulo out-of-order execution.
 *
 * Read ports (combinational — evaluated every cycle):
 *   Port 1 (Rn)      → RD1_Value_o / RD1_Qi_o / RD1_QiValid_o
 *   Port 2 (Rm)      → RD2_Value_o / RD2_Qi_o / RD2_QiValid_o
 *   Port 3 (Store)   → RD_StoreValue_o / RD_StoreQi_o / RD_StoreQiValid_o
 *                      (also reads Rm — the value to be written to memory by STR)
 *   Seg. regs        → UPPER_o, LOWER_o (always architectural, never renamed)
 *
 * Write (clockEdge — priority order):
 *   1. Flush    → clear all Qi tags (no architectural value change).
 *   2. Commit   → write architectural value; clear tag if it still points to
 *                 the committed ROB entry (a later issue may have overwritten it).
 *   3. Issue    → write ROBTail as the Qi tag for the destination register.
 *   4. Sync     → copy tags to the architectural file for UI display.
 *
 * @note SRP: Only handles register read/write and tag management.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>
#include <array>

class TomasuloRegisterFile;

class RegisterFilePipeline : public ITomasuloComponent {
public:
    static constexpr int NUM_REGS = 16;  ///< REG0–REG12, UPPER(13), LOWER(14), PEID(15).

    explicit RegisterFilePipeline(TomasuloRegisterFile* archFile);

    const char* name() const override { return "RegisterFile"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    TomasuloRegisterFile* m_archFile = nullptr; ///< Architectural register file (committed values).

    /** @brief ROB index that will produce this register's next value. */
    std::array<uint8_t, NUM_REGS> m_qi{};
    /** @brief True when the corresponding @p m_qi entry is valid (register is being renamed). */
    std::array<bool, NUM_REGS> m_qiValid{};

    /**
     * @brief Shared read-port helper.
     *
     * If the register has an outstanding rename tag (qiValid), returns the tag
     * so the RS can wait for it on the CDB.  Otherwise returns the current
     * architectural value directly.
     *
     * @param regIdx   Register index to read.
     * @param[out] value    Architectural value (valid only when qiValid is false).
     * @param[out] qi       ROB tag (valid only when qiValid is true).
     * @param[out] qiValid  Whether the register is currently renamed.
     */
    void readPort(uint8_t regIdx, const TomasuloBus& bus,
        uint64_t& value, uint8_t& qi, bool& qiValid) const;
};