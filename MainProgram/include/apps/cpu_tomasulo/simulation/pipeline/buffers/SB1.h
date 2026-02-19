// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/buffers/SB1.h
// ============================================================================

#pragma once

/**
 * @file SB1.h
 * @brief Store Buffer 1 (Component 13).
 *
 * Concrete StoreBuffer binding to the SB1_* bus signals.
 * Second store slot; allocated when SB0 is already busy and a second
 * concurrent store is issued in the same or subsequent cycle.
 *
 * All state machine logic is in the StoreBuffer base class.
 *
 * @note OCP: Bus wiring is isolated here; StoreBuffer logic is unchanged.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/buffers/StoreBuffer.h"

class SB1 : public StoreBuffer {
public:
    SB1() : StoreBuffer(ID::SB1) {}
    const char* name() const override { return "SB1"; }

protected:
    bool     readAlloc(const TomasuloBus& bus)      const override;
    bool     readFree(const TomasuloBus& bus)       const override;
    bool     readAGUDone(const TomasuloBus& bus)    const override;
    uint64_t readAGUAddress(const TomasuloBus& bus) const override;
    bool     readAGUSegFault(const TomasuloBus& bus)const override;

    void writeBusy(TomasuloBus& bus, bool v)                const override;
    void writeAGUReq(TomasuloBus& bus, bool v)              const override;
    void writeAGUBase(TomasuloBus& bus, uint64_t v)         const override;
    void writeAGUOffset(TomasuloBus& bus, uint64_t v)       const override;
    void writeAGUROBTag(TomasuloBus& bus, uint8_t v)        const override;
    void writeStCompleteValid(TomasuloBus& bus, bool v)     const override;
    void writeStCompleteROBTag(TomasuloBus& bus, uint8_t v) const override;
    void writeStCompleteAddr(TomasuloBus& bus, uint64_t v)  const override;
    void writeStCompleteData(TomasuloBus& bus, uint64_t v)  const override;
};