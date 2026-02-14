/**
 * @file IntMUL.cpp
 * @brief Component 31 implementation -- Integer MUL/DIV.
 * Non-pipelined: MUL 4 cycles, DIV 16 cycles.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/IntMUL.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void IntMUL::evaluate(TomasuloBus& bus) {
    bus.IntMUL_Free_o = !m_busy;
    bus.IntMUL_CDBReq_o = false;

    if (m_done) {
        bus.IntMUL_CDBReq_o = true;
        bus.IntMUL_CDBROBTag_o = m_robTag;
        bus.IntMUL_CDBValue_o = m_result;
        bus.IntMUL_CDBException_o = m_exception;
        bus.IntMUL_CDBFlags_o = m_flags;
        bus.IntMUL_CDBFlagsValid_o = m_flagsValid;
    }
}

void IntMUL::clockEdge(TomasuloBus& bus) {
    if (bus.Flush_o) {
        m_busy = false; m_done = false;
        return;
    }

    // Release on CDB accept
    if (m_done && !bus.IntMUL_CDBStall_o) {
        m_busy = false;
        m_done = false;
        std::cout << "[IntMUL] CDB accepted. ROB#" << (int)m_robTag << "\n";
    }

    // Count down
    if (m_busy && !m_done) {
        m_cyclesRemaining--;
        if (m_cyclesRemaining == 0) {
            int64_t sA = (int64_t)m_A;
            int64_t sB = (int64_t)m_B;

            switch (m_op) {
            case 0x04: case 0x12:  // MUL, MULI
                m_result = (uint64_t)(sA * sB);
                m_exception = 0x0;
                break;
            case 0x05: case 0x13:  // DIV, DIVI
                if (m_B == 0) {
                    m_result = 0;
                    m_exception = 0x3;  // Divide by zero
                }
                else {
                    m_result = (uint64_t)(sA / sB);
                    m_exception = 0x0;
                }
                break;
            }

            uint8_t N = (m_result >> 63) & 1;
            uint8_t Z = (m_result == 0) ? 1 : 0;
            m_flags = (N << 3) | (Z << 2);  // C=0, V=0 (simplified)
            m_flagsValid = true;
            m_done = true;

            std::cout << "[IntMUL] Complete: op=0x" << std::hex << (int)m_op
                << " result=0x" << m_result << std::dec
                << " ROB#" << (int)m_robTag << "\n";
        }
    }

    // Accept new
    if (!m_busy && bus.IntMUL_Start_o) {
        m_busy = true;
        m_done = false;
        m_op = bus.IntMUL_Op_o;
        m_A = bus.IntMUL_A_o;
        m_B = bus.IntMUL_B_o;
        m_robTag = bus.IntMUL_DestROBTag_o;
        m_cyclesRemaining = (m_op == 0x04 || m_op == 0x12) ? 4 : 16;
        std::cout << "[IntMUL] Start: op=0x" << std::hex << (int)m_op
            << std::dec << " cycles=" << m_cyclesRemaining
            << " ROB#" << (int)m_robTag << "\n";
    }
}

void IntMUL::reset() {
    m_busy = false; m_done = false; m_cyclesRemaining = 0;
    std::cout << "[IntMUL] reset()\n";
}