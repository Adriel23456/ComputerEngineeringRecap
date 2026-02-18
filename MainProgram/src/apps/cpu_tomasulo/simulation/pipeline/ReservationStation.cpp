/**
 * @file ReservationStation.cpp
 * @brief Components 17-21 implementation -- common RS core logic.
 *
 * Implements the full Tomasulo reservation station protocol:
 *   IDLE -> ALLOCATED -> operand wait (CDB snoop) -> READY -> DISPATCHED/EXECUTING -> FREED
 *
 * Matches spec pseudocode for RS_IntALU0/1 (Components 17-18),
 * RS_FPALU0 (19), RS_IntMUL0 (20), RS_FPMUL0 (21).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ReservationStation.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

ReservationStation::ReservationStation(const char* name) : m_name(name) {}

// ============================================================================
// Evaluate (combinational outputs)
// ============================================================================

void ReservationStation::evaluate(TomasuloBus& bus) {
    // -- Busy signal to Control_Unit --
    writeBusy(bus, m_busy);

    // -- Ready detection --
    bool dispatchBlock = isMultiCycle() ? m_executing : m_dispatched;

    bool ready = m_busy
        && !m_qjValid
        && !m_qkValid
        && !dispatchBlock
        && (!m_needsFlags || !m_flagsQiValid);

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
        m_destROBTag = bus.ROBTail_o;
        m_useImm = bus.UseImm_o;
        m_modifiesFlags = bus.ModifiesFlags_o;
        m_needsFlags = bus.NeedsFlags_o;
        m_dispatched = false;
        m_executing = false;

        // === OPERAND 1 (Vj) -- from Rn via read port 1 ===
        if (isUnaryOp(m_op) || isMOVLikeOp(m_op)) {
            // Unary FP ops and MOV/MVN: no Vj needed
            m_qjValid = false;
            m_vj = 0;
        }
        else if (!bus.RD1_QiValid_o) {
            m_vj = bus.RD1_Value_o;
            m_qjValid = false;
        }
        else {
            if (bus.ROBReadReady1_o) {
                m_vj = bus.ROBReadValue1_o;
                m_qjValid = false;
            }
            else {
                m_qj = bus.RD1_Qi_o;
                m_qjValid = true;
            }
        }

        // === OPERAND 2 (Vk) -- from Imm or Rm via read port 2 ===
        if (m_useImm) {
            m_vk = bus.ImmExt_o;
            m_qkValid = false;
        }
        else if (!bus.RD2_QiValid_o) {
            m_vk = bus.RD2_Value_o;
            m_qkValid = false;
        }
        else {
            if (bus.ROBReadReady2_o) {
                m_vk = bus.ROBReadValue2_o;
                m_qkValid = false;
            }
            else {
                m_qk = bus.RD2_Qi_o;
                m_qkValid = true;
            }
        }

        // === FLAGS DEPENDENCY (3-tier resolution) ===
        if (m_needsFlags) {
            if (!bus.FlagsQiValid_o) {
                // TIER 1: No pending producer -- use architectural flags
                m_flagsValue = bus.FlagsArch_o;
                m_flagsQiValid = false;
            }
            else if (bus.ROBReadFlagsReady_o) {
                // TIER 2: Producer completed -- forward from ROB
                m_flagsValue = bus.ROBReadFlagsResult_o;
                m_flagsQiValid = false;
            }
            else {
                // TIER 3: Producer still executing -- wait for CDB
                m_flagsQi = bus.FlagsQi_o;
                m_flagsQiValid = true;
            }
        }
        else {
            m_flagsQiValid = false;
        }
    }

    // -- 4. CDB snoop (every cycle while busy) --
    if (m_busy) {
        // CDB_A
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
        // CDB_B
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

        // FALLBACK: If Flags_Unit reports no pending producer anymore,
        // the architectural flags are authoritative.
        if (m_flagsQiValid && m_needsFlags && !bus.FlagsQiValid_o) {
            m_flagsValue = bus.FlagsArch_o;
            m_flagsQiValid = false;
        }
    }

    // -- 5. Grant latch --
    if (m_busy && readGrant(bus)) {
        if (isMultiCycle()) {
            m_executing = true;
        }
        else {
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
    m_vj = 0; m_qj = 0; m_qjValid = false;
    m_vk = 0; m_qk = 0; m_qkValid = false;
    m_destROBTag = 0;
    m_useImm = false;
    m_modifiesFlags = false;
    m_needsFlags = false;
    m_flagsQi = 0; m_flagsQiValid = false; m_flagsValue = 0;
    m_dispatched = false;
    m_executing = false;
    std::cout << "[" << m_name << "] reset()\n";
}