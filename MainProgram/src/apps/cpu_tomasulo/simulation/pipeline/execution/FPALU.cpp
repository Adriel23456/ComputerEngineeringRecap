// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/execution/FPALU.cpp
// ============================================================================

/**
 * @file FPALU.cpp
 * @brief Component 29 implementation -- FP ALU, 3-stage pipeline.
 *
 * Supported operations (register and immediate variants):
 *   FADD, FSUB, FCOPYSIGN, FNEG, FABS, CDTI, CDTD,
 *   RTNR, RTZ, RTP, RTNE, FCMP, FCMN, FCMPS.
 *
 * Pipeline flow per clock edge:
 *   INPUT  -> Stage 1 : Latch operands and opcode.
 *   Stage 1 -> Stage 2 : Execute the core arithmetic (double precision).
 *   Stage 2 -> Stage 3 : Finalize — compute FCMP flags, detect exceptions.
 *   Stage 3 -> CDB     : Assert CDBReq; if stalled, park in outputHold.
 *
 * FPALU_Free_o is de-asserted while Stage 1 is occupied, preventing
 * FPALU_Arbiter from dispatching a new operation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/execution/FPALU.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>
#include <cmath>
#include <cstring>

 // ── Helper: raw bit-pattern reinterpretation ─────────────────────────────────

double FPALU::toDouble(uint64_t bits) {
    double d;
    std::memcpy(&d, &bits, sizeof(d));
    return d;
}

uint64_t FPALU::fromDouble(double val) {
    uint64_t bits;
    std::memcpy(&bits, &val, sizeof(bits));
    return bits;
}

// ----------------------------------------------------------------------------
// computeFCMPFlags
// Computes (N,Z,C,V) for a direct ordered/unordered comparison of A vs B.
//   N=1 : A < B
//   Z=1 : A == B
//   C=1 : A >= B
//   V=1 : either operand is NaN (unordered)
// ----------------------------------------------------------------------------
uint8_t FPALU::computeFCMPFlags(uint64_t A, uint64_t B) {
    double a = toDouble(A);
    double b = toDouble(B);
    bool unordered = std::isnan(a) || std::isnan(b);
    uint8_t N = (!unordered && a < b) ? 1 : 0;
    uint8_t Z = (!unordered && a == b) ? 1 : 0;
    uint8_t C = (!unordered && a >= b) ? 1 : 0;
    uint8_t V = unordered ? 1 : 0;
    return (N << 3) | (Z << 2) | (C << 1) | V;
}

// ============================================================================
// Evaluate (combinational outputs)
// ============================================================================

void FPALU::evaluate(TomasuloBus& bus) {
    // FPALU is free only when Stage 1 and the output hold register are both empty
    bus.FPALU_Free_o = (!m_stage1.active) && (!m_outputHold.active);
    bus.FPALU_CDBReq_o = false;

    // outputHold takes priority: result was stalled last cycle
    if (m_outputHold.active) {
        bus.FPALU_CDBReq_o = true;
        bus.FPALU_CDBROBTag_o = m_outputHold.robTag;
        bus.FPALU_CDBValue_o = m_outputHold.result;
        bus.FPALU_CDBException_o = m_outputHold.exception;
        bus.FPALU_CDBFlags_o = m_outputHold.flags;
        bus.FPALU_CDBFlagsValid_o = m_outputHold.flagsValid;
    }
    else if (m_stage3.active) {
        // Fresh result from stage 3
        bus.FPALU_CDBReq_o = true;
        bus.FPALU_CDBROBTag_o = m_stage3.robTag;
        bus.FPALU_CDBValue_o = m_stage3.result;
        bus.FPALU_CDBException_o = m_stage3.exception;
        bus.FPALU_CDBFlags_o = m_stage3.flags;
        bus.FPALU_CDBFlagsValid_o = m_stage3.flagsValid;
    }
}

// ============================================================================
// Clock Edge (sequential -- pipeline advancement)
// ============================================================================

void FPALU::clockEdge(TomasuloBus& bus) {
    // Flush: drain all pipeline stages immediately
    if (bus.Flush_o) {
        m_stage1 = {}; m_stage2 = {}; m_stage3 = {}; m_outputHold = {};
        return;
    }

    // ── Stage 3 -> output or outputHold ──────────────────────────────
    if (m_stage3.active) {
        if (!bus.FPALU_CDBStall_o) {
            // CDB accepted the result — clear both stage3 and the hold register
            m_stage3.active = false;
            m_outputHold.active = false;
        }
        else {
            // CDB stalled — park the result in outputHold for next cycle
            m_outputHold = m_stage3;
            m_stage3.active = false;
        }
    }

    // ── Stage 2 -> Stage 3 (finalization) ────────────────────────────
    if (m_stage2.active && !m_stage3.active) {
        m_stage3 = m_stage2;
        m_stage3.active = true;

        uint8_t op = m_stage2.op;

        // Compute FCMP-family flags in stage 3 (after arithmetic in stage 2)
        if (op >= 0x3F && op <= 0x44) {
            double a = toDouble(m_stage2.A);
            double b = toDouble(m_stage2.B);
            bool unordered = std::isnan(a) || std::isnan(b);
            uint8_t N = 0, Z = 0, C = 0, V = unordered ? 1 : 0;

            if (op == 0x3F || op == 0x42) {
                // FCMP / FCMPI: direct comparison (A vs B)
                N = (!unordered && a < b) ? 1 : 0;
                Z = (!unordered && a == b) ? 1 : 0;
                C = (!unordered && a >= b) ? 1 : 0;
            }
            else if (op == 0x40 || op == 0x43) {
                // FCMN / FCMNI: flags derived from (A + B)
                double sum = a + b;
                if (!unordered) {
                    N = (sum < 0.0) ? 1 : 0;
                    Z = (sum == 0.0) ? 1 : 0;
                    C = (sum >= 0.0) ? 1 : 0;
                }
            }
            else if (op == 0x41 || op == 0x44) {
                // FCMPS / FCMPSI: compare magnitudes (|A| vs |B|)
                double absA = std::fabs(a);
                double absB = std::fabs(b);
                N = (!unordered && absA < absB) ? 1 : 0;
                Z = (!unordered && absA == absB) ? 1 : 0;
                C = (!unordered && absA >= absB) ? 1 : 0;
            }

            m_stage3.flags = (N << 3) | (Z << 2) | (C << 1) | V;
            m_stage3.flagsValid = true;
        }
        else {
            m_stage3.flagsValid = false;
        }

        // Simplified exception detection: NaN output from non-NaN inputs = invalid op
        double r = toDouble(m_stage3.result);
        if (std::isnan(r)
            && !std::isnan(toDouble(m_stage2.A))
            && !std::isnan(toDouble(m_stage2.B)))
        {
            m_stage3.exception = 0x1;  // Invalid operation
        }
        else {
            m_stage3.exception = 0x0;
        }
        m_stage2.active = false;
    }

    // ── Stage 1 -> Stage 2 (main arithmetic) ─────────────────────────
    if (m_stage1.active && !m_stage2.active) {
        m_stage2 = m_stage1;
        m_stage2.active = true;

        double a = toDouble(m_stage1.A);
        double b = toDouble(m_stage1.B);
        double partial = 0.0;

        switch (m_stage1.op) {
        case 0x1E: case 0x23: partial = a + b; break;         // FADD, FADDI
        case 0x1F: case 0x24: partial = a - b; break;         // FSUB, FSUBI
        case 0x22: case 0x27:                                   // FCOPYSIGN, FCOPYSIGNI
            partial = std::copysign(a, b); break;
        case 0x29: case 0x53: partial = -b; break;             // FNEG, FNEGI
        case 0x2A: case 0x54: partial = std::fabs(b); break;   // FABS, FABSI
        case 0x2B: case 0x55:                                   // CDTI, CDTII: double->int64 bits
        {
            int64_t iv = (int64_t)b;
            partial = 0;
            m_stage2.result = (uint64_t)iv;  // Store integer bits directly
        }
        break;
        case 0x2C: case 0x56:                                   // CDTD, CDTDI: int64 bits->double
        {
            int64_t iv;
            std::memcpy(&iv, &m_stage1.B, sizeof(iv));
            partial = (double)iv;
        }
        break;
        case 0x2D: case 0x57: partial = std::nearbyint(b); break;  // RTNR (round to nearest)
        case 0x2E: case 0x58: partial = std::trunc(b);     break;  // RTZ  (round toward zero)
        case 0x2F: case 0x59: partial = std::ceil(b);      break;  // RTP  (round toward +inf)
        case 0x30: case 0x5A: partial = std::rint(b);      break;  // RTNE (round to nearest even)
        case 0x3F: case 0x40: case 0x41:                           // FCMP, FCMN, FCMPS
        case 0x42: case 0x43: case 0x44: partial = 0; break;      // (flags computed in stage 3)
        default: partial = 0; break;
        }

        // For CDTI/CDTII result is already set as raw integer bits above
        if (m_stage1.op != 0x2B && m_stage1.op != 0x55) {
            m_stage2.result = fromDouble(partial);
        }
        // Preserve operands for stage 3 (FCMP needs them)
        m_stage2.A = m_stage1.A;
        m_stage2.B = m_stage1.B;
        m_stage1.active = false;
    }

    // ── INPUT -> Stage 1 ──────────────────────────────────────────────
    if (bus.FPALU_Start_o && !m_stage1.active) {
        m_stage1.active = true;
        m_stage1.op = bus.FPALU_Op_o;
        m_stage1.A = bus.FPALU_A_o;
        m_stage1.B = bus.FPALU_B_o;
        m_stage1.robTag = bus.FPALU_DestROBTag_o;
        m_stage1.modFlags = bus.FPALU_ModFlags_o;
    }
}

void FPALU::reset() {
    m_stage1 = {}; m_stage2 = {}; m_stage3 = {}; m_outputHold = {};
    std::cout << "[FPALU] reset()\n";
}