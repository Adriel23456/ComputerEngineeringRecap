// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/execution/IntMUL.cpp
// ============================================================================

/**
 * @file IntMUL.cpp
 * @brief Component 31 implementation -- Integer MUL/DIV.
 *
 * Non-pipelined multi-cycle unit:
 *   MUL  — 4 cycles latency
 *   DIV  — 16 cycles latency
 *
 * State machine:
 *   IDLE  -> START  : accept new operation from RS, begin countdown.
 *   START -> DONE   : m_cyclesRemaining reaches zero, compute result.
 *   DONE  -> IDLE   : CDB accepts the result (no stall on CDB bus).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/execution/IntMUL.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ============================================================================
 // Evaluate (combinational)
 // ============================================================================

void IntMUL::evaluate(TomasuloBus& bus) {
    // Report free only when not currently executing or holding a result.
    bus.IntMUL_Free_o = !m_busy;
    bus.IntMUL_CDBReq_o = false;

    // When a result is ready, drive the CDB request signals.
    if (m_done) {
        bus.IntMUL_CDBReq_o = true;
        bus.IntMUL_CDBROBTag_o = m_robTag;
        bus.IntMUL_CDBValue_o = m_result;
        bus.IntMUL_CDBException_o = m_exception;
        bus.IntMUL_CDBFlags_o = m_flags;
        bus.IntMUL_CDBFlagsValid_o = m_flagsValid;
    }
}

// ============================================================================
// Clock Edge (sequential)
// ============================================================================

void IntMUL::clockEdge(TomasuloBus& bus) {

    // ── Flush: abort in-flight operation immediately ─────────────
    if (bus.Flush_o) {
        m_busy = false;
        m_done = false;
        return;
    }

    // ── Release when CDB accepts our result ──────────────────────
    if (m_done && !bus.IntMUL_CDBStall_o) {
        m_busy = false;
        m_done = false;
    }

    // ── Countdown while executing ─────────────────────────────────
    if (m_busy && !m_done) {
        --m_cyclesRemaining;
        if (m_cyclesRemaining == 0) {
            // Execution complete — compute the final result.
            int64_t sA = (int64_t)m_A;
            int64_t sB = (int64_t)m_B;

            switch (m_op) {
            case 0x04: case 0x12:  // MUL, MULI  (signed 64×64 -> lower 64 bits)
                m_result = (uint64_t)(sA * sB);
                m_exception = 0x0;
                break;

            case 0x05: case 0x13:  // DIV, DIVI  (signed division)
                if (m_B == 0) {
                    m_result = 0;
                    m_exception = 0x3;  // Divide-by-zero exception
                }
                else {
                    m_result = (uint64_t)(sA / sB);
                    m_exception = 0x0;
                }
                break;
            }

            // Compute simplified N/Z flags (C and V are not produced by MUL/DIV).
            uint8_t N = (m_result >> 63) & 1;
            uint8_t Z = (m_result == 0) ? 1 : 0;
            m_flags = (N << 3) | (Z << 2);  // C=0, V=0
            m_flagsValid = true;
            m_done = true;
        }
    }

    // ── Accept new operation when idle ───────────────────────────
    if (!m_busy && bus.IntMUL_Start_o) {
        m_busy = true;
        m_done = false;
        m_op = bus.IntMUL_Op_o;
        m_A = bus.IntMUL_A_o;
        m_B = bus.IntMUL_B_o;
        m_robTag = bus.IntMUL_DestROBTag_o;
        // Latency: MUL = 4 cycles, DIV = 16 cycles.
        m_cyclesRemaining = (m_op == 0x04 || m_op == 0x12) ? 4 : 16;
    }
}

// ============================================================================
// Reset
// ============================================================================

void IntMUL::reset() {
    m_busy = false;
    m_done = false;
    m_cyclesRemaining = 0;
    std::cout << "[IntMUL] reset()\n";
}