// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/branch/RS_Branch0.cpp
// ============================================================================

/**
 * @file RS_Branch0.cpp
 * @brief Implementation of RS_Branch0 (Component 22).
 *
 * evaluate():
 *   Presents RS_Branch0_Busy_o and RSBr0_Req_o based on internal state.
 *   Unconditional B (0x45) is ready as soon as it is allocated.
 *   Conditional branches are ready when m_flagsQiValid == false.
 *
 * clockEdge() — sequential events in priority order:
 *   1. Flush   — clear all state immediately.
 *   2. Free    — Commit_Unit signalled this RS is done.
 *   3. Allocate — latch opcode, ROB tag, target PC, and resolve flags.
 *   4. CDB snoop — update flags if a matching broadcast arrives.
 *   5. Fallback — if FlagsQiValid_o dropped to false while waiting,
 *                 adopt the now-authoritative architectural flags.
 *
 * 3-tier flags resolution at allocation:
 *   Tier 1 — FlagsQiValid_o == false → no in-flight writer; latch FlagsArch_o.
 *   Tier 2 — ROBReadFlagsReady_o     → producer done; forward ROBReadFlagsResult_o.
 *   Tier 3 — producer still running  → save FlagsQi, wait for CDB.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/branch/RS_Branch0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Evaluate (combinational)
 // ============================================================================

void RS_Branch0::evaluate(TomasuloBus& bus) {
    bus.RS_Branch0_Busy_o = m_busy;

    bool ready = false;
    if (m_op == 0x45) {
        // B (unconditional): always ready once allocated
        ready = m_busy;
    }
    else {
        // Conditional: ready when flags dependency is resolved
        ready = m_busy && !m_flagsQiValid;
    }

    bus.RSBr0_Req_o = ready;
    bus.RSBr0_Op_o = m_op;
    bus.RSBr0_TargetPC_o = m_targetPC;
    bus.RSBr0_FlagsValue_o = m_flagsValue;
    bus.RSBr0_PredTaken_o = m_predictedTaken;
    bus.RSBr0_ROBTag_o = m_robTag;
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void RS_Branch0::clockEdge(TomasuloBus& bus) {

    // ── 1. Flush ──────────────────────────────────────────────────
    if (bus.Flush_o) {
        m_busy = false;
        return;
    }

    // ── 2. Free (Commit_Unit signalled this instruction committed) ─
    if (bus.FreeRSBr0_o) {
        m_busy = false;
        return;
    }

    // ── 3. Allocate ───────────────────────────────────────────────
    if (bus.AllocRSBranch0_o && !m_busy) {
        m_busy = true;
        m_op = bus.Op_in_o;
        m_robTag = bus.ROBTail_o;
        m_targetPC = bus.PCCurrent_o + bus.ImmExt_o;
        m_predictedTaken = false;  // Static: always predict not-taken

        if (m_op == 0x45) {
            // Unconditional branch — no flags dependency
            m_flagsQiValid = false;
        }
        else {
            // 3-tier flags resolution
            if (!bus.FlagsQiValid_o) {
                // Tier 1: no in-flight writer
                m_flagsValue = bus.FlagsArch_o;
                m_flagsQiValid = false;
            }
            else if (bus.ROBReadFlagsReady_o) {
                // Tier 2: writer finished — forward from ROB
                m_flagsValue = bus.ROBReadFlagsResult_o;
                m_flagsQiValid = false;
            }
            else {
                // Tier 3: writer still executing — wait for CDB
                m_flagsQi = bus.FlagsQi_o;
                m_flagsQiValid = true;
            }
        }
    }

    // ── 4. CDB snoop (flags only) ─────────────────────────────────
    if (m_busy && m_flagsQiValid) {
        if (bus.CDBA_Valid_o && bus.CDBA_FlagsValid_o
            && m_flagsQi == bus.CDBA_ROBTag_o) {
            m_flagsValue = bus.CDBA_Flags_o;
            m_flagsQiValid = false;
        }
        if (bus.CDBB_Valid_o && bus.CDBB_FlagsValid_o
            && m_flagsQiValid && m_flagsQi == bus.CDBB_ROBTag_o) {
            m_flagsValue = bus.CDBB_Flags_o;
            m_flagsQiValid = false;
        }

        // ── 5. Fallback ────────────────────────────────────────────
        // The flags producer committed and updated the architectural flags.
        if (m_flagsQiValid && !bus.FlagsQiValid_o) {
            m_flagsValue = bus.FlagsArch_o;
            m_flagsQiValid = false;
        }
    }
}

// ============================================================================
// Reset
// ============================================================================

void RS_Branch0::reset() {
    m_busy = false;
    m_op = 0;
    m_targetPC = 0;
    m_predictedTaken = false;
    m_robTag = 0;
    m_flagsQi = 0;
    m_flagsQiValid = false;
    m_flagsValue = 0;
    std::cout << "[RS_Branch0] reset()\n";
}