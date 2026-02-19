// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/agu/AGU.cpp
// ============================================================================

/**
 * @file AGU.cpp
 * @brief Implementation of the AGU base class (Components 24–25).
 *
 * evaluate():
 *   Asserts Free, reads Base + Offset + SourceID from the bus (set by
 *   AGU_Arbiter), computes address = Base + Offset, performs the
 *   segmentation check, then calls routeResult() to write the
 *   per-requestor AGUDone / AGUAddress / AGUSegFault signals.
 *
 * routeResult():
 *   Writes the result to the correct SBx / LBx AGU done signals based
 *   on SourceID (0=SB0, 1=SB1, 2=LB0, 3=LB1, 4=LB2).
 *
 * Segmentation:
 *   Valid data addresses must satisfy UPPER_o ≤ address ≤ LOWER_o.
 *   UPPER_o is the byte address one past the loaded program;
 *   LOWER_o is the memory ceiling (0xFFFF…FFFF at reset).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/agu/AGU.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

AGU::AGU(ID id) : m_id(id) {}

const char* AGU::idStr() const {
    return (m_id == ID::AGU0) ? "AGU_0" : "AGU_1";
}

// ============================================================================
// Evaluate
// ============================================================================

void AGU::evaluate(TomasuloBus& bus) {
    writeFree(bus, true);
    writeDone(bus, false);

    if (!readStart(bus)) return;

    uint64_t base = readBase(bus);
    uint64_t offset = readOffset(bus);
    uint8_t  srcID = readSourceID(bus);

    uint64_t address = base + offset;

    // Valid data range: [UPPER_o, LOWER_o]  (UPPER = code end, LOWER = mem ceiling)
    bool segFault = (address < bus.UPPER_o) || (address > bus.LOWER_o);

    writeDone(bus, true);
    writeAddress(bus, address);
    writeSegFault(bus, segFault);
    writeSourceIDOut(bus, srcID);

    routeResult(bus, srcID, address, segFault);
}

// ============================================================================
// Route Result to Requesting Buffer
// ============================================================================

void AGU::routeResult(TomasuloBus& bus, uint8_t sourceID,
    uint64_t address, bool segFault) const {
    switch (sourceID) {
    case 0: bus.SB0_AGUDone_o = true; bus.SB0_AGUAddress_o = address; bus.SB0_AGUSegFault_o = segFault; break;
    case 1: bus.SB1_AGUDone_o = true; bus.SB1_AGUAddress_o = address; bus.SB1_AGUSegFault_o = segFault; break;
    case 2: bus.LB0_AGUDone_o = true; bus.LB0_AGUAddress_o = address; bus.LB0_AGUSegFault_o = segFault; break;
    case 3: bus.LB1_AGUDone_o = true; bus.LB1_AGUAddress_o = address; bus.LB1_AGUSegFault_o = segFault; break;
    case 4: bus.LB2_AGUDone_o = true; bus.LB2_AGUAddress_o = address; bus.LB2_AGUSegFault_o = segFault; break;
    }
}

// ============================================================================
// Reset
// ============================================================================

void AGU::reset() {
    std::cout << "[" << idStr() << "] reset()\n";
}