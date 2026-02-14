#pragma once

/**
 * @file AGU.h
 * @brief Components 24-25: Address Generation Unit (base class).
 *
 * Computes effective address = Base + Offset, checks segmentation bounds.
 * Purely combinational. Routes result directly to the requesting SB/LB
 * based on SourceID.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/ITomasuloComponent.h"
#include <cstdint>

class AGU : public ITomasuloComponent {
public:
    enum class ID : uint8_t { AGU0 = 0, AGU1 = 1 };
    explicit AGU(ID id);

    void evaluate(TomasuloBus& bus) override;
    void clockEdge(TomasuloBus& bus) override {}
    void reset() override;

protected:
    virtual bool     readStart(const TomasuloBus& bus) const = 0;
    virtual uint64_t readBase(const TomasuloBus& bus) const = 0;
    virtual uint64_t readOffset(const TomasuloBus& bus) const = 0;
    virtual uint8_t  readSourceID(const TomasuloBus& bus) const = 0;

    virtual void writeFree(TomasuloBus& bus, bool v) const = 0;
    virtual void writeDone(TomasuloBus& bus, bool v) const = 0;
    virtual void writeAddress(TomasuloBus& bus, uint64_t v) const = 0;
    virtual void writeSegFault(TomasuloBus& bus, bool v) const = 0;
    virtual void writeSourceIDOut(TomasuloBus& bus, uint8_t v) const = 0;

    ID m_id;

private:
    void routeResult(TomasuloBus& bus, uint8_t sourceID,
        uint64_t address, bool segFault) const;
    const char* idStr() const;
};