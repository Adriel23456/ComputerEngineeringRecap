// ============================================================================
// File: src/apps/cpu_tomasulo/simulation/pipeline/commit/Commit_Unit.cpp
// ============================================================================

/**
 * @file Commit_Unit.cpp
 * @brief Component 41 implementation -- Commit Unit.
 *
 * Retires instructions from the ROB head in program order. At most one
 * instruction commits per cycle. Instruction types and their commit actions:
 *
 *   000 IntALU  — register writeback, optional flag commit, free RS.
 *   001 IntMUL  — register writeback, optional flag commit, free RS.
 *   010 FPALU   — register writeback, optional flag commit, free RS.
 *   011 FPMUL   — register writeback, free RS.
 *   100 Load    — register writeback, free LB.
 *   101 Store   — two-phase D_Cache write handshake, free SB.
 *   110 Branch  — optional flush + redirect on mispredict, free RS.
 *   111 CMP     — flag commit only, free RS.
 *
 * Store commit is a two-phase handshake:
 *   Phase 1: StoreCommit_Req_o is held HIGH until Memory_Arbiter responds.
 *   Phase 2: StoreCommit_Done_o is latched by clockEdge; ROB pop follows.
 */

#include "apps/cpu_tomasulo/simulation/pipeline/commit/Commit_Unit.h"
#include "apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h"
#include <iostream>

 // ----------------------------------------------------------------------------
 // activateFree
 // Asserts the appropriate Free signal for a given 4-bit source station ID.
 // Called after a commit so the station can accept a new instruction.
 // ----------------------------------------------------------------------------
void Commit_Unit::activateFree(TomasuloBus& bus, uint8_t stationID) const {
    switch (stationID) {
    case 0x0: bus.FreeSB0_o = true; break;  // Store Buffer 0
    case 0x1: bus.FreeSB1_o = true; break;  // Store Buffer 1
    case 0x2: bus.FreeLB0_o = true; break;  // Load Buffer 0
    case 0x3: bus.FreeLB1_o = true; break;  // Load Buffer 1
    case 0x4: bus.FreeLB2_o = true; break;  // Load Buffer 2
    case 0x5: bus.FreeRSIAU0_o = true; break;  // RS_IntALU0
    case 0x6: bus.FreeRSIAU1_o = true; break;  // RS_IntALU1
    case 0x7: bus.FreeRSFPAU0_o = true; break; // RS_FPALU0
    case 0x8: bus.FreeRSIMUL0_o = true; break; // RS_IntMUL0
    case 0x9: bus.FreeRSFPMUL0_o = true; break;// RS_FPMUL0
    case 0xA: bus.FreeRSBr0_o = true; break;  // RS_Branch0
    case 0xF: /* NOP/SWI -- no execution unit was allocated */ break;
    default: break;
    }
}

// ============================================================================
// Evaluate (combinational outputs)
// ============================================================================

void Commit_Unit::evaluate(TomasuloBus& bus) {
    // ── Default all outputs to inactive ───────────────────────────────
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

    // ── Guard: ROB head must be occupied and ready ────────────────────
    if (!bus.ROBHeadBusy_o)  return;
    if (!bus.ROBHeadReady_o) return;

    // ================================================================
    // NOP / SWI (sourceStation == 0x0F — no execution unit allocated)
    // ================================================================
    if (bus.ROBHeadSourceStation_o == 0x0F) {
        uint8_t headOp = bus.ROBHeadOp_o;

        if (headOp == 0x4C) {
            // SWI: halt the CPU — stop all further execution until reset
            bus.Halt_o = true;
            bus.CommitPop_i = true;
            bus.CommitROBIdx_i = bus.ROBHead_o;
            return;
        }

        // NOP: pop and do nothing else
        bus.CommitPop_i = true;
        bus.CommitROBIdx_i = bus.ROBHead_o;
        return;
    }

    // ── Exception check — flush and signal before any writeback ──────
    if (bus.ROBHeadException_o != 0x0) {
        bus.ExceptionSignal_o = true;
        bus.ExceptionCode_o = bus.ROBHeadException_o;
        bus.ExceptionPC_o = bus.ROBHeadPC_o;
        bus.Flush_o = true;
        bus.Flush_PC_i = true;
        bus.CommitPop_i = true;
        bus.CommitROBIdx_i = bus.ROBHead_o;
        activateFree(bus, bus.ROBHeadSourceStation_o);
        return;
    }

    uint8_t type = bus.ROBHeadType_o;

    // ================================================================
    // ALU / MUL / FPALU / FPMUL / LOAD (types 000..100)
    // Register writeback + optional flag commit.
    // ================================================================
    if (type <= 0x04) {
        // Write result to register file (skip if no destination, e.g. 0x0F)
        if (bus.ROBHeadDestReg_o != 0x0F) {
            bus.CommitWrEn_i = true;
            bus.CommitWrAddr_i = bus.ROBHeadDestReg_o;
            bus.CommitWrData_i = bus.ROBHeadValue_o;
            bus.CommitROBIdx_i = bus.ROBHead_o;
        }
        // Commit flags if this instruction produced a valid flag result
        if (bus.ROBHeadFlagsValid_o) {
            bus.FlagsCommitEn_i = true;
            bus.FlagsCommitValue_i = bus.ROBHeadFlagsResult_o;
            bus.CommitROBIdx_i = bus.ROBHead_o;
        }
        activateFree(bus, bus.ROBHeadSourceStation_o);
        bus.CommitPop_i = true;
    }

    // ================================================================
    // STORE (type 101 = 0x05)
    // Two-phase handshake with Memory_Arbiter via StoreCommit signals.
    // ================================================================
    else if (type == 0x05) {
        // Wait until both address and data have been resolved
        if (!bus.ROBHeadStoreReady_o) return;

        // Phase 1: Hold request HIGH until Memory_Arbiter acknowledges
        if (!m_storeCommitDone) {
            bus.StoreCommit_Req_o = true;
            bus.StoreCommit_Addr_o = bus.ROBHeadStoreAddr_o;
            bus.StoreCommit_Data_o = bus.ROBHeadStoreData_o;
            bus.StoreCommit_Op_o = bus.ROBHeadOp_o;
        }

        // Phase 2: Done latched by clockEdge — now safe to pop the ROB
        if (m_storeCommitDone) {
            activateFree(bus, bus.ROBHeadSourceStation_o);
            bus.CommitPop_i = true;
            bus.CommitROBIdx_i = bus.ROBHead_o;
        }
    }

    // ================================================================
    // BRANCH (type 110 = 0x06)
    // On mispredict: flush pipeline and redirect PC to correct target.
    // ================================================================
    else if (type == 0x06) {
        if (bus.ROBHeadMispredict_o) {
            bus.BranchRedirect_o = true;
            bus.BranchRedirect_i = true;
            bus.BranchTarget_o = bus.ROBHeadBranchTarget_o;
            bus.BranchTarget_i = bus.ROBHeadBranchTarget_o;
            bus.Flush_o = true;
            bus.Flush_PC_i = true;
        }
        activateFree(bus, bus.ROBHeadSourceStation_o);
        bus.CommitPop_i = true;
        bus.CommitROBIdx_i = bus.ROBHead_o;
    }

    // ================================================================
    // CMP / FLAGS-ONLY (type 111 = 0x07)
    // No register writeback — only the flag register is updated.
    // ================================================================
    else if (type == 0x07) {
        if (bus.ROBHeadFlagsValid_o) {
            bus.FlagsCommitEn_i = true;
            bus.FlagsCommitValue_i = bus.ROBHeadFlagsResult_o;
            bus.CommitROBIdx_i = bus.ROBHead_o;
        }
        activateFree(bus, bus.ROBHeadSourceStation_o);
        bus.CommitPop_i = true;
    }
}

// ============================================================================
// Clock Edge (sequential -- tracks store-commit progress across cycles)
// ============================================================================

void Commit_Unit::clockEdge(TomasuloBus& bus) {
    // On flush, abandon any in-progress store commit
    if (bus.Flush_o) {
        m_storeCommitInProgress = false;
        m_storeCommitDone = false;
        return;
    }

    // 1. ROB pop completed for a store — reset flags for the next store
    if (m_storeCommitDone && bus.CommitPop_i) {
        m_storeCommitDone = false;
        m_storeCommitInProgress = false;
        return;
    }

    // 2. Memory_Arbiter acknowledged the write — latch done for next evaluate
    if (bus.StoreCommit_Done_o && m_storeCommitInProgress && !m_storeCommitDone) {
        m_storeCommitDone = true;
    }

    // 3. Request first raised this cycle — arm in-progress flag
    if (bus.StoreCommit_Req_o && !m_storeCommitInProgress && !m_storeCommitDone) {
        m_storeCommitInProgress = true;
    }
}

void Commit_Unit::reset() {
    m_storeCommitInProgress = false;
    m_storeCommitDone = false;
    std::cout << "[Commit_Unit] reset()\n";
}