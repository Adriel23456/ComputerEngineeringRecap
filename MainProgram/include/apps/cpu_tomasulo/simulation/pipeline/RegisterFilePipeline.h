#pragma once

/**
 * @file RegisterFilePipeline.h
 * @brief Component 9: Register File pipeline interface.
 *
 * Wraps the existing TomasuloRegisterFile (which stores architectural values)
 * and adds per-register Qi tags for Tomasulo renaming.
 *
 * Read ports (combinational):
 *   RD1 = Rn (source 1)
 *   RD2 = Rm (source 2) OR Imm (if UseImm)
 *   RD_Store = Rd (for STR — the value to be stored)
 *
 * Write (clockEdge):
 *   - Issue: Write ROBTail as Qi tag for destination register
 *   - Commit: Write architectural value + clear tag if matching
 *   - Flush: Clear all Qi tags
 *
 * @note SRP: Only handles register read/write and tag management.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>
#include <array>

class TomasuloRegisterFile;

class RegisterFilePipeline : public ITomasuloComponent {
public:
    static constexpr int NUM_REGS = 16;  // REG0-REG12, UPPER(13), LOWER(14), PEID(15)

    explicit RegisterFilePipeline(TomasuloRegisterFile* archFile);

    const char* name() const override { return "RegisterFile"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    TomasuloRegisterFile* m_archFile = nullptr;

    // Qi tags: ROB index that will produce this register's value
    // qiValid[r] = true means register r is waiting on ROB entry qi[r]
    std::array<uint8_t, NUM_REGS> m_qi{};
    std::array<bool, NUM_REGS>    m_qiValid{};

    void readPort(uint8_t regIdx, const TomasuloBus& bus,
        uint64_t& value, uint8_t& qi, bool& qiValid) const;
};