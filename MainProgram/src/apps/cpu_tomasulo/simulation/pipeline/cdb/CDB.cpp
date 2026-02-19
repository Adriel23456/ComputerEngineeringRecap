// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/cdb/CDB.cpp
// ============================================================================

/**
 * @file CDB.cpp
 * @brief Components 37-38 implementation -- CDB_A, CDB_B.
 * Pure combinational wire routing from CDB_Arbiter to all snooping components.
 *
 * CDB_A carries the highest-priority result selected by CDB_Arbiter.
 * CDB_B carries the second result (if any). Both are broadcast every cycle
 * to all reservation stations, load buffers, and the ROB for tag matching.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/cdb/CDB.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"

 // ----------------------------------------------------------------------------
 // CDB_A::evaluate
 // Routes ArbToCDBA_* signals onto the CDBA_* broadcast wires.
 // ----------------------------------------------------------------------------
void CDB_A::evaluate(TomasuloBus& bus) {
    bus.CDBA_Valid_o = bus.ArbToCDBA_Valid_o;
    bus.CDBA_ROBTag_o = bus.ArbToCDBA_ROBTag_o;
    bus.CDBA_Value_o = bus.ArbToCDBA_Value_o;
    bus.CDBA_Exception_o = bus.ArbToCDBA_Exception_o;
    bus.CDBA_Flags_o = bus.ArbToCDBA_Flags_o;
    bus.CDBA_FlagsValid_o = bus.ArbToCDBA_FlagsValid_o;
}

// ----------------------------------------------------------------------------
// CDB_B::evaluate
// Routes ArbToCDBB_* signals onto the CDBB_* broadcast wires.
// ----------------------------------------------------------------------------
void CDB_B::evaluate(TomasuloBus& bus) {
    bus.CDBB_Valid_o = bus.ArbToCDBB_Valid_o;
    bus.CDBB_ROBTag_o = bus.ArbToCDBB_ROBTag_o;
    bus.CDBB_Value_o = bus.ArbToCDBB_Value_o;
    bus.CDBB_Exception_o = bus.ArbToCDBB_Exception_o;
    bus.CDBB_Flags_o = bus.ArbToCDBB_Flags_o;
    bus.CDBB_FlagsValid_o = bus.ArbToCDBB_FlagsValid_o;
}