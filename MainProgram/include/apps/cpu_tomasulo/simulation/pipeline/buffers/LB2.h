// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/buffers/LB2.h
// ============================================================================

#pragma once

/**
 * @file LB2.h
 * @brief Load Buffer 2 (Component 16).
 *
 * Concrete LoadBuffer binding to the LB2_* bus signals.
 * Third load slot; allocated only when LB0 and LB1 are both busy.
 *
 * All state machine logic is in the LoadBuffer base class.
 *
 * @note OCP: Bus wiring is isolated here; LoadBuffer logic is unchanged.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/buffers/LoadBuffer.h"

class LB2 : public LoadBuffer {
public:
    LB2() : LoadBuffer(ID::LB2) {}
    const char* name() const override { return "LB2"; }

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