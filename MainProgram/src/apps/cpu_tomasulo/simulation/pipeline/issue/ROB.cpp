// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/issue/ROB.cpp
// ============================================================================

/**
 * @file ROB.cpp
 * @brief Component 10 implementation — Reorder Buffer (32-entry circular queue).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/issue/ROB.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Helpers
 // ============================================================================

bool ROB::isFull() const {
    // Full when tail has wrapped all the way around to meet head (and is not empty).
    return !m_empty && (m_head == m_tail);
}

int ROB::count() const {
    if (m_empty)          return 0;
    if (m_head == m_tail) return ROB_SIZE;
    return (m_tail >= m_head)
        ? (m_tail - m_head)
        : (ROB_SIZE - m_head + m_tail);
}

// ============================================================================
// Evaluate (combinational)
// ============================================================================

void ROB::evaluate(TomasuloBus& bus) {

    // ── ROB status signals ────────────────────────────────────────
    bus.ROBTail_o = m_tail;   ///< Next free slot index (used by issue as the new ROB tag).
    bus.ROBFull_o = isFull(); ///< Asserted to stall issue when no free entries remain.
    bus.ROBHead_o = m_head;   ///< Head index (for commit and store-forwarding).

    // ── Head entry exposure (consumed by Commit_Unit) ────────────
    if (!m_empty) {
        const Entry& h = m_entries[m_head];
        bus.ROBHeadBusy_o = h.busy;
        bus.ROBHeadReady_o = h.ready;
        bus.ROBHeadType_o = h.type;
        bus.ROBHeadDestReg_o = h.destReg;
        bus.ROBHeadValue_o = h.value;
        bus.ROBHeadException_o = h.exception;
        bus.ROBHeadPC_o = h.pc;
        bus.ROBHeadOp_o = h.op;
        bus.ROBHeadFlagsResult_o = h.flagsResult;
        bus.ROBHeadFlagsValid_o = h.flagsValid;
        bus.ROBHeadMispredict_o = h.mispredict;
        bus.ROBHeadBranchTarget_o = h.branchTarget;
        bus.ROBHeadStoreAddr_o = h.storeAddr;
        bus.ROBHeadStoreData_o = h.storeData;
        bus.ROBHeadStoreReady_o = h.storeReady;
        bus.ROBHeadSourceStation_o = h.sourceStation;
    }
    else {
        bus.ROBHeadBusy_o = false;
        bus.ROBHeadReady_o = false;
        bus.ROBHeadOp_o = 0;
    }

    // ── Read port 1: operand forwarding for source register Rn ───
    // If the register has a pending Qi tag, check whether the ROB entry
    // already holds a ready result that can bypass to the issuing RS.
    if (bus.RD1_QiValid_o) {
        uint8_t idx = bus.RD1_Qi_o;
        if (idx < ROB_SIZE && m_entries[idx].busy) {
            bus.ROBReadReady1_o = m_entries[idx].ready;
            bus.ROBReadValue1_o = m_entries[idx].value;
        }
        else {
            bus.ROBReadReady1_o = false;
            bus.ROBReadValue1_o = 0;
        }
    }
    else {
        bus.ROBReadReady1_o = false;
    }

    // ── Read port 2: operand forwarding for source register Rm ───
    if (bus.RD2_QiValid_o) {
        uint8_t idx = bus.RD2_Qi_o;
        if (idx < ROB_SIZE && m_entries[idx].busy) {
            bus.ROBReadReady2_o = m_entries[idx].ready;
            bus.ROBReadValue2_o = m_entries[idx].value;
        }
        else {
            bus.ROBReadReady2_o = false;
            bus.ROBReadValue2_o = 0;
        }
    }
    else {
        bus.ROBReadReady2_o = false;
    }

    // ── Flags read port: forwarding for flag-consuming instructions ─
    // Only forward if the entry is ready AND has produced valid flags.
    if (bus.FlagsQiValid_o) {
        uint8_t idx = bus.FlagsQi_o;
        if (idx < ROB_SIZE
            && m_entries[idx].busy
            && m_entries[idx].ready
            && m_entries[idx].flagsValid)
        {
            bus.ROBReadFlagsReady_o = true;
            bus.ROBReadFlagsResult_o = m_entries[idx].flagsResult;
        }
        else {
            bus.ROBReadFlagsReady_o = false;
            bus.ROBReadFlagsResult_o = 0;
        }
    }
    else {
        bus.ROBReadFlagsReady_o = false;
    }
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void ROB::clockEdge(TomasuloBus& bus) {

    // ── 1. Flush: discard all speculative entries ─────────────────
    if (bus.Flush_o) {
        for (auto& e : m_entries) e.busy = false;
        m_head = 0;
        m_tail = 0;
        m_empty = true;
        return;
    }

    // ── 2. Commit pop: retire the head entry ──────────────────────
    if (bus.CommitPop_i && !m_empty) {
        m_entries[m_head].busy = false;
        m_head = nextIdx(m_head);
        if (m_head == m_tail) m_empty = true;
    }

    // ── 3. CDB snoop channel A: mark matching entry ready ────────
    if (bus.CDBA_Valid_o) {
        uint8_t tag = bus.CDBA_ROBTag_o;
        if (tag < ROB_SIZE && m_entries[tag].busy && !m_entries[tag].ready) {
            m_entries[tag].value = bus.CDBA_Value_o;
            m_entries[tag].exception = bus.CDBA_Exception_o;
            m_entries[tag].ready = true;
            if (bus.CDBA_FlagsValid_o) {
                m_entries[tag].flagsResult = bus.CDBA_Flags_o;
                m_entries[tag].flagsValid = true;
            }
        }
    }

    // ── 3. CDB snoop channel B ────────────────────────────────────
    if (bus.CDBB_Valid_o) {
        uint8_t tag = bus.CDBB_ROBTag_o;
        if (tag < ROB_SIZE && m_entries[tag].busy && !m_entries[tag].ready) {
            m_entries[tag].value = bus.CDBB_Value_o;
            m_entries[tag].exception = bus.CDBB_Exception_o;
            m_entries[tag].ready = true;
            if (bus.CDBB_FlagsValid_o) {
                m_entries[tag].flagsResult = bus.CDBB_Flags_o;
                m_entries[tag].flagsValid = true;
            }
        }
    }

    // ── 4. Branch result snoop ────────────────────────────────────
    if (bus.BrExResult_Valid_o) {
        uint8_t tag = bus.BrExResult_ROBTag_o;
        if (tag < ROB_SIZE && m_entries[tag].busy) {
            m_entries[tag].ready = true;
            m_entries[tag].mispredict = bus.BrExResult_Mispredict_o;
            m_entries[tag].branchTarget = bus.BrExResult_Target_o;
        }
    }

    // ── 5. Store-complete snoop (SB0 and SB1) ────────────────────
    if (bus.SB0_StComplete_Valid_o) {
        uint8_t tag = bus.SB0_StComplete_ROBTag_o;
        if (tag < ROB_SIZE && m_entries[tag].busy) {
            m_entries[tag].storeAddr = bus.SB0_StComplete_Addr_o;
            m_entries[tag].storeData = bus.SB0_StComplete_Data_o;
            m_entries[tag].storeReady = true;
            m_entries[tag].ready = true;
        }
    }
    if (bus.SB1_StComplete_Valid_o) {
        uint8_t tag = bus.SB1_StComplete_ROBTag_o;
        if (tag < ROB_SIZE && m_entries[tag].busy) {
            m_entries[tag].storeAddr = bus.SB1_StComplete_Addr_o;
            m_entries[tag].storeData = bus.SB1_StComplete_Data_o;
            m_entries[tag].storeReady = true;
            m_entries[tag].ready = true;
        }
    }

    // ── 6. Allocate new entry at tail ─────────────────────────────
    if (bus.ROBAlloc_o && !isFull()) {
        Entry& e = m_entries[m_tail];
        e.busy = true;
        e.ready = false;
        e.type = bus.AllocType_o;
        e.destReg = bus.Dest_in_o;
        e.value = 0;
        e.exception = 0;
        e.pc = bus.PCCurrent_o;
        e.op = bus.Op_in_o;
        e.sourceStation = bus.AllocSourceStation_o;
        e.flagsResult = 0;
        e.flagsValid = false;
        e.modifiesFlags = bus.ModifiesFlags_o;
        e.predicted = bus.AllocPredicted_o;
        e.mispredict = false;
        e.branchTarget = 0;
        e.storeAddr = 0;
        e.storeData = 0;
        e.storeReady = false;

        // NOP (0x4D) and SWI (0x4C) have no execution unit — mark ready
        // immediately so they do not permanently block commit.
        if (e.sourceStation == 0x0F) {
            e.ready = true;
        }

        m_tail = nextIdx(m_tail);
        m_empty = false;
    }
}

// ============================================================================
// UI Snapshot
// ============================================================================

ROB::EntryView ROB::getEntryView(int idx) const {
    EntryView v{};
    if (idx < 0 || idx >= ROB_SIZE) return v;
    const Entry& e = m_entries[idx];

    // ── Core fields ───────────────────────────────────────────────
    v.busy = e.busy;
    v.ready = e.ready;
    v.type = e.type;
    v.destReg = e.destReg;
    v.value = e.value;
    v.exception = e.exception;
    v.pc = e.pc;
    v.sourceStation = e.sourceStation;

    // ── Flags fields ──────────────────────────────────────────────
    v.flagsResult = e.flagsResult;
    v.flagsValid = e.flagsValid;
    v.modifiesFlags = e.modifiesFlags;

    // ── Branch fields ─────────────────────────────────────────────
    v.predicted = e.predicted;
    v.mispredict = e.mispredict;
    v.branchTarget = e.branchTarget;

    // ── Store fields ──────────────────────────────────────────────
    v.storeAddr = e.storeAddr;
    v.storeData = e.storeData;
    v.storeReady = e.storeReady;

    return v;
}

// ============================================================================
// Reset
// ============================================================================

void ROB::reset() {
    for (auto& e : m_entries) e.busy = false;
    m_head = 0;
    m_tail = 0;
    m_empty = true;
    std::cout << "[ROB] reset()\n";
}