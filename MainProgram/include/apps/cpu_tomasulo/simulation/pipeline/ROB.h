#pragma once

/**
 * @file ROB.h
 * @brief Component 10: Reorder Buffer (32-entry circular queue).
 *
 * Supports:
 *   - Allocation at tail (issue stage)
 *   - CDB snooping to mark entries ready
 *   - Branch result snooping
 *   - Store-complete snooping
 *   - Read ports for operand forwarding
 *   - Head entry exposure for Commit_Unit
 *   - Commit pop and full flush
 *
 * @note SRP: Only manages ROB entries and their lifecycle.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>
#include <array>

class ROB : public ITomasuloComponent {
public:
    static constexpr int ROB_SIZE = 32;

    const char* name() const override { return "ROB"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

    /** @brief Entry count (for debug/UI). */
    int count() const;

private:
    struct Entry {
        bool     busy = false;
        bool     ready = false;
        uint8_t  type = 0;    // AllocType (3 bits)
        uint8_t  destReg = 0;    // 4 bits
        uint64_t value = 0;
        uint8_t  exception = 0;
        uint64_t pc = 0;
        uint8_t  op = 0;
        uint8_t  sourceStation = 0; // 4 bits

        // Flags
        uint8_t  flagsResult = 0;   // 4 bits (N,Z,C,V)
        bool     flagsValid = false;
        bool     modifiesFlags = false;

        // Branch-specific
        bool     predicted = false;
        bool     mispredict = false;
        uint64_t branchTarget = 0;

        // Store-specific
        uint64_t storeAddr = 0;
        uint64_t storeData = 0;
        bool     storeReady = false;
    };

    std::array<Entry, ROB_SIZE> m_entries{};
    uint8_t m_head = 0;
    uint8_t m_tail = 0;
    bool    m_empty = true;

    uint8_t nextIdx(uint8_t idx) const { return (idx + 1) % ROB_SIZE; }
    bool    isFull() const;
};