/**
 * @file LB2.cpp
 * @brief Component 16 -- LB2 bus accessor implementation.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/LB2.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"

bool     LB2::readAlloc(const TomasuloBus& bus)      const { return bus.AllocLB2_o; }
bool     LB2::readFree(const TomasuloBus& bus)       const { return bus.FreeLB2_o; }
bool     LB2::readAGUDone(const TomasuloBus& bus)    const { return bus.LB2_AGUDone_o; }
uint64_t LB2::readAGUAddress(const TomasuloBus& bus) const { return bus.LB2_AGUAddress_o; }
bool     LB2::readAGUSegFault(const TomasuloBus& bus)const { return bus.LB2_AGUSegFault_o; }
bool     LB2::readMemDone(const TomasuloBus& bus)    const { return bus.LB2_MemDone_o; }
uint64_t LB2::readMemData(const TomasuloBus& bus)    const { return bus.LB2_MemData_o; }
bool     LB2::readCDBStall(const TomasuloBus& bus)   const { return bus.LB2_CDBStall_o; }

void LB2::writeBusy(TomasuloBus& bus, bool v)              const { bus.LB2_Busy_o = v; }
void LB2::writeAGUReq(TomasuloBus& bus, bool v)            const { bus.LB2_AGUReq_o = v; }
void LB2::writeAGUBase(TomasuloBus& bus, uint64_t v)       const { bus.LB2_AGUBase_o = v; }
void LB2::writeAGUOffset(TomasuloBus& bus, uint64_t v)     const { bus.LB2_AGUOffset_o = v; }
void LB2::writeAGUROBTag(TomasuloBus& bus, uint8_t v)      const { bus.LB2_AGUROBTag_o = v; }
void LB2::writeMemReq(TomasuloBus& bus, bool v)            const { bus.LB2_MemReq_o = v; }
void LB2::writeMemAddr(TomasuloBus& bus, uint64_t v)       const { bus.LB2_MemAddr_o = v; }
void LB2::writeMemOp(TomasuloBus& bus, uint8_t v)          const { bus.LB2_MemOp_o = v; }
void LB2::writeMemROBTag(TomasuloBus& bus, uint8_t v)      const { bus.LB2_MemROBTag_o = v; }
void LB2::writeCDBReq(TomasuloBus& bus, bool v)            const { bus.LB2_CDBReq_o = v; }
void LB2::writeCDBValue(TomasuloBus& bus, uint64_t v)      const { bus.LB2_CDBValue_o = v; }
void LB2::writeCDBROBTag(TomasuloBus& bus, uint8_t v)      const { bus.LB2_CDBROBTag_o = v; }
void LB2::writeCDBException(TomasuloBus& bus, uint8_t v)   const { bus.LB2_CDBException_o = v; }