// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_1.h
// ============================================================================

#pragma once

/**
 * @file AGU_1.h
 * @brief AGU Unit 1 (Component 25).
 *
 * Concrete instantiation of AGU that binds to the AGU1_* bus signals.
 * AGU_Arbiter assigns the second-highest-priority pending request to AGU_1
 * when AGU_0 is already consuming the top request in the same cycle.
 *
 * All computation logic is in the AGU base class.
 *
 * @note
 *   - OCP: Bus wiring is isolated here; AGU logic is unchanged.
 *   - LSP: Fully substitutable for AGU in any context.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/agu/AGU.h"

class AGU_1 : public AGU {
public:
    AGU_1() : AGU(ID::AGU1) {}
    const char* name() const override { return "AGU_1"; }

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