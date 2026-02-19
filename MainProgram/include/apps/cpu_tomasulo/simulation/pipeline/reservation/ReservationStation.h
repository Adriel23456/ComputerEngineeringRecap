// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/reservation/ReservationStation.h
// ============================================================================

#pragma once

/**
 * @file ReservationStation.h
 * @brief Base class for Components 17-21 (RS_IntALU0/1, RS_FPALU0, RS_IntMUL0, RS_FPMUL0).
 *
 * Implements the common two-operand Tomasulo reservation station protocol.
 * Each instance tracks a single instruction slot through its lifecycle:
 *
 *   IDLE → ALLOCATED → operand wait → READY → DISPATCHED/EXECUTING → FREED
 *
 * Operand resolution — 3-tier for both data (Vj/Vk) and flags:
 *   Tier 1: No rename tag (QiValid == false)  → use architectural value directly.
 *   Tier 2: Tag valid, ROB entry ready        → forward value from ROB.
 *   Tier 3: Tag valid, ROB entry not ready    → wait for CDB broadcast.
 *   Fallback (flags only): FlagsQiValid drops → switch to architectural flags.
 *
 * Ready condition:
 *   m_busy && !m_qjValid && !m_qkValid && !dispatchBlock
 *   && (!m_needsFlags || !m_flagsQiValid)
 *
 * Subclasses supply only bus accessor overrides and behavioral queries:
 *   - isUnaryOp(op)    : FPALU unary ops / FSQRT — skip Vj, only use Vk.
 *   - isMOVLikeOp(op)  : MOV/MVN — skip Vj, only use Vk.
 *   - isMultiCycle()   : IntMUL/FPMUL — set m_executing instead of m_dispatched.
 *
 * @note RS_Branch0 is NOT derived from this class — it uses a fundamentally
 *       different structure (no register operands, only flags + target PC).
 * @note SRP: Each subclass encapsulates exactly one pipeline station slot.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/core/ITomasuloComponent.h"
#include <cstdint>

class ReservationStation : public ITomasuloComponent {
public:
    explicit ReservationStation(const char* name);

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override;
    void reset() override;

protected:
    // ── Behavioral queries (overridden by subclasses) ─────────────────────
    /** @brief True for multi-cycle units (IntMUL, FPMUL) — uses m_executing. */
    virtual bool isMultiCycle() const { return false; }
    /** @brief True for unary ops (FPALU unary, FSQRT) — Vj is not needed. */
    virtual bool isUnaryOp(uint8_t op) const { return false; }
    /** @brief True for MOV/MVN — Vj is not needed, only Vk from Rm. */
    virtual bool isMOVLikeOp(uint8_t op) const { return false; }

    // ── Bus accessor interface (pure virtual — one override per instance) ──

    /** @brief Read the allocation signal for this station from the bus. */
    virtual bool readAlloc(const TomasuloBus& bus) const = 0;
    /** @brief Read the free/retire signal for this station from the bus. */
    virtual bool readFree(const TomasuloBus& bus)  const = 0;
    /** @brief Read the arbiter grant signal for this station from the bus. */
    virtual bool readGrant(const TomasuloBus& bus) const = 0;

    /** @brief Write m_busy to the station's Busy output bus field. */
    virtual void writeBusy(TomasuloBus& bus, bool v)     const = 0;
    /** @brief Write the ready/request signal to the arbiter bus field. */
    virtual void writeReq(TomasuloBus& bus, bool v)      const = 0;
    /** @brief Write the latched opcode to the execution unit bus field. */
    virtual void writeOp(TomasuloBus& bus, uint8_t v)    const = 0;
    /** @brief Write the resolved first operand (Vj) to the bus. */
    virtual void writeVj(TomasuloBus& bus, uint64_t v)   const = 0;
    /** @brief Write the resolved second operand (Vk) to the bus. */
    virtual void writeVk(TomasuloBus& bus, uint64_t v)   const = 0;
    /** @brief Write the destination ROB tag to the bus. */
    virtual void writeDestROBTag(TomasuloBus& bus, uint8_t v) const = 0;
    /** @brief Write the modifies-flags bit to the execution unit bus field. */
    virtual void writeModFlags(TomasuloBus& bus, bool v)    const = 0;
    /** @brief Write the needs-flags bit to the execution unit bus field. */
    virtual void writeNeedsFlags(TomasuloBus& bus, bool v)  const = 0;
    /** @brief Write the resolved flags value to the execution unit bus field. */
    virtual void writeFlagsValue(TomasuloBus& bus, uint8_t v) const = 0;

private:
    const char* m_name; ///< Station name string (used in reset log message).

    // ── Slot state ────────────────────────────────────────────────────────
    bool    m_busy = false; ///< True when this slot holds a pending instruction.
    uint8_t m_op = 0;     ///< Latched opcode.

    // ── Operand 1 (Vj = Rn) ──────────────────────────────────────────────
    uint64_t m_vj = 0;     ///< Resolved value of source operand 1.
    uint8_t  m_qj = 0;     ///< ROB tag to wait on for operand 1.
    bool     m_qjValid = false; ///< True while waiting on m_qj from CDB.

    // ── Operand 2 (Vk = Rm or Imm) ────────────────────────────────────────
    uint64_t m_vk = 0;     ///< Resolved value of source operand 2 (or immediate).
    uint8_t  m_qk = 0;     ///< ROB tag to wait on for operand 2.
    bool     m_qkValid = false; ///< True while waiting on m_qk from CDB.

    // ── Destination ───────────────────────────────────────────────────────
    uint8_t m_destROBTag = 0;   ///< ROB entry that will receive the result.

    // ── Instruction attributes ────────────────────────────────────────────
    bool m_useImm = false; ///< True when operand 2 is an immediate (Vk = ImmExt).
    bool m_modifiesFlags = false;///< True when this instruction produces NZCV flags.
    bool m_needsFlags = false;///< True when this instruction consumes flags (ADC/SBC/etc).

    // ── Flags dependency ─────────────────────────────────────────────────
    uint8_t m_flagsQi = 0;     ///< ROB tag of the in-flight flags producer.
    bool    m_flagsQiValid = false; ///< True while waiting for flags from CDB.
    uint8_t m_flagsValue = 0;     ///< Resolved NZCV value (written when qiValid clears).

    // ── Dispatch tracking ─────────────────────────────────────────────────
    bool m_dispatched = false; ///< Set on grant for single-cycle units (IntALU, FPALU).
    bool m_executing = false; ///< Set on grant for multi-cycle units (IntMUL, FPMUL).
};