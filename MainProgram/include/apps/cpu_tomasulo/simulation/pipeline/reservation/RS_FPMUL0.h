// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_FPMUL0.h
// ============================================================================

#pragma once

/**
 * @file RS_FPMUL0.h
 * @brief Component 21: Floating-Point MUL/DIV/SQRT Reservation Station 0.
 *
 * Serves the FPMUL execution unit.  Multi-cycle unit — uses m_executing
 * instead of m_dispatched so the slot remains occupied for the full
 * latency (FPMUL = 4 cycles, FPDIV = 16 cycles, FSQRT = 16 cycles).
 *
 * Unary ops (isUnaryOp returns true):
 *   0x28 FSQRT  (register form)
 *   0x52 FSQRTI (immediate form)
 * Both skip Vj — the radicand is supplied via Vk (Rm).
 *
 * Inherits all Tomasulo RS logic from ReservationStation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/reservation/ReservationStation.h"

class RS_FPMUL0 : public ReservationStation {
public:
    RS_FPMUL0() : ReservationStation("RS_FPMUL0") {}
    const char* name() const override { return "RS_FPMUL0"; }

protected:
    /** @brief Multi-cycle: slot stays occupied while FPMUL unit is executing. */
    bool isMultiCycle() const override { return true; }

    /**
     * @brief FSQRT (0x28) and FSQRTI (0x52) use only Vk; Vj is skipped.
     */
    bool isUnaryOp(uint8_t op) const override {
        return (op == 0x28 || op == 0x52);
    }

    // ── Bus accessors ─────────────────────────────────────────────
    bool     readAlloc(const TomasuloBus& bus) const override;
    bool     readFree(const TomasuloBus& bus) const override;
    bool     readGrant(const TomasuloBus& bus) const override;
    void writeBusy(TomasuloBus& bus, bool v)     const override;
    void writeReq(TomasuloBus& bus, bool v)     const override;
    void writeOp(TomasuloBus& bus, uint8_t v)  const override;
    void writeVj(TomasuloBus& bus, uint64_t v) const override;
    void writeVk(TomasuloBus& bus, uint64_t v) const override;
    void writeDestROBTag(TomasuloBus& bus, uint8_t v)  const override;
    void writeModFlags(TomasuloBus& bus, bool v)     const override;
    void writeNeedsFlags(TomasuloBus& bus, bool v)     const override;
    void writeFlagsValue(TomasuloBus& bus, uint8_t v)  const override;
};