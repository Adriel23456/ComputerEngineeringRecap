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
    bool memReq = m_busy && m_addressReady && !m_segFault && !m_memRequested;
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

    if (aguReq) {
        std::cout << "[" << idStr() << "] AGU request: base=0x" << std::hex
            << m_baseValue << " offset=0x" << m_offset
            << std::dec << " ROB#" << (int)m_destROBTag << "\n";
    }
    if (memReq) {
        std::cout << "[" << idStr() << "] MEM request: addr=0x" << std::hex
            << m_address << std::dec << " op=0x" << (int)m_op
            << " ROB#" << (int)m_destROBTag << "\n";
    }
    if (cdbReq) {
        std::cout << "[" << idStr() << "] CDB request: value=0x" << std::hex
            << (m_segFault ? (uint64_t)0 : m_loadedData) << std::dec
            << " exc=" << (m_segFault ? 2 : 0)
            << " ROB#" << (int)m_destROBTag << "\n";
    }
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void LoadBuffer::clockEdge(TomasuloBus& bus) {

    // -- 1. Flush --
    if (bus.Flush_o) {
        if (m_busy) {
            std::cout << "[" << idStr() << "] FLUSH: cleared.\n";
        }
        m_busy = false;
        return;
    }

    // -- 2. Free from Commit_Unit --
    if (readFree(bus)) {
        std::cout << "[" << idStr() << "] FREE from Commit. ROB#"
            << (int)m_destROBTag << "\n";
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
        m_memRequested = false;
        m_memDoneInt = false;
        m_cdbRequested = false;

        if (!bus.RD1_QiValid_o) {
            m_baseValue = bus.RD1_Value_o;
            m_baseTagValid = false;
            std::cout << "[" << idStr() << "] Alloc: base=0x" << std::hex
                << m_baseValue << std::dec << " (ready)\n";
        }
        else {
            if (bus.ROBReadReady1_o) {
                m_baseValue = bus.ROBReadValue1_o;
                m_baseTagValid = false;
                std::cout << "[" << idStr() << "] Alloc: base forwarded from ROB#"
                    << (int)bus.RD1_Qi_o << " = 0x" << std::hex
                    << m_baseValue << std::dec << "\n";
            }
            else {
                m_baseTag = bus.RD1_Qi_o;
                m_baseTagValid = true;
                std::cout << "[" << idStr() << "] Alloc: base waiting on ROB#"
                    << (int)m_baseTag << "\n";
            }
        }

        std::cout << "[" << idStr() << "] Allocated: op=0x" << std::hex
            << (int)m_op << std::dec << " ROB#" << (int)m_destROBTag << "\n";
    }

    // -- 4. CDB snoop --
    if (m_busy && m_baseTagValid) {
        if (bus.CDBA_Valid_o && m_baseTag == bus.CDBA_ROBTag_o) {
            m_baseValue = bus.CDBA_Value_o;
            m_baseTagValid = false;
            std::cout << "[" << idStr() << "] CDB_A snoop: base resolved = 0x"
                << std::hex << m_baseValue << std::dec << "\n";
        }
        if (bus.CDBB_Valid_o && m_baseTagValid && m_baseTag == bus.CDBB_ROBTag_o) {
            m_baseValue = bus.CDBB_Value_o;
            m_baseTagValid = false;
            std::cout << "[" << idStr() << "] CDB_B snoop: base resolved = 0x"
                << std::hex << m_baseValue << std::dec << "\n";
        }
    }

    // -- 5. Latch mem-requested (BEFORE AGU done, so it uses PREVIOUS cycle's state)
    if (m_busy && m_addressReady && !m_segFault && !m_memRequested) {
        m_memRequested = true;
        std::cout << "[" << idStr() << "] MEM request latched for addr=0x"
            << std::hex << m_address << std::dec << "\n";
    }

    // -- 6. AGU done --
    if (m_busy && readAGUDone(bus)) {
        m_address = readAGUAddress(bus);
        m_addressReady = true;
        m_segFault = readAGUSegFault(bus);
        std::cout << "[" << idStr() << "] AGU done: addr=0x" << std::hex
            << m_address << std::dec
            << " segfault=" << m_segFault << "\n";
    }

    // -- 7. Memory response --
    if (m_busy && m_memRequested && readMemDone(bus)) {
        m_loadedData = readMemData(bus);
        m_memDoneInt = true;
        std::cout << "[" << idStr() << "] MEM done: data=0x" << std::hex
            << m_loadedData << std::dec << " ROB#" << (int)m_destROBTag << "\n";
    }

    // -- 8. CDB stall --
    if (m_busy && readCDBStall(bus)) {
        m_cdbRequested = false;
        std::cout << "[" << idStr() << "] CDB stall: will retry.\n";
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
    m_memRequested = false;
    m_cdbRequested = false;
    std::cout << "[" << idStr() << "] reset()\n";
}