/**
 * @file CDB_Arbiter.cpp
 * @brief Component 36 implementation -- CDB Arbiter.
 *
 * Selects up to 2 winners from 7 requestors.
 * Winners go to CDB_A and CDB_B. Losers get CDBStall=1.
 * Loads never produce flags (FlagsValid forced to 0).
 */

#include "apps/cpu_tomasulo/simulation/pipeline/CDB_Arbiter.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

enum class CDBSource { IntALU, FPALU, IntMUL, FPMUL, LB0, LB1, LB2, NONE };

struct CDBCandidate {
    CDBSource source;
    uint8_t   robTag;
    uint64_t  value;
    uint8_t   exception;
    uint8_t   flags;
    bool      flagsValid;
};

void CDB_Arbiter::evaluate(TomasuloBus& bus) {
    bus.ArbToCDBA_Valid_o = false;
    bus.ArbToCDBB_Valid_o = false;
    bus.IntALU_CDBStall_o = false;
    bus.FPALU_CDBStall_o = false;
    bus.IntMUL_CDBStall_o = false;
    bus.FPMUL_CDBStall_o = false;
    bus.LB0_CDBStall_o = false;
    bus.LB1_CDBStall_o = false;
    bus.LB2_CDBStall_o = false;

    CDBCandidate candidates[7];
    int count = 0;

    // Priority order: IntALU > FPALU > IntMUL > FPMUL > LB0 > LB1 > LB2
    if (bus.IntALU_CDBReq_o) {
        candidates[count++] = { CDBSource::IntALU, bus.IntALU_CDBROBTag_o,
            bus.IntALU_CDBValue_o, bus.IntALU_CDBException_o,
            bus.IntALU_CDBFlags_o, bus.IntALU_CDBFlagsValid_o };
    }
    if (bus.FPALU_CDBReq_o) {
        candidates[count++] = { CDBSource::FPALU, bus.FPALU_CDBROBTag_o,
            bus.FPALU_CDBValue_o, bus.FPALU_CDBException_o,
            bus.FPALU_CDBFlags_o, bus.FPALU_CDBFlagsValid_o };
    }
    if (bus.IntMUL_CDBReq_o) {
        candidates[count++] = { CDBSource::IntMUL, bus.IntMUL_CDBROBTag_o,
            bus.IntMUL_CDBValue_o, bus.IntMUL_CDBException_o,
            bus.IntMUL_CDBFlags_o, bus.IntMUL_CDBFlagsValid_o };
    }
    if (bus.FPMUL_CDBReq_o) {
        candidates[count++] = { CDBSource::FPMUL, bus.FPMUL_CDBROBTag_o,
            bus.FPMUL_CDBValue_o, bus.FPMUL_CDBException_o,
            bus.FPMUL_CDBFlags_o, bus.FPMUL_CDBFlagsValid_o };
    }
    if (bus.LB0_CDBReq_o) {
        candidates[count++] = { CDBSource::LB0, bus.LB0_CDBROBTag_o,
            bus.LB0_CDBValue_o, bus.LB0_CDBException_o, 0, false };
    }
    if (bus.LB1_CDBReq_o) {
        candidates[count++] = { CDBSource::LB1, bus.LB1_CDBROBTag_o,
            bus.LB1_CDBValue_o, bus.LB1_CDBException_o, 0, false };
    }
    if (bus.LB2_CDBReq_o) {
        candidates[count++] = { CDBSource::LB2, bus.LB2_CDBROBTag_o,
            bus.LB2_CDBValue_o, bus.LB2_CDBException_o, 0, false };
    }

    int winnerIdx = 0;

    // Winner 1 -> CDB_A
    if (winnerIdx < count) {
        auto& w = candidates[winnerIdx];
        bus.ArbToCDBA_Valid_o = true;
        bus.ArbToCDBA_ROBTag_o = w.robTag;
        bus.ArbToCDBA_Value_o = w.value;
        bus.ArbToCDBA_Exception_o = w.exception;
        bus.ArbToCDBA_Flags_o = w.flags;
        bus.ArbToCDBA_FlagsValid_o = w.flagsValid;
        winnerIdx++;
    }

    // Winner 2 -> CDB_B
    if (winnerIdx < count) {
        auto& w = candidates[winnerIdx];
        bus.ArbToCDBB_Valid_o = true;
        bus.ArbToCDBB_ROBTag_o = w.robTag;
        bus.ArbToCDBB_Value_o = w.value;
        bus.ArbToCDBB_Exception_o = w.exception;
        bus.ArbToCDBB_Flags_o = w.flags;
        bus.ArbToCDBB_FlagsValid_o = w.flagsValid;
        winnerIdx++;
    }

    // Stall the losers
    for (int i = winnerIdx; i < count; i++) {
        switch (candidates[i].source) {
        case CDBSource::IntALU: bus.IntALU_CDBStall_o = true; break;
        case CDBSource::FPALU:  bus.FPALU_CDBStall_o = true; break;
        case CDBSource::IntMUL: bus.IntMUL_CDBStall_o = true; break;
        case CDBSource::FPMUL:  bus.FPMUL_CDBStall_o = true; break;
        case CDBSource::LB0:    bus.LB0_CDBStall_o = true; break;
        case CDBSource::LB1:    bus.LB1_CDBStall_o = true; break;
        case CDBSource::LB2:    bus.LB2_CDBStall_o = true; break;
        default: break;
        }
    }
}

void CDB_Arbiter::reset() {
    std::cout << "[CDB_Arbiter] reset()\n";
}