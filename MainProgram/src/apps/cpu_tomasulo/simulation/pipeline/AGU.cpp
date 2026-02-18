/**
 * @file AGU.cpp
 * @brief Components 24-25 implementation -- Address Generation Unit.
 *
 * Purely combinational: Base + Offset -> Address, segmentation check.
 * Routes result directly to requesting SB/LB via SourceID.
 *
 * SourceID mapping: 0=SB0, 1=SB1, 2=LB0, 3=LB1, 4=LB2
 */

#include "apps/cpu_tomasulo/simulation/pipeline/AGU.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

AGU::AGU(ID id) : m_id(id) {}

const char* AGU::idStr() const {
    return (m_id == ID::AGU0) ? "AGU_0" : "AGU_1";
}

void AGU::evaluate(TomasuloBus& bus) {
    writeFree(bus, true);
    writeDone(bus, false);

    if (!readStart(bus)) return;

    uint64_t base = readBase(bus);
    uint64_t offset = readOffset(bus);
    uint8_t  srcID = readSourceID(bus);

    uint64_t address = base + offset;

    // UPPER = program end (lower bound of data area)
    // LOWER = memory ceiling (upper bound of data area)
    bool segFault = (address < bus.UPPER_o) || (address > bus.LOWER_o);

    writeDone(bus, true);
    writeAddress(bus, address);
    writeSegFault(bus, segFault);
    writeSourceIDOut(bus, srcID);

    routeResult(bus, srcID, address, segFault);
}

void AGU::routeResult(TomasuloBus& bus, uint8_t sourceID,
    uint64_t address, bool segFault) const {
    switch (sourceID) {
    case 0:
        bus.SB0_AGUDone_o = true;
        bus.SB0_AGUAddress_o = address;
        bus.SB0_AGUSegFault_o = segFault;
        break;
    case 1:
        bus.SB1_AGUDone_o = true;
        bus.SB1_AGUAddress_o = address;
        bus.SB1_AGUSegFault_o = segFault;
        break;
    case 2:
        bus.LB0_AGUDone_o = true;
        bus.LB0_AGUAddress_o = address;
        bus.LB0_AGUSegFault_o = segFault;
        break;
    case 3:
        bus.LB1_AGUDone_o = true;
        bus.LB1_AGUAddress_o = address;
        bus.LB1_AGUSegFault_o = segFault;
        break;
    case 4:
        bus.LB2_AGUDone_o = true;
        bus.LB2_AGUAddress_o = address;
        bus.LB2_AGUSegFault_o = segFault;
        break;
    }
}

void AGU::reset() {
    std::cout << "[" << idStr() << "] reset()\n";
}