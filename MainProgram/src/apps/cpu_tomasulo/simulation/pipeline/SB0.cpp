/**
 * @file SB0.cpp
 * @brief Component 12 -- SB0 bus accessor implementation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/SB0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"

bool     SB0::readAlloc(const TomasuloBus& bus)      const { return bus.AllocSB0_o; }
bool     SB0::readFree(const TomasuloBus& bus)       const { return bus.FreeSB0_o; }
bool     SB0::readAGUDone(const TomasuloBus& bus)    const { return bus.SB0_AGUDone_o; }
uint64_t SB0::readAGUAddress(const TomasuloBus& bus) const { return bus.SB0_AGUAddress_o; }
bool     SB0::readAGUSegFault(const TomasuloBus& bus)const { return bus.SB0_AGUSegFault_o; }

void SB0::writeBusy(TomasuloBus& bus, bool v)                const { bus.SB0_Busy_o = v; }
void SB0::writeAGUReq(TomasuloBus& bus, bool v)              const { bus.SB0_AGUReq_o = v; }
void SB0::writeAGUBase(TomasuloBus& bus, uint64_t v)         const { bus.SB0_AGUBase_o = v; }
void SB0::writeAGUOffset(TomasuloBus& bus, uint64_t v)       const { bus.SB0_AGUOffset_o = v; }
void SB0::writeAGUROBTag(TomasuloBus& bus, uint8_t v)        const { bus.SB0_AGUROBTag_o = v; }
void SB0::writeStCompleteValid(TomasuloBus& bus, bool v)     const { bus.SB0_StComplete_Valid_o = v; }
void SB0::writeStCompleteROBTag(TomasuloBus& bus, uint8_t v) const { bus.SB0_StComplete_ROBTag_o = v; }
void SB0::writeStCompleteAddr(TomasuloBus& bus, uint64_t v)  const { bus.SB0_StComplete_Addr_o = v; }
void SB0::writeStCompleteData(TomasuloBus& bus, uint64_t v)  const { bus.SB0_StComplete_Data_o = v; }