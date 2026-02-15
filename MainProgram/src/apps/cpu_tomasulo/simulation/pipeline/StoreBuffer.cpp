/**
 * @file StoreBuffer.cpp
 * @brief Components 12-13 implementation -- Store Buffer core logic.
 *
 * State machine:
 *   IDLE -> ALLOCATED (on Alloc)
 *   ALLOCATED: wait for base operand (CDB snoop) + data operand (CDB snoop)
 *   BASE_READY: request AGU
 *   AGU_DONE: address computed, wait for data
 *   COMPLETE: address + data ready -> notify ROB via StoreComplete
 *   FREED: on commit (FreeSBx) or flush
 */

#include "apps/cpu_tomasulo/simulation/pipeline/StoreBuffer.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

StoreBuffer::StoreBuffer(ID id) : m_id(id) {}

const char* StoreBuffer::idStr() const {
    return (m_id == ID::SB0) ? "SB0" : "SB1";
}

// ============================================================================
// Evaluate (combinational outputs)
// ============================================================================

void StoreBuffer::evaluate(TomasuloBus& bus) {
    // -- Busy signal to Control_Unit --
    writeBusy(bus, m_busy);

    // -- AGU request: base ready AND address not yet computed --
    bool aguReq = m_busy && !m_baseTagValid && !m_addressReady;
    writeAGUReq(bus, aguReq);
    writeAGUBase(bus, m_baseValue);
    writeAGUOffset(bus, m_offset);
    writeAGUROBTag(bus, m_robTag);

    // -- Store complete: address ready AND data ready AND not yet notified --
    bool complete = m_busy && m_addressReady && !m_dataTagValid && !m_stCompleteNotified;
    writeStCompleteValid(bus, complete);
    writeStCompleteROBTag(bus, m_robTag);
    writeStCompleteAddr(bus, m_address);
    writeStCompleteData(bus, m_dataValue);

    if (aguReq) {
        std::cout << "[" << idStr() << "] AGU request: base=0x" << std::hex
            << m_baseValue << " offset=0x" << m_offset
            << std::dec << " ROB#" << (int)m_robTag << "\n";
    }
    if (complete) {
        std::cout << "[" << idStr() << "] StoreComplete: addr=0x" << std::hex
            << m_address << " data=0x" << m_dataValue
            << std::dec << " ROB#" << (int)m_robTag << "\n";
    }
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void StoreBuffer::clockEdge(TomasuloBus& bus) {

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
            << (int)m_robTag << "\n";
        m_busy = false;
        return;
    }

    // -- 3. Allocate --
    if (readAlloc(bus) && !m_busy) {
        m_busy = true;
        m_op = bus.Op_in_o;
        m_offset = bus.ImmExt_o;
        m_robTag = bus.ROBTail_o;
        m_addressReady = false;
        m_segFault = false;
        m_stCompleteNotified = false;

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

        if (!bus.RD_StoreQiValid_o) {
            m_dataValue = bus.RD_StoreValue_o;
            m_dataTagValid = false;
            std::cout << "[" << idStr() << "] Alloc: data=0x" << std::hex
                << m_dataValue << std::dec << " (ready)\n";
        }
        else {
            if (bus.ROBReadReady2_o) {
                m_dataValue = bus.ROBReadValue2_o;
                m_dataTagValid = false;
                std::cout << "[" << idStr() << "] Alloc: data forwarded from ROB#"
                    << (int)bus.RD_StoreQi_o << " = 0x" << std::hex
                    << m_dataValue << std::dec << "\n";
            }
            else {
                m_dataTag = bus.RD_StoreQi_o;
                m_dataTagValid = true;
                std::cout << "[" << idStr() << "] Alloc: data waiting on ROB#"
                    << (int)m_dataTag << "\n";
            }
        }

        std::cout << "[" << idStr() << "] Allocated: op=0x" << std::hex
            << (int)m_op << std::dec << " ROB#" << (int)m_robTag << "\n";
    }

    // -- 4. CDB snoop --
    if (m_busy) {
        if (bus.CDBA_Valid_o) {
            if (m_baseTagValid && m_baseTag == bus.CDBA_ROBTag_o) {
                m_baseValue = bus.CDBA_Value_o;
                m_baseTagValid = false;
                std::cout << "[" << idStr() << "] CDB_A snoop: base resolved = 0x"
                    << std::hex << m_baseValue << std::dec << "\n";
            }
            if (m_dataTagValid && m_dataTag == bus.CDBA_ROBTag_o) {
                m_dataValue = bus.CDBA_Value_o;
                m_dataTagValid = false;
                std::cout << "[" << idStr() << "] CDB_A snoop: data resolved = 0x"
                    << std::hex << m_dataValue << std::dec << "\n";
            }
        }
        if (bus.CDBB_Valid_o) {
            if (m_baseTagValid && m_baseTag == bus.CDBB_ROBTag_o) {
                m_baseValue = bus.CDBB_Value_o;
                m_baseTagValid = false;
                std::cout << "[" << idStr() << "] CDB_B snoop: base resolved = 0x"
                    << std::hex << m_baseValue << std::dec << "\n";
            }
            if (m_dataTagValid && m_dataTag == bus.CDBB_ROBTag_o) {
                m_dataValue = bus.CDBB_Value_o;
                m_dataTagValid = false;
                std::cout << "[" << idStr() << "] CDB_B snoop: data resolved = 0x"
                    << std::hex << m_dataValue << std::dec << "\n";
            }
        }
    }

    // -- 5. Latch StoreComplete (BEFORE AGU done, uses PREVIOUS cycle's state)
    if (m_busy && m_addressReady && !m_dataTagValid && !m_stCompleteNotified) {
        m_stCompleteNotified = true;
        std::cout << "[" << idStr() << "] StoreComplete latched for ROB#"
            << (int)m_robTag << "\n";
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
}

// ============================================================================
// Reset
// ============================================================================

void StoreBuffer::reset() {
    m_busy = false;
    m_op = 0;
    m_baseValue = 0; m_baseTag = 0; m_baseTagValid = false;
    m_offset = 0;
    m_address = 0; m_addressReady = false;
    m_dataValue = 0; m_dataTag = 0; m_dataTagValid = false;
    m_robTag = 0;
    m_segFault = false;
    m_stCompleteNotified = false;
    std::cout << "[" << idStr() << "] reset()\n";
}