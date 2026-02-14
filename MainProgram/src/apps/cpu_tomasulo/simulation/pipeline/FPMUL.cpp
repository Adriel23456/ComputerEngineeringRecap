/**
 * @file FPMUL.cpp
 * @brief Component 33 implementation -- FP MUL/DIV/SQRT.
 * Non-pipelined: FMUL 5, FDIV 20, FSQRT 25 cycles.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/FPMUL.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>
#include <cmath>
#include <cstring>

double FPMUL::toDouble(uint64_t bits) {
    double d; std::memcpy(&d, &bits, sizeof(d)); return d;
}

uint64_t FPMUL::fromDouble(double val) {
    uint64_t b; std::memcpy(&b, &val, sizeof(b)); return b;
}

void FPMUL::evaluate(TomasuloBus& bus) {
    bus.FPMUL_Free_o = !m_busy;
    bus.FPMUL_CDBReq_o = false;

    if (m_done) {
        bus.FPMUL_CDBReq_o = true;
        bus.FPMUL_CDBROBTag_o = m_robTag;
        bus.FPMUL_CDBValue_o = m_result;
        bus.FPMUL_CDBException_o = m_exception;
        bus.FPMUL_CDBFlags_o = 0;
        bus.FPMUL_CDBFlagsValid_o = false;  // FPMUL never generates flags
    }
}

void FPMUL::clockEdge(TomasuloBus& bus) {
    if (bus.Flush_o) {
        m_busy = false; m_done = false;
        return;
    }

    // Release on CDB accept
    if (m_done && !bus.FPMUL_CDBStall_o) {
        m_busy = false;
        m_done = false;
        std::cout << "[FPMUL] CDB accepted. ROB#" << (int)m_robTag << "\n";
    }

    // Count down
    if (m_busy && !m_done) {
        m_cyclesRemaining--;
        if (m_cyclesRemaining == 0) {
            double a = toDouble(m_A);
            double b = toDouble(m_B);
            double r = 0.0;
            m_exception = 0x0;

            switch (m_op) {
            case 0x20: case 0x25: r = a * b; break;       // FMUL, FMULI
            case 0x21: case 0x26:                          // FDIV, FDIVI
                if (b == 0.0 && !std::isnan(b)) {
                    m_exception = 0x4;
                    r = std::copysign(INFINITY, a);
                }
                else { r = a / b; }
                break;
            case 0x28: case 0x52:                          // FSQRT, FSQRTI
                if (b < 0.0 && !std::isnan(b)) {
                    m_exception = 0x5;
                    r = NAN;
                }
                else { r = std::sqrt(b); }
                break;
            }
            m_result = fromDouble(r);
            m_done = true;

            std::cout << "[FPMUL] Complete: op=0x" << std::hex << (int)m_op
                << " result=0x" << m_result << std::dec
                << " ROB#" << (int)m_robTag << "\n";
        }
    }

    // Accept new
    if (!m_busy && bus.FPMUL_Start_o) {
        m_busy = true;
        m_done = false;
        m_op = bus.FPMUL_Op_o;
        m_A = bus.FPMUL_A_o;
        m_B = bus.FPMUL_B_o;
        m_robTag = bus.FPMUL_DestROBTag_o;

        switch (m_op) {
        case 0x20: case 0x25: m_cyclesRemaining = 5; break;
        case 0x21: case 0x26: m_cyclesRemaining = 20; break;
        case 0x28: case 0x52: m_cyclesRemaining = 25; break;
        default: m_cyclesRemaining = 5; break;
        }

        std::cout << "[FPMUL] Start: op=0x" << std::hex << (int)m_op
            << std::dec << " cycles=" << m_cyclesRemaining
            << " ROB#" << (int)m_robTag << "\n";
    }
}

void FPMUL::reset() {
    m_busy = false; m_done = false; m_cyclesRemaining = 0;
    std::cout << "[FPMUL] reset()\n";
}