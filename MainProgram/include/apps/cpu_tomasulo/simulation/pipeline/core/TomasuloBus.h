// ============================================================================
// File: include/apps/cpu_tomasulo/simulation/pipeline/core/TomasuloBus.h
// ============================================================================

#pragma once

/**
 * @file TomasuloBus.h
 * @brief Flat structure holding EVERY wire in the Tomasulo microarchitecture.
 *
 * All components read inputs and write outputs through this shared bus.
 * One bus instance exists per cycle; it is cleared at the start of each cycle
 * and populated as components are evaluated in topological order.
 *
 * Wire naming: exactly matches the spec — e.g. PCCurrent_o, PCNext_i, etc.
 * Each field is written by ONE producer and read by one or more consumers.
 *
 * @note SRP: Only holds signal values, no logic.
 */

#include <cstdint>

struct TomasuloBus {

    // ════════════════════════════════════════════════════════════
    // GLOBAL CONTROL
    // ════════════════════════════════════════════════════════════
    bool     CLK = false;
    bool     RESET = false;

    // ════════════════════════════════════════════════════════════
    // COMPONENT 1: PC_C
    // ════════════════════════════════════════════════════════════
    uint64_t PCNext_i = 0;      // <- PC_MUX.PCNext_o
    bool     StallIF_i = false;  // <- Control_Unit.StallIF_o
    bool     Flush_PC_i = false;  // <- Commit_Unit.Flush_o (for PC)
    uint64_t PCCurrent_o = 0;      // -> PC_Adder, I_Cache, ROB, RS_Branch0

    // ════════════════════════════════════════════════════════════
    // COMPONENT 2: PC_MUX
    // ════════════════════════════════════════════════════════════
    uint64_t PCPlus8_i = 0;      // <- PC_Adder.PCPlus8_o
    uint64_t BranchTarget_i = 0;      // <- Commit_Unit.BranchTarget_o
    bool     BranchRedirect_i = false;  // <- Commit_Unit.BranchRedirect_o
    uint64_t PCNext_o = 0;      // -> PC_C.PCNext_i

    // ════════════════════════════════════════════════════════════
    // COMPONENT 3: PC_Adder
    // ════════════════════════════════════════════════════════════
    uint64_t PCPlus8_o = 0;      // -> PC_MUX.PCPlus8_i

    // ════════════════════════════════════════════════════════════
    // COMPONENT 4: I_Cache
    // ════════════════════════════════════════════════════════════
    uint64_t InstrF_o = 0;      // -> Instruction_Decoder
    bool     InstReady_o = false;  // -> Control_Unit
    uint64_t ICtoRAM_Addr_o = 0;      // -> RAM
    bool     ICtoRAM_Req_o = false;  // -> RAM
    // Inputs from RAM
    uint64_t RAMtoIC_Data_o[8] = {};   // 512 bits = 8 x 64-bit words
    bool     RAMtoIC_Ready_o = false;

    // ════════════════════════════════════════════════════════════
    // COMPONENT 5: RAM (outputs to caches)
    // ════════════════════════════════════════════════════════════
    uint64_t RAMtoDC_Data_o[8] = {};
    bool     RAMtoDC_Ready_o = false;
    // D-Cache -> RAM inputs
    uint64_t DCtoRAM_Addr_i = 0;
    bool     DCtoRAM_RdReq_i = false;
    bool     DCtoRAM_WrReq_i = false;
    uint64_t DCtoRAM_WrData_i[8] = {};

    // ════════════════════════════════════════════════════════════
    // COMPONENT 6: Instruction_Decoder
    // ════════════════════════════════════════════════════════════
    uint8_t  Op_in_o = 0;    // 8 bits
    uint8_t  Dest_in_o = 0;    // 4 bits  (Rd)
    uint8_t  Rn_in_o = 0;    // 4 bits  (source register 1)
    uint8_t  Rm_in_o = 0;    // 4 bits  (source register 2)
    uint32_t Imm_in_o = 0;    // 32 bits (raw immediate / offset field)

    // ════════════════════════════════════════════════════════════
    // COMPONENT 7: Extends
    // ════════════════════════════════════════════════════════════
    bool     IsFPALU_o = false;  // <- Control_Unit (used downstream, not by Extends itself)
    bool     IsFPMUL_o = false;  // <- Control_Unit
    uint64_t ImmExt_o = 0;      // -> RS/LB/SB: sign-extended or float32->float64 immediate

    // ════════════════════════════════════════════════════════════
    // COMPONENT 8: Control_Unit
    // ════════════════════════════════════════════════════════════
    bool     StallIF_o = false;  // -> PC_C: freeze the PC this cycle
    bool     ROBAlloc_o = false;  // -> ROB: allocate a new entry at tail
    bool     AllocSB0_o = false;  // -> SB0: allocate store buffer 0
    bool     AllocSB1_o = false;  // -> SB1: allocate store buffer 1
    bool     AllocLB0_o = false;  // -> LB0: allocate load buffer 0
    bool     AllocLB1_o = false;  // -> LB1: allocate load buffer 1
    bool     AllocLB2_o = false;  // -> LB2: allocate load buffer 2
    bool     AllocRSIntALU0_o = false;  // -> RS_IntALU0: allocate integer ALU RS 0
    bool     AllocRSIntALU1_o = false;  // -> RS_IntALU1: allocate integer ALU RS 1
    bool     AllocRSFPALU0_o = false;  // -> RS_FPALU0:  allocate FP ALU RS
    bool     AllocRSIntMUL0_o = false;  // -> RS_IntMUL0: allocate integer MUL RS
    bool     AllocRSFPMUL0_o = false;  // -> RS_FPMUL0:  allocate FP MUL RS
    bool     AllocRSBranch0_o = false;  // -> RS_Branch0: allocate branch RS
    bool     TagWrEn_o = false;  // -> Register_File: write ROB tag to destination register
    bool     FlagsTagWrEn_o = false;  // -> Flags_Unit: write ROB tag for pending flag result
    uint8_t  AllocType_o = 0;      // 3-bit ROB entry type (000=IntALU … 111=CMP)
    uint8_t  AllocSourceStation_o = 0;    // 4-bit station ID stored in the ROB entry
    bool     ModifiesFlags_o = false;  // -> RS/ROB: instruction produces a flag result
    bool     NeedsFlags_o = false;  // -> RS: instruction reads the current flag register
    bool     UseImm_o = false;  // -> RS/LB/SB: use ImmExt_o as second operand
    bool     IsCMPonly_o = false;  // -> ROB: instruction updates flags only, no Rd
    bool     AllocPredicted_o = false;  // -> ROB (Branch): predicted-taken value

    // ════════════════════════════════════════════════════════════
    // COMPONENT 9: Register_File
    // ════════════════════════════════════════════════════════════
    uint64_t RD1_Value_o = 0;      // Value of Rn (or 0 if renamed)
    uint8_t  RD1_Qi_o = 0;      // 5-bit ROB tag for Rn (valid when RD1_QiValid_o)
    bool     RD1_QiValid_o = false;  // True if Rn result is still pending in the ROB
    uint64_t RD2_Value_o = 0;      // Value of Rm (or 0 if renamed)
    uint8_t  RD2_Qi_o = 0;      // 5-bit ROB tag for Rm
    bool     RD2_QiValid_o = false;
    uint64_t UPPER_o = 0;      // Upper half of a wide register read (future use)
    uint64_t LOWER_o = 0;      // Lower half of a wide register read
    // Commit write-back inputs
    bool     CommitWrEn_i = false;  // <- Commit_Unit: write enable
    uint8_t  CommitWrAddr_i = 0;      // <- Commit_Unit: destination register index
    uint64_t CommitWrData_i = 0;      // <- Commit_Unit: value to write
    uint8_t  CommitROBIdx_i = 0;      // 5-bit ROB index to clear the tag
    // Store data port (Rd is the data source for STR instructions)
    uint64_t RD_StoreValue_o = 0;      // Value of Rd for store data
    uint8_t  RD_StoreQi_o = 0;      // 5-bit ROB tag for Rd (valid when RD_StoreQiValid_o)
    bool     RD_StoreQiValid_o = false;

    // ════════════════════════════════════════════════════════════
    // COMPONENT 10: ROB
    // ════════════════════════════════════════════════════════════
    uint8_t  ROBTail_o = 0;      // 5-bit tail pointer (next free slot index)
    bool     ROBFull_o = false;  // -> Control_Unit: stall issue when full
    // Issue-time value forwarding (checked while RS is being filled)
    uint64_t ROBReadValue1_o = 0;
    bool     ROBReadReady1_o = false;
    uint64_t ROBReadValue2_o = 0;
    bool     ROBReadReady2_o = false;
    bool     ROBReadFlagsReady_o = false;
    uint8_t  ROBReadFlagsResult_o = 0;   // 4-bit flag result if already computed
    // Head outputs for Commit_Unit
    uint8_t  ROBHead_o = 0;   // 5-bit head pointer (oldest in-flight entry)
    bool     ROBHeadBusy_o = false;
    bool     ROBHeadReady_o = false;  // True when result has been written back
    uint8_t  ROBHeadType_o = 0;      // 3-bit instruction type
    uint8_t  ROBHeadDestReg_o = 0;      // Destination register (0xF = no destination)
    uint64_t ROBHeadValue_o = 0;      // Computed result
    uint8_t  ROBHeadException_o = 0;      // Non-zero = exception occurred
    uint64_t ROBHeadPC_o = 0;      // PC of the instruction at head
    uint8_t  ROBHeadFlagsResult_o = 0;      // Flag bits (N,Z,C,V) computed by head entry
    bool     ROBHeadFlagsValid_o = false;
    bool     ROBHeadMispredict_o = false;  // Branch mispredicted
    uint64_t ROBHeadBranchTarget_o = 0;      // Correct branch target on mispredict
    uint64_t ROBHeadStoreAddr_o = 0;      // Resolved store address (type=Store)
    uint64_t ROBHeadStoreData_o = 0;      // Store data value
    bool     ROBHeadStoreReady_o = false;  // True when both addr and data are known
    uint8_t  ROBHeadSourceStation_o = 0;      // 4-bit station ID (for Free signal routing)
    // ROB inputs
    bool     CommitPop_i = false;  // <- Commit_Unit: advance head pointer

    // ════════════════════════════════════════════════════════════
    // COMPONENT 11: Flags_Unit
    // ════════════════════════════════════════════════════════════
    uint8_t  FlagsArch_o = 0;      // 4-bit architectural flags (N,Z,C,V)
    uint8_t  FlagsQi_o = 0;      // 5-bit ROB tag of the in-flight flags producer
    bool     FlagsQiValid_o = false;  // True if a flags result is still pending in ROB
    // Commit inputs
    bool     FlagsCommitEn_i = false;  // <- Commit_Unit: write committed flags
    uint8_t  FlagsCommitValue_i = 0;    // <- Commit_Unit: committed (N,Z,C,V) value

    // ════════════════════════════════════════════════════════════
    // RESERVATION STATION & BUFFER BUSY SIGNALS (-> Control_Unit)
    // ════════════════════════════════════════════════════════════
    bool     SB0_Busy_o = false;
    bool     SB1_Busy_o = false;
    bool     LB0_Busy_o = false;
    bool     LB1_Busy_o = false;
    bool     LB2_Busy_o = false;
    bool     RS_IntALU0_Busy_o = false;
    bool     RS_IntALU1_Busy_o = false;
    bool     RS_FPALU0_Busy_o = false;
    bool     RS_IntMUL0_Busy_o = false;
    bool     RS_FPMUL0_Busy_o = false;
    bool     RS_Branch0_Busy_o = false;

    // ════════════════════════════════════════════════════════════
    // AGU ARBITER (Component 23) & AGU units (24-25)
    // ════════════════════════════════════════════════════════════
    // SB/LB -> AGU_Arbiter requests (base + offset for address calculation)
    bool     SB0_AGUReq_o = false; uint64_t SB0_AGUBase_o = 0; uint64_t SB0_AGUOffset_o = 0; uint8_t SB0_AGUROBTag_o = 0;
    bool     SB1_AGUReq_o = false; uint64_t SB1_AGUBase_o = 0; uint64_t SB1_AGUOffset_o = 0; uint8_t SB1_AGUROBTag_o = 0;
    bool     LB0_AGUReq_o = false; uint64_t LB0_AGUBase_o = 0; uint64_t LB0_AGUOffset_o = 0; uint8_t LB0_AGUROBTag_o = 0;
    bool     LB1_AGUReq_o = false; uint64_t LB1_AGUBase_o = 0; uint64_t LB1_AGUOffset_o = 0; uint8_t LB1_AGUROBTag_o = 0;
    bool     LB2_AGUReq_o = false; uint64_t LB2_AGUBase_o = 0; uint64_t LB2_AGUOffset_o = 0; uint8_t LB2_AGUROBTag_o = 0;
    // AGU_Arbiter -> AGU units (dispatched work)
    bool     AGU0_Start_o = false; uint64_t AGU0_Base_o = 0; uint64_t AGU0_Offset_o = 0; uint8_t AGU0_SourceID_o = 0;
    bool     AGU1_Start_o = false; uint64_t AGU1_Base_o = 0; uint64_t AGU1_Offset_o = 0; uint8_t AGU1_SourceID_o = 0;
    // AGU units -> AGU_Arbiter (results)
    bool     AGU0_Free_o = true;  bool AGU0_Done_o = false; uint64_t AGU0_Address_o = 0; bool AGU0_SegFault_o = false; uint8_t AGU0_SourceID_out = 0;
    bool     AGU1_Free_o = true;  bool AGU1_Done_o = false; uint64_t AGU1_Address_o = 0; bool AGU1_SegFault_o = false; uint8_t AGU1_SourceID_out = 0;
    // AGU_Arbiter -> SB/LB done signals (resolved effective addresses)
    bool     SB0_AGUDone_o = false; uint64_t SB0_AGUAddress_o = 0; bool SB0_AGUSegFault_o = false;
    bool     SB1_AGUDone_o = false; uint64_t SB1_AGUAddress_o = 0; bool SB1_AGUSegFault_o = false;
    bool     LB0_AGUDone_o = false; uint64_t LB0_AGUAddress_o = 0; bool LB0_AGUSegFault_o = false;
    bool     LB1_AGUDone_o = false; uint64_t LB1_AGUAddress_o = 0; bool LB1_AGUSegFault_o = false;
    bool     LB2_AGUDone_o = false; uint64_t LB2_AGUAddress_o = 0; bool LB2_AGUSegFault_o = false;

    // ════════════════════════════════════════════════════════════
    // STORE COMPLETE SIGNALS (SB -> ROB)
    // ════════════════════════════════════════════════════════════
    // Store buffer notifies ROB when both address and data are resolved
    bool     SB0_StComplete_Valid_o = false; uint8_t SB0_StComplete_ROBTag_o = 0; uint64_t SB0_StComplete_Addr_o = 0; uint64_t SB0_StComplete_Data_o = 0;
    bool     SB1_StComplete_Valid_o = false; uint8_t SB1_StComplete_ROBTag_o = 0; uint64_t SB1_StComplete_Addr_o = 0; uint64_t SB1_StComplete_Data_o = 0;

    // Store buffer address exposure (for load-ordering checks in Memory_Arbiter)
    bool     SB0_AddrReady_o = false;
    uint64_t SB0_ExposedAddr_o = 0;
    bool     SB1_AddrReady_o = false;
    uint64_t SB1_ExposedAddr_o = 0;

    // Store buffer ROB tag exposure (for ROB age comparison in storeConflict)
    uint8_t SB0_ExposedROBTag_o = 0;
    uint8_t SB1_ExposedROBTag_o = 0;

    // ════════════════════════════════════════════════════════════
    // RS -> ARBITER REQUEST SIGNALS
    // ════════════════════════════════════════════════════════════
    // IntALU RS -> IntALU_Arbiter
    bool     RSIAU0_Req_o = false; uint8_t RSIAU0_Op_o = 0; uint64_t RSIAU0_Vj_o = 0; uint64_t RSIAU0_Vk_o = 0;
    uint8_t  RSIAU0_DestROBTag_o = 0; bool RSIAU0_ModFlags_o = false; bool RSIAU0_NeedsFlags_o = false; uint8_t RSIAU0_FlagsValue_o = 0;
    bool     RSIAU1_Req_o = false; uint8_t RSIAU1_Op_o = 0; uint64_t RSIAU1_Vj_o = 0; uint64_t RSIAU1_Vk_o = 0;
    uint8_t  RSIAU1_DestROBTag_o = 0; bool RSIAU1_ModFlags_o = false; bool RSIAU1_NeedsFlags_o = false; uint8_t RSIAU1_FlagsValue_o = 0;
    // FPALU RS -> FPALU_Arbiter
    bool     RSFPAU0_Req_o = false; uint8_t RSFPAU0_Op_o = 0; uint64_t RSFPAU0_Vj_o = 0; uint64_t RSFPAU0_Vk_o = 0;
    uint8_t  RSFPAU0_DestROBTag_o = 0; bool RSFPAU0_ModFlags_o = false; bool RSFPAU0_NeedsFlags_o = false; uint8_t RSFPAU0_FlagsValue_o = 0;
    // IntMUL RS -> IntMUL_Arbiter
    bool     RSIMUL0_Req_o = false; uint8_t RSIMUL0_Op_o = 0; uint64_t RSIMUL0_Vj_o = 0; uint64_t RSIMUL0_Vk_o = 0;
    uint8_t  RSIMUL0_DestROBTag_o = 0; bool RSIMUL0_ModFlags_o = false; bool RSIMUL0_NeedsFlags_o = false; uint8_t RSIMUL0_FlagsValue_o = 0;
    // FPMUL RS -> FPMUL_Arbiter
    bool     RSFPMUL0_Req_o = false; uint8_t RSFPMUL0_Op_o = 0; uint64_t RSFPMUL0_Vj_o = 0; uint64_t RSFPMUL0_Vk_o = 0;
    uint8_t  RSFPMUL0_DestROBTag_o = 0; bool RSFPMUL0_ModFlags_o = false; bool RSFPMUL0_NeedsFlags_o = false; uint8_t RSFPMUL0_FlagsValue_o = 0;
    // Branch RS -> Branch_Arbiter
    bool     RSBr0_Req_o = false; uint8_t RSBr0_Op_o = 0; uint64_t RSBr0_TargetPC_o = 0;
    uint8_t  RSBr0_FlagsValue_o = 0; bool RSBr0_PredTaken_o = false; uint8_t RSBr0_ROBTag_o = 0;

    // ════════════════════════════════════════════════════════════
    // ARBITER -> RS GRANT SIGNALS
    // ════════════════════════════════════════════════════════════
    bool     RSIAU0_Grant_o = false;  ///< RS_IntALU0 was selected and its request forwarded.
    bool     RSIAU1_Grant_o = false;  ///< RS_IntALU1 was selected.
    bool     RSFPAU0_Grant_o = false;  ///< RS_FPALU0 was selected.
    bool     RSIMUL0_Grant_o = false;  ///< RS_IntMUL0 was selected.
    bool     RSFPMUL0_Grant_o = false; ///< RS_FPMUL0 was selected.
    bool     RSBr0_Grant_o = false;  ///< RS_Branch0 was selected.

    // ════════════════════════════════════════════════════════════
    // ARBITER -> EXECUTION UNIT SIGNALS
    // ════════════════════════════════════════════════════════════
    // IntALU dispatch
    bool     IntALU_Start_o = false; uint8_t IntALU_Op_o = 0; uint64_t IntALU_A_o = 0; uint64_t IntALU_B_o = 0;
    uint8_t  IntALU_DestROBTag_o = 0;     bool IntALU_ModFlags_o = false; bool IntALU_NeedsFlags_o = false; uint8_t IntALU_FlagsIn_o = 0;
    bool     IntALU_Free_o = true;  ///< True when IntALU can accept a new operation.
    // FPALU dispatch
    bool     FPALU_Start_o = false; uint8_t FPALU_Op_o = 0; uint64_t FPALU_A_o = 0; uint64_t FPALU_B_o = 0;
    uint8_t  FPALU_DestROBTag_o = 0;     bool FPALU_ModFlags_o = false; bool FPALU_NeedsFlags_o = false; uint8_t FPALU_FlagsIn_o = 0;
    bool     FPALU_Free_o = true;  ///< True when stage 1 of FPALU pipeline is empty.
    // IntMUL dispatch
    bool     IntMUL_Start_o = false; uint8_t IntMUL_Op_o = 0; uint64_t IntMUL_A_o = 0; uint64_t IntMUL_B_o = 0;
    uint8_t  IntMUL_DestROBTag_o = 0;     bool IntMUL_ModFlags_o = false; bool IntMUL_NeedsFlags_o = false; uint8_t IntMUL_FlagsIn_o = 0;
    bool     IntMUL_Free_o = true;  ///< True when IntMUL is not busy.
    // FPMUL dispatch
    bool     FPMUL_Start_o = false; uint8_t FPMUL_Op_o = 0; uint64_t FPMUL_A_o = 0; uint64_t FPMUL_B_o = 0;
    uint8_t  FPMUL_DestROBTag_o = 0;     bool FPMUL_ModFlags_o = false; bool FPMUL_NeedsFlags_o = false; uint8_t FPMUL_FlagsIn_o = 0;
    bool     FPMUL_Free_o = true;  ///< True when FPMUL is not busy.
    // Branch_Executor dispatch
    bool     BrEx_Start_o = false; uint8_t BrEx_Op_o = 0; uint64_t BrEx_TargetPC_o = 0;
    uint8_t  BrEx_FlagsValue_o = 0;     bool BrEx_PredTaken_o = false; uint8_t BrEx_ROBTag_o = 0;

    // ════════════════════════════════════════════════════════════
    // EXECUTION UNIT -> CDB_ARBITER / ROB
    // ════════════════════════════════════════════════════════════
    // IntALU -> CDB_Arbiter
    bool     IntALU_CDBReq_o = false; uint8_t IntALU_CDBROBTag_o = 0; uint64_t IntALU_CDBValue_o = 0;
    uint8_t  IntALU_CDBException_o = 0;     uint8_t IntALU_CDBFlags_o = 0;  bool IntALU_CDBFlagsValid_o = false;
    bool     IntALU_CDBStall_o = false; ///< CDB arbiter denied this cycle; hold result.
    // FPALU -> CDB_Arbiter
    bool     FPALU_CDBReq_o = false; uint8_t FPALU_CDBROBTag_o = 0; uint64_t FPALU_CDBValue_o = 0;
    uint8_t  FPALU_CDBException_o = 0;     uint8_t FPALU_CDBFlags_o = 0;  bool FPALU_CDBFlagsValid_o = false;
    bool     FPALU_CDBStall_o = false;
    // IntMUL -> CDB_Arbiter
    bool     IntMUL_CDBReq_o = false; uint8_t IntMUL_CDBROBTag_o = 0; uint64_t IntMUL_CDBValue_o = 0;
    uint8_t  IntMUL_CDBException_o = 0;     uint8_t IntMUL_CDBFlags_o = 0;  bool IntMUL_CDBFlagsValid_o = false;
    bool     IntMUL_CDBStall_o = false;
    // FPMUL -> CDB_Arbiter
    bool     FPMUL_CDBReq_o = false; uint8_t FPMUL_CDBROBTag_o = 0; uint64_t FPMUL_CDBValue_o = 0;
    uint8_t  FPMUL_CDBException_o = 0;     uint8_t FPMUL_CDBFlags_o = 0;  bool FPMUL_CDBFlagsValid_o = false;
    bool     FPMUL_CDBStall_o = false;
    // LB -> CDB_Arbiter (loads never produce flags)
    bool     LB0_CDBReq_o = false; uint8_t LB0_CDBROBTag_o = 0; uint64_t LB0_CDBValue_o = 0; uint8_t LB0_CDBException_o = 0; bool LB0_CDBStall_o = false;
    bool     LB1_CDBReq_o = false; uint8_t LB1_CDBROBTag_o = 0; uint64_t LB1_CDBValue_o = 0; uint8_t LB1_CDBException_o = 0; bool LB1_CDBStall_o = false;
    bool     LB2_CDBReq_o = false; uint8_t LB2_CDBROBTag_o = 0; uint64_t LB2_CDBValue_o = 0; uint8_t LB2_CDBException_o = 0; bool LB2_CDBStall_o = false;
    // Branch_Executor -> ROB (branches bypass the CDB; result written directly to ROB)
    bool     BrExResult_Valid_o = false; uint8_t BrExResult_ROBTag_o = 0; bool BrExResult_Taken_o = false;
    uint64_t BrExResult_Target_o = 0;     bool BrExResult_Mispredict_o = false;

    // ════════════════════════════════════════════════════════════
    // CDB_ARBITER -> CDB_A / CDB_B
    // ════════════════════════════════════════════════════════════
    // Winner 1 payload (forwarded to CDB_A)
    bool     ArbToCDBA_Valid_o = false; uint8_t ArbToCDBA_ROBTag_o = 0; uint64_t ArbToCDBA_Value_o = 0;
    uint8_t  ArbToCDBA_Exception_o = 0; uint8_t ArbToCDBA_Flags_o = 0; bool ArbToCDBA_FlagsValid_o = false;
    // Winner 2 payload (forwarded to CDB_B)
    bool     ArbToCDBB_Valid_o = false; uint8_t ArbToCDBB_ROBTag_o = 0; uint64_t ArbToCDBB_Value_o = 0;
    uint8_t  ArbToCDBB_Exception_o = 0; uint8_t ArbToCDBB_Flags_o = 0; bool ArbToCDBB_FlagsValid_o = false;

    // ════════════════════════════════════════════════════════════
    // CDB_A / CDB_B BROADCAST (-> all snooping components)
    // ════════════════════════════════════════════════════════════
    // All RS, LB, SB and the ROB snoop these wires every cycle.
    bool     CDBA_Valid_o = false; uint8_t CDBA_ROBTag_o = 0; uint64_t CDBA_Value_o = 0;
    uint8_t  CDBA_Exception_o = 0; uint8_t CDBA_Flags_o = 0; bool CDBA_FlagsValid_o = false;
    bool     CDBB_Valid_o = false; uint8_t CDBB_ROBTag_o = 0; uint64_t CDBB_Value_o = 0;
    uint8_t  CDBB_Exception_o = 0; uint8_t CDBB_Flags_o = 0; bool CDBB_FlagsValid_o = false;

    // ════════════════════════════════════════════════════════════
    // MEMORY_ARBITER (Component 39)
    // ════════════════════════════════════════════════════════════
    // LB -> Memory_Arbiter load requests
    bool     LB0_MemReq_o = false; uint64_t LB0_MemAddr_o = 0; uint8_t LB0_MemOp_o = 0; uint8_t LB0_MemROBTag_o = 0;
    bool     LB1_MemReq_o = false; uint64_t LB1_MemAddr_o = 0; uint8_t LB1_MemOp_o = 0; uint8_t LB1_MemROBTag_o = 0;
    bool     LB2_MemReq_o = false; uint64_t LB2_MemAddr_o = 0; uint8_t LB2_MemOp_o = 0; uint8_t LB2_MemROBTag_o = 0;
    // Memory_Arbiter -> LB completion
    bool     LB0_MemDone_o = false; uint64_t LB0_MemData_o = 0;
    bool     LB1_MemDone_o = false; uint64_t LB1_MemData_o = 0;
    bool     LB2_MemDone_o = false; uint64_t LB2_MemData_o = 0;
    // Memory_Arbiter -> D_Cache request
    bool     DC_Req_o = false;   ///< Valid D_Cache access this cycle.
    bool     DC_RW_o = false;   ///< false = read, true = write.
    uint64_t DC_Addr_o = 0;
    uint64_t DC_WData_o = 0;
    uint8_t  DC_Size_o = 0;      ///< 0x00 = byte, 0x03 = 8-byte word.
    // D_Cache -> Memory_Arbiter response
    bool     DC_Done_o = false;  ///< D_Cache completed the request this cycle.
    uint64_t DC_RData_o = 0;      ///< Read data (valid when DC_Done_o and !DC_RW_o).
    bool     DC_Ready_o = true;   ///< D_Cache is idle and can accept a new request.
    // Store commit handshake (Commit_Unit <-> Memory_Arbiter)
    bool     StoreCommit_Req_o = false; uint64_t StoreCommit_Addr_o = 0; uint64_t StoreCommit_Data_o = 0; uint8_t StoreCommit_Op_o = 0;
    bool     StoreCommit_Done_o = false; ///< Memory_Arbiter signals the write has completed.

    // ════════════════════════════════════════════════════════════
    // COMMIT_UNIT (Component 41)
    // ════════════════════════════════════════════════════════════
    bool     Flush_o = false;  ///< Squash all in-flight speculative state.
    bool     BranchRedirect_o = false;  ///< Redirect PC to BranchTarget_o.
    uint64_t BranchTarget_o = 0;      ///< Correct target PC after branch mispredict.
    // Station free signals (one per RS/buffer type)
    bool     FreeSB0_o = false; bool FreeSB1_o = false;
    bool     FreeLB0_o = false; bool FreeLB1_o = false; bool FreeLB2_o = false;
    bool     FreeRSIAU0_o = false; bool FreeRSIAU1_o = false;
    bool     FreeRSFPAU0_o = false; bool FreeRSIMUL0_o = false; bool FreeRSFPMUL0_o = false;
    bool     FreeRSBr0_o = false;
    // Exception signals
    bool     ExceptionSignal_o = false; ///< An architectural exception was committed.
    uint8_t  ExceptionCode_o = 0;     ///< Exception type code.
    uint64_t ExceptionPC_o = 0;     ///< PC of the faulting instruction.

    /** @brief Zero-initialize every field for a new cycle. */
    void clearOutputs() { *this = TomasuloBus{}; }

    // ROB head opcode (forwarded for Commit_Unit to distinguish NOP vs SWI)
    uint8_t ROBHeadOp_o = 0;

    // CPU halt flag (set when SWI is committed; clears on reset)
    bool Halt_o = false;
};