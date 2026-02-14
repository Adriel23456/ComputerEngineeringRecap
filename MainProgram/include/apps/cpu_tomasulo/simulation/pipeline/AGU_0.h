#pragma once

#include "apps/cpu_tomasulo/simulation/pipeline/AGU.h"

class AGU_0 : public AGU {
public:
    AGU_0() : AGU(ID::AGU0) {}
    const char* name() const override { return "AGU_0"; }

protected:
    bool     readStart(const TomasuloBus& bus) const override;
    uint64_t readBase(const TomasuloBus& bus) const override;
    uint64_t readOffset(const TomasuloBus& bus) const override;
    uint8_t  readSourceID(const TomasuloBus& bus) const override;
    void writeFree(TomasuloBus& bus, bool v) const override;
    void writeDone(TomasuloBus& bus, bool v) const override;
    void writeAddress(TomasuloBus& bus, uint64_t v) const override;
    void writeSegFault(TomasuloBus& bus, bool v) const override;
    void writeSourceIDOut(TomasuloBus& bus, uint8_t v) const override;
};