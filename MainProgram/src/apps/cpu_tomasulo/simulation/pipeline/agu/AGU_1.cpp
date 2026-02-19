// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/agu/AGU_1.cpp
// ============================================================================

/**
 * @file AGU_1.cpp
 * @brief Bus accessor implementation for AGU_1 (Component 25).
 *
 * Maps the abstract AGU read/write interface to the AGU1_* bus fields
 * set by AGU_Arbiter. No logic here — all in AGU base class.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/agu/AGU_1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"

bool     AGU_1::readStart(const TomasuloBus& bus)    const { return bus.AGU1_Start_o; }
uint64_t AGU_1::readBase(const TomasuloBus& bus)     const { return bus.AGU1_Base_o; }
uint64_t AGU_1::readOffset(const TomasuloBus& bus)   const { return bus.AGU1_Offset_o; }
uint8_t  AGU_1::readSourceID(const TomasuloBus& bus) const { return bus.AGU1_SourceID_o; }

void AGU_1::writeFree(TomasuloBus& bus, bool v)           const { bus.AGU1_Free_o = v; }
void AGU_1::writeDone(TomasuloBus& bus, bool v)           const { bus.AGU1_Done_o = v; }
void AGU_1::writeAddress(TomasuloBus& bus, uint64_t v)    const { bus.AGU1_Address_o = v; }
void AGU_1::writeSegFault(TomasuloBus& bus, bool v)       const { bus.AGU1_SegFault_o = v; }
void AGU_1::writeSourceIDOut(TomasuloBus& bus, uint8_t v) const { bus.AGU1_SourceID_out = v; }