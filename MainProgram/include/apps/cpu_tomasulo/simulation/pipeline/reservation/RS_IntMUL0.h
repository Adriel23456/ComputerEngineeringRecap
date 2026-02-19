// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntMUL0.h
// ============================================================================

#pragma once

/**
 * @file RS_IntMUL0.h
 * @brief Component 20: Integer MUL/DIV Reservation Station 0.
 *
 * Serves the IntMUL execution unit.  Multi-cycle — uses m_executing so the
 * slot remains occupied for the full latency (MUL = 4 cycles, DIV = 16 cycles).
 * The slot is freed only when the execution unit signals completion via the
 * FreeRSIMUL0_o bus line.
 *
 * Supported ops: MUL (0x04), MULI (0x12), DIV (0x05), DIVI (0x13).
 * No MOV-like or unary variants — both Vj and Vk are always required.
 *
 * Inherits all Tomasulo RS logic from ReservationStation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/reservation/ReservationStation.h"

class RS_IntMUL0 : public ReservationStation {
public:
    RS_IntMUL0() : ReservationStation("RS_IntMUL0") {}
    const char* name() const override { return "RS_IntMUL0"; }

protected:
    /** @brief Multi-cycle: slot stays occupied while IntMUL unit is executing. */
    bool isMultiCycle() const override { return true; }

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