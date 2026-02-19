// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntALU0.h
// ============================================================================

#pragma once

/**
 * @file RS_IntALU0.h
 * @brief Component 17: Integer ALU Reservation Station 0.
 *
 * Serves IntALU execution unit 0.  Single-cycle — uses m_dispatched.
 * Handles the full integer ALU instruction set: ADD, SUB, ADC, SBC, AND,
 * ORR, EOR, BIC, LSL, LSR, ASR, ROR, INC, DEC, MOV, MVN, CMP, CMN, TST, TEQ,
 * plus their immediate variants.
 *
 * MOV-like ops (isMOVLikeOp returns true):
 *   0x31 MOV, 0x32 MVN — skip Vj, result is derived solely from Vk (Rm/Imm).
 *
 * Inherits all Tomasulo RS logic from ReservationStation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/reservation/ReservationStation.h"

class RS_IntALU0 : public ReservationStation {
public:
    RS_IntALU0() : ReservationStation("RS_IntALU0") {}
    const char* name() const override { return "RS_IntALU0"; }

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