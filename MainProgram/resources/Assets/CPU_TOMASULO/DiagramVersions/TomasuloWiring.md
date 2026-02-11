# Tomasulo Micro-Architecture — Definitive Wiring (41 Components)

**RULE**: Each wire has ONE global name. Output = `Name_o`. Input = `Name_i`. No exceptions.

---

## COMPONENT 1: PC_C

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | PCNext_i | 64 | PC_MUX.PCNext_o |
| IN | CLK | 1 | Global |
| IN | RESET | 1 | Global |
| IN | StallIF_i | 1 | Control_Unit.StallIF_o |
| OUT | PCCurrent_o | 64 | PC_Adder.PCCurrent_i, I_Cache.PCCurrent_i, ROB.PCCurrent_i, RS_Branch0.PCCurrent_i |

---
## COMPONENT 2: PC_MUX

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | PCPlus8_i | 64 | PC_Adder.PCPlus8_o |
| IN | BranchTarget_i | 64 | Commit_Unit.BranchTarget_o |
| IN | BranchRedirect_i | 1 | Commit_Unit.BranchRedirect_o |
| OUT | PCNext_o | 64 | PC_C.PCNext_i |

---
## COMPONENT 3: PC_Adder

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | PCCurrent_i | 64 | PC_C.PCCurrent_o |
| OUT | PCPlus8_o | 64 | PC_MUX.PCPlus8_i |

---
## COMPONENT 4: I_Cache

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | PCCurrent_i | 64 | PC_C.PCCurrent_o |
| IN | CLK | 1 | Global |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| IN | RAMtoIC_Data_i | 512 | RAM.RAMtoIC_Data_o |
| IN | RAMtoIC_Ready_i | 1 | RAM.RAMtoIC_Ready_o |
| OUT | InstrF_o | 64 | Instruction_Decoder.InstrF_i |
| OUT | InstReady_o | 1 | Control_Unit.InstReady_i |
| OUT | ICtoRAM_Addr_o | 64 | RAM.ICtoRAM_Addr_i |
| OUT | ICtoRAM_Req_o | 1 | RAM.ICtoRAM_Req_i |

---
## COMPONENT 5: RAM

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | ICtoRAM_Addr_i | 64 | I_Cache.ICtoRAM_Addr_o |
| IN | ICtoRAM_Req_i | 1 | I_Cache.ICtoRAM_Req_o |
| IN | DCtoRAM_Addr_i | 64 | D_Cache.DCtoRAM_Addr_o |
| IN | DCtoRAM_RdReq_i | 1 | D_Cache.DCtoRAM_RdReq_o |
| IN | DCtoRAM_WrReq_i | 1 | D_Cache.DCtoRAM_WrReq_o |
| IN | DCtoRAM_WrData_i | 512 | D_Cache.DCtoRAM_WrData_o |
| IN | CLK | 1 | Global |
| OUT | RAMtoIC_Data_o | 512 | I_Cache.RAMtoIC_Data_i |
| OUT | RAMtoIC_Ready_o | 1 | I_Cache.RAMtoIC_Ready_i |
| OUT | RAMtoDC_Data_o | 512 | D_Cache.RAMtoDC_Data_i |
| OUT | RAMtoDC_Ready_o | 1 | D_Cache.RAMtoDC_Ready_i |

---
## COMPONENT 6: Instruction_Decoder

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | InstrF_i | 64 | I_Cache.InstrF_o |
| OUT | Op_in_o | 8 | Control_Unit.Op_in_i, SB0.Op_in_i, SB1.Op_in_i, LB0.Op_in_i, LB1.Op_in_i, LB2.Op_in_i, RS_IntALU0.Op_in_i, RS_IntALU1.Op_in_i, RS_FPALU0.Op_in_i, RS_IntMUL0.Op_in_i, RS_FPMUL0.Op_in_i, RS_Branch0.Op_in_i |
| OUT | Dest_in_o | 4 | Control_Unit.Dest_in_i, Register_File.Dest_in_i, ROB.Dest_in_i |
| OUT | Rn_in_o | 4 | Register_File.Rn_in_i |
| OUT | Rm_in_o | 4 | Register_File.Rm_in_i |
| OUT | Imm_in_o | 32 | Extends.Imm_in_i |

---
## COMPONENT 7: Extends

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | Imm_in_i | 32 | Instruction_Decoder.Imm_in_o |
| IN | IsFPALU_i | 1 | Control_Unit.IsFPALU_o |
| IN | IsFPMUL_i | 1 | Control_Unit.IsFPMUL_o |
| OUT | ImmExt_o | 64 | SB0.ImmExt_i, SB1.ImmExt_i, LB0.ImmExt_i, LB1.ImmExt_i, LB2.ImmExt_i, RS_IntALU0.ImmExt_i, RS_IntALU1.ImmExt_i, RS_FPALU0.ImmExt_i, RS_IntMUL0.ImmExt_i, RS_FPMUL0.ImmExt_i, RS_Branch0.ImmExt_i |

---
Internal signals (NOT wires): IsALU, IsMUL, IsFPALU_int, IsFPMUL_int, IsLoad, IsStore, IsBranch, IsUnconditional.

## COMPONENT 8: Control_Unit

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | Dest_in_i | 4 | Instruction_Decoder.Dest_in_o |
| IN | InstReady_i | 1 | I_Cache.InstReady_o |
| IN | ROBFull_i | 1 | ROB.ROBFull_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | SB0_Busy_i | 1 | SB0.SB0_Busy_o |
| IN | SB1_Busy_i | 1 | SB1.SB1_Busy_o |
| IN | LB0_Busy_i | 1 | LB0.LB0_Busy_o |
| IN | LB1_Busy_i | 1 | LB1.LB1_Busy_o |
| IN | LB2_Busy_i | 1 | LB2.LB2_Busy_o |
| IN | RS_IntALU0_Busy_i | 1 | RS_IntALU0.RS_IntALU0_Busy_o |
| IN | RS_IntALU1_Busy_i | 1 | RS_IntALU1.RS_IntALU1_Busy_o |
| IN | RS_FPALU0_Busy_i | 1 | RS_FPALU0.RS_FPALU0_Busy_o |
| IN | RS_IntMUL0_Busy_i | 1 | RS_IntMUL0.RS_IntMUL0_Busy_o |
| IN | RS_FPMUL0_Busy_i | 1 | RS_FPMUL0.RS_FPMUL0_Busy_o |
| IN | RS_Branch0_Busy_i | 1 | RS_Branch0.RS_Branch0_Busy_o |
| OUT | StallIF_o | 1 | PC_C.StallIF_i |
| OUT | ROBAlloc_o | 1 | ROB.ROBAlloc_i |
| OUT | AllocSB0_o | 1 | SB0.AllocSB0_i |
| OUT | AllocSB1_o | 1 | SB1.AllocSB1_i |
| OUT | AllocLB0_o | 1 | LB0.AllocLB0_i |
| OUT | AllocLB1_o | 1 | LB1.AllocLB1_i |
| OUT | AllocLB2_o | 1 | LB2.AllocLB2_i |
| OUT | AllocRSIntALU0_o | 1 | RS_IntALU0.AllocRSIntALU0_i |
| OUT | AllocRSIntALU1_o | 1 | RS_IntALU1.AllocRSIntALU1_i |
| OUT | AllocRSFPALU0_o | 1 | RS_FPALU0.AllocRSFPALU0_i |
| OUT | AllocRSIntMUL0_o | 1 | RS_IntMUL0.AllocRSIntMUL0_i |
| OUT | AllocRSFPMUL0_o | 1 | RS_FPMUL0.AllocRSFPMUL0_i |
| OUT | AllocRSBranch0_o | 1 | RS_Branch0.AllocRSBranch0_i |
| OUT | TagWrEn_o | 1 | Register_File.TagWrEn_i |
| OUT | FlagsTagWrEn_o | 1 | Flags_Unit.FlagsTagWrEn_i |
| OUT | IsFPALU_o | 1 | Extends.IsFPALU_i |
| OUT | IsFPMUL_o | 1 | Extends.IsFPMUL_i |
| OUT | AllocType_o | 3 | ROB.AllocType_i |
| OUT | AllocSourceStation_o | 4 | ROB.AllocSourceStation_i |
| OUT | ModifiesFlags_o | 1 | ROB.ModifiesFlags_i, SB0.ModifiesFlags_i, SB1.ModifiesFlags_i, LB0.ModifiesFlags_i, LB1.ModifiesFlags_i, LB2.ModifiesFlags_i, RS_IntALU0.ModifiesFlags_i, RS_IntALU1.ModifiesFlags_i, RS_FPALU0.ModifiesFlags_i, RS_IntMUL0.ModifiesFlags_i, RS_FPMUL0.ModifiesFlags_i, RS_Branch0.ModifiesFlags_i |
| OUT | NeedsFlags_o | 1 | RS_IntALU0.NeedsFlags_i, RS_IntALU1.NeedsFlags_i, RS_FPALU0.NeedsFlags_i, RS_IntMUL0.NeedsFlags_i, RS_FPMUL0.NeedsFlags_i, RS_Branch0.NeedsFlags_i |
| OUT | UseImm_o | 1 | RS_IntALU0.UseImm_i, RS_IntALU1.UseImm_i, RS_FPALU0.UseImm_i, RS_IntMUL0.UseImm_i, RS_FPMUL0.UseImm_i |
| OUT | IsCMPonly_o | 1 | ROB.IsCMPonly_i |
| OUT | AllocPredicted_o | 1 | ROB.AllocPredicted_i |

---
## COMPONENT 9: Register_File

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | Rn_in_i | 4 | Instruction_Decoder.Rn_in_o |
| IN | Rm_in_i | 4 | Instruction_Decoder.Rm_in_o |
| IN | TagWrEn_i | 1 | Control_Unit.TagWrEn_o |
| IN | Dest_in_i | 4 | Instruction_Decoder.Dest_in_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | CommitWrEn_i | 1 | Commit_Unit.CommitWrEn_o |
| IN | CommitWrAddr_i | 4 | Commit_Unit.CommitWrAddr_o |
| IN | CommitWrData_i | 64 | Commit_Unit.CommitWrData_o |
| IN | CommitROBIdx_i | 5 | Commit_Unit.CommitROBIdx_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | RD1_Value_o | 64 | SB0.RD1_Value_i, SB1.RD1_Value_i, LB0.RD1_Value_i, LB1.RD1_Value_i, LB2.RD1_Value_i, RS_IntALU0.RD1_Value_i, RS_IntALU1.RD1_Value_i, RS_FPALU0.RD1_Value_i, RS_IntMUL0.RD1_Value_i, RS_FPMUL0.RD1_Value_i |
| OUT | RD1_Qi_o | 5 | SB0.RD1_Qi_i, SB1.RD1_Qi_i, LB0.RD1_Qi_i, LB1.RD1_Qi_i, LB2.RD1_Qi_i, RS_IntALU0.RD1_Qi_i, RS_IntALU1.RD1_Qi_i, RS_FPALU0.RD1_Qi_i, RS_IntMUL0.RD1_Qi_i, RS_FPMUL0.RD1_Qi_i, ROB.RD1_Qi_i |
| OUT | RD1_QiValid_o | 1 | SB0.RD1_QiValid_i, SB1.RD1_QiValid_i, LB0.RD1_QiValid_i, LB1.RD1_QiValid_i, LB2.RD1_QiValid_i, RS_IntALU0.RD1_QiValid_i, RS_IntALU1.RD1_QiValid_i, RS_FPALU0.RD1_QiValid_i, RS_IntMUL0.RD1_QiValid_i, RS_FPMUL0.RD1_QiValid_i |
| OUT | RD2_Value_o | 64 | SB0.RD2_Value_i, SB1.RD2_Value_i, RS_IntALU0.RD2_Value_i, RS_IntALU1.RD2_Value_i, RS_FPALU0.RD2_Value_i, RS_IntMUL0.RD2_Value_i, RS_FPMUL0.RD2_Value_i |
| OUT | RD2_Qi_o | 5 | SB0.RD2_Qi_i, SB1.RD2_Qi_i, RS_IntALU0.RD2_Qi_i, RS_IntALU1.RD2_Qi_i, RS_FPALU0.RD2_Qi_i, RS_IntMUL0.RD2_Qi_i, RS_FPMUL0.RD2_Qi_i, ROB.RD2_Qi_i |
| OUT | RD2_QiValid_o | 1 | SB0.RD2_QiValid_i, SB1.RD2_QiValid_i, RS_IntALU0.RD2_QiValid_i, RS_IntALU1.RD2_QiValid_i, RS_FPALU0.RD2_QiValid_i, RS_IntMUL0.RD2_QiValid_i, RS_FPMUL0.RD2_QiValid_i |
| OUT | UPPER_o | 64 | AGU_0.UPPER_i, AGU_1.UPPER_i |
| OUT | LOWER_o | 64 | AGU_0.LOWER_i, AGU_1.LOWER_i |

---
## COMPONENT 10: ROB

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | ROBAlloc_i | 1 | Control_Unit.ROBAlloc_o |
| IN | AllocType_i | 3 | Control_Unit.AllocType_o |
| IN | Dest_in_i | 4 | Instruction_Decoder.Dest_in_o |
| IN | PCCurrent_i | 64 | PC_C.PCCurrent_o |
| IN | AllocPredicted_i | 1 | Control_Unit.AllocPredicted_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | IsCMPonly_i | 1 | Control_Unit.IsCMPonly_o |
| IN | AllocSourceStation_i | 4 | Control_Unit.AllocSourceStation_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD2_Qi_i | 5 | Register_File.RD2_Qi_o |
| IN | FlagsQi_i | 5 | Flags_Unit.FlagsQi_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBA_Exception_i | 4 | CDB_A.CDBA_Exception_o |
| IN | CDBA_Flags_i | 4 | CDB_A.CDBA_Flags_o |
| IN | CDBA_FlagsValid_i | 1 | CDB_A.CDBA_FlagsValid_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | CDBB_Exception_i | 4 | CDB_B.CDBB_Exception_o |
| IN | CDBB_Flags_i | 4 | CDB_B.CDBB_Flags_o |
| IN | CDBB_FlagsValid_i | 1 | CDB_B.CDBB_FlagsValid_o |
| IN | BrExResult_Valid_i | 1 | Branch_Executor.BrExResult_Valid_o |
| IN | BrExResult_ROBTag_i | 5 | Branch_Executor.BrExResult_ROBTag_o |
| IN | BrExResult_Taken_i | 1 | Branch_Executor.BrExResult_Taken_o |
| IN | BrExResult_Target_i | 64 | Branch_Executor.BrExResult_Target_o |
| IN | BrExResult_Mispredict_i | 1 | Branch_Executor.BrExResult_Mispredict_o |
| IN | SB0_StComplete_Valid_i | 1 | SB0.SB0_StComplete_Valid_o |
| IN | SB0_StComplete_ROBTag_i | 5 | SB0.SB0_StComplete_ROBTag_o |
| IN | SB0_StComplete_Addr_i | 64 | SB0.SB0_StComplete_Addr_o |
| IN | SB0_StComplete_Data_i | 64 | SB0.SB0_StComplete_Data_o |
| IN | SB1_StComplete_Valid_i | 1 | SB1.SB1_StComplete_Valid_o |
| IN | SB1_StComplete_ROBTag_i | 5 | SB1.SB1_StComplete_ROBTag_o |
| IN | SB1_StComplete_Addr_i | 64 | SB1.SB1_StComplete_Addr_o |
| IN | SB1_StComplete_Data_i | 64 | SB1.SB1_StComplete_Data_o |
| IN | CommitPop_i | 1 | Commit_Unit.CommitPop_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | ROBTail_o | 5 | Control_Unit.ROBTail_i, Register_File.ROBTail_i, Flags_Unit.ROBTail_i, SB0.ROBTail_i, SB1.ROBTail_i, LB0.ROBTail_i, LB1.ROBTail_i, LB2.ROBTail_i, RS_IntALU0.ROBTail_i, RS_IntALU1.ROBTail_i, RS_FPALU0.ROBTail_i, RS_IntMUL0.ROBTail_i, RS_FPMUL0.ROBTail_i, RS_Branch0.ROBTail_i |
| OUT | ROBFull_o | 1 | Control_Unit.ROBFull_i |
| OUT | ROBReadValue1_o | 64 | SB0.ROBReadValue1_i, SB1.ROBReadValue1_i, LB0.ROBReadValue1_i, LB1.ROBReadValue1_i, LB2.ROBReadValue1_i, RS_IntALU0.ROBReadValue1_i, RS_IntALU1.ROBReadValue1_i, RS_FPALU0.ROBReadValue1_i, RS_IntMUL0.ROBReadValue1_i, RS_FPMUL0.ROBReadValue1_i |
| OUT | ROBReadReady1_o | 1 | SB0.ROBReadReady1_i, SB1.ROBReadReady1_i, LB0.ROBReadReady1_i, LB1.ROBReadReady1_i, LB2.ROBReadReady1_i, RS_IntALU0.ROBReadReady1_i, RS_IntALU1.ROBReadReady1_i, RS_FPALU0.ROBReadReady1_i, RS_IntMUL0.ROBReadReady1_i, RS_FPMUL0.ROBReadReady1_i |
| OUT | ROBReadValue2_o | 64 | SB0.ROBReadValue2_i, SB1.ROBReadValue2_i, RS_IntALU0.ROBReadValue2_i, RS_IntALU1.ROBReadValue2_i, RS_FPALU0.ROBReadValue2_i, RS_IntMUL0.ROBReadValue2_i, RS_FPMUL0.ROBReadValue2_i |
| OUT | ROBReadReady2_o | 1 | SB0.ROBReadReady2_i, SB1.ROBReadReady2_i, RS_IntALU0.ROBReadReady2_i, RS_IntALU1.ROBReadReady2_i, RS_FPALU0.ROBReadReady2_i, RS_IntMUL0.ROBReadReady2_i, RS_FPMUL0.ROBReadReady2_i |
| OUT | ROBReadFlagsReady_o | 1 | RS_IntALU0.ROBReadFlagsReady_i, RS_IntALU1.ROBReadFlagsReady_i, RS_FPALU0.ROBReadFlagsReady_i, RS_IntMUL0.ROBReadFlagsReady_i, RS_FPMUL0.ROBReadFlagsReady_i, RS_Branch0.ROBReadFlagsReady_i |
| OUT | ROBReadFlagsResult_o | 4 | RS_IntALU0.ROBReadFlagsResult_i, RS_IntALU1.ROBReadFlagsResult_i, RS_FPALU0.ROBReadFlagsResult_i, RS_IntMUL0.ROBReadFlagsResult_i, RS_FPMUL0.ROBReadFlagsResult_i, RS_Branch0.ROBReadFlagsResult_i |
| OUT | ROBHead_o | 5 | Commit_Unit.ROBHead_i |
| OUT | ROBHeadBusy_o | 1 | Commit_Unit.ROBHeadBusy_i |
| OUT | ROBHeadReady_o | 1 | Commit_Unit.ROBHeadReady_i |
| OUT | ROBHeadType_o | 3 | Commit_Unit.ROBHeadType_i |
| OUT | ROBHeadDestReg_o | 4 | Commit_Unit.ROBHeadDestReg_i |
| OUT | ROBHeadValue_o | 64 | Commit_Unit.ROBHeadValue_i |
| OUT | ROBHeadException_o | 4 | Commit_Unit.ROBHeadException_i |
| OUT | ROBHeadPC_o | 64 | Commit_Unit.ROBHeadPC_i |
| OUT | ROBHeadFlagsResult_o | 4 | Commit_Unit.ROBHeadFlagsResult_i |
| OUT | ROBHeadFlagsValid_o | 1 | Commit_Unit.ROBHeadFlagsValid_i |
| OUT | ROBHeadMispredict_o | 1 | Commit_Unit.ROBHeadMispredict_i |
| OUT | ROBHeadBranchTarget_o | 64 | Commit_Unit.ROBHeadBranchTarget_i |
| OUT | ROBHeadStoreAddr_o | 64 | Commit_Unit.ROBHeadStoreAddr_i |
| OUT | ROBHeadStoreData_o | 64 | Commit_Unit.ROBHeadStoreData_i |
| OUT | ROBHeadStoreReady_o | 1 | Commit_Unit.ROBHeadStoreReady_i |
| OUT | ROBHeadSourceStation_o | 4 | Commit_Unit.ROBHeadSourceStation_i |

---
## COMPONENT 11: Flags_Unit

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | FlagsTagWrEn_i | 1 | Control_Unit.FlagsTagWrEn_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | FlagsCommitEn_i | 1 | Commit_Unit.FlagsCommitEn_o |
| IN | FlagsCommitValue_i | 4 | Commit_Unit.FlagsCommitValue_o |
| IN | CommitROBIdx_i | 5 | Commit_Unit.CommitROBIdx_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_FlagsValid_i | 1 | CDB_A.CDBA_FlagsValid_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_FlagsValid_i | 1 | CDB_B.CDBB_FlagsValid_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | FlagsArch_o | 4 | RS_IntALU0.FlagsArch_i, RS_IntALU1.FlagsArch_i, RS_FPALU0.FlagsArch_i, RS_IntMUL0.FlagsArch_i, RS_FPMUL0.FlagsArch_i, RS_Branch0.FlagsArch_i |
| OUT | FlagsQi_o | 5 | ROB.FlagsQi_i, RS_IntALU0.FlagsQi_i, RS_IntALU1.FlagsQi_i, RS_FPALU0.FlagsQi_i, RS_IntMUL0.FlagsQi_i, RS_FPMUL0.FlagsQi_i, RS_Branch0.FlagsQi_i |
| OUT | FlagsQiValid_o | 1 | RS_IntALU0.FlagsQiValid_i, RS_IntALU1.FlagsQiValid_i, RS_FPALU0.FlagsQiValid_i, RS_IntMUL0.FlagsQiValid_i, RS_FPMUL0.FlagsQiValid_i, RS_Branch0.FlagsQiValid_i |

---
## COMPONENT 12: SB0

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocSB0_i | 1 | Control_Unit.AllocSB0_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | RD1_Value_i | 64 | Register_File.RD1_Value_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD1_QiValid_i | 1 | Register_File.RD1_QiValid_o |
| IN | RD2_Value_i | 64 | Register_File.RD2_Value_o |
| IN | RD2_Qi_i | 5 | Register_File.RD2_Qi_o |
| IN | RD2_QiValid_i | 1 | Register_File.RD2_QiValid_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | ROBReadReady1_i | 1 | ROB.ROBReadReady1_o |
| IN | ROBReadValue1_i | 64 | ROB.ROBReadValue1_o |
| IN | ROBReadReady2_i | 1 | ROB.ROBReadReady2_o |
| IN | ROBReadValue2_i | 64 | ROB.ROBReadValue2_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | SB0_AGUDone_i | 1 | AGU_Arbiter.SB0_AGUDone_o |
| IN | SB0_AGUAddress_i | 64 | AGU_Arbiter.SB0_AGUAddress_o |
| IN | SB0_AGUSegFault_i | 1 | AGU_Arbiter.SB0_AGUSegFault_o |
| IN | FreeSB0_i | 1 | Commit_Unit.FreeSB0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | SB0_Busy_o | 1 | Control_Unit.SB0_Busy_i |
| OUT | SB0_AGUReq_o | 1 | AGU_Arbiter.SB0_AGUReq_i |
| OUT | SB0_AGUBase_o | 64 | AGU_Arbiter.SB0_AGUBase_i |
| OUT | SB0_AGUOffset_o | 64 | AGU_Arbiter.SB0_AGUOffset_i |
| OUT | SB0_AGUROBTag_o | 5 | AGU_Arbiter.SB0_AGUROBTag_i |
| OUT | SB0_StComplete_Valid_o | 1 | ROB.SB0_StComplete_Valid_i |
| OUT | SB0_StComplete_ROBTag_o | 5 | ROB.SB0_StComplete_ROBTag_i |
| OUT | SB0_StComplete_Addr_o | 64 | ROB.SB0_StComplete_Addr_i |
| OUT | SB0_StComplete_Data_o | 64 | ROB.SB0_StComplete_Data_i |

---
## COMPONENT 13: SB1

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocSB1_i | 1 | Control_Unit.AllocSB1_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | RD1_Value_i | 64 | Register_File.RD1_Value_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD1_QiValid_i | 1 | Register_File.RD1_QiValid_o |
| IN | RD2_Value_i | 64 | Register_File.RD2_Value_o |
| IN | RD2_Qi_i | 5 | Register_File.RD2_Qi_o |
| IN | RD2_QiValid_i | 1 | Register_File.RD2_QiValid_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | ROBReadReady1_i | 1 | ROB.ROBReadReady1_o |
| IN | ROBReadValue1_i | 64 | ROB.ROBReadValue1_o |
| IN | ROBReadReady2_i | 1 | ROB.ROBReadReady2_o |
| IN | ROBReadValue2_i | 64 | ROB.ROBReadValue2_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | SB1_AGUDone_i | 1 | AGU_Arbiter.SB1_AGUDone_o |
| IN | SB1_AGUAddress_i | 64 | AGU_Arbiter.SB1_AGUAddress_o |
| IN | SB1_AGUSegFault_i | 1 | AGU_Arbiter.SB1_AGUSegFault_o |
| IN | FreeSB1_i | 1 | Commit_Unit.FreeSB1_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | SB1_Busy_o | 1 | Control_Unit.SB1_Busy_i |
| OUT | SB1_AGUReq_o | 1 | AGU_Arbiter.SB1_AGUReq_i |
| OUT | SB1_AGUBase_o | 64 | AGU_Arbiter.SB1_AGUBase_i |
| OUT | SB1_AGUOffset_o | 64 | AGU_Arbiter.SB1_AGUOffset_i |
| OUT | SB1_AGUROBTag_o | 5 | AGU_Arbiter.SB1_AGUROBTag_i |
| OUT | SB1_StComplete_Valid_o | 1 | ROB.SB1_StComplete_Valid_i |
| OUT | SB1_StComplete_ROBTag_o | 5 | ROB.SB1_StComplete_ROBTag_i |
| OUT | SB1_StComplete_Addr_o | 64 | ROB.SB1_StComplete_Addr_i |
| OUT | SB1_StComplete_Data_o | 64 | ROB.SB1_StComplete_Data_i |

---
## COMPONENT 14: LB0

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocLB0_i | 1 | Control_Unit.AllocLB0_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | RD1_Value_i | 64 | Register_File.RD1_Value_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD1_QiValid_i | 1 | Register_File.RD1_QiValid_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | ROBReadReady1_i | 1 | ROB.ROBReadReady1_o |
| IN | ROBReadValue1_i | 64 | ROB.ROBReadValue1_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | LB0_AGUDone_i | 1 | AGU_Arbiter.LB0_AGUDone_o |
| IN | LB0_AGUAddress_i | 64 | AGU_Arbiter.LB0_AGUAddress_o |
| IN | LB0_AGUSegFault_i | 1 | AGU_Arbiter.LB0_AGUSegFault_o |
| IN | LB0_MemDone_i | 1 | Memory_Arbiter.LB0_MemDone_o |
| IN | LB0_MemData_i | 64 | Memory_Arbiter.LB0_MemData_o |
| IN | LB0_CDBStall_i | 1 | CDB_Arbiter.LB0_CDBStall_o |
| IN | FreeLB0_i | 1 | Commit_Unit.FreeLB0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | LB0_Busy_o | 1 | Control_Unit.LB0_Busy_i |
| OUT | LB0_AGUReq_o | 1 | AGU_Arbiter.LB0_AGUReq_i |
| OUT | LB0_AGUBase_o | 64 | AGU_Arbiter.LB0_AGUBase_i |
| OUT | LB0_AGUOffset_o | 64 | AGU_Arbiter.LB0_AGUOffset_i |
| OUT | LB0_AGUROBTag_o | 5 | AGU_Arbiter.LB0_AGUROBTag_i |
| OUT | LB0_MemReq_o | 1 | Memory_Arbiter.LB0_MemReq_i |
| OUT | LB0_MemAddr_o | 64 | Memory_Arbiter.LB0_MemAddr_i |
| OUT | LB0_MemOp_o | 8 | Memory_Arbiter.LB0_MemOp_i |
| OUT | LB0_MemROBTag_o | 5 | Memory_Arbiter.LB0_MemROBTag_i |
| OUT | LB0_CDBReq_o | 1 | CDB_Arbiter.LB0_CDBReq_i |
| OUT | LB0_CDBROBTag_o | 5 | CDB_Arbiter.LB0_CDBROBTag_i |
| OUT | LB0_CDBValue_o | 64 | CDB_Arbiter.LB0_CDBValue_i |
| OUT | LB0_CDBException_o | 4 | CDB_Arbiter.LB0_CDBException_i |

---
## COMPONENT 15: LB1

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocLB1_i | 1 | Control_Unit.AllocLB1_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | RD1_Value_i | 64 | Register_File.RD1_Value_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD1_QiValid_i | 1 | Register_File.RD1_QiValid_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | ROBReadReady1_i | 1 | ROB.ROBReadReady1_o |
| IN | ROBReadValue1_i | 64 | ROB.ROBReadValue1_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | LB1_AGUDone_i | 1 | AGU_Arbiter.LB1_AGUDone_o |
| IN | LB1_AGUAddress_i | 64 | AGU_Arbiter.LB1_AGUAddress_o |
| IN | LB1_AGUSegFault_i | 1 | AGU_Arbiter.LB1_AGUSegFault_o |
| IN | LB1_MemDone_i | 1 | Memory_Arbiter.LB1_MemDone_o |
| IN | LB1_MemData_i | 64 | Memory_Arbiter.LB1_MemData_o |
| IN | LB1_CDBStall_i | 1 | CDB_Arbiter.LB1_CDBStall_o |
| IN | FreeLB1_i | 1 | Commit_Unit.FreeLB1_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | LB1_Busy_o | 1 | Control_Unit.LB1_Busy_i |
| OUT | LB1_AGUReq_o | 1 | AGU_Arbiter.LB1_AGUReq_i |
| OUT | LB1_AGUBase_o | 64 | AGU_Arbiter.LB1_AGUBase_i |
| OUT | LB1_AGUOffset_o | 64 | AGU_Arbiter.LB1_AGUOffset_i |
| OUT | LB1_AGUROBTag_o | 5 | AGU_Arbiter.LB1_AGUROBTag_i |
| OUT | LB1_MemReq_o | 1 | Memory_Arbiter.LB1_MemReq_i |
| OUT | LB1_MemAddr_o | 64 | Memory_Arbiter.LB1_MemAddr_i |
| OUT | LB1_MemOp_o | 8 | Memory_Arbiter.LB1_MemOp_i |
| OUT | LB1_MemROBTag_o | 5 | Memory_Arbiter.LB1_MemROBTag_i |
| OUT | LB1_CDBReq_o | 1 | CDB_Arbiter.LB1_CDBReq_i |
| OUT | LB1_CDBROBTag_o | 5 | CDB_Arbiter.LB1_CDBROBTag_i |
| OUT | LB1_CDBValue_o | 64 | CDB_Arbiter.LB1_CDBValue_i |
| OUT | LB1_CDBException_o | 4 | CDB_Arbiter.LB1_CDBException_i |

---
## COMPONENT 16: LB2

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocLB2_i | 1 | Control_Unit.AllocLB2_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | RD1_Value_i | 64 | Register_File.RD1_Value_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD1_QiValid_i | 1 | Register_File.RD1_QiValid_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | ROBReadReady1_i | 1 | ROB.ROBReadReady1_o |
| IN | ROBReadValue1_i | 64 | ROB.ROBReadValue1_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | LB2_AGUDone_i | 1 | AGU_Arbiter.LB2_AGUDone_o |
| IN | LB2_AGUAddress_i | 64 | AGU_Arbiter.LB2_AGUAddress_o |
| IN | LB2_AGUSegFault_i | 1 | AGU_Arbiter.LB2_AGUSegFault_o |
| IN | LB2_MemDone_i | 1 | Memory_Arbiter.LB2_MemDone_o |
| IN | LB2_MemData_i | 64 | Memory_Arbiter.LB2_MemData_o |
| IN | LB2_CDBStall_i | 1 | CDB_Arbiter.LB2_CDBStall_o |
| IN | FreeLB2_i | 1 | Commit_Unit.FreeLB2_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | LB2_Busy_o | 1 | Control_Unit.LB2_Busy_i |
| OUT | LB2_AGUReq_o | 1 | AGU_Arbiter.LB2_AGUReq_i |
| OUT | LB2_AGUBase_o | 64 | AGU_Arbiter.LB2_AGUBase_i |
| OUT | LB2_AGUOffset_o | 64 | AGU_Arbiter.LB2_AGUOffset_i |
| OUT | LB2_AGUROBTag_o | 5 | AGU_Arbiter.LB2_AGUROBTag_i |
| OUT | LB2_MemReq_o | 1 | Memory_Arbiter.LB2_MemReq_i |
| OUT | LB2_MemAddr_o | 64 | Memory_Arbiter.LB2_MemAddr_i |
| OUT | LB2_MemOp_o | 8 | Memory_Arbiter.LB2_MemOp_i |
| OUT | LB2_MemROBTag_o | 5 | Memory_Arbiter.LB2_MemROBTag_i |
| OUT | LB2_CDBReq_o | 1 | CDB_Arbiter.LB2_CDBReq_i |
| OUT | LB2_CDBROBTag_o | 5 | CDB_Arbiter.LB2_CDBROBTag_i |
| OUT | LB2_CDBValue_o | 64 | CDB_Arbiter.LB2_CDBValue_i |
| OUT | LB2_CDBException_o | 4 | CDB_Arbiter.LB2_CDBException_i |

---
## COMPONENT 17: RS_IntALU0

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocRSIntALU0_i | 1 | Control_Unit.AllocRSIntALU0_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | RD1_Value_i | 64 | Register_File.RD1_Value_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD1_QiValid_i | 1 | Register_File.RD1_QiValid_o |
| IN | RD2_Value_i | 64 | Register_File.RD2_Value_o |
| IN | RD2_Qi_i | 5 | Register_File.RD2_Qi_o |
| IN | RD2_QiValid_i | 1 | Register_File.RD2_QiValid_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | UseImm_i | 1 | Control_Unit.UseImm_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | NeedsFlags_i | 1 | Control_Unit.NeedsFlags_o |
| IN | FlagsArch_i | 4 | Flags_Unit.FlagsArch_o |
| IN | FlagsQi_i | 5 | Flags_Unit.FlagsQi_o |
| IN | FlagsQiValid_i | 1 | Flags_Unit.FlagsQiValid_o |
| IN | ROBReadReady1_i | 1 | ROB.ROBReadReady1_o |
| IN | ROBReadValue1_i | 64 | ROB.ROBReadValue1_o |
| IN | ROBReadReady2_i | 1 | ROB.ROBReadReady2_o |
| IN | ROBReadValue2_i | 64 | ROB.ROBReadValue2_o |
| IN | ROBReadFlagsReady_i | 1 | ROB.ROBReadFlagsReady_o |
| IN | ROBReadFlagsResult_i | 4 | ROB.ROBReadFlagsResult_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBA_Flags_i | 4 | CDB_A.CDBA_Flags_o |
| IN | CDBA_FlagsValid_i | 1 | CDB_A.CDBA_FlagsValid_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | CDBB_Flags_i | 4 | CDB_B.CDBB_Flags_o |
| IN | CDBB_FlagsValid_i | 1 | CDB_B.CDBB_FlagsValid_o |
| IN | RSIAU0_Grant_i | 1 | IntALU_Arbiter.RSIAU0_Grant_o |
| IN | FreeRSIAU0_i | 1 | Commit_Unit.FreeRSIAU0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | RS_IntALU0_Busy_o | 1 | Control_Unit.RS_IntALU0_Busy_i |
| OUT | RSIAU0_Req_o | 1 | IntALU_Arbiter.RSIAU0_Req_i |
| OUT | RSIAU0_Op_o | 8 | IntALU_Arbiter.RSIAU0_Op_i |
| OUT | RSIAU0_Vj_o | 64 | IntALU_Arbiter.RSIAU0_Vj_i |
| OUT | RSIAU0_Vk_o | 64 | IntALU_Arbiter.RSIAU0_Vk_i |
| OUT | RSIAU0_DestROBTag_o | 5 | IntALU_Arbiter.RSIAU0_DestROBTag_i |
| OUT | RSIAU0_ModFlags_o | 1 | IntALU_Arbiter.RSIAU0_ModFlags_i |
| OUT | RSIAU0_NeedsFlags_o | 1 | IntALU_Arbiter.RSIAU0_NeedsFlags_i |
| OUT | RSIAU0_FlagsValue_o | 4 | IntALU_Arbiter.RSIAU0_FlagsValue_i |

---
## COMPONENT 18: RS_IntALU1

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocRSIntALU1_i | 1 | Control_Unit.AllocRSIntALU1_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | RD1_Value_i | 64 | Register_File.RD1_Value_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD1_QiValid_i | 1 | Register_File.RD1_QiValid_o |
| IN | RD2_Value_i | 64 | Register_File.RD2_Value_o |
| IN | RD2_Qi_i | 5 | Register_File.RD2_Qi_o |
| IN | RD2_QiValid_i | 1 | Register_File.RD2_QiValid_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | UseImm_i | 1 | Control_Unit.UseImm_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | NeedsFlags_i | 1 | Control_Unit.NeedsFlags_o |
| IN | FlagsArch_i | 4 | Flags_Unit.FlagsArch_o |
| IN | FlagsQi_i | 5 | Flags_Unit.FlagsQi_o |
| IN | FlagsQiValid_i | 1 | Flags_Unit.FlagsQiValid_o |
| IN | ROBReadReady1_i | 1 | ROB.ROBReadReady1_o |
| IN | ROBReadValue1_i | 64 | ROB.ROBReadValue1_o |
| IN | ROBReadReady2_i | 1 | ROB.ROBReadReady2_o |
| IN | ROBReadValue2_i | 64 | ROB.ROBReadValue2_o |
| IN | ROBReadFlagsReady_i | 1 | ROB.ROBReadFlagsReady_o |
| IN | ROBReadFlagsResult_i | 4 | ROB.ROBReadFlagsResult_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBA_Flags_i | 4 | CDB_A.CDBA_Flags_o |
| IN | CDBA_FlagsValid_i | 1 | CDB_A.CDBA_FlagsValid_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | CDBB_Flags_i | 4 | CDB_B.CDBB_Flags_o |
| IN | CDBB_FlagsValid_i | 1 | CDB_B.CDBB_FlagsValid_o |
| IN | RSIAU1_Grant_i | 1 | IntALU_Arbiter.RSIAU1_Grant_o |
| IN | FreeRSIAU1_i | 1 | Commit_Unit.FreeRSIAU1_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | RS_IntALU1_Busy_o | 1 | Control_Unit.RS_IntALU1_Busy_i |
| OUT | RSIAU1_Req_o | 1 | IntALU_Arbiter.RSIAU1_Req_i |
| OUT | RSIAU1_Op_o | 8 | IntALU_Arbiter.RSIAU1_Op_i |
| OUT | RSIAU1_Vj_o | 64 | IntALU_Arbiter.RSIAU1_Vj_i |
| OUT | RSIAU1_Vk_o | 64 | IntALU_Arbiter.RSIAU1_Vk_i |
| OUT | RSIAU1_DestROBTag_o | 5 | IntALU_Arbiter.RSIAU1_DestROBTag_i |
| OUT | RSIAU1_ModFlags_o | 1 | IntALU_Arbiter.RSIAU1_ModFlags_i |
| OUT | RSIAU1_NeedsFlags_o | 1 | IntALU_Arbiter.RSIAU1_NeedsFlags_i |
| OUT | RSIAU1_FlagsValue_o | 4 | IntALU_Arbiter.RSIAU1_FlagsValue_i |

---
## COMPONENT 19: RS_FPALU0

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocRSFPALU0_i | 1 | Control_Unit.AllocRSFPALU0_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | RD1_Value_i | 64 | Register_File.RD1_Value_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD1_QiValid_i | 1 | Register_File.RD1_QiValid_o |
| IN | RD2_Value_i | 64 | Register_File.RD2_Value_o |
| IN | RD2_Qi_i | 5 | Register_File.RD2_Qi_o |
| IN | RD2_QiValid_i | 1 | Register_File.RD2_QiValid_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | UseImm_i | 1 | Control_Unit.UseImm_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | NeedsFlags_i | 1 | Control_Unit.NeedsFlags_o |
| IN | FlagsArch_i | 4 | Flags_Unit.FlagsArch_o |
| IN | FlagsQi_i | 5 | Flags_Unit.FlagsQi_o |
| IN | FlagsQiValid_i | 1 | Flags_Unit.FlagsQiValid_o |
| IN | ROBReadReady1_i | 1 | ROB.ROBReadReady1_o |
| IN | ROBReadValue1_i | 64 | ROB.ROBReadValue1_o |
| IN | ROBReadReady2_i | 1 | ROB.ROBReadReady2_o |
| IN | ROBReadValue2_i | 64 | ROB.ROBReadValue2_o |
| IN | ROBReadFlagsReady_i | 1 | ROB.ROBReadFlagsReady_o |
| IN | ROBReadFlagsResult_i | 4 | ROB.ROBReadFlagsResult_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBA_Flags_i | 4 | CDB_A.CDBA_Flags_o |
| IN | CDBA_FlagsValid_i | 1 | CDB_A.CDBA_FlagsValid_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | CDBB_Flags_i | 4 | CDB_B.CDBB_Flags_o |
| IN | CDBB_FlagsValid_i | 1 | CDB_B.CDBB_FlagsValid_o |
| IN | RSFPAU0_Grant_i | 1 | FPALU_Arbiter.RSFPAU0_Grant_o |
| IN | FreeRSFPAU0_i | 1 | Commit_Unit.FreeRSFPAU0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | RS_FPALU0_Busy_o | 1 | Control_Unit.RS_FPALU0_Busy_i |
| OUT | RSFPAU0_Req_o | 1 | FPALU_Arbiter.RSFPAU0_Req_i |
| OUT | RSFPAU0_Op_o | 8 | FPALU_Arbiter.RSFPAU0_Op_i |
| OUT | RSFPAU0_Vj_o | 64 | FPALU_Arbiter.RSFPAU0_Vj_i |
| OUT | RSFPAU0_Vk_o | 64 | FPALU_Arbiter.RSFPAU0_Vk_i |
| OUT | RSFPAU0_DestROBTag_o | 5 | FPALU_Arbiter.RSFPAU0_DestROBTag_i |
| OUT | RSFPAU0_ModFlags_o | 1 | FPALU_Arbiter.RSFPAU0_ModFlags_i |
| OUT | RSFPAU0_NeedsFlags_o | 1 | FPALU_Arbiter.RSFPAU0_NeedsFlags_i |
| OUT | RSFPAU0_FlagsValue_o | 4 | FPALU_Arbiter.RSFPAU0_FlagsValue_i |

---
## COMPONENT 20: RS_IntMUL0 (internal bit: Executing)

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocRSIntMUL0_i | 1 | Control_Unit.AllocRSIntMUL0_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | RD1_Value_i | 64 | Register_File.RD1_Value_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD1_QiValid_i | 1 | Register_File.RD1_QiValid_o |
| IN | RD2_Value_i | 64 | Register_File.RD2_Value_o |
| IN | RD2_Qi_i | 5 | Register_File.RD2_Qi_o |
| IN | RD2_QiValid_i | 1 | Register_File.RD2_QiValid_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | UseImm_i | 1 | Control_Unit.UseImm_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | NeedsFlags_i | 1 | Control_Unit.NeedsFlags_o |
| IN | FlagsArch_i | 4 | Flags_Unit.FlagsArch_o |
| IN | FlagsQi_i | 5 | Flags_Unit.FlagsQi_o |
| IN | FlagsQiValid_i | 1 | Flags_Unit.FlagsQiValid_o |
| IN | ROBReadReady1_i | 1 | ROB.ROBReadReady1_o |
| IN | ROBReadValue1_i | 64 | ROB.ROBReadValue1_o |
| IN | ROBReadReady2_i | 1 | ROB.ROBReadReady2_o |
| IN | ROBReadValue2_i | 64 | ROB.ROBReadValue2_o |
| IN | ROBReadFlagsReady_i | 1 | ROB.ROBReadFlagsReady_o |
| IN | ROBReadFlagsResult_i | 4 | ROB.ROBReadFlagsResult_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBA_Flags_i | 4 | CDB_A.CDBA_Flags_o |
| IN | CDBA_FlagsValid_i | 1 | CDB_A.CDBA_FlagsValid_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | CDBB_Flags_i | 4 | CDB_B.CDBB_Flags_o |
| IN | CDBB_FlagsValid_i | 1 | CDB_B.CDBB_FlagsValid_o |
| IN | RSIMUL0_Grant_i | 1 | IntMUL_Arbiter.RSIMUL0_Grant_o |
| IN | FreeRSIMUL0_i | 1 | Commit_Unit.FreeRSIMUL0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | RS_IntMUL0_Busy_o | 1 | Control_Unit.RS_IntMUL0_Busy_i |
| OUT | RSIMUL0_Req_o | 1 | IntMUL_Arbiter.RSIMUL0_Req_i |
| OUT | RSIMUL0_Op_o | 8 | IntMUL_Arbiter.RSIMUL0_Op_i |
| OUT | RSIMUL0_Vj_o | 64 | IntMUL_Arbiter.RSIMUL0_Vj_i |
| OUT | RSIMUL0_Vk_o | 64 | IntMUL_Arbiter.RSIMUL0_Vk_i |
| OUT | RSIMUL0_DestROBTag_o | 5 | IntMUL_Arbiter.RSIMUL0_DestROBTag_i |
| OUT | RSIMUL0_ModFlags_o | 1 | IntMUL_Arbiter.RSIMUL0_ModFlags_i |
| OUT | RSIMUL0_NeedsFlags_o | 1 | IntMUL_Arbiter.RSIMUL0_NeedsFlags_i |
| OUT | RSIMUL0_FlagsValue_o | 4 | IntMUL_Arbiter.RSIMUL0_FlagsValue_i |

---
## COMPONENT 21: RS_FPMUL0 (internal bit: Executing)

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocRSFPMUL0_i | 1 | Control_Unit.AllocRSFPMUL0_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | RD1_Value_i | 64 | Register_File.RD1_Value_o |
| IN | RD1_Qi_i | 5 | Register_File.RD1_Qi_o |
| IN | RD1_QiValid_i | 1 | Register_File.RD1_QiValid_o |
| IN | RD2_Value_i | 64 | Register_File.RD2_Value_o |
| IN | RD2_Qi_i | 5 | Register_File.RD2_Qi_o |
| IN | RD2_QiValid_i | 1 | Register_File.RD2_QiValid_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | UseImm_i | 1 | Control_Unit.UseImm_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | NeedsFlags_i | 1 | Control_Unit.NeedsFlags_o |
| IN | FlagsArch_i | 4 | Flags_Unit.FlagsArch_o |
| IN | FlagsQi_i | 5 | Flags_Unit.FlagsQi_o |
| IN | FlagsQiValid_i | 1 | Flags_Unit.FlagsQiValid_o |
| IN | ROBReadReady1_i | 1 | ROB.ROBReadReady1_o |
| IN | ROBReadValue1_i | 64 | ROB.ROBReadValue1_o |
| IN | ROBReadReady2_i | 1 | ROB.ROBReadReady2_o |
| IN | ROBReadValue2_i | 64 | ROB.ROBReadValue2_o |
| IN | ROBReadFlagsReady_i | 1 | ROB.ROBReadFlagsReady_o |
| IN | ROBReadFlagsResult_i | 4 | ROB.ROBReadFlagsResult_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Value_i | 64 | CDB_A.CDBA_Value_o |
| IN | CDBA_Flags_i | 4 | CDB_A.CDBA_Flags_o |
| IN | CDBA_FlagsValid_i | 1 | CDB_A.CDBA_FlagsValid_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Value_i | 64 | CDB_B.CDBB_Value_o |
| IN | CDBB_Flags_i | 4 | CDB_B.CDBB_Flags_o |
| IN | CDBB_FlagsValid_i | 1 | CDB_B.CDBB_FlagsValid_o |
| IN | RSFPMUL0_Grant_i | 1 | FPMUL_Arbiter.RSFPMUL0_Grant_o |
| IN | FreeRSFPMUL0_i | 1 | Commit_Unit.FreeRSFPMUL0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | RS_FPMUL0_Busy_o | 1 | Control_Unit.RS_FPMUL0_Busy_i |
| OUT | RSFPMUL0_Req_o | 1 | FPMUL_Arbiter.RSFPMUL0_Req_i |
| OUT | RSFPMUL0_Op_o | 8 | FPMUL_Arbiter.RSFPMUL0_Op_i |
| OUT | RSFPMUL0_Vj_o | 64 | FPMUL_Arbiter.RSFPMUL0_Vj_i |
| OUT | RSFPMUL0_Vk_o | 64 | FPMUL_Arbiter.RSFPMUL0_Vk_i |
| OUT | RSFPMUL0_DestROBTag_o | 5 | FPMUL_Arbiter.RSFPMUL0_DestROBTag_i |
| OUT | RSFPMUL0_ModFlags_o | 1 | FPMUL_Arbiter.RSFPMUL0_ModFlags_i |
| OUT | RSFPMUL0_NeedsFlags_o | 1 | FPMUL_Arbiter.RSFPMUL0_NeedsFlags_i |
| OUT | RSFPMUL0_FlagsValue_o | 4 | FPMUL_Arbiter.RSFPMUL0_FlagsValue_i |

---
## COMPONENT 22: RS_Branch0

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | AllocRSBranch0_i | 1 | Control_Unit.AllocRSBranch0_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | PCCurrent_i | 64 | PC_C.PCCurrent_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | NeedsFlags_i | 1 | Control_Unit.NeedsFlags_o |
| IN | FlagsArch_i | 4 | Flags_Unit.FlagsArch_o |
| IN | FlagsQi_i | 5 | Flags_Unit.FlagsQi_o |
| IN | FlagsQiValid_i | 1 | Flags_Unit.FlagsQiValid_o |
| IN | ROBReadFlagsReady_i | 1 | ROB.ROBReadFlagsReady_o |
| IN | ROBReadFlagsResult_i | 4 | ROB.ROBReadFlagsResult_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Flags_i | 4 | CDB_A.CDBA_Flags_o |
| IN | CDBA_FlagsValid_i | 1 | CDB_A.CDBA_FlagsValid_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Flags_i | 4 | CDB_B.CDBB_Flags_o |
| IN | CDBB_FlagsValid_i | 1 | CDB_B.CDBB_FlagsValid_o |
| IN | RSBr0_Grant_i | 1 | Branch_Arbiter.RSBr0_Grant_o |
| IN | FreeRSBr0_i | 1 | Commit_Unit.FreeRSBr0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | RS_Branch0_Busy_o | 1 | Control_Unit.RS_Branch0_Busy_i |
| OUT | RSBr0_Req_o | 1 | Branch_Arbiter.RSBr0_Req_i |
| OUT | RSBr0_Op_o | 8 | Branch_Arbiter.RSBr0_Op_i |
| OUT | RSBr0_TargetPC_o | 64 | Branch_Arbiter.RSBr0_TargetPC_i |
| OUT | RSBr0_FlagsValue_o | 4 | Branch_Arbiter.RSBr0_FlagsValue_i |
| OUT | RSBr0_PredTaken_o | 1 | Branch_Arbiter.RSBr0_PredTaken_i |
| OUT | RSBr0_ROBTag_o | 5 | Branch_Arbiter.RSBr0_ROBTag_i |

---
## COMPONENT 23: AGU_Arbiter

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | SB0_AGUReq_i | 1 | SB0.SB0_AGUReq_o |
| IN | SB0_AGUBase_i | 64 | SB0.SB0_AGUBase_o |
| IN | SB0_AGUOffset_i | 64 | SB0.SB0_AGUOffset_o |
| IN | SB0_AGUROBTag_i | 5 | SB0.SB0_AGUROBTag_o |
| IN | SB1_AGUReq_i | 1 | SB1.SB1_AGUReq_o |
| IN | SB1_AGUBase_i | 64 | SB1.SB1_AGUBase_o |
| IN | SB1_AGUOffset_i | 64 | SB1.SB1_AGUOffset_o |
| IN | SB1_AGUROBTag_i | 5 | SB1.SB1_AGUROBTag_o |
| IN | LB0_AGUReq_i | 1 | LB0.LB0_AGUReq_o |
| IN | LB0_AGUBase_i | 64 | LB0.LB0_AGUBase_o |
| IN | LB0_AGUOffset_i | 64 | LB0.LB0_AGUOffset_o |
| IN | LB0_AGUROBTag_i | 5 | LB0.LB0_AGUROBTag_o |
| IN | LB1_AGUReq_i | 1 | LB1.LB1_AGUReq_o |
| IN | LB1_AGUBase_i | 64 | LB1.LB1_AGUBase_o |
| IN | LB1_AGUOffset_i | 64 | LB1.LB1_AGUOffset_o |
| IN | LB1_AGUROBTag_i | 5 | LB1.LB1_AGUROBTag_o |
| IN | LB2_AGUReq_i | 1 | LB2.LB2_AGUReq_o |
| IN | LB2_AGUBase_i | 64 | LB2.LB2_AGUBase_o |
| IN | LB2_AGUOffset_i | 64 | LB2.LB2_AGUOffset_o |
| IN | LB2_AGUROBTag_i | 5 | LB2.LB2_AGUROBTag_o |
| IN | AGU0_Free_i | 1 | AGU_0.AGU0_Free_o |
| IN | AGU1_Free_i | 1 | AGU_1.AGU1_Free_o |
| IN | AGU0_Done_i | 1 | AGU_0.AGU0_Done_o |
| IN | AGU0_Address_i | 64 | AGU_0.AGU0_Address_o |
| IN | AGU0_SegFault_i | 1 | AGU_0.AGU0_SegFault_o |
| IN | AGU0_SourceID_i | 3 | AGU_0.AGU0_SourceID_o |
| IN | AGU1_Done_i | 1 | AGU_1.AGU1_Done_o |
| IN | AGU1_Address_i | 64 | AGU_1.AGU1_Address_o |
| IN | AGU1_SegFault_i | 1 | AGU_1.AGU1_SegFault_o |
| IN | AGU1_SourceID_i | 3 | AGU_1.AGU1_SourceID_o |
| OUT | AGU0_Start_o | 1 | AGU_0.AGU0_Start_i |
| OUT | AGU0_Base_o | 64 | AGU_0.AGU0_Base_i |
| OUT | AGU0_Offset_o | 64 | AGU_0.AGU0_Offset_i |
| OUT | AGU0_SourceID_o | 3 | AGU_0.AGU0_SourceID_i |
| OUT | AGU1_Start_o | 1 | AGU_1.AGU1_Start_i |
| OUT | AGU1_Base_o | 64 | AGU_1.AGU1_Base_i |
| OUT | AGU1_Offset_o | 64 | AGU_1.AGU1_Offset_i |
| OUT | AGU1_SourceID_o | 3 | AGU_1.AGU1_SourceID_i |
| OUT | SB0_AGUDone_o | 1 | SB0.SB0_AGUDone_i |
| OUT | SB0_AGUAddress_o | 64 | SB0.SB0_AGUAddress_i |
| OUT | SB0_AGUSegFault_o | 1 | SB0.SB0_AGUSegFault_i |
| OUT | SB1_AGUDone_o | 1 | SB1.SB1_AGUDone_i |
| OUT | SB1_AGUAddress_o | 64 | SB1.SB1_AGUAddress_i |
| OUT | SB1_AGUSegFault_o | 1 | SB1.SB1_AGUSegFault_i |
| OUT | LB0_AGUDone_o | 1 | LB0.LB0_AGUDone_i |
| OUT | LB0_AGUAddress_o | 64 | LB0.LB0_AGUAddress_i |
| OUT | LB0_AGUSegFault_o | 1 | LB0.LB0_AGUSegFault_i |
| OUT | LB1_AGUDone_o | 1 | LB1.LB1_AGUDone_i |
| OUT | LB1_AGUAddress_o | 64 | LB1.LB1_AGUAddress_i |
| OUT | LB1_AGUSegFault_o | 1 | LB1.LB1_AGUSegFault_i |
| OUT | LB2_AGUDone_o | 1 | LB2.LB2_AGUDone_i |
| OUT | LB2_AGUAddress_o | 64 | LB2.LB2_AGUAddress_i |
| OUT | LB2_AGUSegFault_o | 1 | LB2.LB2_AGUSegFault_i |

---
## COMPONENT 24: AGU_0

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | AGU0_Start_i | 1 | AGU_Arbiter.AGU0_Start_o |
| IN | AGU0_Base_i | 64 | AGU_Arbiter.AGU0_Base_o |
| IN | AGU0_Offset_i | 64 | AGU_Arbiter.AGU0_Offset_o |
| IN | AGU0_SourceID_i | 3 | AGU_Arbiter.AGU0_SourceID_o |
| IN | UPPER_i | 64 | Register_File.UPPER_o |
| IN | LOWER_i | 64 | Register_File.LOWER_o |
| OUT | AGU0_Free_o | 1 | AGU_Arbiter.AGU0_Free_i |
| OUT | AGU0_Done_o | 1 | AGU_Arbiter.AGU0_Done_i |
| OUT | AGU0_Address_o | 64 | AGU_Arbiter.AGU0_Address_i |
| OUT | AGU0_SegFault_o | 1 | AGU_Arbiter.AGU0_SegFault_i |
| OUT | AGU0_SourceID_o | 3 | AGU_Arbiter.AGU0_SourceID_i |

---
## COMPONENT 25: AGU_1

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | AGU1_Start_i | 1 | AGU_Arbiter.AGU1_Start_o |
| IN | AGU1_Base_i | 64 | AGU_Arbiter.AGU1_Base_o |
| IN | AGU1_Offset_i | 64 | AGU_Arbiter.AGU1_Offset_o |
| IN | AGU1_SourceID_i | 3 | AGU_Arbiter.AGU1_SourceID_o |
| IN | UPPER_i | 64 | Register_File.UPPER_o |
| IN | LOWER_i | 64 | Register_File.LOWER_o |
| OUT | AGU1_Free_o | 1 | AGU_Arbiter.AGU1_Free_i |
| OUT | AGU1_Done_o | 1 | AGU_Arbiter.AGU1_Done_i |
| OUT | AGU1_Address_o | 64 | AGU_Arbiter.AGU1_Address_i |
| OUT | AGU1_SegFault_o | 1 | AGU_Arbiter.AGU1_SegFault_i |
| OUT | AGU1_SourceID_o | 3 | AGU_Arbiter.AGU1_SourceID_i |

---
## COMPONENT 26: IntALU_Arbiter

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | RSIAU0_Req_i | 1 | RS_IntALU0.RSIAU0_Req_o |
| IN | RSIAU0_Op_i | 8 | RS_IntALU0.RSIAU0_Op_o |
| IN | RSIAU0_Vj_i | 64 | RS_IntALU0.RSIAU0_Vj_o |
| IN | RSIAU0_Vk_i | 64 | RS_IntALU0.RSIAU0_Vk_o |
| IN | RSIAU0_DestROBTag_i | 5 | RS_IntALU0.RSIAU0_DestROBTag_o |
| IN | RSIAU0_ModFlags_i | 1 | RS_IntALU0.RSIAU0_ModFlags_o |
| IN | RSIAU0_NeedsFlags_i | 1 | RS_IntALU0.RSIAU0_NeedsFlags_o |
| IN | RSIAU0_FlagsValue_i | 4 | RS_IntALU0.RSIAU0_FlagsValue_o |
| IN | RSIAU1_Req_i | 1 | RS_IntALU1.RSIAU1_Req_o |
| IN | RSIAU1_Op_i | 8 | RS_IntALU1.RSIAU1_Op_o |
| IN | RSIAU1_Vj_i | 64 | RS_IntALU1.RSIAU1_Vj_o |
| IN | RSIAU1_Vk_i | 64 | RS_IntALU1.RSIAU1_Vk_o |
| IN | RSIAU1_DestROBTag_i | 5 | RS_IntALU1.RSIAU1_DestROBTag_o |
| IN | RSIAU1_ModFlags_i | 1 | RS_IntALU1.RSIAU1_ModFlags_o |
| IN | RSIAU1_NeedsFlags_i | 1 | RS_IntALU1.RSIAU1_NeedsFlags_o |
| IN | RSIAU1_FlagsValue_i | 4 | RS_IntALU1.RSIAU1_FlagsValue_o |
| IN | IntALU_Free_i | 1 | IntALU.IntALU_Free_o |
| OUT | RSIAU0_Grant_o | 1 | RS_IntALU0.RSIAU0_Grant_i |
| OUT | RSIAU1_Grant_o | 1 | RS_IntALU1.RSIAU1_Grant_i |
| OUT | IntALU_Start_o | 1 | IntALU.IntALU_Start_i |
| OUT | IntALU_Op_o | 8 | IntALU.IntALU_Op_i |
| OUT | IntALU_A_o | 64 | IntALU.IntALU_A_i |
| OUT | IntALU_B_o | 64 | IntALU.IntALU_B_i |
| OUT | IntALU_DestROBTag_o | 5 | IntALU.IntALU_DestROBTag_i |
| OUT | IntALU_ModFlags_o | 1 | IntALU.IntALU_ModFlags_i |
| OUT | IntALU_NeedsFlags_o | 1 | IntALU.IntALU_NeedsFlags_i |
| OUT | IntALU_FlagsIn_o | 4 | IntALU.IntALU_FlagsIn_i |

---
## COMPONENT 27: IntALU

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | IntALU_Start_i | 1 | IntALU_Arbiter.IntALU_Start_o |
| IN | IntALU_Op_i | 8 | IntALU_Arbiter.IntALU_Op_o |
| IN | IntALU_A_i | 64 | IntALU_Arbiter.IntALU_A_o |
| IN | IntALU_B_i | 64 | IntALU_Arbiter.IntALU_B_o |
| IN | IntALU_DestROBTag_i | 5 | IntALU_Arbiter.IntALU_DestROBTag_o |
| IN | IntALU_ModFlags_i | 1 | IntALU_Arbiter.IntALU_ModFlags_o |
| IN | IntALU_NeedsFlags_i | 1 | IntALU_Arbiter.IntALU_NeedsFlags_o |
| IN | IntALU_FlagsIn_i | 4 | IntALU_Arbiter.IntALU_FlagsIn_o |
| IN | IntALU_CDBStall_i | 1 | CDB_Arbiter.IntALU_CDBStall_o |
| OUT | IntALU_Free_o | 1 | IntALU_Arbiter.IntALU_Free_i |
| OUT | IntALU_CDBReq_o | 1 | CDB_Arbiter.IntALU_CDBReq_i |
| OUT | IntALU_CDBROBTag_o | 5 | CDB_Arbiter.IntALU_CDBROBTag_i |
| OUT | IntALU_CDBValue_o | 64 | CDB_Arbiter.IntALU_CDBValue_i |
| OUT | IntALU_CDBException_o | 4 | CDB_Arbiter.IntALU_CDBException_i |
| OUT | IntALU_CDBFlags_o | 4 | CDB_Arbiter.IntALU_CDBFlags_i |
| OUT | IntALU_CDBFlagsValid_o | 1 | CDB_Arbiter.IntALU_CDBFlagsValid_i |

---
## COMPONENT 28: FPALU_Arbiter

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | RSFPAU0_Req_i | 1 | RS_FPALU0.RSFPAU0_Req_o |
| IN | RSFPAU0_Op_i | 8 | RS_FPALU0.RSFPAU0_Op_o |
| IN | RSFPAU0_Vj_i | 64 | RS_FPALU0.RSFPAU0_Vj_o |
| IN | RSFPAU0_Vk_i | 64 | RS_FPALU0.RSFPAU0_Vk_o |
| IN | RSFPAU0_DestROBTag_i | 5 | RS_FPALU0.RSFPAU0_DestROBTag_o |
| IN | RSFPAU0_ModFlags_i | 1 | RS_FPALU0.RSFPAU0_ModFlags_o |
| IN | RSFPAU0_NeedsFlags_i | 1 | RS_FPALU0.RSFPAU0_NeedsFlags_o |
| IN | RSFPAU0_FlagsValue_i | 4 | RS_FPALU0.RSFPAU0_FlagsValue_o |
| IN | FPALU_Free_i | 1 | FPALU.FPALU_Free_o |
| OUT | RSFPAU0_Grant_o | 1 | RS_FPALU0.RSFPAU0_Grant_i |
| OUT | FPALU_Start_o | 1 | FPALU.FPALU_Start_i |
| OUT | FPALU_Op_o | 8 | FPALU.FPALU_Op_i |
| OUT | FPALU_A_o | 64 | FPALU.FPALU_A_i |
| OUT | FPALU_B_o | 64 | FPALU.FPALU_B_i |
| OUT | FPALU_DestROBTag_o | 5 | FPALU.FPALU_DestROBTag_i |
| OUT | FPALU_ModFlags_o | 1 | FPALU.FPALU_ModFlags_i |
| OUT | FPALU_NeedsFlags_o | 1 | FPALU.FPALU_NeedsFlags_i |
| OUT | FPALU_FlagsIn_o | 4 | FPALU.FPALU_FlagsIn_i |

---
## COMPONENT 29: FPALU

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | FPALU_Start_i | 1 | FPALU_Arbiter.FPALU_Start_o |
| IN | FPALU_Op_i | 8 | FPALU_Arbiter.FPALU_Op_o |
| IN | FPALU_A_i | 64 | FPALU_Arbiter.FPALU_A_o |
| IN | FPALU_B_i | 64 | FPALU_Arbiter.FPALU_B_o |
| IN | FPALU_DestROBTag_i | 5 | FPALU_Arbiter.FPALU_DestROBTag_o |
| IN | FPALU_ModFlags_i | 1 | FPALU_Arbiter.FPALU_ModFlags_o |
| IN | FPALU_NeedsFlags_i | 1 | FPALU_Arbiter.FPALU_NeedsFlags_o |
| IN | FPALU_FlagsIn_i | 4 | FPALU_Arbiter.FPALU_FlagsIn_o |
| IN | FPALU_CDBStall_i | 1 | CDB_Arbiter.FPALU_CDBStall_o |
| OUT | FPALU_Free_o | 1 | FPALU_Arbiter.FPALU_Free_i |
| OUT | FPALU_CDBReq_o | 1 | CDB_Arbiter.FPALU_CDBReq_i |
| OUT | FPALU_CDBROBTag_o | 5 | CDB_Arbiter.FPALU_CDBROBTag_i |
| OUT | FPALU_CDBValue_o | 64 | CDB_Arbiter.FPALU_CDBValue_i |
| OUT | FPALU_CDBException_o | 4 | CDB_Arbiter.FPALU_CDBException_i |
| OUT | FPALU_CDBFlags_o | 4 | CDB_Arbiter.FPALU_CDBFlags_i |
| OUT | FPALU_CDBFlagsValid_o | 1 | CDB_Arbiter.FPALU_CDBFlagsValid_i |

---
## COMPONENT 30: IntMUL_Arbiter

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | RSIMUL0_Req_i | 1 | RS_IntMUL0.RSIMUL0_Req_o |
| IN | RSIMUL0_Op_i | 8 | RS_IntMUL0.RSIMUL0_Op_o |
| IN | RSIMUL0_Vj_i | 64 | RS_IntMUL0.RSIMUL0_Vj_o |
| IN | RSIMUL0_Vk_i | 64 | RS_IntMUL0.RSIMUL0_Vk_o |
| IN | RSIMUL0_DestROBTag_i | 5 | RS_IntMUL0.RSIMUL0_DestROBTag_o |
| IN | RSIMUL0_ModFlags_i | 1 | RS_IntMUL0.RSIMUL0_ModFlags_o |
| IN | RSIMUL0_NeedsFlags_i | 1 | RS_IntMUL0.RSIMUL0_NeedsFlags_o |
| IN | RSIMUL0_FlagsValue_i | 4 | RS_IntMUL0.RSIMUL0_FlagsValue_o |
| IN | IntMUL_Free_i | 1 | IntMUL.IntMUL_Free_o |
| OUT | RSIMUL0_Grant_o | 1 | RS_IntMUL0.RSIMUL0_Grant_i |
| OUT | IntMUL_Start_o | 1 | IntMUL.IntMUL_Start_i |
| OUT | IntMUL_Op_o | 8 | IntMUL.IntMUL_Op_i |
| OUT | IntMUL_A_o | 64 | IntMUL.IntMUL_A_i |
| OUT | IntMUL_B_o | 64 | IntMUL.IntMUL_B_i |
| OUT | IntMUL_DestROBTag_o | 5 | IntMUL.IntMUL_DestROBTag_i |
| OUT | IntMUL_ModFlags_o | 1 | IntMUL.IntMUL_ModFlags_i |
| OUT | IntMUL_NeedsFlags_o | 1 | IntMUL.IntMUL_NeedsFlags_i |
| OUT | IntMUL_FlagsIn_o | 4 | IntMUL.IntMUL_FlagsIn_i |

---
## COMPONENT 31: IntMUL

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | IntMUL_Start_i | 1 | IntMUL_Arbiter.IntMUL_Start_o |
| IN | IntMUL_Op_i | 8 | IntMUL_Arbiter.IntMUL_Op_o |
| IN | IntMUL_A_i | 64 | IntMUL_Arbiter.IntMUL_A_o |
| IN | IntMUL_B_i | 64 | IntMUL_Arbiter.IntMUL_B_o |
| IN | IntMUL_DestROBTag_i | 5 | IntMUL_Arbiter.IntMUL_DestROBTag_o |
| IN | IntMUL_ModFlags_i | 1 | IntMUL_Arbiter.IntMUL_ModFlags_o |
| IN | IntMUL_NeedsFlags_i | 1 | IntMUL_Arbiter.IntMUL_NeedsFlags_o |
| IN | IntMUL_FlagsIn_i | 4 | IntMUL_Arbiter.IntMUL_FlagsIn_o |
| IN | IntMUL_CDBStall_i | 1 | CDB_Arbiter.IntMUL_CDBStall_o |
| OUT | IntMUL_Free_o | 1 | IntMUL_Arbiter.IntMUL_Free_i |
| OUT | IntMUL_CDBReq_o | 1 | CDB_Arbiter.IntMUL_CDBReq_i |
| OUT | IntMUL_CDBROBTag_o | 5 | CDB_Arbiter.IntMUL_CDBROBTag_i |
| OUT | IntMUL_CDBValue_o | 64 | CDB_Arbiter.IntMUL_CDBValue_i |
| OUT | IntMUL_CDBException_o | 4 | CDB_Arbiter.IntMUL_CDBException_i |
| OUT | IntMUL_CDBFlags_o | 4 | CDB_Arbiter.IntMUL_CDBFlags_i |
| OUT | IntMUL_CDBFlagsValid_o | 1 | CDB_Arbiter.IntMUL_CDBFlagsValid_i |

---
## COMPONENT 32: FPMUL_Arbiter

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | RSFPMUL0_Req_i | 1 | RS_FPMUL0.RSFPMUL0_Req_o |
| IN | RSFPMUL0_Op_i | 8 | RS_FPMUL0.RSFPMUL0_Op_o |
| IN | RSFPMUL0_Vj_i | 64 | RS_FPMUL0.RSFPMUL0_Vj_o |
| IN | RSFPMUL0_Vk_i | 64 | RS_FPMUL0.RSFPMUL0_Vk_o |
| IN | RSFPMUL0_DestROBTag_i | 5 | RS_FPMUL0.RSFPMUL0_DestROBTag_o |
| IN | RSFPMUL0_ModFlags_i | 1 | RS_FPMUL0.RSFPMUL0_ModFlags_o |
| IN | RSFPMUL0_NeedsFlags_i | 1 | RS_FPMUL0.RSFPMUL0_NeedsFlags_o |
| IN | RSFPMUL0_FlagsValue_i | 4 | RS_FPMUL0.RSFPMUL0_FlagsValue_o |
| IN | FPMUL_Free_i | 1 | FPMUL.FPMUL_Free_o |
| OUT | RSFPMUL0_Grant_o | 1 | RS_FPMUL0.RSFPMUL0_Grant_i |
| OUT | FPMUL_Start_o | 1 | FPMUL.FPMUL_Start_i |
| OUT | FPMUL_Op_o | 8 | FPMUL.FPMUL_Op_i |
| OUT | FPMUL_A_o | 64 | FPMUL.FPMUL_A_i |
| OUT | FPMUL_B_o | 64 | FPMUL.FPMUL_B_i |
| OUT | FPMUL_DestROBTag_o | 5 | FPMUL.FPMUL_DestROBTag_i |
| OUT | FPMUL_ModFlags_o | 1 | FPMUL.FPMUL_ModFlags_i |
| OUT | FPMUL_NeedsFlags_o | 1 | FPMUL.FPMUL_NeedsFlags_i |
| OUT | FPMUL_FlagsIn_o | 4 | FPMUL.FPMUL_FlagsIn_i |

---
## COMPONENT 33: FPMUL

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | FPMUL_Start_i | 1 | FPMUL_Arbiter.FPMUL_Start_o |
| IN | FPMUL_Op_i | 8 | FPMUL_Arbiter.FPMUL_Op_o |
| IN | FPMUL_A_i | 64 | FPMUL_Arbiter.FPMUL_A_o |
| IN | FPMUL_B_i | 64 | FPMUL_Arbiter.FPMUL_B_o |
| IN | FPMUL_DestROBTag_i | 5 | FPMUL_Arbiter.FPMUL_DestROBTag_o |
| IN | FPMUL_ModFlags_i | 1 | FPMUL_Arbiter.FPMUL_ModFlags_o |
| IN | FPMUL_NeedsFlags_i | 1 | FPMUL_Arbiter.FPMUL_NeedsFlags_o |
| IN | FPMUL_FlagsIn_i | 4 | FPMUL_Arbiter.FPMUL_FlagsIn_o |
| IN | FPMUL_CDBStall_i | 1 | CDB_Arbiter.FPMUL_CDBStall_o |
| OUT | FPMUL_Free_o | 1 | FPMUL_Arbiter.FPMUL_Free_i |
| OUT | FPMUL_CDBReq_o | 1 | CDB_Arbiter.FPMUL_CDBReq_i |
| OUT | FPMUL_CDBROBTag_o | 5 | CDB_Arbiter.FPMUL_CDBROBTag_i |
| OUT | FPMUL_CDBValue_o | 64 | CDB_Arbiter.FPMUL_CDBValue_i |
| OUT | FPMUL_CDBException_o | 4 | CDB_Arbiter.FPMUL_CDBException_i |
| OUT | FPMUL_CDBFlags_o | 4 | CDB_Arbiter.FPMUL_CDBFlags_i |
| OUT | FPMUL_CDBFlagsValid_o | 1 | CDB_Arbiter.FPMUL_CDBFlagsValid_i |

---
## COMPONENT 34: Branch_Arbiter

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | RSBr0_Req_i | 1 | RS_Branch0.RSBr0_Req_o |
| IN | RSBr0_Op_i | 8 | RS_Branch0.RSBr0_Op_o |
| IN | RSBr0_TargetPC_i | 64 | RS_Branch0.RSBr0_TargetPC_o |
| IN | RSBr0_FlagsValue_i | 4 | RS_Branch0.RSBr0_FlagsValue_o |
| IN | RSBr0_PredTaken_i | 1 | RS_Branch0.RSBr0_PredTaken_o |
| IN | RSBr0_ROBTag_i | 5 | RS_Branch0.RSBr0_ROBTag_o |
| OUT | RSBr0_Grant_o | 1 | RS_Branch0.RSBr0_Grant_i |
| OUT | BrEx_Start_o | 1 | Branch_Executor.BrEx_Start_i |
| OUT | BrEx_Op_o | 8 | Branch_Executor.BrEx_Op_i |
| OUT | BrEx_TargetPC_o | 64 | Branch_Executor.BrEx_TargetPC_i |
| OUT | BrEx_FlagsValue_o | 4 | Branch_Executor.BrEx_FlagsValue_i |
| OUT | BrEx_PredTaken_o | 1 | Branch_Executor.BrEx_PredTaken_i |
| OUT | BrEx_ROBTag_o | 5 | Branch_Executor.BrEx_ROBTag_i |

---
## COMPONENT 35: Branch_Executor

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | BrEx_Start_i | 1 | Branch_Arbiter.BrEx_Start_o |
| IN | BrEx_Op_i | 8 | Branch_Arbiter.BrEx_Op_o |
| IN | BrEx_TargetPC_i | 64 | Branch_Arbiter.BrEx_TargetPC_o |
| IN | BrEx_FlagsValue_i | 4 | Branch_Arbiter.BrEx_FlagsValue_o |
| IN | BrEx_PredTaken_i | 1 | Branch_Arbiter.BrEx_PredTaken_o |
| IN | BrEx_ROBTag_i | 5 | Branch_Arbiter.BrEx_ROBTag_o |
| OUT | BrExResult_Valid_o | 1 | ROB.BrExResult_Valid_i |
| OUT | BrExResult_ROBTag_o | 5 | ROB.BrExResult_ROBTag_i |
| OUT | BrExResult_Taken_o | 1 | ROB.BrExResult_Taken_i |
| OUT | BrExResult_Target_o | 64 | ROB.BrExResult_Target_i |
| OUT | BrExResult_Mispredict_o | 1 | ROB.BrExResult_Mispredict_i |

---
## COMPONENT 36: CDB_Arbiter

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | IntALU_CDBReq_i | 1 | IntALU.IntALU_CDBReq_o |
| IN | IntALU_CDBROBTag_i | 5 | IntALU.IntALU_CDBROBTag_o |
| IN | IntALU_CDBValue_i | 64 | IntALU.IntALU_CDBValue_o |
| IN | IntALU_CDBException_i | 4 | IntALU.IntALU_CDBException_o |
| IN | IntALU_CDBFlags_i | 4 | IntALU.IntALU_CDBFlags_o |
| IN | IntALU_CDBFlagsValid_i | 1 | IntALU.IntALU_CDBFlagsValid_o |
| IN | FPALU_CDBReq_i | 1 | FPALU.FPALU_CDBReq_o |
| IN | FPALU_CDBROBTag_i | 5 | FPALU.FPALU_CDBROBTag_o |
| IN | FPALU_CDBValue_i | 64 | FPALU.FPALU_CDBValue_o |
| IN | FPALU_CDBException_i | 4 | FPALU.FPALU_CDBException_o |
| IN | FPALU_CDBFlags_i | 4 | FPALU.FPALU_CDBFlags_o |
| IN | FPALU_CDBFlagsValid_i | 1 | FPALU.FPALU_CDBFlagsValid_o |
| IN | IntMUL_CDBReq_i | 1 | IntMUL.IntMUL_CDBReq_o |
| IN | IntMUL_CDBROBTag_i | 5 | IntMUL.IntMUL_CDBROBTag_o |
| IN | IntMUL_CDBValue_i | 64 | IntMUL.IntMUL_CDBValue_o |
| IN | IntMUL_CDBException_i | 4 | IntMUL.IntMUL_CDBException_o |
| IN | IntMUL_CDBFlags_i | 4 | IntMUL.IntMUL_CDBFlags_o |
| IN | IntMUL_CDBFlagsValid_i | 1 | IntMUL.IntMUL_CDBFlagsValid_o |
| IN | FPMUL_CDBReq_i | 1 | FPMUL.FPMUL_CDBReq_o |
| IN | FPMUL_CDBROBTag_i | 5 | FPMUL.FPMUL_CDBROBTag_o |
| IN | FPMUL_CDBValue_i | 64 | FPMUL.FPMUL_CDBValue_o |
| IN | FPMUL_CDBException_i | 4 | FPMUL.FPMUL_CDBException_o |
| IN | FPMUL_CDBFlags_i | 4 | FPMUL.FPMUL_CDBFlags_o |
| IN | FPMUL_CDBFlagsValid_i | 1 | FPMUL.FPMUL_CDBFlagsValid_o |
| IN | LB0_CDBReq_i | 1 | LB0.LB0_CDBReq_o |
| IN | LB0_CDBROBTag_i | 5 | LB0.LB0_CDBROBTag_o |
| IN | LB0_CDBValue_i | 64 | LB0.LB0_CDBValue_o |
| IN | LB0_CDBException_i | 4 | LB0.LB0_CDBException_o |
| IN | LB1_CDBReq_i | 1 | LB1.LB1_CDBReq_o |
| IN | LB1_CDBROBTag_i | 5 | LB1.LB1_CDBROBTag_o |
| IN | LB1_CDBValue_i | 64 | LB1.LB1_CDBValue_o |
| IN | LB1_CDBException_i | 4 | LB1.LB1_CDBException_o |
| IN | LB2_CDBReq_i | 1 | LB2.LB2_CDBReq_o |
| IN | LB2_CDBROBTag_i | 5 | LB2.LB2_CDBROBTag_o |
| IN | LB2_CDBValue_i | 64 | LB2.LB2_CDBValue_o |
| IN | LB2_CDBException_i | 4 | LB2.LB2_CDBException_o |
| OUT | ArbToCDBA_Valid_o | 1 | CDB_A.ArbToCDBA_Valid_i |
| OUT | ArbToCDBA_ROBTag_o | 5 | CDB_A.ArbToCDBA_ROBTag_i |
| OUT | ArbToCDBA_Value_o | 64 | CDB_A.ArbToCDBA_Value_i |
| OUT | ArbToCDBA_Exception_o | 4 | CDB_A.ArbToCDBA_Exception_i |
| OUT | ArbToCDBA_Flags_o | 4 | CDB_A.ArbToCDBA_Flags_i |
| OUT | ArbToCDBA_FlagsValid_o | 1 | CDB_A.ArbToCDBA_FlagsValid_i |
| OUT | ArbToCDBB_Valid_o | 1 | CDB_B.ArbToCDBB_Valid_i |
| OUT | ArbToCDBB_ROBTag_o | 5 | CDB_B.ArbToCDBB_ROBTag_i |
| OUT | ArbToCDBB_Value_o | 64 | CDB_B.ArbToCDBB_Value_i |
| OUT | ArbToCDBB_Exception_o | 4 | CDB_B.ArbToCDBB_Exception_i |
| OUT | ArbToCDBB_Flags_o | 4 | CDB_B.ArbToCDBB_Flags_i |
| OUT | ArbToCDBB_FlagsValid_o | 1 | CDB_B.ArbToCDBB_FlagsValid_i |
| OUT | IntALU_CDBStall_o | 1 | IntALU.IntALU_CDBStall_i |
| OUT | FPALU_CDBStall_o | 1 | FPALU.FPALU_CDBStall_i |
| OUT | IntMUL_CDBStall_o | 1 | IntMUL.IntMUL_CDBStall_i |
| OUT | FPMUL_CDBStall_o | 1 | FPMUL.FPMUL_CDBStall_i |
| OUT | LB0_CDBStall_o | 1 | LB0.LB0_CDBStall_i |
| OUT | LB1_CDBStall_o | 1 | LB1.LB1_CDBStall_i |
| OUT | LB2_CDBStall_o | 1 | LB2.LB2_CDBStall_i |

---
## COMPONENT 37: CDB_A

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | ArbToCDBA_Valid_i | 1 | CDB_Arbiter.ArbToCDBA_Valid_o |
| IN | ArbToCDBA_ROBTag_i | 5 | CDB_Arbiter.ArbToCDBA_ROBTag_o |
| IN | ArbToCDBA_Value_i | 64 | CDB_Arbiter.ArbToCDBA_Value_o |
| IN | ArbToCDBA_Exception_i | 4 | CDB_Arbiter.ArbToCDBA_Exception_o |
| IN | ArbToCDBA_Flags_i | 4 | CDB_Arbiter.ArbToCDBA_Flags_o |
| IN | ArbToCDBA_FlagsValid_i | 1 | CDB_Arbiter.ArbToCDBA_FlagsValid_o |
| OUT | CDBA_Valid_o | 1 | ROB.CDBA_Valid_i, SB0.CDBA_Valid_i, SB1.CDBA_Valid_i, LB0.CDBA_Valid_i, LB1.CDBA_Valid_i, LB2.CDBA_Valid_i, RS_IntALU0.CDBA_Valid_i, RS_IntALU1.CDBA_Valid_i, RS_FPALU0.CDBA_Valid_i, RS_IntMUL0.CDBA_Valid_i, RS_FPMUL0.CDBA_Valid_i, RS_Branch0.CDBA_Valid_i, Flags_Unit.CDBA_Valid_i |
| OUT | CDBA_ROBTag_o | 5 | ROB.CDBA_ROBTag_i, SB0.CDBA_ROBTag_i, SB1.CDBA_ROBTag_i, LB0.CDBA_ROBTag_i, LB1.CDBA_ROBTag_i, LB2.CDBA_ROBTag_i, RS_IntALU0.CDBA_ROBTag_i, RS_IntALU1.CDBA_ROBTag_i, RS_FPALU0.CDBA_ROBTag_i, RS_IntMUL0.CDBA_ROBTag_i, RS_FPMUL0.CDBA_ROBTag_i, RS_Branch0.CDBA_ROBTag_i, Flags_Unit.CDBA_ROBTag_i |
| OUT | CDBA_Value_o | 64 | ROB.CDBA_Value_i, SB0.CDBA_Value_i, SB1.CDBA_Value_i, LB0.CDBA_Value_i, LB1.CDBA_Value_i, LB2.CDBA_Value_i, RS_IntALU0.CDBA_Value_i, RS_IntALU1.CDBA_Value_i, RS_FPALU0.CDBA_Value_i, RS_IntMUL0.CDBA_Value_i, RS_FPMUL0.CDBA_Value_i |
| OUT | CDBA_Exception_o | 4 | ROB.CDBA_Exception_i |
| OUT | CDBA_Flags_o | 4 | ROB.CDBA_Flags_i, RS_IntALU0.CDBA_Flags_i, RS_IntALU1.CDBA_Flags_i, RS_FPALU0.CDBA_Flags_i, RS_IntMUL0.CDBA_Flags_i, RS_FPMUL0.CDBA_Flags_i, RS_Branch0.CDBA_Flags_i |
| OUT | CDBA_FlagsValid_o | 1 | ROB.CDBA_FlagsValid_i, RS_IntALU0.CDBA_FlagsValid_i, RS_IntALU1.CDBA_FlagsValid_i, RS_FPALU0.CDBA_FlagsValid_i, RS_IntMUL0.CDBA_FlagsValid_i, RS_FPMUL0.CDBA_FlagsValid_i, RS_Branch0.CDBA_FlagsValid_i, Flags_Unit.CDBA_FlagsValid_i |

---
## COMPONENT 38: CDB_B

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | ArbToCDBB_Valid_i | 1 | CDB_Arbiter.ArbToCDBB_Valid_o |
| IN | ArbToCDBB_ROBTag_i | 5 | CDB_Arbiter.ArbToCDBB_ROBTag_o |
| IN | ArbToCDBB_Value_i | 64 | CDB_Arbiter.ArbToCDBB_Value_o |
| IN | ArbToCDBB_Exception_i | 4 | CDB_Arbiter.ArbToCDBB_Exception_o |
| IN | ArbToCDBB_Flags_i | 4 | CDB_Arbiter.ArbToCDBB_Flags_o |
| IN | ArbToCDBB_FlagsValid_i | 1 | CDB_Arbiter.ArbToCDBB_FlagsValid_o |
| OUT | CDBB_Valid_o | 1 | ROB.CDBB_Valid_i, SB0.CDBB_Valid_i, SB1.CDBB_Valid_i, LB0.CDBB_Valid_i, LB1.CDBB_Valid_i, LB2.CDBB_Valid_i, RS_IntALU0.CDBB_Valid_i, RS_IntALU1.CDBB_Valid_i, RS_FPALU0.CDBB_Valid_i, RS_IntMUL0.CDBB_Valid_i, RS_FPMUL0.CDBB_Valid_i, RS_Branch0.CDBB_Valid_i, Flags_Unit.CDBB_Valid_i |
| OUT | CDBB_ROBTag_o | 5 | ROB.CDBB_ROBTag_i, SB0.CDBB_ROBTag_i, SB1.CDBB_ROBTag_i, LB0.CDBB_ROBTag_i, LB1.CDBB_ROBTag_i, LB2.CDBB_ROBTag_i, RS_IntALU0.CDBB_ROBTag_i, RS_IntALU1.CDBB_ROBTag_i, RS_FPALU0.CDBB_ROBTag_i, RS_IntMUL0.CDBB_ROBTag_i, RS_FPMUL0.CDBB_ROBTag_i, RS_Branch0.CDBB_ROBTag_i, Flags_Unit.CDBB_ROBTag_i |
| OUT | CDBB_Value_o | 64 | ROB.CDBB_Value_i, SB0.CDBB_Value_i, SB1.CDBB_Value_i, LB0.CDBB_Value_i, LB1.CDBB_Value_i, LB2.CDBB_Value_i, RS_IntALU0.CDBB_Value_i, RS_IntALU1.CDBB_Value_i, RS_FPALU0.CDBB_Value_i, RS_IntMUL0.CDBB_Value_i, RS_FPMUL0.CDBB_Value_i |
| OUT | CDBB_Exception_o | 4 | ROB.CDBB_Exception_i |
| OUT | CDBB_Flags_o | 4 | ROB.CDBB_Flags_i, RS_IntALU0.CDBB_Flags_i, RS_IntALU1.CDBB_Flags_i, RS_FPALU0.CDBB_Flags_i, RS_IntMUL0.CDBB_Flags_i, RS_FPMUL0.CDBB_Flags_i, RS_Branch0.CDBB_Flags_i |
| OUT | CDBB_FlagsValid_o | 1 | ROB.CDBB_FlagsValid_i, RS_IntALU0.CDBB_FlagsValid_i, RS_IntALU1.CDBB_FlagsValid_i, RS_FPALU0.CDBB_FlagsValid_i, RS_IntMUL0.CDBB_FlagsValid_i, RS_FPMUL0.CDBB_FlagsValid_i, RS_Branch0.CDBB_FlagsValid_i, Flags_Unit.CDBB_FlagsValid_i |

---
## COMPONENT 39: Memory_Arbiter

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | LB0_MemReq_i | 1 | LB0.LB0_MemReq_o |
| IN | LB0_MemAddr_i | 64 | LB0.LB0_MemAddr_o |
| IN | LB0_MemOp_i | 8 | LB0.LB0_MemOp_o |
| IN | LB0_MemROBTag_i | 5 | LB0.LB0_MemROBTag_o |
| IN | LB1_MemReq_i | 1 | LB1.LB1_MemReq_o |
| IN | LB1_MemAddr_i | 64 | LB1.LB1_MemAddr_o |
| IN | LB1_MemOp_i | 8 | LB1.LB1_MemOp_o |
| IN | LB1_MemROBTag_i | 5 | LB1.LB1_MemROBTag_o |
| IN | LB2_MemReq_i | 1 | LB2.LB2_MemReq_o |
| IN | LB2_MemAddr_i | 64 | LB2.LB2_MemAddr_o |
| IN | LB2_MemOp_i | 8 | LB2.LB2_MemOp_o |
| IN | LB2_MemROBTag_i | 5 | LB2.LB2_MemROBTag_o |
| IN | StoreCommit_Req_i | 1 | Commit_Unit.StoreCommit_Req_o |
| IN | StoreCommit_Addr_i | 64 | Commit_Unit.StoreCommit_Addr_o |
| IN | StoreCommit_Data_i | 64 | Commit_Unit.StoreCommit_Data_o |
| IN | StoreCommit_Op_i | 8 | Commit_Unit.StoreCommit_Op_o |
| IN | DC_Done_i | 1 | D_Cache.DC_Done_o |
| IN | DC_RData_i | 64 | D_Cache.DC_RData_o |
| IN | DC_Ready_i | 1 | D_Cache.DC_Ready_o |
| OUT | LB0_MemDone_o | 1 | LB0.LB0_MemDone_i |
| OUT | LB0_MemData_o | 64 | LB0.LB0_MemData_i |
| OUT | LB1_MemDone_o | 1 | LB1.LB1_MemDone_i |
| OUT | LB1_MemData_o | 64 | LB1.LB1_MemData_i |
| OUT | LB2_MemDone_o | 1 | LB2.LB2_MemDone_i |
| OUT | LB2_MemData_o | 64 | LB2.LB2_MemData_i |
| OUT | StoreCommit_Done_o | 1 | Commit_Unit.StoreCommit_Done_i |
| OUT | DC_Req_o | 1 | D_Cache.DC_Req_i |
| OUT | DC_RW_o | 1 | D_Cache.DC_RW_i |
| OUT | DC_Addr_o | 64 | D_Cache.DC_Addr_i |
| OUT | DC_WData_o | 64 | D_Cache.DC_WData_i |
| OUT | DC_Size_o | 2 | D_Cache.DC_Size_i |

---
## COMPONENT 40: D_Cache

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | DC_Req_i | 1 | Memory_Arbiter.DC_Req_o |
| IN | DC_RW_i | 1 | Memory_Arbiter.DC_RW_o |
| IN | DC_Addr_i | 64 | Memory_Arbiter.DC_Addr_o |
| IN | DC_WData_i | 64 | Memory_Arbiter.DC_WData_o |
| IN | DC_Size_i | 2 | Memory_Arbiter.DC_Size_o |
| IN | RAMtoDC_Data_i | 512 | RAM.RAMtoDC_Data_o |
| IN | RAMtoDC_Ready_i | 1 | RAM.RAMtoDC_Ready_o |
| OUT | DC_Done_o | 1 | Memory_Arbiter.DC_Done_i |
| OUT | DC_RData_o | 64 | Memory_Arbiter.DC_RData_i |
| OUT | DC_Ready_o | 1 | Memory_Arbiter.DC_Ready_i |
| OUT | DCtoRAM_Addr_o | 64 | RAM.DCtoRAM_Addr_i |
| OUT | DCtoRAM_RdReq_o | 1 | RAM.DCtoRAM_RdReq_i |
| OUT | DCtoRAM_WrReq_o | 1 | RAM.DCtoRAM_WrReq_i |
| OUT | DCtoRAM_WrData_o | 512 | RAM.DCtoRAM_WrData_i |

---
## COMPONENT 41: Commit_Unit

| Dir | Wire | Bits | Connected Component |
|-----|------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | ROBHead_i | 5 | ROB.ROBHead_o |
| IN | ROBHeadBusy_i | 1 | ROB.ROBHeadBusy_o |
| IN | ROBHeadReady_i | 1 | ROB.ROBHeadReady_o |
| IN | ROBHeadType_i | 3 | ROB.ROBHeadType_o |
| IN | ROBHeadDestReg_i | 4 | ROB.ROBHeadDestReg_o |
| IN | ROBHeadValue_i | 64 | ROB.ROBHeadValue_o |
| IN | ROBHeadException_i | 4 | ROB.ROBHeadException_o |
| IN | ROBHeadPC_i | 64 | ROB.ROBHeadPC_o |
| IN | ROBHeadFlagsResult_i | 4 | ROB.ROBHeadFlagsResult_o |
| IN | ROBHeadFlagsValid_i | 1 | ROB.ROBHeadFlagsValid_o |
| IN | ROBHeadMispredict_i | 1 | ROB.ROBHeadMispredict_o |
| IN | ROBHeadBranchTarget_i | 64 | ROB.ROBHeadBranchTarget_o |
| IN | ROBHeadStoreAddr_i | 64 | ROB.ROBHeadStoreAddr_o |
| IN | ROBHeadStoreData_i | 64 | ROB.ROBHeadStoreData_o |
| IN | ROBHeadStoreReady_i | 1 | ROB.ROBHeadStoreReady_o |
| IN | ROBHeadSourceStation_i | 4 | ROB.ROBHeadSourceStation_o |
| IN | StoreCommit_Done_i | 1 | Memory_Arbiter.StoreCommit_Done_o |
| OUT | CommitPop_o | 1 | ROB.CommitPop_i |
| OUT | CommitWrEn_o | 1 | Register_File.CommitWrEn_i |
| OUT | CommitWrAddr_o | 4 | Register_File.CommitWrAddr_i |
| OUT | CommitWrData_o | 64 | Register_File.CommitWrData_i |
| OUT | CommitROBIdx_o | 5 | Register_File.CommitROBIdx_i, Flags_Unit.CommitROBIdx_i |
| OUT | FlagsCommitEn_o | 1 | Flags_Unit.FlagsCommitEn_i |
| OUT | FlagsCommitValue_o | 4 | Flags_Unit.FlagsCommitValue_i |
| OUT | StoreCommit_Req_o | 1 | Memory_Arbiter.StoreCommit_Req_i |
| OUT | StoreCommit_Addr_o | 64 | Memory_Arbiter.StoreCommit_Addr_i |
| OUT | StoreCommit_Data_o | 64 | Memory_Arbiter.StoreCommit_Data_i |
| OUT | StoreCommit_Op_o | 8 | Memory_Arbiter.StoreCommit_Op_i |
| OUT | BranchRedirect_o | 1 | PC_MUX.BranchRedirect_i |
| OUT | BranchTarget_o | 64 | PC_MUX.BranchTarget_i |
| OUT | Flush_o | 1 | PC_C.Flush_i, I_Cache.Flush_i, Register_File.Flush_i, ROB.Flush_i, Flags_Unit.Flush_i, SB0.Flush_i, SB1.Flush_i, LB0.Flush_i, LB1.Flush_i, LB2.Flush_i, RS_IntALU0.Flush_i, RS_IntALU1.Flush_i, RS_FPALU0.Flush_i, RS_IntMUL0.Flush_i, RS_FPMUL0.Flush_i, RS_Branch0.Flush_i |
| OUT | FreeSB0_o | 1 | SB0.FreeSB0_i |
| OUT | FreeSB1_o | 1 | SB1.FreeSB1_i |
| OUT | FreeLB0_o | 1 | LB0.FreeLB0_i |
| OUT | FreeLB1_o | 1 | LB1.FreeLB1_i |
| OUT | FreeLB2_o | 1 | LB2.FreeLB2_i |
| OUT | FreeRSIAU0_o | 1 | RS_IntALU0.FreeRSIAU0_i |
| OUT | FreeRSIAU1_o | 1 | RS_IntALU1.FreeRSIAU1_i |
| OUT | FreeRSFPAU0_o | 1 | RS_FPALU0.FreeRSFPAU0_i |
| OUT | FreeRSIMUL0_o | 1 | RS_IntMUL0.FreeRSIMUL0_i |
| OUT | FreeRSFPMUL0_o | 1 | RS_FPMUL0.FreeRSFPMUL0_i |
| OUT | FreeRSBr0_o | 1 | RS_Branch0.FreeRSBr0_i |
| OUT | ExceptionSignal_o | 1 | (external trap handler) |
| OUT | ExceptionCode_o | 4 | (external trap handler) |
| OUT | ExceptionPC_o | 64 | (external trap handler) |

---

## MASTER COMPONENT LIST (41)

| # | Name | Type |
|---|------|------|
| 1 | PC_C | Sequential (FF 64b) |
| 2 | PC_MUX | Combinational |
| 3 | PC_Adder | Combinational |
| 4 | I_Cache | Sequential |
| 5 | RAM | Sequential |
| 6 | Instruction_Decoder | Combinational |
| 7 | Extends | Combinational |
| 8 | Control_Unit | Combinational |
| 9 | Register_File | Sequential |
| 10 | ROB | Sequential |
| 11 | Flags_Unit | Sequential |
| 12 | SB0 | Sequential |
| 13 | SB1 | Sequential |
| 14 | LB0 | Sequential |
| 15 | LB1 | Sequential |
| 16 | LB2 | Sequential |
| 17 | RS_IntALU0 | Sequential |
| 18 | RS_IntALU1 | Sequential |
| 19 | RS_FPALU0 | Sequential |
| 20 | RS_IntMUL0 | Sequential |
| 21 | RS_FPMUL0 | Sequential |
| 22 | RS_Branch0 | Sequential |
| 23 | AGU_Arbiter | Combinational |
| 24 | AGU_0 | Combinational |
| 25 | AGU_1 | Combinational |
| 26 | IntALU_Arbiter | Combinational |
| 27 | IntALU | Combinational |
| 28 | FPALU_Arbiter | Combinational |
| 29 | FPALU | Sequential (pipelined) |
| 30 | IntMUL_Arbiter | Combinational |
| 31 | IntMUL | Sequential |
| 32 | FPMUL_Arbiter | Combinational |
| 33 | FPMUL | Sequential |
| 34 | Branch_Arbiter | Combinational |
| 35 | Branch_Executor | Combinational |
| 36 | CDB_Arbiter | Combinational |
| 37 | CDB_A | Combinational (bus) |
| 38 | CDB_B | Combinational (bus) |
| 39 | Memory_Arbiter | Sequential |
| 40 | D_Cache | Sequential |
| 41 | Commit_Unit | Combinational/Sequential |