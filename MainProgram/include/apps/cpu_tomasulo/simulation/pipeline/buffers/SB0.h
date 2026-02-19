// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/buffers/SB0.h
// ============================================================================

#pragma once

/**
 * @file SB0.h
 * @brief Store Buffer 0 (Component 12).
 *
 * Concrete StoreBuffer binding to the SB0_* bus signals.
 * Receives the higher-priority store allocation from Control_Unit
 * (AllocSB0_o) and is freed by Commit_Unit (FreeSB0_o) after the
 * store has been written to D-Cache.
 *
 * All state machine logic is in the StoreBuffer base class.
 *
 * @note OCP: Bus wiring is isolated here; StoreBuffer logic is unchanged.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/buffers/StoreBuffer.h"

class SB0 : public StoreBuffer {
public:
    SB0() : StoreBuffer(ID::SB0) {}
    const char* name() const override { return "SB0"; }

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