// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/commit/Memory_Arbiter.cpp
// ============================================================================

/**
 * @file Memory_Arbiter.cpp
 * @brief Component 39 implementation -- Memory Arbiter.
 *
 * Serializes access to the single D_Cache port.
 * Priority: StoreCommit > LB0 > LB1 > LB2.
 *
 * Only one requestor may have an in-flight D_Cache access at a time.
 * m_currentSource tracks which requestor owns the port so that
 * DC_Done_o / DC_RData_o can be routed back to the correct consumer.
 *
 * Load-ordering safety (storeConflict):
 *   A load is suppressed when an older store in the ROB has already
 *   resolved its effective address to the same aligned 8-byte word.
 *   The load must wait until that store commits and Memory_Arbiter
 *   services StoreCommit_Req_o, clearing the hazard.
 *
 * On Flush_o: in-flight store accesses are allowed to complete naturally
 * (stores are already committed); in-flight load accesses are cancelled
 * (loads are speculative and will be re-issued after redirect).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/commit/Memory_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ----------------------------------------------------------------------------
 // decodeSize
 // Maps a full memory opcode to the DC_Size_o encoding consumed by D_Cache.
 //   LDR (0x4E) / STR (0x4F)   -> 0x03  (8-byte word)
 //   LDRB(0x50) / STRB(0x51)   -> 0x00  (1-byte)
 //   Fallback (ROB type code)   -> 0x03  (assume 8-byte)
 // ----------------------------------------------------------------------------
uint8_t Memory_Arbiter::decodeSize(uint8_t op) {
    if (op == 0x4E || op == 0x4F) return 0x03;  // LDR, STR  — 8 bytes
    if (op == 0x50 || op == 0x51) return 0x00;  // LDRB, STRB — 1 byte
    return 0x03;  // Fallback
}

// ============================================================================
// Evaluate (combinational outputs)
// ============================================================================

void Memory_Arbiter::evaluate(TomasuloBus& bus) {
    // All meaningful work is done in clockEdge (sequential arbitration).
    // evaluate() is intentionally empty: no combinational outputs to drive
    // independently of the clock edge in this component.
}

// ============================================================================
// Clock Edge (sequential -- arbitration and response routing)
// ============================================================================

void Memory_Arbiter::clockEdge(TomasuloBus& bus) {
    // ── Clear per-cycle done signals ─────────────────────────────────
    bus.StoreCommit_Done_o = false;
    bus.LB0_MemDone_o = false;
    bus.LB1_MemDone_o = false;
    bus.LB2_MemDone_o = false;
    bus.DC_Req_o = false;

    // ── Flush: cancel pending speculative loads; preserve store commits ──
    if (bus.Flush_o) {
        if (m_waitingForDCache && m_currentSource != Source::STORE_COMMIT) {
            // Cancel in-flight load — it will be re-issued after pipeline redirect
            m_waitingForDCache = false;
            m_currentSource = Source::NONE;
        }
        return;
    }

    // ── Route D_Cache response to the correct consumer ───────────────
    if (m_waitingForDCache && bus.DC_Done_o) {
        switch (m_currentSource) {
        case Source::STORE_COMMIT:
            // Committed store acknowledged — signal Commit_Unit
            bus.StoreCommit_Done_o = true;
            break;
        case Source::LB0:
            bus.LB0_MemDone_o = true;
            bus.LB0_MemData_o = bus.DC_RData_o;
            break;
        case Source::LB1:
            bus.LB1_MemDone_o = true;
            bus.LB1_MemData_o = bus.DC_RData_o;
            break;
        case Source::LB2:
            bus.LB2_MemDone_o = true;
            bus.LB2_MemData_o = bus.DC_RData_o;
            break;
        default: break;
        }
        m_waitingForDCache = false;
        m_currentSource = Source::NONE;
    }

    // ── Issue new D_Cache request if port is free ─────────────────────
    if (!m_waitingForDCache && bus.DC_Ready_o) {
        // Priority: StoreCommit > LB0 > LB1 > LB2
        if (bus.StoreCommit_Req_o) {
            // Committed store — highest priority, always safe to issue
            bus.DC_Req_o = true;
            bus.DC_RW_o = true;  // Write
            bus.DC_Addr_o = bus.StoreCommit_Addr_o;
            bus.DC_WData_o = bus.StoreCommit_Data_o;
            bus.DC_Size_o = decodeSize(bus.StoreCommit_Op_o);
            m_currentSource = Source::STORE_COMMIT;
            m_waitingForDCache = true;
        }
        else if (bus.LB0_MemReq_o && !storeConflict(bus, bus.LB0_MemAddr_o, bus.LB0_MemROBTag_o)) {
            // LB0 load — suppressed if an older store targets the same word
            bus.DC_Req_o = true;
            bus.DC_RW_o = false;  // Read
            bus.DC_Addr_o = bus.LB0_MemAddr_o;
            bus.DC_Size_o = decodeSize(bus.LB0_MemOp_o);
            m_currentSource = Source::LB0;
            m_waitingForDCache = true;
        }
        else if (bus.LB1_MemReq_o && !storeConflict(bus, bus.LB1_MemAddr_o, bus.LB1_MemROBTag_o)) {
            bus.DC_Req_o = true;
            bus.DC_RW_o = false;
            bus.DC_Addr_o = bus.LB1_MemAddr_o;
            bus.DC_Size_o = decodeSize(bus.LB1_MemOp_o);
            m_currentSource = Source::LB1;
            m_waitingForDCache = true;
        }
        else if (bus.LB2_MemReq_o && !storeConflict(bus, bus.LB2_MemAddr_o, bus.LB2_MemROBTag_o)) {
            bus.DC_Req_o = true;
            bus.DC_RW_o = false;
            bus.DC_Addr_o = bus.LB2_MemAddr_o;
            bus.DC_Size_o = decodeSize(bus.LB2_MemOp_o);
            m_currentSource = Source::LB2;
            m_waitingForDCache = true;
        }
    }
}

void Memory_Arbiter::reset() {
    m_currentSource = Source::NONE;
    m_waitingForDCache = false;
    std::cout << "[Memory_Arbiter] reset()\n";
}

// ----------------------------------------------------------------------------
// isOlderInROB
// Returns true if @p storeTag is closer to the ROB head than @p loadTag,
// meaning the store was dispatched before the load (program-order older).
// Uses modular subtraction to handle circular ROB wrap-around correctly.
// ----------------------------------------------------------------------------
bool Memory_Arbiter::isOlderInROB(uint8_t storeTag, uint8_t loadTag,
    uint8_t head, uint8_t robSize) {
    uint8_t storeAge = (storeTag - head) % robSize;
    uint8_t loadAge = (loadTag - head) % robSize;
    return storeAge < loadAge;  // Smaller age = closer to head = older
}

// ----------------------------------------------------------------------------
// storeConflict
// Returns true if any store buffer (SB0 or SB1) that is:
//   (a) address-resolved, and
//   (b) older than the load in program order
// overlaps the same aligned 8-byte word as the given load address.
// If true, the load must be deferred until the conflicting store commits.
// ----------------------------------------------------------------------------
bool Memory_Arbiter::storeConflict(const TomasuloBus& bus,
    uint64_t loadAddr, uint8_t loadROBTag) {
    uint64_t loadWord = loadAddr & ~7ULL;  // Align to 8-byte boundary
    uint8_t  head = bus.ROBHead_o;
    const uint8_t ROB_SIZE = 32;

    if (bus.SB0_AddrReady_o
        && (bus.SB0_ExposedAddr_o & ~7ULL) == loadWord
        && isOlderInROB(bus.SB0_ExposedROBTag_o, loadROBTag, head, ROB_SIZE))
        return true;

    if (bus.SB1_AddrReady_o
        && (bus.SB1_ExposedAddr_o & ~7ULL) == loadWord
        && isOlderInROB(bus.SB1_ExposedROBTag_o, loadROBTag, head, ROB_SIZE))
        return true;

    return false;
}