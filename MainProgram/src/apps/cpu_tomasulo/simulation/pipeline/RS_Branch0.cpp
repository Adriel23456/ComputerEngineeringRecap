/**
 * @file RS_Branch0.cpp
 * @brief Component 22 implementation -- Branch Reservation Station.
 *
 * Branches have no register operands. Conditional branches depend only on flags.
 * Unconditional B (0x45) is always immediately ready.
 *
 * 3-tier flags resolution:
 *   TIER 1: FlagsQiValid_i == 0 -> use architectural flags
 *   TIER 2: ROBReadFlagsReady_i == 1 -> forward from ROB
 *   TIER 3: wait for CDB broadcast
 *   FALLBACK: FlagsQiValid_i drops to 0 -> use updated arch flags
 */

#include "apps/cpu_tomasulo/simulation/pipeline/RS_Branch0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Evaluate (combinational outputs)
 // ============================================================================

void RS_Branch0::evaluate(TomasuloBus& bus) {
    bus.RS_Branch0_Busy_o = m_busy;

    bool ready;
    if (m_op == 0x45) {
        // B (unconditional): always ready once allocated
        ready = m_busy;
    }
    else {
        // Conditional: ready when flags are resolved
        ready = m_busy && !m_flagsQiValid;
    }

    bus.RSBr0_Req_o = ready;
    bus.RSBr0_Op_o = m_op;
    bus.RSBr0_TargetPC_o = m_targetPC;
    bus.RSBr0_FlagsValue_o = m_flagsValue;
    bus.RSBr0_PredTaken_o = m_predictedTaken;
    bus.RSBr0_ROBTag_o = m_robTag;

    if (ready) {
        std::cout << "[RS_Branch0] Ready: op=0x" << std::hex << (int)m_op
            << " target=0x" << m_targetPC
            << " flags=0x" << (int)m_flagsValue
            << std::dec << " ROB#" << (int)m_robTag << "\n";
    }
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void RS_Branch0::clockEdge(TomasuloBus& bus) {

    // -- 1. Flush --
    if (bus.Flush_o) {
        if (m_busy) {
            std::cout << "[RS_Branch0] FLUSH: cleared.\n";
        }
        m_busy = false;
        return;
    }

    // -- 2. Free from Commit_Unit --
    if (bus.FreeRSBr0_o) {
        std::cout << "[RS_Branch0] FREE from Commit. ROB#"
            << (int)m_robTag << "\n";
        m_busy = false;
        return;
    }

    // -- 3. Allocate --
    if (bus.AllocRSBranch0_o && !m_busy) {
        m_busy = true;
        m_op = bus.Op_in_o;
        m_robTag = bus.ROBTail_o;
        m_targetPC = bus.PCCurrent_o + bus.ImmExt_o;
        m_predictedTaken = false;  // Always predict not-taken

        // === FLAGS DEPENDENCY (3-tier resolution) ===
        if (m_op == 0x45) {
            // B (unconditional): no flags needed
            m_flagsQiValid = false;
            std::cout << "[RS_Branch0] Alloc: B unconditional, no flags needed\n";
        }
        else {
            // Conditional branches need flags
            if (!bus.FlagsQiValid_o) {
                // TIER 1: No pending producer -- use architectural flags
                m_flagsValue = bus.FlagsArch_o;
                m_flagsQiValid = false;
                std::cout << "[RS_Branch0] Alloc: flags=0x" << std::hex
                    << (int)m_flagsValue << std::dec << " (arch, Tier1)\n";
            }
            else if (bus.ROBReadFlagsReady_o) {
                // TIER 2: Producer completed -- forward from ROB
                m_flagsValue = bus.ROBReadFlagsResult_o;
                m_flagsQiValid = false;
                std::cout << "[RS_Branch0] Alloc: flags=0x" << std::hex
                    << (int)m_flagsValue << std::dec << " (ROB fwd, Tier2)\n";
            }
            else {
                // TIER 3: Producer still executing -- wait for CDB
                m_flagsQi = bus.FlagsQi_o;
                m_flagsQiValid = true;
                std::cout << "[RS_Branch0] Alloc: flags waiting on ROB#"
                    << (int)m_flagsQi << " (Tier3)\n";
            }
        }

        std::cout << "[RS_Branch0] Allocated: op=0x" << std::hex << (int)m_op
            << " target=0x" << m_targetPC << std::dec
            << " ROB#" << (int)m_robTag << "\n";
    }

    // -- 4. CDB snoop (only for flags) --
    if (m_busy && m_flagsQiValid) {
        // CDB_A
        if (bus.CDBA_Valid_o && bus.CDBA_FlagsValid_o
            && m_flagsQi == bus.CDBA_ROBTag_o) {
            m_flagsValue = bus.CDBA_Flags_o;
            m_flagsQiValid = false;
            std::cout << "[RS_Branch0] CDB_A snoop: flags resolved = 0x"
                << std::hex << (int)m_flagsValue << std::dec << "\n";
        }
        // CDB_B (re-check m_flagsQiValid)
        if (bus.CDBB_Valid_o && bus.CDBB_FlagsValid_o
            && m_flagsQiValid && m_flagsQi == bus.CDBB_ROBTag_o) {
            m_flagsValue = bus.CDBB_Flags_o;
            m_flagsQiValid = false;
            std::cout << "[RS_Branch0] CDB_B snoop: flags resolved = 0x"
                << std::hex << (int)m_flagsValue << std::dec << "\n";
        }

        // FALLBACK: architectural flags became authoritative
        if (m_flagsQiValid && !bus.FlagsQiValid_o) {
            m_flagsValue = bus.FlagsArch_o;
            m_flagsQiValid = false;
            std::cout << "[RS_Branch0] Flags fallback: arch flags = 0x"
                << std::hex << (int)m_flagsValue << std::dec << "\n";
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