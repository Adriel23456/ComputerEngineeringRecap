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

    // Expose address for store-to-load ordering check
    if (m_id == ID::SB0) {
        bus.SB0_AddrReady_o = m_busy && m_addressReady;
        bus.SB0_ExposedAddr_o = m_address;
        bus.SB0_ExposedROBTag_o = m_robTag;
    }
    else {
        bus.SB1_AddrReady_o = m_busy && m_addressReady;
        bus.SB1_ExposedAddr_o = m_address;
        bus.SB1_ExposedROBTag_o = m_robTag;
    }
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void StoreBuffer::clockEdge(TomasuloBus& bus) {

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
        m_robTag = bus.ROBTail_o;
        m_addressReady = false;
        m_segFault = false;
        m_stCompleteNotified = false;

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

        if (!bus.RD_StoreQiValid_o) {
            m_dataValue = bus.RD_StoreValue_o;
            m_dataTagValid = false;
        }
        else {
            if (bus.ROBReadReady2_o) {
                m_dataValue = bus.ROBReadValue2_o;
                m_dataTagValid = false;
            }
            else {
                m_dataTag = bus.RD_StoreQi_o;
                m_dataTagValid = true;
            }
        }
    }

    // -- 4. CDB snoop --
    if (m_busy) {
        if (bus.CDBA_Valid_o) {
            if (m_baseTagValid && m_baseTag == bus.CDBA_ROBTag_o) {
                m_baseValue = bus.CDBA_Value_o;
                m_baseTagValid = false;
            }
            if (m_dataTagValid && m_dataTag == bus.CDBA_ROBTag_o) {
                m_dataValue = bus.CDBA_Value_o;
                m_dataTagValid = false;
            }
        }
        if (bus.CDBB_Valid_o) {
            if (m_baseTagValid && m_baseTag == bus.CDBB_ROBTag_o) {
                m_baseValue = bus.CDBB_Value_o;
                m_baseTagValid = false;
            }
            if (m_dataTagValid && m_dataTag == bus.CDBB_ROBTag_o) {
                m_dataValue = bus.CDBB_Value_o;
                m_dataTagValid = false;
            }
        }
    }

    // -- 5. Latch StoreComplete (read back what evaluate() actually asserted)
    if (m_id == ID::SB0 && bus.SB0_StComplete_Valid_o) {
        m_stCompleteNotified = true;
    }
    else if (m_id == ID::SB1 && bus.SB1_StComplete_Valid_o) {
        m_stCompleteNotified = true;
    }

    // -- 6. AGU done --
    if (m_busy && readAGUDone(bus)) {
        m_address = readAGUAddress(bus);
        m_addressReady = true;
        m_segFault = readAGUSegFault(bus);
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