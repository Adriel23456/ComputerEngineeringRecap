// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/execution/FPMUL.h
// ============================================================================

#pragma once

/**
 * @file FPMUL.h
 * @brief Component 33: FP MUL/DIV/SQRT. NOT pipelined.
 *
 * Executes one operation at a time with a fixed latency:
 *   FMUL / FMULI  —  5 cycles
 *   FDIV / FDIVI  — 20 cycles
 *   FSQRT / FSQRTI — 25 cycles
 *
 * The unit is considered busy from the cycle it accepts an operation
 * until the cycle it successfully broadcasts on the CDB.
 * Division-by-zero and sqrt-of-negative produce IEEE 754 results (±Inf/NaN)
 * rather than CPU exceptions — the program checks via FCMPS + BUN.
 *
 * @note FPMUL never generates flag results; FPMUL_CDBFlagsValid_o is always 0.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class FPMUL : public ITomasuloComponent {
public:
    const char* name() const override { return "FPMUL"; }
    /** @brief Drives FPMUL_Free_o and FPMUL_CDBReq_o when m_done is true. */
    void evaluate(TomasuloBus& bus) override;
    /** @brief Counts down m_cyclesRemaining, computes the result, accepts new ops. */
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    bool     m_busy = false; ///< True while an operation is in-flight.
    int      m_cyclesRemaining = 0;     ///< Cycles left before the result is ready.
    uint8_t  m_op = 0;     ///< Opcode of the in-flight operation.
    uint64_t m_A = 0;     ///< First operand (raw double bits).
    uint64_t m_B = 0;     ///< Second operand (raw double bits).
    uint8_t  m_robTag = 0;     ///< ROB destination tag.
    uint64_t m_result = 0;     ///< Computed result (raw double bits).
    uint8_t  m_exception = 0;     ///< Exception code (always 0 for FPMUL).
    bool     m_done = false; ///< True once result is ready to broadcast.

    /** @brief Reinterprets 64-bit raw bits as a double. */
    static double   toDouble(uint64_t bits);
    /** @brief Reinterprets a double as 64-bit raw bits. */
    static uint64_t fromDouble(double val);
};