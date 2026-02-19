// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/reservation/ReservationStation.cpp
// ============================================================================

/**
 * @file ReservationStation.cpp
 * @brief Components 17-21 implementation — common RS core logic.
 *
 * Implements the full Tomasulo reservation station protocol:
 *
 *   IDLE → ALLOCATED → operand wait (CDB snoop) → READY → DISPATCHED/EXECUTING → FREED
 *
 * Clock-edge priority (highest first):
 *   1. Flush      — unconditionally clear m_busy and return.
 *   2. Free       — Commit_Unit retires the slot; clear m_busy and return.
 *   3. Allocate   — Issue stage fills the slot if it is idle.
 *   4. CDB snoop  — resolve pending Qj / Qk / FlagsQi tags from either CDB channel.
 *   5. Grant      — arbiter awards the execution unit; latch m_dispatched / m_executing.
 *
 * Matches spec pseudocode for RS_IntALU0/1 (Components 17-18),
 * RS_FPALU0 (19), RS_IntMUL0 (20), RS_FPMUL0 (21).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/reservation/ReservationStation.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Construction
 // ============================================================================

ReservationStation::ReservationStation(const char* name) : m_name(name) {}

// ============================================================================
// Evaluate (combinational outputs)
// ============================================================================

void ReservationStation::evaluate(TomasuloBus& bus) {

    // ── Drive Busy signal to Control_Unit ────────────────────────
    writeBusy(bus, m_busy);

    // ── Ready / request detection ─────────────────────────────────
    // For multi-cycle units, the slot is blocked while the EU is still running.
    // For single-cycle units, it is blocked for the one cycle after dispatch.
    bool dispatchBlock = isMultiCycle() ? m_executing : m_dispatched;

    bool ready = m_busy
        && !m_qjValid                              // Operand 1 resolved
        && !m_qkValid                              // Operand 2 resolved
        && !dispatchBlock                          // Not already dispatched
        && (!m_needsFlags || !m_flagsQiValid);     // Flags resolved (or not needed)

    // ── Drive all output operand fields to the execution unit bus ─
    writeReq(bus, ready);
    writeOp(bus, m_op);
    writeVj(bus, m_vj);
    writeVk(bus, m_vk);
    writeDestROBTag(bus, m_destROBTag);
    writeModFlags(bus, m_modifiesFlags);
    writeNeedsFlags(bus, m_needsFlags);
    writeFlagsValue(bus, m_flagsValue);
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void ReservationStation::clockEdge(TomasuloBus& bus) {

    // ── 1. Flush: discard slot on branch misprediction ───────────
    if (bus.Flush_o) {
        m_busy = false;
        return;
    }

    // ── 2. Free: Commit_Unit retires this slot ───────────────────
    if (readFree(bus)) {
        m_busy = false;
        return;
    }

    // ── 3. Allocate: issue stage fills an idle slot ───────────────
    if (readAlloc(bus) && !m_busy) {
        m_busy = true;
        m_op = bus.Op_in_o;
        m_destROBTag = bus.ROBTail_o;
        m_useImm = bus.UseImm_o;
        m_modifiesFlags = bus.ModifiesFlags_o;
        m_needsFlags = bus.NeedsFlags_o;
        m_dispatched = false;
        m_executing = false;

        // ── Operand 1 (Vj) — from Rn via register file read port 1 ──
        if (isUnaryOp(m_op) || isMOVLikeOp(m_op)) {
            // Unary FP ops and MOV/MVN do not consume a first source operand.
            m_qjValid = false;
            m_vj = 0;
        }
        else if (!bus.RD1_QiValid_o) {
            // Tier 1: register is architecturally current — read directly.
            m_vj = bus.RD1_Value_o;
            m_qjValid = false;
        }
        else {
            if (bus.ROBReadReady1_o) {
                // Tier 2: ROB entry already has the result — forward it.
                m_vj = bus.ROBReadValue1_o;
                m_qjValid = false;
            }
            else {
                // Tier 3: ROB entry not ready — wait for CDB broadcast.
                m_qj = bus.RD1_Qi_o;
                m_qjValid = true;
            }
        }

        // ── Operand 2 (Vk) — from Imm or Rm via register file port 2 ─
        if (m_useImm) {
            // Immediate operand is always ready at issue time.
            m_vk = bus.ImmExt_o;
            m_qkValid = false;
        }
        else if (!bus.RD2_QiValid_o) {
            // Tier 1: architecturally current.
            m_vk = bus.RD2_Value_o;
            m_qkValid = false;
        }
        else {
            if (bus.ROBReadReady2_o) {
                // Tier 2: forward from ROB.
                m_vk = bus.ROBReadValue2_o;
                m_qkValid = false;
            }
            else {
                // Tier 3: wait for CDB.
                m_qk = bus.RD2_Qi_o;
                m_qkValid = true;
            }
        }

        // ── Flags dependency (ADC, SBC, conditional branch ops) ──
        if (m_needsFlags) {
            if (!bus.FlagsQiValid_o) {
                // Tier 1: no pending flags producer — use architectural NZCV.
                m_flagsValue = bus.FlagsArch_o;
                m_flagsQiValid = false;
            }
            else if (bus.ROBReadFlagsReady_o) {
                // Tier 2: in-flight producer already finished — forward from ROB.
                m_flagsValue = bus.ROBReadFlagsResult_o;
                m_flagsQiValid = false;
            }
            else {
                // Tier 3: producer still executing — wait for CDB broadcast.
                m_flagsQi = bus.FlagsQi_o;
                m_flagsQiValid = true;
            }
        }
        else {
            m_flagsQiValid = false;
        }
    } // end allocate

    // ── 4. CDB snoop: resolve pending tags every cycle while busy ─
    if (m_busy) {

        // ── CDB channel A ─────────────────────────────────────────
        if (bus.CDBA_Valid_o) {
            if (m_qjValid && m_qj == bus.CDBA_ROBTag_o) {
                m_vj = bus.CDBA_Value_o;
                m_qjValid = false;
            }
            if (m_qkValid && m_qk == bus.CDBA_ROBTag_o) {
                m_vk = bus.CDBA_Value_o;
                m_qkValid = false;
            }
            if (m_flagsQiValid && m_needsFlags && m_flagsQi == bus.CDBA_ROBTag_o) {
                if (bus.CDBA_FlagsValid_o) {
                    m_flagsValue = bus.CDBA_Flags_o;
                    m_flagsQiValid = false;
                }
            }
        }

        // ── CDB channel B ─────────────────────────────────────────
        if (bus.CDBB_Valid_o) {
            if (m_qjValid && m_qj == bus.CDBB_ROBTag_o) {
                m_vj = bus.CDBB_Value_o;
                m_qjValid = false;
            }
            if (m_qkValid && m_qk == bus.CDBB_ROBTag_o) {
                m_vk = bus.CDBB_Value_o;
                m_qkValid = false;
            }
            if (m_flagsQiValid && m_needsFlags && m_flagsQi == bus.CDBB_ROBTag_o) {
                if (bus.CDBB_FlagsValid_o) {
                    m_flagsValue = bus.CDBB_Flags_o;
                    m_flagsQiValid = false;
                }
            }
        }

        // ── Flags fallback: if the Flags_Unit clears its tag (e.g. after
        //   a commit or flush), the architectural flags are now authoritative.
        if (m_flagsQiValid && m_needsFlags && !bus.FlagsQiValid_o) {
            m_flagsValue = bus.FlagsArch_o;
            m_flagsQiValid = false;
        }
    }

    // ── 5. Grant latch: arbiter selects this station for execution ─
    if (m_busy && readGrant(bus)) {
        if (isMultiCycle()) {
            // Multi-cycle unit: keep slot occupied until FreeRS signal arrives.
            m_executing = true;
        }
        else {
            // Single-cycle unit: block for one cycle after dispatch, then free.
            m_dispatched = true;
        }
    }
}

// ============================================================================
// Reset
// ============================================================================

void ReservationStation::reset() {
    m_busy = false;
    m_op = 0;
    m_vj = 0;  m_qj = 0;       m_qjValid = false;
    m_vk = 0;  m_qk = 0;       m_qkValid = false;
    m_destROBTag = 0;
    m_useImm = false;
    m_modifiesFlags = false;
    m_needsFlags = false;
    m_flagsQi = 0;  m_flagsQiValid = false;  m_flagsValue = 0;
    m_dispatched = false;
    m_executing = false;
    std::cout << "[" << m_name << "] reset()\n";
}