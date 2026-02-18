/**
 * @file Flags_Unit.cpp
 * @brief Component 11 implementation — Flags with speculative tracking.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/Flags_Unit.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Evaluate (combinational)
 // ============================================================================

void Flags_Unit::evaluate(TomasuloBus& bus) {
    bus.FlagsArch_o = m_archFlags;
    bus.FlagsQi_o = m_qi;
    bus.FlagsQiValid_o = m_qiValid;
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void Flags_Unit::clockEdge(TomasuloBus& bus) {
    // ── 1. Flush — clear speculative tag, keep architectural flags
    if (bus.Flush_o) {
        m_qiValid = false;
        return;
    }

    // ── 2. Commit flags update ──────────────────────────────────
    if (bus.FlagsCommitEn_i) {
        m_archFlags = bus.FlagsCommitValue_i & 0x0F;

        // Clear tag if the committed entry was the one we're waiting on
        if (m_qiValid && m_qi == bus.CommitROBIdx_i) {
            m_qiValid = false;
        }
    }

    // ── 3. Issue — rename flags producer ────────────────────────
    if (bus.ROBAlloc_o && bus.FlagsTagWrEn_o) {
        m_qi = bus.ROBTail_o;
        m_qiValid = true;
    }
}

// ============================================================================
// Reset
// ============================================================================

void Flags_Unit::reset() {
    m_archFlags = 0;
    m_qi = 0;
    m_qiValid = false;
    std::cout << "[Flags_Unit] reset()\n";
}