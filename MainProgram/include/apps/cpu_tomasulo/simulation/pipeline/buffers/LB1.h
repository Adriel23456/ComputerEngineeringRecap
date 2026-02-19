// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/buffers/LB1.h
// ============================================================================

#pragma once

/**
 * @file LB1.h
 * @brief Load Buffer 1 (Component 15).
 *
 * Concrete LoadBuffer binding to the LB1_* bus signals.
 * Allocated when LB0 is already busy and a second concurrent load is issued.
 *
 * All state machine logic is in the LoadBuffer base class.
 *
 * @note OCP: Bus wiring is isolated here; LoadBuffer logic is unchanged.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/buffers/LoadBuffer.h"

class LB1 : public LoadBuffer {
public:
    LB1() : LoadBuffer(ID::LB1) {}
    const char* name() const override { return "LB1"; }

protected:
    bool     readAlloc(const TomasuloBus& bus)      const override;
    bool     readFree(const TomasuloBus& bus)       const override;
    bool     readAGUDone(const TomasuloBus& bus)    const override;
    uint64_t readAGUAddress(const TomasuloBus& bus) const override;
    bool     readAGUSegFault(const TomasuloBus& bus)const override;
    bool     readMemDone(const TomasuloBus& bus)    const override;
    uint64_t readMemData(const TomasuloBus& bus)    const override;
    bool     readCDBStall(const TomasuloBus& bus)   const override;

    void writeBusy(TomasuloBus& bus, bool v)          const override;
    void writeAGUReq(TomasuloBus& bus, bool v)        const override;
    void writeAGUBase(TomasuloBus& bus, uint64_t v)   const override;
    void writeAGUOffset(TomasuloBus& bus, uint64_t v) const override;
    void writeAGUROBTag(TomasuloBus& bus, uint8_t v)  const override;
    void writeMemReq(TomasuloBus& bus, bool v)        const override;
    void writeMemAddr(TomasuloBus& bus, uint64_t v)   const override;
    void writeMemOp(TomasuloBus& bus, uint8_t v)      const override;
    void writeMemROBTag(TomasuloBus& bus, uint8_t v)  const override;
    void writeCDBReq(TomasuloBus& bus, bool v)        const override;
    void writeCDBValue(TomasuloBus& bus, uint64_t v)  const override;
    void writeCDBROBTag(TomasuloBus& bus, uint8_t v)  const override;
    void writeCDBException(TomasuloBus& bus, uint8_t v) const override;
};