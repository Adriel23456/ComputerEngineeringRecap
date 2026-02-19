// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntALU0.cpp
// ============================================================================

/**
 * @file RS_IntALU0.cpp
 * @brief Component 17 — RS_IntALU0 bus accessor implementation.
 *
 * Maps the abstract ReservationStation interface to the concrete
 * RSIAU0_* bus signals used by the IntALU0 arbiter and execution unit.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntALU0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"

 // ── Read accessors ────────────────────────────────────────────────────────
bool RS_IntALU0::readAlloc(const TomasuloBus& bus) const { return bus.AllocRSIntALU0_o; }
bool RS_IntALU0::readFree(const TomasuloBus& bus) const { return bus.FreeRSIAU0_o; }
bool RS_IntALU0::readGrant(const TomasuloBus& bus) const { return bus.RSIAU0_Grant_o; }

// ── Write accessors ───────────────────────────────────────────────────────
void RS_IntALU0::writeBusy(TomasuloBus& bus, bool v)     const { bus.RS_IntALU0_Busy_o = v; }
void RS_IntALU0::writeReq(TomasuloBus& bus, bool v)     const { bus.RSIAU0_Req_o = v; }
void RS_IntALU0::writeOp(TomasuloBus& bus, uint8_t v)  const { bus.RSIAU0_Op_o = v; }
void RS_IntALU0::writeVj(TomasuloBus& bus, uint64_t v) const { bus.RSIAU0_Vj_o = v; }
void RS_IntALU0::writeVk(TomasuloBus& bus, uint64_t v) const { bus.RSIAU0_Vk_o = v; }
void RS_IntALU0::writeDestROBTag(TomasuloBus& bus, uint8_t v)  const { bus.RSIAU0_DestROBTag_o = v; }
void RS_IntALU0::writeModFlags(TomasuloBus& bus, bool v)     const { bus.RSIAU0_ModFlags_o = v; }
void RS_IntALU0::writeNeedsFlags(TomasuloBus& bus, bool v)     const { bus.RSIAU0_NeedsFlags_o = v; }
void RS_IntALU0::writeFlagsValue(TomasuloBus& bus, uint8_t v)  const { bus.RSIAU0_FlagsValue_o = v; }