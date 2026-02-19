// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/buffers/LoadBuffer.cpp
// ============================================================================

/**
 * @file LoadBuffer.cpp
 * @brief Implementation of the LoadBuffer base class (Components 14–16).
 *
 * evaluate() — combinational outputs (reflect current m_* state):
 *   - writeBusy: LBx_Busy_o for Control_Unit.
 *   - writeAGUReq: request address calculation when base is ready
 *                  and address has not yet been computed.
 *   - writeMemReq: request cache read when address is ready,
 *                  no segfault, and memory has not yet responded.
 *   - writeCDBReq: request CDB slot when result is available
 *                  (segfault exception OR loaded data).
 *
 * clockEdge() — sequential state updates:
 *   1. Flush   — wipe state.
 *   2. Free    — Commit_Unit released this slot.
 *   3. Allocate — latch op, offset, ROBTag; resolve base operand (3-tier).
 *   4. CDB snoop — update base if a matching broadcast arrives.
 *   5. Latch cdbRequested — mark request sent (before checking mem response).
 *   6. Memory response — latch loaded data.
 *   7. CDB stall — clear cdbRequested so it retries next cycle.
 *
 * Note on step ordering:
 *   Step 5 (latch cdbRequested) intentionally precedes step 6 (mem response)
 *   so that the very first cycle with m_memDoneInt==true also marks
 *   cdbRequested=true, preventing a spurious second CDB request.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/buffers/LoadBuffer.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

LoadBuffer::LoadBuffer(ID id) : m_id(id) {}

const char* LoadBuffer::idStr() const {
    switch (m_id) {
    case ID::LB0: return "LB0";
    case ID::LB1: return "LB1";
    case ID::LB2: return "LB2";
    }
    return "LB?";
}

// ============================================================================
// Evaluate (combinational)
// ============================================================================

void LoadBuffer::evaluate(TomasuloBus& bus) {
    // ── Busy signal → Control_Unit ────────────────────────────────
    writeBusy(bus, m_busy);

    // ── AGU request: base ready && address not yet computed ───────
    bool aguReq = m_busy && !m_baseTagValid && !m_addressReady;
    writeAGUReq(bus, aguReq);
    writeAGUBase(bus, m_baseValue);
    writeAGUOffset(bus, m_offset);
    writeAGUROBTag(bus, m_destROBTag);

    // ── Memory request: address valid, no segfault, not yet done ──
    bool memReq = m_busy && m_addressReady && !m_segFault && !m_memDoneInt;
    writeMemReq(bus, memReq);
    writeMemAddr(bus, m_address);
    writeMemOp(bus, m_op);
    writeMemROBTag(bus, m_destROBTag);

    // ── CDB request (segfault exception OR normal data) ───────────
    bool cdbReq = false;
    if (m_segFault) {
        cdbReq = m_busy && m_addressReady && !m_cdbRequested;
        writeCDBValue(bus, 0);
        writeCDBException(bus, 0x2);  // SegFault exception code
    }
    else {
        cdbReq = m_busy && m_memDoneInt && !m_cdbRequested;
        writeCDBValue(bus, m_loadedData);
        writeCDBException(bus, 0x0);
    }
    writeCDBReq(bus, cdbReq);
    writeCDBROBTag(bus, m_destROBTag);
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void LoadBuffer::clockEdge(TomasuloBus& bus) {

    // ── 1. Flush ──────────────────────────────────────────────────
    if (bus.Flush_o) {
        m_busy = false;
        return;
    }

    // ── 2. Free ───────────────────────────────────────────────────
    if (readFree(bus)) {
        m_busy = false;
        return;
    }

    // ── 3. Allocate ───────────────────────────────────────────────
    if (readAlloc(bus) && !m_busy) {
        m_busy = true;
        m_op = bus.Op_in_o;
        m_offset = bus.ImmExt_o;
        m_destROBTag = bus.ROBTail_o;
        m_addressReady = false;
        m_segFault = false;
        m_memDoneInt = false;
        m_cdbRequested = false;

        // 3-tier base operand resolution
        if (!bus.RD1_QiValid_o) {
            m_baseValue = bus.RD1_Value_o;
            m_baseTagValid = false;
        }
        else if (bus.ROBReadReady1_o) {
            m_baseValue = bus.ROBReadValue1_o;
            m_baseTagValid = false;
        }
        else {
            m_baseTag = bus.RD1_Qi_o;
            m_baseTagValid = true;
        }
    }

    // ── 4. CDB snoop ──────────────────────────────────────────────
    if (m_busy && m_baseTagValid) {
        if (bus.CDBA_Valid_o && m_baseTag == bus.CDBA_ROBTag_o) {
            m_baseValue = bus.CDBA_Value_o;
            m_baseTagValid = false;
        }
        if (bus.CDBB_Valid_o && m_baseTagValid && m_baseTag == bus.CDBB_ROBTag_o) {
            m_baseValue = bus.CDBB_Value_o;
            m_baseTagValid = false;
        }
    }

    // ── 5. Latch cdbRequested (before checking mem response) ──────
    if (m_busy && (m_memDoneInt || m_segFault) && !m_cdbRequested) {
        m_cdbRequested = true;
    }

    // ── 6. Memory response ────────────────────────────────────────
    if (m_busy && readMemDone(bus)) {
        m_loadedData = readMemData(bus);
        m_memDoneInt = true;
    }

    // ── 7. CDB stall ──────────────────────────────────────────────
    if (m_busy && readCDBStall(bus)) {
        m_cdbRequested = false;
    }

    // ── 8. AGU done ───────────────────────────────────────────────
    if (m_busy && readAGUDone(bus)) {
        m_address = readAGUAddress(bus);
        m_addressReady = true;
        m_segFault = readAGUSegFault(bus);
    }
}

// ============================================================================
// Reset
// ============================================================================

void LoadBuffer::reset() {
    m_busy = false;
    m_op = 0;
    m_baseValue = 0; m_baseTag = 0; m_baseTagValid = false;
    m_offset = 0;
    m_address = 0; m_addressReady = false;
    m_loadedData = 0; m_memDoneInt = false;
    m_destROBTag = 0;
    m_segFault = false;
    m_cdbRequested = false;
    std::cout << "[" << idStr() << "] reset()\n";
}