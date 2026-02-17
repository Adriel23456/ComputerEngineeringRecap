/**
 * @file Commit_Unit.cpp
 * @brief Component 41 implementation -- Commit Unit.
 *
 * Retires from ROB head in program order. One commit per cycle.
 * Types: 000=IntALU, 001=IntMUL, 010=FPALU, 011=FPMUL,
 *        100=Load, 101=Store, 110=Branch, 111=CMP/Flags-only.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/Commit_Unit.h"
#include "apps/cpu_tomasulo/simulation/pipeline/TomasuloBus.h"
#include <iostream>

void Commit_Unit::activateFree(TomasuloBus& bus, uint8_t stationID) const {
    switch (stationID) {
    case 0x0: bus.FreeSB0_o = true; break;
    case 0x1: bus.FreeSB1_o = true; break;
    case 0x2: bus.FreeLB0_o = true; break;
    case 0x3: bus.FreeLB1_o = true; break;
    case 0x4: bus.FreeLB2_o = true; break;
    case 0x5: bus.FreeRSIAU0_o = true; break;
    case 0x6: bus.FreeRSIAU1_o = true; break;
    case 0x7: bus.FreeRSFPAU0_o = true; break;
    case 0x8: bus.FreeRSIMUL0_o = true; break;
    case 0x9: bus.FreeRSFPMUL0_o = true; break;
    case 0xA: bus.FreeRSBr0_o = true; break;
    case 0xF: /* NOP/SWI -- nothing to free */ break;
    default: break;
    }
}

// ============================================================================
// Evaluate (combinational outputs)
// ============================================================================

void Commit_Unit::evaluate(TomasuloBus& bus) {
    // Default all outputs
    bus.CommitPop_i = false;
    bus.CommitWrEn_i = false;
    bus.CommitWrAddr_i = 0;
    bus.CommitWrData_i = 0;
    bus.CommitROBIdx_i = 0;
    bus.FlagsCommitEn_i = false;
    bus.FlagsCommitValue_i = 0;
    bus.StoreCommit_Req_o = false;
    bus.StoreCommit_Addr_o = 0;
    bus.StoreCommit_Data_o = 0;
    bus.StoreCommit_Op_o = 0;
    bus.BranchRedirect_o = false;
    bus.BranchRedirect_i = false;
    bus.BranchTarget_o = 0;
    bus.BranchTarget_i = 0;
    bus.Flush_o = false;
    bus.Flush_PC_i = false;
    bus.FreeSB0_o = false; bus.FreeSB1_o = false;
    bus.FreeLB0_o = false; bus.FreeLB1_o = false; bus.FreeLB2_o = false;
    bus.FreeRSIAU0_o = false; bus.FreeRSIAU1_o = false;
    bus.FreeRSFPAU0_o = false; bus.FreeRSIMUL0_o = false;
    bus.FreeRSFPMUL0_o = false; bus.FreeRSBr0_o = false;
    bus.ExceptionSignal_o = false;
    bus.ExceptionCode_o = 0;
    bus.ExceptionPC_o = 0;
    bus.Halt_o = false;

    // -- Nothing to commit? --
    if (!bus.ROBHeadBusy_o) return;
    if (!bus.ROBHeadReady_o) return;

    // ================================================================
    // NOP / SWI (sourceStation == 0x0F, no execution unit)
    // ================================================================
    if (bus.ROBHeadSourceStation_o == 0x0F) {
        uint8_t headOp = bus.ROBHeadOp_o;

        if (headOp == 0x4C) {
            // SWI: halt the CPU — stop all further execution until reset
            bus.Halt_o = true;
            bus.CommitPop_i = true;
            bus.CommitROBIdx_i = bus.ROBHead_o;
            std::cout << "[Commit_Unit] SWI committed. CPU HALT.\n";
            return;
        }

        // NOP: just pop, do absolutely nothing else
        bus.CommitPop_i = true;
        bus.CommitROBIdx_i = bus.ROBHead_o;
        std::cout << "[Commit_Unit] NOP committed. ROB#"
            << (int)bus.ROBHead_o << "\n";
        return;
    }

    // -- Exception check --
    if (bus.ROBHeadException_o != 0x0) {
        bus.ExceptionSignal_o = true;
        bus.ExceptionCode_o = bus.ROBHeadException_o;
        bus.ExceptionPC_o = bus.ROBHeadPC_o;
        bus.Flush_o = true;
        bus.Flush_PC_i = true;
        bus.CommitPop_i = true;
        bus.CommitROBIdx_i = bus.ROBHead_o;
        activateFree(bus, bus.ROBHeadSourceStation_o);
        std::cout << "[Commit_Unit] EXCEPTION: code=0x" << std::hex
            << (int)bus.ROBHeadException_o << " PC=0x"
            << bus.ROBHeadPC_o << std::dec << "\n";
        return;
    }

    uint8_t type = bus.ROBHeadType_o;

    // ================================================================
    // ALU / MUL / FPALU / FPMUL / LOAD (types 000..100)
    // ================================================================
    if (type <= 0x04) {
        if (bus.ROBHeadDestReg_o != 0x0F) {
            bus.CommitWrEn_i = true;
            bus.CommitWrAddr_i = bus.ROBHeadDestReg_o;
            bus.CommitWrData_i = bus.ROBHeadValue_o;
            bus.CommitROBIdx_i = bus.ROBHead_o;
        }
        if (bus.ROBHeadFlagsValid_o) {
            bus.FlagsCommitEn_i = true;
            bus.FlagsCommitValue_i = bus.ROBHeadFlagsResult_o;
            bus.CommitROBIdx_i = bus.ROBHead_o;
        }
        activateFree(bus, bus.ROBHeadSourceStation_o);
        bus.CommitPop_i = true;

        std::cout << "[Commit_Unit] Commit type=" << (int)type
            << " dest=R" << (int)bus.ROBHeadDestReg_o
            << " val=0x" << std::hex << bus.ROBHeadValue_o
            << std::dec << " ROB#" << (int)bus.ROBHead_o << "\n";
    }

    // ================================================================
    // STORE (type 101 = 0x05)
    // ================================================================
    else if (type == 0x05) {
        if (!bus.ROBHeadStoreReady_o) return;

        // Phase 1: Send request (held high until clockEdge arms InProgress)
        if (!m_storeCommitInProgress && !m_storeCommitDone) {
            bus.StoreCommit_Req_o = true;
            bus.StoreCommit_Addr_o = bus.ROBHeadStoreAddr_o;
            bus.StoreCommit_Data_o = bus.ROBHeadStoreData_o;
            bus.StoreCommit_Op_o = bus.ROBHeadOp_o;
            std::cout << "[Commit_Unit] Store commit req: addr=0x" << std::hex
                << bus.ROBHeadStoreAddr_o << " data=0x"
                << bus.ROBHeadStoreData_o << std::dec << "\n";
        }

        // Phase 2: Done latched by clockEdge -> pop ROB
        if (m_storeCommitDone) {
            activateFree(bus, bus.ROBHeadSourceStation_o);
            bus.CommitPop_i = true;
            bus.CommitROBIdx_i = bus.ROBHead_o;
            std::cout << "[Commit_Unit] Store commit done. ROB#"
                << (int)bus.ROBHead_o << "\n";
        }
    }

    // ================================================================
    // BRANCH (type 110 = 0x06)
    // ================================================================
    else if (type == 0x06) {
        if (bus.ROBHeadMispredict_o) {
            bus.BranchRedirect_o = true;
            bus.BranchRedirect_i = true;
            bus.BranchTarget_o = bus.ROBHeadBranchTarget_o;
            bus.BranchTarget_i = bus.ROBHeadBranchTarget_o;
            bus.Flush_o = true;
            bus.Flush_PC_i = true;
            std::cout << "[Commit_Unit] Branch MISPREDICT: redirect to 0x"
                << std::hex << bus.ROBHeadBranchTarget_o
                << std::dec << " FLUSH!\n";
        }
        else {
            std::cout << "[Commit_Unit] Branch correct prediction. ROB#"
                << (int)bus.ROBHead_o << "\n";
        }
        activateFree(bus, bus.ROBHeadSourceStation_o);
        bus.CommitPop_i = true;
        bus.CommitROBIdx_i = bus.ROBHead_o;
    }

    // ================================================================
    // CMP / FLAGS-ONLY (type 111 = 0x07)
    // ================================================================
    else if (type == 0x07) {
        if (bus.ROBHeadFlagsValid_o) {
            bus.FlagsCommitEn_i = true;
            bus.FlagsCommitValue_i = bus.ROBHeadFlagsResult_o;
            bus.CommitROBIdx_i = bus.ROBHead_o;
        }
        activateFree(bus, bus.ROBHeadSourceStation_o);
        bus.CommitPop_i = true;

        std::cout << "[Commit_Unit] Commit CMP/flags: flags=0x" << std::hex
            << (int)bus.ROBHeadFlagsResult_o << std::dec
            << " ROB#" << (int)bus.ROBHead_o << "\n";
    }
}

// ============================================================================
// Clock Edge (sequential -- store commit progress tracking)
// ============================================================================

void Commit_Unit::clockEdge(TomasuloBus& bus) {
    if (bus.Flush_o) {
        m_storeCommitInProgress = false;
        m_storeCommitDone = false;
        return;
    }

    // 1. Pop completed: reset both flags for next store
    if (m_storeCommitDone && bus.CommitPop_i) {
        m_storeCommitDone = false;
        m_storeCommitInProgress = false;
        return;
    }

    // 2. Done signal arrived: latch for next evaluate
    if (bus.StoreCommit_Done_o && m_storeCommitInProgress && !m_storeCommitDone) {
        m_storeCommitDone = true;
    }

    // 3. Request sent: arm in-progress
    if (bus.StoreCommit_Req_o && !m_storeCommitInProgress && !m_storeCommitDone) {
        m_storeCommitInProgress = true;
    }
}

void Commit_Unit::reset() {
    m_storeCommitInProgress = false;
    std::cout << "[Commit_Unit] reset()\n";
}