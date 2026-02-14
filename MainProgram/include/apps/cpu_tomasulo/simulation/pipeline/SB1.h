#pragma once

/**
 * @file SB1.h
 * @brief Component 13: Store Buffer 1.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/StoreBuffer.h"

class SB1 : public StoreBuffer {
public:
    SB1() : StoreBuffer(ID::SB1) {}
    const char* name() const override { return "SB1"; }

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