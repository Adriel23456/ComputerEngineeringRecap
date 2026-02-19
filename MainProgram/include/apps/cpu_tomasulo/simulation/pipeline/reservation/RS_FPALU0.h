// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_FPALU0.h
// ============================================================================

#pragma once

/**
 * @file RS_FPALU0.h
 * @brief Component 19: Floating-Point ALU Reservation Station 0.
 *
 * Serves the FPALU execution unit.  Handles both binary FP arithmetic
 * (FADD, FSUB, FMUL, FDIV, FMAX, FMIN, FCMP, …) and unary FP operations
 * (FNEG, FABS, conversions, rounding modes) which skip Vj and only use Vk.
 *
 * Unary ops (isUnaryOp returns true):
 *   Binary encoding  0x29–0x30 : FNEG, FABS, CDTI, CDTD, RTNR, RTZ, RTP, RTNE
 *   Immediate encode 0x53–0x5A : FNEGI, FABSI, CDTII, CDTDI, RTNRI, RTZI, RTPI, RTNEI
 *
 * Inherits all Tomasulo RS logic from ReservationStation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/reservation/ReservationStation.h"

class RS_FPALU0 : public ReservationStation {
public:
    RS_FPALU0() : ReservationStation("RS_FPALU0") {}
    const char* name() const override { return "RS_FPALU0"; }

protected:
    /**
     * @brief Identifies FP unary ops that do not require a first source operand (Vj).
     *
     * Ranges:
     *   0x29–0x30 : register-form unary ops.
     *   0x53–0x5A : immediate-form unary ops.
     */
    bool isUnaryOp(uint8_t op) const override {
        return (op >= 0x29 && op <= 0x30)
            || (op >= 0x53 && op <= 0x5A);
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