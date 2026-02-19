// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/issue/Flags_Unit.cpp
// ============================================================================

/**
 * @file Flags_Unit.cpp
 * @brief Component 11 implementation — Flags register with speculative tracking.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/issue/Flags_Unit.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Evaluate (combinational)
 // ============================================================================

void Flags_Unit::evaluate(TomasuloBus& bus) {
    // Expose the architectural flags and the current rename tag to downstream
    // consumers (RS operand-capture, Branch_Executor).
    bus.FlagsArch_o = m_archFlags;
    bus.FlagsQi_o = m_qi;
    bus.FlagsQiValid_o = m_qiValid;
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void Flags_Unit::clockEdge(TomasuloBus& bus) {

    // ── 1. Flush — clear speculative tag, keep architectural flags ─
    // On a misprediction the pipeline is flushed but the architectural
    // flags remain valid (they were committed before the branch executed).
    if (bus.Flush_o) {
        m_qiValid = false;
        return;
    }

    // ── 2. Commit: write architectural flags and retire the tag ────
    // Commit_Unit raises FlagsCommitEn_i when the head ROB entry produces flags.
    if (bus.FlagsCommitEn_i) {
        m_archFlags = bus.FlagsCommitValue_i & 0x0F;  // Keep only NZCV bits.

        // If the committed entry was our tracked producer, clear the tag.
        // A newer issue may have already overwritten the tag, so check first.
        if (m_qiValid && m_qi == bus.CommitROBIdx_i) {
            m_qiValid = false;
        }
    }

    // ── 3. Issue: rename — record new flags producer ────────────
    // When an instruction that modifies flags is issued, point the tag at
    // the newly allocated ROB entry (ROBTail_o).
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