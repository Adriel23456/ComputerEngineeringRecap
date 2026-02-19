// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/decode/Extends.h
// ============================================================================

#pragma once

/**
 * @file Extends.h
 * @brief Component 7: Immediate Extender (32->64 bit).
 *
 * Integer immediates are sign-extended from 32 to 64 bits.
 * Floating-point immediates undergo a bit-level IEEE 754 single->double
 * conversion so that all downstream FP operands are double-precision.
 *
 * @note Extends evaluates BEFORE Control_Unit in topological order, so it
 *       cannot read IsFPALU_o/IsFPMUL_o from the bus. It re-derives the
 *       FP classification directly from the opcode.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"

class Extends : public ITomasuloComponent {
public:
    const char* name() const override { return "Extends"; }
    /** @brief Writes ImmExt_o: sign-extended integer or float32->double64 bits. */
    void evaluate(TomasuloBus& bus) override;
    /** @brief Pure combinational — no state to latch. */
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    /**
     * @brief Sign-extends a 32-bit value to 64 bits.
     * Bit 31 is replicated into bits [63:32].
     */
    static uint64_t signExtend32(uint32_t imm);

    /**
     * @brief Converts a 32-bit IEEE 754 float to its 64-bit double representation.
     * Performs a bit-level rebias of exponent and mantissa; no arithmetic.
     * Preserves +0, -0, ±Inf, and NaN payloads correctly.
     */
    static uint64_t float32ToDouble64Bits(uint32_t imm);
};