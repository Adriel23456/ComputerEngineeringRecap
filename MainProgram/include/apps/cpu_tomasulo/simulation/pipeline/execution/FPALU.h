// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/execution/FPALU.h
// ============================================================================

#pragma once

/**
 * @file FPALU.h
 * @brief Component 29: Floating-Point ALU. 3-stage pipeline.
 *
 * Supports: FADD, FSUB, FCOPYSIGN, FNEG, FABS, CDTI, CDTD, RTNR, RTZ,
 * RTP, RTNE, FCMP, FCMN, FCMPS, and their immediate variants.
 *
 * Pipeline stages:
 *   Stage 1: Accept operands, begin partial computation (input latch).
 *   Stage 2: Execute the main operation (double-precision arithmetic).
 *   Stage 3: Finalize result, compute flags for FCMP variants,
 *            detect FP exceptions (NaN from valid inputs).
 *
 * An outputHold register preserves stage-3 results when the CDB stalls,
 * preventing the stage from being overwritten while waiting for arbitration.
 *
 * @note FPALU_Free_o is de-asserted while stage 1 is occupied.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class FPALU : public ITomasuloComponent {
public:
    const char* name() const override { return "FPALU"; }
    /** @brief Drives FPALU_Free_o and FPALU_CDBReq_o from stage 3 / outputHold. */
    void evaluate(TomasuloBus& bus) override;
    /** @brief Advances the three pipeline stages and accepts new input. */
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    /** @brief State held by each pipeline register. */
    struct Stage {
        bool     active = false;  ///< This stage contains a valid in-flight operation.
        uint8_t  op = 0;      ///< Original opcode.
        uint64_t A = 0;      ///< First operand (raw bits).
        uint64_t B = 0;      ///< Second operand (raw bits).
        uint64_t result = 0;      ///< Partial or final result (raw bits).
        uint8_t  robTag = 0;      ///< ROB destination tag.
        bool     modFlags = false;  ///< Operation produces a flags result.
        uint8_t  flags = 0;      ///< Computed (N,Z,C,V) flags (valid when flagsValid).
        bool     flagsValid = false; ///< True for FCMP/FCMN/FCMPS variants.
        uint8_t  exception = 0;      ///< FP exception code (0x1 = invalid, 0x0 = none).
    };

    Stage m_stage1;     ///< Input latch — holds operands for the current op.
    Stage m_stage2;     ///< Execution stage — partial result computed.
    Stage m_stage3;     ///< Finalization stage — final result and flags ready.
    Stage m_outputHold; ///< Holds stage-3 result while waiting for CDB arbitration.

    /** @brief Reinterprets 64-bit raw bits as a double. */
    static double   toDouble(uint64_t bits);
    /** @brief Reinterprets a double as 64-bit raw bits. */
    static uint64_t fromDouble(double val);
    /**
     * @brief Computes FCMP flag bits (N,Z,C,V) for operands A and B.
     * V is set when either operand is NaN (unordered comparison).
     */
    static uint8_t  computeFCMPFlags(uint64_t A, uint64_t B);
};