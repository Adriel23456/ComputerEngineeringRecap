#pragma once

/**
 * @file RS_FPMUL0.h
 * @brief Component 21: FP MUL/DIV/SQRT Reservation Station 0.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ReservationStation.h"

class RS_FPMUL0 : public ReservationStation {
public:
    RS_FPMUL0() : ReservationStation("RS_FPMUL0") {}
    const char* name() const override { return "RS_FPMUL0"; }

protected:
    bool isMultiCycle() const override { return true; }

    // FSQRT(0x28) and FSQRTI(0x52): only use Vk (from Rm), skip Vj
    bool isUnaryOp(uint8_t op) const override {
        return (op == 0x28 || op == 0x52);
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