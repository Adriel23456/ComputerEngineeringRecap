// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_0.h
// ============================================================================

#pragma once

/**
 * @file AGU_0.h
 * @brief AGU Unit 0 (Component 24).
 *
 * Concrete instantiation of AGU that binds to the AGU0_* bus signals.
 * AGU_Arbiter assigns the highest-priority pending request to AGU_0;
 * this class reads from AGU0_Start_o / AGU0_Base_o / AGU0_Offset_o and
 * writes results to AGU0_Done_o / AGU0_Address_o / AGU0_SegFault_o.
 *
 * All computation logic is in the AGU base class.
 *
 * @note
 *   - OCP: Bus wiring is isolated here; AGU logic is unchanged.
 *   - LSP: Fully substitutable for AGU in any context.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/agu/AGU.h"

class AGU_0 : public AGU {
public:
    AGU_0() : AGU(ID::AGU0) {}
    const char* name() const override { return "AGU_0"; }

protected:
    bool     readStart(const TomasuloBus& bus)    const override;
    uint64_t readBase(const TomasuloBus& bus)     const override;
    uint64_t readOffset(const TomasuloBus& bus)   const override;
    uint8_t  readSourceID(const TomasuloBus& bus) const override;

    void writeFree(TomasuloBus& bus, bool v)           const override;
    void writeDone(TomasuloBus& bus, bool v)           const override;
    void writeAddress(TomasuloBus& bus, uint64_t v)    const override;
    void writeSegFault(TomasuloBus& bus, bool v)       const override;
    void writeSourceIDOut(TomasuloBus& bus, uint8_t v) const override;
};