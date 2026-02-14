/**
 * @file RS_IntALU1.cpp
 * @brief Component 18 -- RS_IntALU1 bus accessor implementation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/RS_IntALU1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"

bool     RS_IntALU1::readAlloc(const TomasuloBus& bus) const { return bus.AllocRSIntALU1_o; }
bool     RS_IntALU1::readFree(const TomasuloBus& bus)  const { return bus.FreeRSIAU1_o; }
bool     RS_IntALU1::readGrant(const TomasuloBus& bus) const { return bus.RSIAU1_Grant_o; }

void RS_IntALU1::writeBusy(TomasuloBus& bus, bool v)           const { bus.RS_IntALU1_Busy_o = v; }
void RS_IntALU1::writeReq(TomasuloBus& bus, bool v)            const { bus.RSIAU1_Req_o = v; }
void RS_IntALU1::writeOp(TomasuloBus& bus, uint8_t v)          const { bus.RSIAU1_Op_o = v; }
void RS_IntALU1::writeVj(TomasuloBus& bus, uint64_t v)         const { bus.RSIAU1_Vj_o = v; }
void RS_IntALU1::writeVk(TomasuloBus& bus, uint64_t v)         const { bus.RSIAU1_Vk_o = v; }
void RS_IntALU1::writeDestROBTag(TomasuloBus& bus, uint8_t v)  const { bus.RSIAU1_DestROBTag_o = v; }
void RS_IntALU1::writeModFlags(TomasuloBus& bus, bool v)       const { bus.RSIAU1_ModFlags_o = v; }
void RS_IntALU1::writeNeedsFlags(TomasuloBus& bus, bool v)     const { bus.RSIAU1_NeedsFlags_o = v; }
void RS_IntALU1::writeFlagsValue(TomasuloBus& bus, uint8_t v)  const { bus.RSIAU1_FlagsValue_o = v; }