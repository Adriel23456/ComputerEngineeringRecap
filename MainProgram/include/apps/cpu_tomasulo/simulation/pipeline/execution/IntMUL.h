// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/execution/IntMUL.h
// ============================================================================

#pragma once

/**
 * @file IntMUL.h
 * @brief Component 31: Integer Multiplier/Divider. NOT pipelined.
 *
 * Fixed latencies:
 *   MUL / MULI —  4 cycles
 *   DIV / DIVI — 16 cycles
 *
 * Division by zero raises exception code 0x3; the result is set to 0.
 * Both operations produce simplified flags (N and Z only; C=V=0).
 *
 * The unit holds its result until the CDB arbiter grants access. A new
 * operation is only accepted once the previous result has been broadcast.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class IntMUL : public ITomasuloComponent {
public:
    const char* name() const override { return "IntMUL"; }
    /** @brief Drives IntMUL_Free_o and IntMUL_CDBReq_o when m_done is true. */
    void evaluate(TomasuloBus& bus) override;
    /** @brief Counts down m_cyclesRemaining, computes result, accepts new ops. */
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    bool     m_busy = false; ///< True while an operation is in-flight.
    int      m_cyclesRemaining = 0;     ///< Cycles left before the result is ready.
    uint8_t  m_op = 0;     ///< Opcode of the in-flight operation.
    uint64_t m_A = 0;     ///< First operand.
    uint64_t m_B = 0;     ///< Second operand (divisor for DIV).
    uint8_t  m_robTag = 0;     ///< ROB destination tag.
    uint64_t m_result = 0;     ///< Computed result.
    uint8_t  m_flags = 0;     ///< (N,Z,0,0) flags derived from m_result.
    bool     m_flagsValid = false; ///< Always true once m_done is set.
    uint8_t  m_exception = 0;     ///< 0x3 = divide-by-zero, 0x0 = none.
    bool     m_done = false; ///< True once result is ready to broadcast.
};