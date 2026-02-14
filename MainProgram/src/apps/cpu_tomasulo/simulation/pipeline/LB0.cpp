/**
 * @file LB0.cpp
 * @brief Component 14 -- LB0 bus accessor implementation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/LB0.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"

bool     LB0::readAlloc(const TomasuloBus& bus)      const { return bus.AllocLB0_o; }
bool     LB0::readFree(const TomasuloBus& bus)       const { return bus.FreeLB0_o; }
bool     LB0::readAGUDone(const TomasuloBus& bus)    const { return bus.LB0_AGUDone_o; }
uint64_t LB0::readAGUAddress(const TomasuloBus& bus) const { return bus.LB0_AGUAddress_o; }
bool     LB0::readAGUSegFault(const TomasuloBus& bus)const { return bus.LB0_AGUSegFault_o; }
bool     LB0::readMemDone(const TomasuloBus& bus)    const { return bus.LB0_MemDone_o; }
uint64_t LB0::readMemData(const TomasuloBus& bus)    const { return bus.LB0_MemData_o; }
bool     LB0::readCDBStall(const TomasuloBus& bus)   const { return bus.LB0_CDBStall_o; }

void LB0::writeBusy(TomasuloBus& bus, bool v)              const { bus.LB0_Busy_o = v; }
void LB0::writeAGUReq(TomasuloBus& bus, bool v)            const { bus.LB0_AGUReq_o = v; }
void LB0::writeAGUBase(TomasuloBus& bus, uint64_t v)       const { bus.LB0_AGUBase_o = v; }
void LB0::writeAGUOffset(TomasuloBus& bus, uint64_t v)     const { bus.LB0_AGUOffset_o = v; }
void LB0::writeAGUROBTag(TomasuloBus& bus, uint8_t v)      const { bus.LB0_AGUROBTag_o = v; }
void LB0::writeMemReq(TomasuloBus& bus, bool v)            const { bus.LB0_MemReq_o = v; }
void LB0::writeMemAddr(TomasuloBus& bus, uint64_t v)       const { bus.LB0_MemAddr_o = v; }
void LB0::writeMemOp(TomasuloBus& bus, uint8_t v)          const { bus.LB0_MemOp_o = v; }
void LB0::writeMemROBTag(TomasuloBus& bus, uint8_t v)      const { bus.LB0_MemROBTag_o = v; }
void LB0::writeCDBReq(TomasuloBus& bus, bool v)            const { bus.LB0_CDBReq_o = v; }
void LB0::writeCDBValue(TomasuloBus& bus, uint64_t v)      const { bus.LB0_CDBValue_o = v; }
void LB0::writeCDBROBTag(TomasuloBus& bus, uint8_t v)      const { bus.LB0_CDBROBTag_o = v; }
void LB0::writeCDBException(TomasuloBus& bus, uint8_t v)   const { bus.LB0_CDBException_o = v; }