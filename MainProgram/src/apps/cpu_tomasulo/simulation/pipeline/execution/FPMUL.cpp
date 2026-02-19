// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/execution/FPMUL.cpp
// ============================================================================

/**
 * @file FPMUL.cpp
 * @brief Component 33 implementation -- FP MUL/DIV/SQRT.
 *
 * Non-pipelined; only one operation at a time. Fixed latencies:
 *   FMUL / FMULI  —  5 cycles
 *   FDIV / FDIVI  — 20 cycles
 *   FSQRT / FSQRTI — 25 cycles
 *
 * IEEE 754 edge cases (division by zero, sqrt of negative) produce NaN/±Inf
 * rather than CPU exceptions. Programs detect these via FCMPS + BUN.
 * FPMUL never generates flag results; FPMUL_CDBFlagsValid_o is always 0.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/execution/FPMUL.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>
#include <cmath>
#include <cstring>

 // ── Helper: raw bit-pattern reinterpretation ─────────────────────────────────

double FPMUL::toDouble(uint64_t bits) {
    double d; std::memcpy(&d, &bits, sizeof(d)); return d;
}

uint64_t FPMUL::fromDouble(double val) {
    uint64_t b; std::memcpy(&b, &val, sizeof(b)); return b;
}

// ============================================================================
// Evaluate (combinational outputs)
// ============================================================================

void FPMUL::evaluate(TomasuloBus& bus) {
    bus.FPMUL_Free_o = !m_busy;  // Free only when no operation is in-flight
    bus.FPMUL_CDBReq_o = false;

    if (m_done) {
        // Result is ready — request a CDB slot
        bus.FPMUL_CDBReq_o = true;
        bus.FPMUL_CDBROBTag_o = m_robTag;
        bus.FPMUL_CDBValue_o = m_result;
        bus.FPMUL_CDBException_o = m_exception;
        bus.FPMUL_CDBFlags_o = 0;
        bus.FPMUL_CDBFlagsValid_o = false;  // FPMUL never produces flags
    }
}

// ============================================================================
// Clock Edge (sequential -- countdown, result computation, handoff)
// ============================================================================

void FPMUL::clockEdge(TomasuloBus& bus) {
    // Flush: discard any in-flight operation immediately
    if (bus.Flush_o) {
        m_busy = false; m_done = false;
        return;
    }

    // ── Release after CDB accepted the result ────────────────────────
    if (m_done && !bus.FPMUL_CDBStall_o) {
        m_busy = false;
        m_done = false;
    }

    // ── Count down execution latency ──────────────────────────────────
    if (m_busy && !m_done) {
        m_cyclesRemaining--;
        if (m_cyclesRemaining == 0) {
            double a = toDouble(m_A);
            double b = toDouble(m_B);
            double r = 0.0;
            m_exception = 0x0;

            switch (m_op) {
            case 0x20: case 0x25:  // FMUL, FMULI
                r = a * b;
                break;
            case 0x21: case 0x26:  // FDIV, FDIVI
                r = a / b;          // IEEE 754: 0/0->NaN, nonzero/0->±Inf
                m_exception = 0x0;  // Not a CPU trap; checked by program via FCMPS
                break;
            case 0x28: case 0x52:  // FSQRT, FSQRTI
                r = std::sqrt(b);   // IEEE 754: negative->NaN
                m_exception = 0x0;
                break;
            }
            m_result = fromDouble(r);
            m_done = true;
        }
    }

    // ── Accept a new operation when idle ─────────────────────────────
    if (!m_busy && bus.FPMUL_Start_o) {
        m_busy = true;
        m_done = false;
        m_op = bus.FPMUL_Op_o;
        m_A = bus.FPMUL_A_o;
        m_B = bus.FPMUL_B_o;
        m_robTag = bus.FPMUL_DestROBTag_o;

        // Set countdown based on opcode latency
        switch (m_op) {
        case 0x20: case 0x25: m_cyclesRemaining = 5; break;  // FMUL
        case 0x21: case 0x26: m_cyclesRemaining = 20; break;  // FDIV
        case 0x28: case 0x52: m_cyclesRemaining = 25; break;  // FSQRT
        default:              m_cyclesRemaining = 5; break;
        }
    }
}

void FPMUL::reset() {
    m_busy = false; m_done = false; m_cyclesRemaining = 0;
    std::cout << "[FPMUL] reset()\n";
}