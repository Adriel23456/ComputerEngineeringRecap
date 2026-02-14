#pragma once

/**
 * @file RS_IntALU1.h
 * @brief Component 18: Integer ALU Reservation Station 1.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ReservationStation.h"

class RS_IntALU1 : public ReservationStation {
public:
    RS_IntALU1() : ReservationStation("RS_IntALU1") {}
    const char* name() const override { return "RS_IntALU1"; }

protected:
    bool isMOVLikeOp(uint8_t op) const override {
        return (op == 0x31 || op == 0x32);
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