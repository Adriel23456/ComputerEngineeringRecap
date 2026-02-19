// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/decode/Control_Unit.h
// ============================================================================

#pragma once

/**
 * @file Control_Unit.h
 * @brief Component 8: Issue-stage control — decode, resource check, allocate.
 *
 * Determines instruction class, checks resource availability,
 * drives allocation signals for RS/Buffers/ROB, and controls
 * register renaming tag writes.
 *
 * Issue proceeds in seven steps each cycle:
 *   1. Wait for I-Cache (InstReady_o).
 *   2. Decode opcode into a DecodeResult.
 *   3. Stall if ROB is full.
 *   4. Route NOP/SWI directly to ROB (no execution unit needed).
 *   5. Find a free RS or buffer for the instruction class.
 *   6. Stall if no resource is available.
 *   7. Assert ROBAlloc_o and tag-write enables.
 *
 * @note SRP: Only handles issue-stage decode and allocation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class Control_Unit : public ITomasuloComponent {
public:
    const char* name() const override { return "Control_Unit"; }
    /** @brief Drives all issue-stage allocation and stall signals. */
    void evaluate(TomasuloBus& bus) override;
    /** @brief Pure combinational — no state to latch. */
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

private:
    /**
     * @brief Internal decode result — local to evaluate(), never placed on the bus directly.
     *
     * Populated by decode() and consumed by the resource-allocation logic
     * and allocTypeFromDecode() to build the bus signals.
     */
    struct DecodeResult {
        bool isIntALU = false;  ///< Routes to RS_IntALU0/1 and IntALU.
        bool isIntMUL = false;  ///< Routes to RS_IntMUL0 and IntMUL.
        bool isFPALU = false;  ///< Routes to RS_FPALU0 and FPALU.
        bool isFPMUL = false;  ///< Routes to RS_FPMUL0 and FPMUL.
        bool isLoad = false;  ///< Routes to LB0/1/2.
        bool isStore = false;  ///< Routes to SB0/1.
        bool isBranch = false;  ///< Routes to RS_Branch0 and Branch_Executor.
        bool isNOP = false;  ///< No execution unit; ROB-only allocation.
        bool isSWI = false;  ///< Software interrupt; ROB-only, triggers halt on commit.
        bool isCMPonly = false;  ///< Produces flags only, no destination register write.
        bool isUnconditional = false; ///< Unconditional branch (B); no flags needed.
        bool modifiesFlags = false;  ///< Instruction produces a new (N,Z,C,V) result.
        bool needsFlags = false;  ///< Instruction reads the current flag register (ADC/SBC/branches).
        bool useImm = false;  ///< Second operand is ImmExt_o rather than Rm.
        bool writesRd = false;  ///< Instruction writes a result to Rd (drives TagWrEn_o).
    };

    /**
     * @brief Decodes an 8-bit opcode into a DecodeResult.
     * @param op  Raw opcode from Op_in_o.
     */
    static DecodeResult decode(uint8_t op);

    /**
     * @brief Maps a DecodeResult to the 3-bit AllocType_o encoding.
     * 000=IntALU, 001=IntMUL, 010=FPALU, 011=FPMUL, 100=Load,
     * 101=Store, 110=Branch, 111=CMP-only.
     */
    static uint8_t allocTypeFromDecode(const DecodeResult& d);
};