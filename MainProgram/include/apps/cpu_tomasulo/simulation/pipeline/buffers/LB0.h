// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/buffers/LB0.h
// ============================================================================

#pragma once

/**
 * @file LB0.h
 * @brief Load Buffer 0 (Component 14).
 *
 * Concrete LoadBuffer binding to the LB0_* bus signals.
 * Receives the highest-priority load allocation from Control_Unit
 * (AllocLB0_o) and is freed by Commit_Unit (FreeLB0_o) after the
 * loaded value has been committed.
 *
 * All state machine logic is in the LoadBuffer base class.
 *
 * @note OCP: Bus wiring is isolated here; LoadBuffer logic is unchanged.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/buffers/LoadBuffer.h"

class LB0 : public LoadBuffer {
public:
    LB0() : LoadBuffer(ID::LB0) {}
    const char* name() const override { return "LB0"; }

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