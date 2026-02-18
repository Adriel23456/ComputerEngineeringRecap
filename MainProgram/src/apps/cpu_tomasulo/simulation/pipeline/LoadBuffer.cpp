/**
 * @file LoadBuffer.cpp
 * @brief Components 14-16 implementation -- Load Buffer core logic.
 *
 * State machine:
 *   IDLE -> ALLOCATED (on Alloc)
 *   ALLOCATED: wait for base operand (CDB snoop)
 *   BASE_READY: request AGU
 *   AGU_DONE: address computed -> request memory read (or CDB if segfault)
 *   MEM_DONE: data returned from D_Cache -> request CDB broadcast
 *   CDB_DONE: result published -> wait for free/flush
 *
 * Matches spec pseudocode for LB0/LB1/LB2 (Components 14-16).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/LoadBuffer.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
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
// Evaluate (combinational outputs)
// ============================================================================

void LoadBuffer::evaluate(TomasuloBus& bus) {
    // -- Busy signal to Control_Unit --
    writeBusy(bus, m_busy);

    // -- AGU request: base ready AND address not yet computed --
    bool aguReq = m_busy && !m_baseTagValid && !m_addressReady;
    writeAGUReq(bus, aguReq);
    writeAGUBase(bus, m_baseValue);
    writeAGUOffset(bus, m_offset);
    writeAGUROBTag(bus, m_destROBTag);

    // -- Memory request: address ready AND no segfault AND not yet requested --
    bool memReq = m_busy && m_addressReady && !m_segFault && !m_memDoneInt;
    writeMemReq(bus, memReq);
    writeMemAddr(bus, m_address);
    writeMemOp(bus, m_op);
    writeMemROBTag(bus, m_destROBTag);

    // -- CDB request (two paths per spec) --
    bool cdbReq = false;
    if (m_segFault) {
        // SegFault path: publish exception as soon as address is ready
        cdbReq = m_busy && m_addressReady && !m_cdbRequested;
        writeCDBValue(bus, 0);
        writeCDBException(bus, 0x2);
    }
    else {
        // Normal path: publish loaded data after memory completes
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

    // -- 1. Flush --
    if (bus.Flush_o) {
        m_busy = false;
        return;
    }

    // -- 2. Free from Commit_Unit --
    if (readFree(bus)) {
        m_busy = false;
        return;
    }

    // -- 3. Allocate --
    if (readAlloc(bus) && !m_busy) {
        m_busy = true;
        m_op = bus.Op_in_o;
        m_offset = bus.ImmExt_o;
        m_destROBTag = bus.ROBTail_o;
        m_addressReady = false;
        m_segFault = false;
        m_memDoneInt = false;
        m_cdbRequested = false;

        if (!bus.RD1_QiValid_o) {
            m_baseValue = bus.RD1_Value_o;
            m_baseTagValid = false;
        }
        else {
            if (bus.ROBReadReady1_o) {
                m_baseValue = bus.ROBReadValue1_o;
                m_baseTagValid = false;
            }
            else {
                m_baseTag = bus.RD1_Qi_o;
                m_baseTagValid = true;
            }
        }
    }

    // -- 4. CDB snoop --
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

    // -- 5. AGU done --
    if (m_busy && readAGUDone(bus)) {
        m_address = readAGUAddress(bus);
        m_addressReady = true;
        m_segFault = readAGUSegFault(bus);
    }

    // -- 6. Latch CDB requested (BEFORE mem response, uses PREVIOUS cycle's state)
    if (m_busy && (m_memDoneInt || m_segFault) && !m_cdbRequested) {
        m_cdbRequested = true;
    }

    // -- 7. Memory response --
    if (m_busy && readMemDone(bus)) {
        m_loadedData = readMemData(bus);
        m_memDoneInt = true;
    }

    // -- 8. CDB stall --
    if (m_busy && readCDBStall(bus)) {
        m_cdbRequested = false;
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