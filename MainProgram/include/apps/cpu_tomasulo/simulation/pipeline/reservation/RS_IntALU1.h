// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntALU1.h
// ============================================================================

#pragma once

/**
 * @file RS_IntALU1.h
 * @brief Component 18: Integer ALU Reservation Station 1.
 *
 * Serves IntALU execution unit 1.  Identical behavioral policy to RS_IntALU0
 * (single-cycle, MOV-like ops skip Vj) but maps to a separate set of bus
 * signals (RSIAU1_*) allowing the Control_Unit to issue two independent
 * integer ALU operations per cycle.
 *
 * MOV-like ops (isMOVLikeOp returns true):
 *   0x31 MOV, 0x32 MVN — skip Vj, result derived solely from Vk (Rm/Imm).
 *
 * Inherits all Tomasulo RS logic from ReservationStation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/reservation/ReservationStation.h"

class RS_IntALU1 : public ReservationStation {
public:
    RS_IntALU1() : ReservationStation("RS_IntALU1") {}
    const char* name() const override { return "RS_IntALU1"; }

protected:
    /**
     * @brief MOV (0x31) and MVN (0x32) do not use a first source register (Vj).
     */
    bool isMOVLikeOp(uint8_t op) const override {
        return (op == 0x31 || op == 0x32);
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