#include "apps/cpu_tomasulo/simulation/pipeline/AGU_1.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"

bool     AGU_1::readStart(const TomasuloBus& bus)    const { return bus.AGU1_Start_o; }
uint64_t AGU_1::readBase(const TomasuloBus& bus)     const { return bus.AGU1_Base_o; }
uint64_t AGU_1::readOffset(const TomasuloBus& bus)   const { return bus.AGU1_Offset_o; }
uint8_t  AGU_1::readSourceID(const TomasuloBus& bus) const { return bus.AGU1_SourceID_o; }

void AGU_1::writeFree(TomasuloBus& bus, bool v)            const { bus.AGU1_Free_o = v; }
void AGU_1::writeDone(TomasuloBus& bus, bool v)            const { bus.AGU1_Done_o = v; }
void AGU_1::writeAddress(TomasuloBus& bus, uint64_t v)     const { bus.AGU1_Address_o = v; }
void AGU_1::writeSegFault(TomasuloBus& bus, bool v)        const { bus.AGU1_SegFault_o = v; }
void AGU_1::writeSourceIDOut(TomasuloBus& bus, uint8_t v)  const { bus.AGU1_SourceID_out = v; }