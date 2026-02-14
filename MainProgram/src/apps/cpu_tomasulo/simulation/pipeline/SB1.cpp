/**
 * @file SB1.cpp
 * @brief Component 13 -- SB1 bus accessor implementation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/SB1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"

bool     SB1::readAlloc(const TomasuloBus& bus)      const { return bus.AllocSB1_o; }
bool     SB1::readFree(const TomasuloBus& bus)       const { return bus.FreeSB1_o; }
bool     SB1::readAGUDone(const TomasuloBus& bus)    const { return bus.SB1_AGUDone_o; }
uint64_t SB1::readAGUAddress(const TomasuloBus& bus) const { return bus.SB1_AGUAddress_o; }
bool     SB1::readAGUSegFault(const TomasuloBus& bus)const { return bus.SB1_AGUSegFault_o; }

void SB1::writeBusy(TomasuloBus& bus, bool v)                const { bus.SB1_Busy_o = v; }
void SB1::writeAGUReq(TomasuloBus& bus, bool v)              const { bus.SB1_AGUReq_o = v; }
void SB1::writeAGUBase(TomasuloBus& bus, uint64_t v)         const { bus.SB1_AGUBase_o = v; }
void SB1::writeAGUOffset(TomasuloBus& bus, uint64_t v)       const { bus.SB1_AGUOffset_o = v; }
void SB1::writeAGUROBTag(TomasuloBus& bus, uint8_t v)        const { bus.SB1_AGUROBTag_o = v; }
void SB1::writeStCompleteValid(TomasuloBus& bus, bool v)     const { bus.SB1_StComplete_Valid_o = v; }
void SB1::writeStCompleteROBTag(TomasuloBus& bus, uint8_t v) const { bus.SB1_StComplete_ROBTag_o = v; }
void SB1::writeStCompleteAddr(TomasuloBus& bus, uint64_t v)  const { bus.SB1_StComplete_Addr_o = v; }
void SB1::writeStCompleteData(TomasuloBus& bus, uint64_t v)  const { bus.SB1_StComplete_Data_o = v; }