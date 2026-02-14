#pragma once

/**
 * @file RS_FPALU0.h
 * @brief Component 19: FP ALU Reservation Station 0.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ReservationStation.h"

class RS_FPALU0 : public ReservationStation {
public:
    RS_FPALU0() : ReservationStation("RS_FPALU0") {}
    const char* name() const override { return "RS_FPALU0"; }

protected:
    // Unary FP ops: FNEG(0x29), FABS(0x2A), CDTI(0x2B), CDTD(0x2C),
    // RTNR(0x2D), RTZ(0x2E), RTP(0x2F), RTNE(0x30)
    // Unary FP imm: FNEGI(0x53), FABSI(0x54), CDTII(0x55), CDTDI(0x56),
    // RTNRI(0x57), RTZI(0x58), RTPI(0x59), RTNEI(0x5A)
    bool isUnaryOp(uint8_t op) const override {
        return (op >= 0x29 && op <= 0x30)
            || (op >= 0x53 && op <= 0x5A);
    }

    bool     readAlloc(const TomasuloBus& bus) const override;
    bool     readFree(const TomasuloBus& bus) const override;
    bool     readGrant(const TomasuloBus& bus) const override;
    void writeBusy(TomasuloBus& bus, bool v) const override;
    void writeReq(TomasuloBus& bus, bool v) const override;
    void writeOp(TomasuloBus& bus, uint8_t v) const override;
    void writeVj(TomasuloBus& bus, uint64_t v) const override;
    void writeVk(TomasuloBus& bus, uint64_t v) const override;
    void writeDestROBTag(TomasuloBus& bus, uint8_t v) const override;
    void writeModFlags(TomasuloBus& bus, bool v) const override;
    void writeNeedsFlags(TomasuloBus& bus, bool v) const override;
    void writeFlagsValue(TomasuloBus& bus, uint8_t v) const override;
};