// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntMUL0.cpp
// ============================================================================

/**
 * @file RS_IntMUL0.cpp
 * @brief Component 20 — RS_IntMUL0 bus accessor implementation.
 *
 * Maps the abstract ReservationStation interface to the concrete
 * RSIMUL0_* bus signals used by the IntMUL arbiter and execution unit.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/reservation/RS_IntMUL0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"

 // ── Read accessors ────────────────────────────────────────────────────────
bool RS_IntMUL0::readAlloc(const TomasuloBus& bus) const { return bus.AllocRSIntMUL0_o; }
bool RS_IntMUL0::readFree(const TomasuloBus& bus) const { return bus.FreeRSIMUL0_o; }
bool RS_IntMUL0::readGrant(const TomasuloBus& bus) const { return bus.RSIMUL0_Grant_o; }

// ── Write accessors ───────────────────────────────────────────────────────
void RS_IntMUL0::writeBusy(TomasuloBus& bus, bool v)     const { bus.RS_IntMUL0_Busy_o = v; }
void RS_IntMUL0::writeReq(TomasuloBus& bus, bool v)     const { bus.RSIMUL0_Req_o = v; }
void RS_IntMUL0::writeOp(TomasuloBus& bus, uint8_t v)  const { bus.RSIMUL0_Op_o = v; }
void RS_IntMUL0::writeVj(TomasuloBus& bus, uint64_t v) const { bus.RSIMUL0_Vj_o = v; }
void RS_IntMUL0::writeVk(TomasuloBus& bus, uint64_t v) const { bus.RSIMUL0_Vk_o = v; }
void RS_IntMUL0::writeDestROBTag(TomasuloBus& bus, uint8_t v)  const { bus.RSIMUL0_DestROBTag_o = v; }
void RS_IntMUL0::writeModFlags(TomasuloBus& bus, bool v)     const { bus.RSIMUL0_ModFlags_o = v; }
void RS_IntMUL0::writeNeedsFlags(TomasuloBus& bus, bool v)     const { bus.RSIMUL0_NeedsFlags_o = v; }
void RS_IntMUL0::writeFlagsValue(TomasuloBus& bus, uint8_t v)  const { bus.RSIMUL0_FlagsValue_o = v; }