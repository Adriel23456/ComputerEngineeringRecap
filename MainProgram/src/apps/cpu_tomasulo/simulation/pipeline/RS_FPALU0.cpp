/**
 * @file RS_FPALU0.cpp
 * @brief Component 19 -- RS_FPALU0 bus accessor implementation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/RS_FPALU0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"

bool     RS_FPALU0::readAlloc(const TomasuloBus& bus) const { return bus.AllocRSFPALU0_o; }
bool     RS_FPALU0::readFree(const TomasuloBus& bus)  const { return bus.FreeRSFPAU0_o; }
bool     RS_FPALU0::readGrant(const TomasuloBus& bus) const { return bus.RSFPAU0_Grant_o; }

void RS_FPALU0::writeBusy(TomasuloBus& bus, bool v)           const { bus.RS_FPALU0_Busy_o = v; }
void RS_FPALU0::writeReq(TomasuloBus& bus, bool v)            const { bus.RSFPAU0_Req_o = v; }
void RS_FPALU0::writeOp(TomasuloBus& bus, uint8_t v)          const { bus.RSFPAU0_Op_o = v; }
void RS_FPALU0::writeVj(TomasuloBus& bus, uint64_t v)         const { bus.RSFPAU0_Vj_o = v; }
void RS_FPALU0::writeVk(TomasuloBus& bus, uint64_t v)         const { bus.RSFPAU0_Vk_o = v; }
void RS_FPALU0::writeDestROBTag(TomasuloBus& bus, uint8_t v)  const { bus.RSFPAU0_DestROBTag_o = v; }
void RS_FPALU0::writeModFlags(TomasuloBus& bus, bool v)       const { bus.RSFPAU0_ModFlags_o = v; }
void RS_FPALU0::writeNeedsFlags(TomasuloBus& bus, bool v)     const { bus.RSFPAU0_NeedsFlags_o = v; }
void RS_FPALU0::writeFlagsValue(TomasuloBus& bus, uint8_t v)  const { bus.RSFPAU0_FlagsValue_o = v; }