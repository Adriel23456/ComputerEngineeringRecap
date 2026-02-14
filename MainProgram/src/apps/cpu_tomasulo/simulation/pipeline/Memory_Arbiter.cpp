/**
 * @file Memory_Arbiter.cpp
 * @brief Component 39 implementation -- Memory Arbiter.
 *
 * Priority: StoreCommit > LB0 > LB1 > LB2.
 * Only one D_Cache access at a time. Tracks current requestor to route response.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/Memory_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

uint8_t Memory_Arbiter::decodeSize(uint8_t op) {
    // LDR(0x4E), STR(0x4F) -> 8 bytes (0b11)
    // LDRB(0x50), STRB(0x51) -> 1 byte (0b00)
    if (op == 0x4E || op == 0x4F) return 0x03;
    if (op == 0x50 || op == 0x51) return 0x00;
    // Fallback: if op is a ROB type code (not full opcode), default to 8 bytes
    return 0x03;
}

// ============================================================================
// Evaluate (combinational outputs)
// ============================================================================

void Memory_Arbiter::evaluate(TomasuloBus& bus) {
    // Default: no new D_Cache request, no done signals
    // (done signals are set in clockEdge when D_Cache responds)
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void Memory_Arbiter::clockEdge(TomasuloBus& bus) {
    // Clear per-cycle done signals
    bus.StoreCommit_Done_o = false;
    bus.LB0_MemDone_o = false;
    bus.LB1_MemDone_o = false;
    bus.LB2_MemDone_o = false;
    bus.DC_Req_o = false;

    if (bus.Flush_o) {
        // Don't cancel in-flight D_Cache requests (they'll complete naturally)
        // But do cancel pending load requests (loads are speculative)
        if (m_waitingForDCache && m_currentSource != Source::STORE_COMMIT) {
            m_waitingForDCache = false;
            m_currentSource = Source::NONE;
        }
        return;
    }

    // -- Handle D_Cache response --
    if (m_waitingForDCache && bus.DC_Done_o) {
        switch (m_currentSource) {
        case Source::STORE_COMMIT:
            bus.StoreCommit_Done_o = true;
            std::cout << "[Memory_Arbiter] Store commit done.\n";
            break;
        case Source::LB0:
            bus.LB0_MemDone_o = true;
            bus.LB0_MemData_o = bus.DC_RData_o;
            std::cout << "[Memory_Arbiter] LB0 mem done: data=0x"
                << std::hex << bus.DC_RData_o << std::dec << "\n";
            break;
        case Source::LB1:
            bus.LB1_MemDone_o = true;
            bus.LB1_MemData_o = bus.DC_RData_o;
            std::cout << "[Memory_Arbiter] LB1 mem done: data=0x"
                << std::hex << bus.DC_RData_o << std::dec << "\n";
            break;
        case Source::LB2:
            bus.LB2_MemDone_o = true;
            bus.LB2_MemData_o = bus.DC_RData_o;
            std::cout << "[Memory_Arbiter] LB2 mem done: data=0x"
                << std::hex << bus.DC_RData_o << std::dec << "\n";
            break;
        default: break;
        }
        m_waitingForDCache = false;
        m_currentSource = Source::NONE;
    }

    // -- Accept new request if D_Cache is free --
    if (!m_waitingForDCache && bus.DC_Ready_o) {
        // Priority: StoreCommit > LB0 > LB1 > LB2
        if (bus.StoreCommit_Req_o) {
            bus.DC_Req_o = true;
            bus.DC_RW_o = true;   // Write
            bus.DC_Addr_o = bus.StoreCommit_Addr_o;
            bus.DC_WData_o = bus.StoreCommit_Data_o;
            bus.DC_Size_o = decodeSize(bus.StoreCommit_Op_o);
            m_currentSource = Source::STORE_COMMIT;
            m_waitingForDCache = true;
            std::cout << "[Memory_Arbiter] Store commit req: addr=0x"
                << std::hex << bus.StoreCommit_Addr_o << std::dec << "\n";
        }
        else if (bus.LB0_MemReq_o) {
            bus.DC_Req_o = true;
            bus.DC_RW_o = false;  // Read
            bus.DC_Addr_o = bus.LB0_MemAddr_o;
            bus.DC_Size_o = decodeSize(bus.LB0_MemOp_o);
            m_currentSource = Source::LB0;
            m_waitingForDCache = true;
            std::cout << "[Memory_Arbiter] LB0 mem req: addr=0x"
                << std::hex << bus.LB0_MemAddr_o << std::dec << "\n";
        }
        else if (bus.LB1_MemReq_o) {
            bus.DC_Req_o = true;
            bus.DC_RW_o = false;
            bus.DC_Addr_o = bus.LB1_MemAddr_o;
            bus.DC_Size_o = decodeSize(bus.LB1_MemOp_o);
            m_currentSource = Source::LB1;
            m_waitingForDCache = true;
            std::cout << "[Memory_Arbiter] LB1 mem req: addr=0x"
                << std::hex << bus.LB1_MemAddr_o << std::dec << "\n";
        }
        else if (bus.LB2_MemReq_o) {
            bus.DC_Req_o = true;
            bus.DC_RW_o = false;
            bus.DC_Addr_o = bus.LB2_MemAddr_o;
            bus.DC_Size_o = decodeSize(bus.LB2_MemOp_o);
            m_currentSource = Source::LB2;
            m_waitingForDCache = true;
            std::cout << "[Memory_Arbiter] LB2 mem req: addr=0x"
                << std::hex << bus.LB2_MemAddr_o << std::dec << "\n";
        }
    }
}

void Memory_Arbiter::reset() {
    m_currentSource = Source::NONE;
    m_waitingForDCache = false;
    std::cout << "[Memory_Arbiter] reset()\n";
}