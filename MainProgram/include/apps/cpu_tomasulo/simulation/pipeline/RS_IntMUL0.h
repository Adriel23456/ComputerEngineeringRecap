#pragma once

/**
 * @file RS_IntMUL0.h
 * @brief Component 20: Integer MUL/DIV Reservation Station 0.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ReservationStation.h"

class RS_IntMUL0 : public ReservationStation {
public:
    RS_IntMUL0() : ReservationStation("RS_IntMUL0") {}
    const char* name() const override { return "RS_IntMUL0"; }

protected:
    // Multi-cycle: uses Executing bit instead of Dispatched
    bool isMultiCycle() const override { return true; }

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