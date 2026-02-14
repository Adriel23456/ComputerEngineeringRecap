/**
 * @file CDB.cpp
 * @brief Components 37-38 implementation -- CDB_A, CDB_B.
 * Pure combinational wire routing from CDB_Arbiter to all snooping components.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/CDB.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"

void CDB_A::evaluate(TomasuloBus& bus) {
    bus.CDBA_Valid_o = bus.ArbToCDBA_Valid_o;
    bus.CDBA_ROBTag_o = bus.ArbToCDBA_ROBTag_o;
    bus.CDBA_Value_o = bus.ArbToCDBA_Value_o;
    bus.CDBA_Exception_o = bus.ArbToCDBA_Exception_o;
    bus.CDBA_Flags_o = bus.ArbToCDBA_Flags_o;
    bus.CDBA_FlagsValid_o = bus.ArbToCDBA_FlagsValid_o;
}

void CDB_B::evaluate(TomasuloBus& bus) {
    bus.CDBB_Valid_o = bus.ArbToCDBB_Valid_o;
    bus.CDBB_ROBTag_o = bus.ArbToCDBB_ROBTag_o;
    bus.CDBB_Value_o = bus.ArbToCDBB_Value_o;
    bus.CDBB_Exception_o = bus.ArbToCDBB_Exception_o;
    bus.CDBB_Flags_o = bus.ArbToCDBB_Flags_o;
    bus.CDBB_FlagsValid_o = bus.ArbToCDBB_FlagsValid_o;
}