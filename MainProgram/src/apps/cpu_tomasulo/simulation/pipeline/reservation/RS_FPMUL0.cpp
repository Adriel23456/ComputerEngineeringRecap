// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_FPMUL0.cpp
// ============================================================================

/**
 * @file RS_FPMUL0.cpp
 * @brief Component 21 — RS_FPMUL0 bus accessor implementation.
 *
 * Maps the abstract ReservationStation interface to the concrete
 * RSFPMUL0_* bus signals used by the FPMUL arbiter and execution unit.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/reservation/RS_FPMUL0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"

 // ── Read accessors ────────────────────────────────────────────────────────
bool RS_FPMUL0::readAlloc(const TomasuloBus& bus) const { return bus.AllocRSFPMUL0_o; }
bool RS_FPMUL0::readFree(const TomasuloBus& bus) const { return bus.FreeRSFPMUL0_o; }
bool RS_FPMUL0::readGrant(const TomasuloBus& bus) const { return bus.RSFPMUL0_Grant_o; }

// ── Write accessors ───────────────────────────────────────────────────────
void RS_FPMUL0::writeBusy(TomasuloBus& bus, bool v)     const { bus.RS_FPMUL0_Busy_o = v; }
void RS_FPMUL0::writeReq(TomasuloBus& bus, bool v)     const { bus.RSFPMUL0_Req_o = v; }
void RS_FPMUL0::writeOp(TomasuloBus& bus, uint8_t v)  const { bus.RSFPMUL0_Op_o = v; }
void RS_FPMUL0::writeVj(TomasuloBus& bus, uint64_t v) const { bus.RSFPMUL0_Vj_o = v; }
void RS_FPMUL0::writeVk(TomasuloBus& bus, uint64_t v) const { bus.RSFPMUL0_Vk_o = v; }
void RS_FPMUL0::writeDestROBTag(TomasuloBus& bus, uint8_t v)  const { bus.RSFPMUL0_DestROBTag_o = v; }
void RS_FPMUL0::writeModFlags(TomasuloBus& bus, bool v)     const { bus.RSFPMUL0_ModFlags_o = v; }
void RS_FPMUL0::writeNeedsFlags(TomasuloBus& bus, bool v)     const { bus.RSFPMUL0_NeedsFlags_o = v; }
void RS_FPMUL0::writeFlagsValue(TomasuloBus& bus, uint8_t v)  const { bus.RSFPMUL0_FlagsValue_o = v; }