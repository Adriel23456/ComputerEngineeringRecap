// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_0.cpp
// ============================================================================

/**
 * @file AGU_0.cpp
 * @brief Bus accessor implementation for AGU_0 (Component 24).
 *
 * Maps the abstract AGU read/write interface to the AGU0_* bus fields
 * set by AGU_Arbiter. No logic here — all in AGU base class.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/agu/AGU_0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"

bool     AGU_0::readStart(const TomasuloBus& bus)    const { return bus.AGU0_Start_o; }
uint64_t AGU_0::readBase(const TomasuloBus& bus)     const { return bus.AGU0_Base_o; }
uint64_t AGU_0::readOffset(const TomasuloBus& bus)   const { return bus.AGU0_Offset_o; }
uint8_t  AGU_0::readSourceID(const TomasuloBus& bus) const { return bus.AGU0_SourceID_o; }

void AGU_0::writeFree(TomasuloBus& bus, bool v)           const { bus.AGU0_Free_o = v; }
void AGU_0::writeDone(TomasuloBus& bus, bool v)           const { bus.AGU0_Done_o = v; }
void AGU_0::writeAddress(TomasuloBus& bus, uint64_t v)    const { bus.AGU0_Address_o = v; }
void AGU_0::writeSegFault(TomasuloBus& bus, bool v)       const { bus.AGU0_SegFault_o = v; }
void AGU_0::writeSourceIDOut(TomasuloBus& bus, uint8_t v) const { bus.AGU0_SourceID_out = v; }