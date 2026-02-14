#pragma once

/**
 * @file SB0.h
 * @brief Component 12: Store Buffer 0.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/StoreBuffer.h"

class SB0 : public StoreBuffer {
public:
    SB0() : StoreBuffer(ID::SB0) {}
    const char* name() const override { return "SB0"; }

protected:
    bool     readAlloc(const TomasuloBus& bus) const override;
    bool     readFree(const TomasuloBus& bus) const override;
    bool     readAGUDone(const TomasuloBus& bus) const override;
    uint64_t readAGUAddress(const TomasuloBus& bus) const override;
    bool     readAGUSegFault(const TomasuloBus& bus) const override;

    void writeBusy(TomasuloBus& bus, bool v) const override;
    void writeAGUReq(TomasuloBus& bus, bool v) const override;
    void writeAGUBase(TomasuloBus& bus, uint64_t v) const override;
    void writeAGUOffset(TomasuloBus& bus, uint64_t v) const override;
    void writeAGUROBTag(TomasuloBus& bus, uint8_t v) const override;
    void writeStCompleteValid(TomasuloBus& bus, bool v) const override;
    void writeStCompleteROBTag(TomasuloBus& bus, uint8_t v) const override;
    void writeStCompleteAddr(TomasuloBus& bus, uint64_t v) const override;
    void writeStCompleteData(TomasuloBus& bus, uint64_t v) const override;
};