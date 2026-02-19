// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/buffers/LB1.cpp
// ============================================================================

/**
 * @file LB1.cpp
 * @brief Bus accessor implementation for LB1 (Component 15).
 *
 * Maps the abstract LoadBuffer read/write interface to LB1_* bus fields.
 * No logic here — all in LoadBuffer base class.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/buffers/LB1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"

bool     LB1::readAlloc(const TomasuloBus& bus)      const { return bus.AllocLB1_o; }
bool     LB1::readFree(const TomasuloBus& bus)       const { return bus.FreeLB1_o; }
bool     LB1::readAGUDone(const TomasuloBus& bus)    const { return bus.LB1_AGUDone_o; }
uint64_t LB1::readAGUAddress(const TomasuloBus& bus) const { return bus.LB1_AGUAddress_o; }
bool     LB1::readAGUSegFault(const TomasuloBus& bus)const { return bus.LB1_AGUSegFault_o; }
bool     LB1::readMemDone(const TomasuloBus& bus)    const { return bus.LB1_MemDone_o; }
uint64_t LB1::readMemData(const TomasuloBus& bus)    const { return bus.LB1_MemData_o; }
bool     LB1::readCDBStall(const TomasuloBus& bus)   const { return bus.LB1_CDBStall_o; }

void LB1::writeBusy(TomasuloBus& bus, bool v)             const { bus.LB1_Busy_o = v; }
void LB1::writeAGUReq(TomasuloBus& bus, bool v)           const { bus.LB1_AGUReq_o = v; }
void LB1::writeAGUBase(TomasuloBus& bus, uint64_t v)      const { bus.LB1_AGUBase_o = v; }
void LB1::writeAGUOffset(TomasuloBus& bus, uint64_t v)    const { bus.LB1_AGUOffset_o = v; }
void LB1::writeAGUROBTag(TomasuloBus& bus, uint8_t v)     const { bus.LB1_AGUROBTag_o = v; }
void LB1::writeMemReq(TomasuloBus& bus, bool v)           const { bus.LB1_MemReq_o = v; }
void LB1::writeMemAddr(TomasuloBus& bus, uint64_t v)      const { bus.LB1_MemAddr_o = v; }
void LB1::writeMemOp(TomasuloBus& bus, uint8_t v)         const { bus.LB1_MemOp_o = v; }
void LB1::writeMemROBTag(TomasuloBus& bus, uint8_t v)     const { bus.LB1_MemROBTag_o = v; }
void LB1::writeCDBReq(TomasuloBus& bus, bool v)           const { bus.LB1_CDBReq_o = v; }
void LB1::writeCDBValue(TomasuloBus& bus, uint64_t v)     const { bus.LB1_CDBValue_o = v; }
void LB1::writeCDBROBTag(TomasuloBus& bus, uint8_t v)     const { bus.LB1_CDBROBTag_o = v; }
void LB1::writeCDBException(TomasuloBus& bus, uint8_t v)  const { bus.LB1_CDBException_o = v; }