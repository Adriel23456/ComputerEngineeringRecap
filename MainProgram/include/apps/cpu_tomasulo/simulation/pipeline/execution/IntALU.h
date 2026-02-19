// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/execution/IntALU.h
// ============================================================================

#pragma once

/**
 * @file IntALU.h
 * @brief Component 27: Integer ALU. Single-cycle, purely combinational.
 *
 * Executes ADD, SUB, ADC, SBC, AND, ORR, EOR, BIC, LSL, LSR, ASR, ROR,
 * INC, DEC, MOV, MVN, MOVI, MVNI, FMOVI, FMVNI, CMP, CMN, TST, TEQ
 * (and their immediate variants) in a single evaluate() call.
 *
 * The unit is considered free unless the CDB arbiter stalls it
 * (IntALU_CDBStall_o == 1). Stalling holds the result for one extra cycle
 * without losing data, since the ALU is purely combinational.
 *
 * Flag computation:
 *   N = result[63], Z = (result == 0), C = carry-out, V = signed overflow.
 *   For CMP/CMN/TST/TEQ the flags are derived from a temporary value;
 *   no result register is written.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class IntALU : public ITomasuloComponent {
public:
    const char* name() const override { return "IntALU"; }
    /** @brief Computes the ALU result and drives IntALU_CDB* signals. */
    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}  ///< Pure combinational — no state.
    void reset() override;
};