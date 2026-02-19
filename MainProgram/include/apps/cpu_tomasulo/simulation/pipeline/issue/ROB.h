// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/issue/ROB.h
// ============================================================================

#pragma once

/**
 * @file ROB.h
 * @brief Component 10: Reorder Buffer — 32-entry circular queue.
 *
 * Central in-order tracking structure for the out-of-order pipeline.
 * Each entry is allocated at the tail on issue and retired from the head
 * on commit, ensuring in-order retirement despite out-of-order execution.
 *
 * Features:
 *   - Tail allocation  : new entry assigned to the issuing instruction.
 *   - CDB snooping     : two CDB channels (A/B) mark matching entries ready.
 *   - Branch snooping  : Branch_Executor writes mispredict + target.
 *   - Store snooping   : Store Buffers write addr/data when address is resolved.
 *   - Read ports       : forward ready values to RS operand capture (Tomasulo).
 *   - Head exposure    : all head-entry fields driven on bus for Commit_Unit.
 *   - Commit pop       : head advanced when Commit_Unit raises CommitPop_i.
 *   - Full flush       : all entries cleared on branch misprediction.
 *
 * Entry lifecycle:
 *   ALLOC (busy=true, ready=false) → COMPLETE (ready=true) → COMMIT (busy=false)
 *
 * Special case:
 *   NOP / SWI instructions (sourceStation == 0x0F) are marked ready immediately
 *   at allocation because no execution unit will complete them.
 *
 * @note SRP: Only manages ROB entries and their lifecycle.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>
#include <array>

class ROB : public ITomasuloComponent {
public:
    static constexpr int ROB_SIZE = 32; ///< Number of ROB entries (must be a power of two).

    const char* name() const override { return "ROB"; }
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

    /** @brief Number of currently allocated (busy) entries — for debug/UI. */
    int count() const;

    // ── UI read-only snapshot ────────────────────────────────────
    /**
     * @brief Flat view of a single ROB entry for the UI/debug panel.
     *
     * Copied out of the internal @c Entry struct so the UI does not need
     * to hold the sim mutex for longer than necessary.
     */
    struct EntryView {
        // Core
        bool     busy = false;
        bool     ready = false;
        uint8_t  type = 0;   ///< AllocType: ALU/Load/Store/Branch/…
        uint8_t  destReg = 0;
        uint64_t value = 0;
        uint8_t  exception = 0;
        uint64_t pc = 0;
        uint8_t  sourceStation = 0;
        // Flags
        uint8_t  flagsResult = 0;   ///< NZCV bits produced by this instruction.
        bool     flagsValid = false;
        bool     modifiesFlags = false;
        // Branch
        bool     predicted = false;
        bool     mispredict = false;
        uint64_t branchTarget = 0;
        // Store
        uint64_t storeAddr = 0;
        uint64_t storeData = 0;
        bool     storeReady = false;
    };

    EntryView getEntryView(int idx) const; ///< Snapshot entry @p idx for the UI.
    uint8_t   head() const { return m_head; }
    uint8_t   tail() const { return m_tail; }
    bool      empty() const { return m_empty; }

private:
    /** @brief Internal ROB entry — all pipeline-visible fields. */
    struct Entry {
        bool     busy = false;
        bool     ready = false;
        uint8_t  type = 0;    ///< AllocType (3 bits).
        uint8_t  destReg = 0;    ///< Destination architectural register (4 bits).
        uint64_t value = 0;    ///< Result value (written by CDB snoop or store complete).
        uint8_t  exception = 0;    ///< Exception code (0 = none).
        uint64_t pc = 0;    ///< PC of the issuing instruction.
        uint8_t  op = 0;    ///< Opcode (for commit-side behaviour decisions).
        uint8_t  sourceStation = 0;    ///< Which RS/buffer issued this entry (4 bits).

        // Flags
        uint8_t  flagsResult = 0;    ///< NZCV produced by this instruction.
        bool     flagsValid = false; ///< True when flagsResult has been written.
        bool     modifiesFlags = false; ///< True if this instruction is a flags producer.

        // Branch-specific
        bool     predicted = false; ///< Static predictor's guess (taken/not-taken).
        bool     mispredict = false; ///< True when Branch_Executor detects a misprediction.
        uint64_t branchTarget = 0;    ///< Resolved branch target address.

        // Store-specific
        uint64_t storeAddr = 0;    ///< Effective address resolved by AGU.
        uint64_t storeData = 0;    ///< Data to be written to memory.
        bool     storeReady = false; ///< True when both address and data are available.
    };

    std::array<Entry, ROB_SIZE> m_entries{};
    uint8_t m_head = 0;
    uint8_t m_tail = 0;
    bool    m_empty = true;

    /** @brief Wrapping increment for the circular buffer. */
    uint8_t nextIdx(uint8_t idx) const { return (idx + 1) % ROB_SIZE; }

    /** @brief True when tail == head and the buffer is not empty. */
    bool isFull() const;
};