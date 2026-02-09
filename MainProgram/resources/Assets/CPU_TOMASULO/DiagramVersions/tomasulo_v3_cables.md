# Micro-Arquitectura Tomasulo v3 — Cableado Definitivo (41 Componentes)

**REGLA**: Cada cable tiene UN nombre global. Output = `Nombre_o`. Input = `Nombre_i`. Sin excepciones.

---

## COMPONENTE 1: PC_C

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | PCNext_i | 64 | PC_MUX.PCNext_o |
| IN | CLK | 1 | Global |
| IN | RESET | 1 | Global |
| IN | StallIF_i | 1 | Control_Unit.StallIF_o |
| OUT | PCCurrent_o | 64 | PC_Adder.PCCurrent_i, I_Cache.PCCurrent_i, ROB.PCCurrent_i, RS_Branch0.PCCurrent_i |

---

## COMPONENTE 2: PC_MUX

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | PCPlus8_i | 64 | PC_Adder.PCPlus8_o |
| IN | BranchTarget_i | 64 | Commit_Unit.BranchTarget_o |
| IN | BranchRedirect_i | 1 | Commit_Unit.BranchRedirect_o |
| OUT | PCNext_o | 64 | PC_C.PCNext_i |

---

## COMPONENTE 3: PC_Adder

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | PCCurrent_i | 64 | PC_C.PCCurrent_o |
| OUT | PCPlus8_o | 64 | PC_MUX.PCPlus8_i |

---

## COMPONENTE 4: I_Cache

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTE 5: RAM

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTE 6: Instruction_Decoder ★NUEVO★

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | InstrF_i | 64 | I_Cache.InstrF_o |
| OUT | Op_in_o | 8 | Control_Unit.Op_in_i, SB0.Op_in_i, SB1.Op_in_i, LB0.Op_in_i, LB1.Op_in_i, LB2.Op_in_i, RS_IntALU0.Op_in_i, RS_IntALU1.Op_in_i, RS_FPALU0.Op_in_i, RS_IntMUL0.Op_in_i, RS_FPMUL0.Op_in_i, RS_Branch0.Op_in_i |
| OUT | Dest_in_o | 4 | Control_Unit.Dest_in_i, Register_File.Dest_in_i, ROB.Dest_in_i |
| OUT | Rn_in_o | 4 | Register_File.Rn_in_i |
| OUT | Rm_in_o | 4 | Register_File.Rm_in_i |
| OUT | Imm_in_o | 32 | Extends.Imm_in_i |

---

## COMPONENTE 7: Extends

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | Imm_in_i | 32 | Instruction_Decoder.Imm_in_o |
| IN | IsFPALU_i | 1 | Control_Unit.IsFPALU_o |
| IN | IsFPMUL_i | 1 | Control_Unit.IsFPMUL_o |
| OUT | ImmExt_o | 64 | SB0.ImmExt_i, SB1.ImmExt_i, LB0.ImmExt_i, LB1.ImmExt_i, LB2.ImmExt_i, RS_IntALU0.ImmExt_i, RS_IntALU1.ImmExt_i, RS_FPALU0.ImmExt_i, RS_IntMUL0.ImmExt_i, RS_FPMUL0.ImmExt_i, RS_Branch0.ImmExt_i |

---

## COMPONENTE 8: Control_Unit

Señales internas (NO son cables): IsALU, IsMUL, IsFPALU_int, IsFPMUL_int, IsLoad, IsStore, IsBranch, IsUnconditional.

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | Dest_in_i | 4 | Instruction_Decoder.Dest_in_o |
| IN | InstReady_i | 1 | I_Cache.InstReady_o |
| IN | ROBFull_i | 1 | ROB.ROBFull_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | SB0_Busy_i | 1 | SB0.Busy_o |
| IN | SB1_Busy_i | 1 | SB1.Busy_o |
| IN | LB0_Busy_i | 1 | LB0.Busy_o |
| IN | LB1_Busy_i | 1 | LB1.Busy_o |
| IN | LB2_Busy_i | 1 | LB2.Busy_o |
| IN | RS_IntALU0_Busy_i | 1 | RS_IntALU0.Busy_o |
| IN | RS_IntALU1_Busy_i | 1 | RS_IntALU1.Busy_o |
| IN | RS_FPALU0_Busy_i | 1 | RS_FPALU0.Busy_o |
| IN | RS_IntMUL0_Busy_i | 1 | RS_IntMUL0.Busy_o |
| IN | RS_FPMUL0_Busy_i | 1 | RS_FPMUL0.Busy_o |
| IN | RS_Branch0_Busy_i | 1 | RS_Branch0.Busy_o |
| OUT | StallIF_o | 1 | PC_C.StallIF_i |
| OUT | ROBAlloc_o | 1 | ROB.ROBAlloc_i |
| OUT | AllocSB0_o | 1 | SB0.Alloc_i |
| OUT | AllocSB1_o | 1 | SB1.Alloc_i |
| OUT | AllocLB0_o | 1 | LB0.Alloc_i |
| OUT | AllocLB1_o | 1 | LB1.Alloc_i |
| OUT | AllocLB2_o | 1 | LB2.Alloc_i |
| OUT | AllocRSIntALU0_o | 1 | RS_IntALU0.Alloc_i |
| OUT | AllocRSIntALU1_o | 1 | RS_IntALU1.Alloc_i |
| OUT | AllocRSFPALU0_o | 1 | RS_FPALU0.Alloc_i |
| OUT | AllocRSIntMUL0_o | 1 | RS_IntMUL0.Alloc_i |
| OUT | AllocRSFPMUL0_o | 1 | RS_FPMUL0.Alloc_i |
| OUT | AllocRSBranch0_o | 1 | RS_Branch0.Alloc_i |
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

## COMPONENTE 9: Register_File

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTE 10: ROB

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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
| IN | SB0_StComplete_Valid_i | 1 | SB0.StComplete_Valid_o |
| IN | SB0_StComplete_ROBTag_i | 5 | SB0.StComplete_ROBTag_o |
| IN | SB0_StComplete_Addr_i | 64 | SB0.StComplete_Addr_o |
| IN | SB0_StComplete_Data_i | 64 | SB0.StComplete_Data_o |
| IN | SB1_StComplete_Valid_i | 1 | SB1.StComplete_Valid_o |
| IN | SB1_StComplete_ROBTag_i | 5 | SB1.StComplete_ROBTag_o |
| IN | SB1_StComplete_Addr_i | 64 | SB1.StComplete_Addr_o |
| IN | SB1_StComplete_Data_i | 64 | SB1.StComplete_Data_o |
| IN | CommitPop_i | 1 | Commit_Unit.CommitPop_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | ROBTail_o | 5 | Control_Unit.ROBTail_i, Register_File.ROBTail_i, Flags_Unit.ROBTail_i, SB0.ROBTail_i, SB1.ROBTail_i, LB0.ROBTail_i, LB1.ROBTail_i, LB2.ROBTail_i, RS_IntALU0.ROBTail_i, RS_IntALU1.ROBTail_i, RS_FPALU0.ROBTail_i, RS_IntMUL0.ROBTail_i, RS_FPMUL0.ROBTail_i, RS_Branch0.ROBTail_i |
| OUT | ROBFull_o | 1 | Control_Unit.ROBFull_i |
| OUT | ROBReadValue1_o | 64 | SB0.ROBReadValue1_i, SB1.ROBReadValue1_i, LB0.ROBReadValue1_i, LB1.ROBReadValue1_i, LB2.ROBReadValue1_i, RS_IntALU0.ROBReadValue1_i, RS_IntALU1.ROBReadValue1_i, RS_FPALU0.ROBReadValue1_i, RS_IntMUL0.ROBReadValue1_i, RS_FPMUL0.ROBReadValue1_i |
| OUT | ROBReadReady1_o | 1 | SB0.ROBReadReady1_i, SB1.ROBReadReady1_i, LB0.ROBReadReady1_i, LB1.ROBReadReady1_i, LB2.ROBReadReady1_i, RS_IntALU0.ROBReadReady1_i, RS_IntALU1.ROBReadReady1_i, RS_FPALU0.ROBReadReady1_i, RS_IntMUL0.ROBReadReady1_i, RS_FPMUL0.ROBReadReady1_i |
| OUT | ROBReadValue2_o | 64 | SB0.ROBReadValue2_i, SB1.ROBReadValue2_i, RS_IntALU0.ROBReadValue2_i, RS_IntALU1.ROBReadValue2_i, RS_FPALU0.ROBReadValue2_i, RS_IntMUL0.ROBReadValue2_i, RS_FPMUL0.ROBReadValue2_i |
| OUT | ROBReadReady2_o | 1 | SB0.ROBReadReady2_i, SB1.ROBReadReady2_i, RS_IntALU0.ROBReadReady2_i, RS_IntALU1.ROBReadReady2_i, RS_FPALU0.ROBReadReady2_i, RS_IntMUL0.ROBReadReady2_i, RS_FPMUL0.ROBReadReady2_i |
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

## COMPONENTE 11: Flags_Unit

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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
| OUT | FlagsQi_o | 5 | RS_IntALU0.FlagsQi_i, RS_IntALU1.FlagsQi_i, RS_FPALU0.FlagsQi_i, RS_IntMUL0.FlagsQi_i, RS_FPMUL0.FlagsQi_i, RS_Branch0.FlagsQi_i |
| OUT | FlagsQiValid_o | 1 | RS_IntALU0.FlagsQiValid_i, RS_IntALU1.FlagsQiValid_i, RS_FPALU0.FlagsQiValid_i, RS_IntMUL0.FlagsQiValid_i, RS_FPMUL0.FlagsQiValid_i, RS_Branch0.FlagsQiValid_i |

---

## COMPONENTES 12-13: SB0, SB1 (Store Buffers)

Tabla para **SB0**. SB1 es idéntico reemplazando TODOS los `SB0_` por `SB1_`, y `AllocSB0` por `AllocSB1`, y `FreeSB0` por `FreeSB1`.

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | Alloc_i | 1 | Control_Unit.AllocSB0_o |
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
| IN | SB0_Free_i | 1 | Commit_Unit.FreeSB0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | Busy_o | 1 | Control_Unit.SB0_Busy_i |
| OUT | SB0_AGUReq_o | 1 | AGU_Arbiter.SB0_AGUReq_i |
| OUT | SB0_AGUBase_o | 64 | AGU_Arbiter.SB0_AGUBase_i |
| OUT | SB0_AGUOffset_o | 64 | AGU_Arbiter.SB0_AGUOffset_i |
| OUT | SB0_AGUROBTag_o | 5 | AGU_Arbiter.SB0_AGUROBTag_i |
| OUT | StComplete_Valid_o | 1 | ROB.SB0_StComplete_Valid_i |
| OUT | StComplete_ROBTag_o | 5 | ROB.SB0_StComplete_ROBTag_i |
| OUT | StComplete_Addr_o | 64 | ROB.SB0_StComplete_Addr_i |
| OUT | StComplete_Data_o | 64 | ROB.SB0_StComplete_Data_i |

---

## COMPONENTES 14-16: LB0, LB1, LB2 (Load Buffers)

Tabla para **LB0**. LB1 y LB2 son idénticos reemplazando `LB0_` por `LB1_`/`LB2_`, y `AllocLB0` por `AllocLB1`/`AllocLB2`, y `FreeLB0` por `FreeLB1`/`FreeLB2`.

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | Alloc_i | 1 | Control_Unit.AllocLB0_o |
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
| IN | LB0_Free_i | 1 | Commit_Unit.FreeLB0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | Busy_o | 1 | Control_Unit.LB0_Busy_i |
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

## COMPONENTES 17-18: RS_IntALU0, RS_IntALU1

Tabla para **RS_IntALU0**. RS_IntALU1 es idéntico reemplazando `RSIAU0_` por `RSIAU1_`, `AllocRSIntALU0` por `AllocRSIntALU1`, `FreeRSIAU0` por `FreeRSIAU1`, `RS_IntALU0_Busy` por `RS_IntALU1_Busy`.

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | Alloc_i | 1 | Control_Unit.AllocRSIntALU0_o |
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
| IN | RSIAU0_Free_i | 1 | Commit_Unit.FreeRSIAU0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | Busy_o | 1 | Control_Unit.RS_IntALU0_Busy_i |
| OUT | RSIAU0_Req_o | 1 | IntALU_Arbiter.RSIAU0_Req_i |
| OUT | RSIAU0_Op_o | 8 | IntALU_Arbiter.RSIAU0_Op_i |
| OUT | RSIAU0_Vj_o | 64 | IntALU_Arbiter.RSIAU0_Vj_i |
| OUT | RSIAU0_Vk_o | 64 | IntALU_Arbiter.RSIAU0_Vk_i |
| OUT | RSIAU0_DestROBTag_o | 5 | IntALU_Arbiter.RSIAU0_DestROBTag_i |
| OUT | RSIAU0_ModFlags_o | 1 | IntALU_Arbiter.RSIAU0_ModFlags_i |
| OUT | RSIAU0_NeedsFlags_o | 1 | IntALU_Arbiter.RSIAU0_NeedsFlags_i |
| OUT | RSIAU0_FlagsValue_o | 4 | IntALU_Arbiter.RSIAU0_FlagsValue_i |

---

## COMPONENTE 19: RS_FPALU0

Idéntico a RS_IntALU0 reemplazando: `RSIAU0_` → `RSFPAU0_`, `AllocRSIntALU0` → `AllocRSFPALU0`, `FreeRSIAU0` → `FreeRSFPAU0`, `RS_IntALU0_Busy` → `RS_FPALU0_Busy`, `IntALU_Arbiter` → `FPALU_Arbiter`.

---

## COMPONENTE 20: RS_IntMUL0

Idéntico a RS_IntALU0 reemplazando: `RSIAU0_` → `RSIMUL0_`, `AllocRSIntALU0` → `AllocRSIntMUL0`, `FreeRSIAU0` → `FreeRSIMUL0`, `RS_IntALU0_Busy` → `RS_IntMUL0_Busy`, `IntALU_Arbiter` → `IntMUL_Arbiter`. Incluye bit interno `Executing`.

---

## COMPONENTE 21: RS_FPMUL0

Idéntico a RS_IntMUL0 reemplazando: `RSIMUL0_` → `RSFPMUL0_`, `AllocRSIntMUL0` → `AllocRSFPMUL0`, `FreeRSIMUL0` → `FreeRSFPMUL0`, `RS_IntMUL0_Busy` → `RS_FPMUL0_Busy`, `IntMUL_Arbiter` → `FPMUL_Arbiter`. Incluye bit interno `Executing`.

---

## COMPONENTE 22: RS_Branch0

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | CLK | 1 | Global |
| IN | Alloc_i | 1 | Control_Unit.AllocRSBranch0_o |
| IN | Op_in_i | 8 | Instruction_Decoder.Op_in_o |
| IN | ImmExt_i | 64 | Extends.ImmExt_o |
| IN | PCCurrent_i | 64 | PC_C.PCCurrent_o |
| IN | ROBTail_i | 5 | ROB.ROBTail_o |
| IN | ModifiesFlags_i | 1 | Control_Unit.ModifiesFlags_o |
| IN | NeedsFlags_i | 1 | Control_Unit.NeedsFlags_o |
| IN | FlagsArch_i | 4 | Flags_Unit.FlagsArch_o |
| IN | FlagsQi_i | 5 | Flags_Unit.FlagsQi_o |
| IN | FlagsQiValid_i | 1 | Flags_Unit.FlagsQiValid_o |
| IN | CDBA_Valid_i | 1 | CDB_A.CDBA_Valid_o |
| IN | CDBA_ROBTag_i | 5 | CDB_A.CDBA_ROBTag_o |
| IN | CDBA_Flags_i | 4 | CDB_A.CDBA_Flags_o |
| IN | CDBA_FlagsValid_i | 1 | CDB_A.CDBA_FlagsValid_o |
| IN | CDBB_Valid_i | 1 | CDB_B.CDBB_Valid_o |
| IN | CDBB_ROBTag_i | 5 | CDB_B.CDBB_ROBTag_o |
| IN | CDBB_Flags_i | 4 | CDB_B.CDBB_Flags_o |
| IN | CDBB_FlagsValid_i | 1 | CDB_B.CDBB_FlagsValid_o |
| IN | RSBr0_Grant_i | 1 | Branch_Arbiter.RSBr0_Grant_o |
| IN | RSBr0_Free_i | 1 | Commit_Unit.FreeRSBr0_o |
| IN | Flush_i | 1 | Commit_Unit.Flush_o |
| OUT | Busy_o | 1 | Control_Unit.RS_Branch0_Busy_i |
| OUT | RSBr0_Req_o | 1 | Branch_Arbiter.RSBr0_Req_i |
| OUT | RSBr0_Op_o | 8 | Branch_Arbiter.RSBr0_Op_i |
| OUT | RSBr0_TargetPC_o | 64 | Branch_Arbiter.RSBr0_TargetPC_i |
| OUT | RSBr0_FlagsValue_o | 4 | Branch_Arbiter.RSBr0_FlagsValue_i |
| OUT | RSBr0_PredTaken_o | 1 | Branch_Arbiter.RSBr0_PredTaken_i |
| OUT | RSBr0_ROBTag_o | 5 | Branch_Arbiter.RSBr0_ROBTag_i |

---

## COMPONENTE 23: AGU_Arbiter

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTES 24-25: AGU_0, AGU_1

Tabla para **AGU_0**. AGU_1 es idéntico reemplazando `AGU0_` por `AGU1_`.

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTE 26: IntALU_Arbiter

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTE 27: IntALU

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTES 28-29: FPALU_Arbiter + FPALU

Idénticos en estructura a IntALU_Arbiter + IntALU. Reemplazar: `RSIAU` → `RSFPAU`, `IntALU` → `FPALU`. FPALU_Arbiter solo tiene 1 RS de entrada (RS_FPALU0), no 2.

---

## COMPONENTES 30-31: IntMUL_Arbiter + IntMUL

Idénticos. Reemplazar: `RSIAU` → `RSIMUL`, `IntALU` → `IntMUL`. IntMUL_Arbiter solo tiene 1 RS.

---

## COMPONENTES 32-33: FPMUL_Arbiter + FPMUL

Idénticos. Reemplazar: `RSIAU` → `RSFPMUL`, `IntALU` → `FPMUL`. FPMUL_Arbiter solo tiene 1 RS.

---

## COMPONENTE 34: Branch_Arbiter

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTE 35: Branch_Executor

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTE 36: CDB_Arbiter

7 solicitantes. Solo se listan los cables distintos; el patrón es `{Unit}_CDB{Signal}`.

**Entradas (por solicitante):**

| Solicitante | Cables IN (todos desde `{Unit}.{Unit}_CDB*_o`) | Bits |
|-------------|------------------------------------------------|------|
| IntALU | IntALU_CDBReq_i(1), IntALU_CDBROBTag_i(5), IntALU_CDBValue_i(64), IntALU_CDBException_i(4), IntALU_CDBFlags_i(4), IntALU_CDBFlagsValid_i(1) | 79 |
| FPALU | FPALU_CDBReq_i(1), FPALU_CDBROBTag_i(5), FPALU_CDBValue_i(64), FPALU_CDBException_i(4), FPALU_CDBFlags_i(4), FPALU_CDBFlagsValid_i(1) | 79 |
| IntMUL | IntMUL_CDBReq_i(1), IntMUL_CDBROBTag_i(5), IntMUL_CDBValue_i(64), IntMUL_CDBException_i(4), IntMUL_CDBFlags_i(4), IntMUL_CDBFlagsValid_i(1) | 79 |
| FPMUL | FPMUL_CDBReq_i(1), FPMUL_CDBROBTag_i(5), FPMUL_CDBValue_i(64), FPMUL_CDBException_i(4), FPMUL_CDBFlags_i(4), FPMUL_CDBFlagsValid_i(1) | 79 |
| LB0 | LB0_CDBReq_i(1), LB0_CDBROBTag_i(5), LB0_CDBValue_i(64), LB0_CDBException_i(4) | 74 |
| LB1 | LB1_CDBReq_i(1), LB1_CDBROBTag_i(5), LB1_CDBValue_i(64), LB1_CDBException_i(4) | 74 |
| LB2 | LB2_CDBReq_i(1), LB2_CDBROBTag_i(5), LB2_CDBValue_i(64), LB2_CDBException_i(4) | 74 |

**Salidas hacia CDB_A:**

| Cable | Bits | Destino |
|-------|------|---------|
| ArbToCDBA_Valid_o | 1 | CDB_A.ArbToCDBA_Valid_i |
| ArbToCDBA_ROBTag_o | 5 | CDB_A.ArbToCDBA_ROBTag_i |
| ArbToCDBA_Value_o | 64 | CDB_A.ArbToCDBA_Value_i |
| ArbToCDBA_Exception_o | 4 | CDB_A.ArbToCDBA_Exception_i |
| ArbToCDBA_Flags_o | 4 | CDB_A.ArbToCDBA_Flags_i |
| ArbToCDBA_FlagsValid_o | 1 | CDB_A.ArbToCDBA_FlagsValid_i |

**Salidas hacia CDB_B:**

| Cable | Bits | Destino |
|-------|------|---------|
| ArbToCDBB_Valid_o | 1 | CDB_B.ArbToCDBB_Valid_i |
| ArbToCDBB_ROBTag_o | 5 | CDB_B.ArbToCDBB_ROBTag_i |
| ArbToCDBB_Value_o | 64 | CDB_B.ArbToCDBB_Value_i |
| ArbToCDBB_Exception_o | 4 | CDB_B.ArbToCDBB_Exception_i |
| ArbToCDBB_Flags_o | 4 | CDB_B.ArbToCDBB_Flags_i |
| ArbToCDBB_FlagsValid_o | 1 | CDB_B.ArbToCDBB_FlagsValid_i |

**Salidas stall:**

| Cable | Bits | Destino |
|-------|------|---------|
| IntALU_CDBStall_o | 1 | IntALU.IntALU_CDBStall_i |
| FPALU_CDBStall_o | 1 | FPALU.FPALU_CDBStall_i |
| IntMUL_CDBStall_o | 1 | IntMUL.IntMUL_CDBStall_i |
| FPMUL_CDBStall_o | 1 | FPMUL.FPMUL_CDBStall_i |
| LB0_CDBStall_o | 1 | LB0.LB0_CDBStall_i |
| LB1_CDBStall_o | 1 | LB1.LB1_CDBStall_i |
| LB2_CDBStall_o | 1 | LB2.LB2_CDBStall_i |

---

## COMPONENTE 37: CDB_A

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
| IN | ArbToCDBA_Valid_i | 1 | CDB_Arbiter.ArbToCDBA_Valid_o |
| IN | ArbToCDBA_ROBTag_i | 5 | CDB_Arbiter.ArbToCDBA_ROBTag_o |
| IN | ArbToCDBA_Value_i | 64 | CDB_Arbiter.ArbToCDBA_Value_o |
| IN | ArbToCDBA_Exception_i | 4 | CDB_Arbiter.ArbToCDBA_Exception_o |
| IN | ArbToCDBA_Flags_i | 4 | CDB_Arbiter.ArbToCDBA_Flags_o |
| IN | ArbToCDBA_FlagsValid_i | 1 | CDB_Arbiter.ArbToCDBA_FlagsValid_o |
| OUT | CDBA_Valid_o | 1 | ROB, SB0, SB1, LB0, LB1, LB2, RS_IntALU0, RS_IntALU1, RS_FPALU0, RS_IntMUL0, RS_FPMUL0, RS_Branch0, Flags_Unit (all `.CDBA_Valid_i`) |
| OUT | CDBA_ROBTag_o | 5 | (mismos destinos `.CDBA_ROBTag_i`) |
| OUT | CDBA_Value_o | 64 | ROB, SB0, SB1, LB0, LB1, LB2, RS_IntALU0, RS_IntALU1, RS_FPALU0, RS_IntMUL0, RS_FPMUL0 (`.CDBA_Value_i`) |
| OUT | CDBA_Exception_o | 4 | ROB.CDBA_Exception_i |
| OUT | CDBA_Flags_o | 4 | ROB, RS_IntALU0, RS_IntALU1, RS_FPALU0, RS_IntMUL0, RS_FPMUL0, RS_Branch0 (`.CDBA_Flags_i`) |
| OUT | CDBA_FlagsValid_o | 1 | ROB, RS_IntALU0, RS_IntALU1, RS_FPALU0, RS_IntMUL0, RS_FPMUL0, RS_Branch0, Flags_Unit (`.CDBA_FlagsValid_i`) |

---

## COMPONENTE 38: CDB_B

Idéntico a CDB_A reemplazando `CDBA_` por `CDBB_` y `ArbToCDBA_` por `ArbToCDBB_`. Mismos destinos.

---

## COMPONENTE 39: Memory_Arbiter

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTE 40: D_Cache

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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

## COMPONENTE 41: Commit_Unit

| Dir | Cable | Bits | Componente conectado |
|-----|-------|------|---------------------|
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
| OUT | FreeSB0_o | 1 | SB0.SB0_Free_i |
| OUT | FreeSB1_o | 1 | SB1.SB1_Free_i |
| OUT | FreeLB0_o | 1 | LB0.LB0_Free_i |
| OUT | FreeLB1_o | 1 | LB1.LB1_Free_i |
| OUT | FreeLB2_o | 1 | LB2.LB2_Free_i |
| OUT | FreeRSIAU0_o | 1 | RS_IntALU0.RSIAU0_Free_i |
| OUT | FreeRSIAU1_o | 1 | RS_IntALU1.RSIAU1_Free_i |
| OUT | FreeRSFPAU0_o | 1 | RS_FPALU0.RSFPAU0_Free_i |
| OUT | FreeRSIMUL0_o | 1 | RS_IntMUL0.RSIMUL0_Free_i |
| OUT | FreeRSFPMUL0_o | 1 | RS_FPMUL0.RSFPMUL0_Free_i |
| OUT | FreeRSBr0_o | 1 | RS_Branch0.RSBr0_Free_i |
| OUT | ExceptionSignal_o | 1 | (sistema/trap handler externo) |
| OUT | ExceptionCode_o | 4 | (sistema/trap handler externo) |
| OUT | ExceptionPC_o | 64 | (sistema/trap handler externo) |

---

## LISTA MAESTRA DE COMPONENTES (41)

| # | Nombre | Tipo |
|---|--------|------|
| 1 | PC_C | Secuencial (FF 64b) |
| 2 | PC_MUX | Combinacional |
| 3 | PC_Adder | Combinacional |
| 4 | I_Cache | Secuencial |
| 5 | RAM | Secuencial |
| 6 | Instruction_Decoder | Combinacional |
| 7 | Extends | Combinacional |
| 8 | Control_Unit | Combinacional |
| 9 | Register_File | Secuencial |
| 10 | ROB | Secuencial |
| 11 | Flags_Unit | Secuencial |
| 12 | SB0 | Secuencial |
| 13 | SB1 | Secuencial |
| 14 | LB0 | Secuencial |
| 15 | LB1 | Secuencial |
| 16 | LB2 | Secuencial |
| 17 | RS_IntALU0 | Secuencial |
| 18 | RS_IntALU1 | Secuencial |
| 19 | RS_FPALU0 | Secuencial |
| 20 | RS_IntMUL0 | Secuencial |
| 21 | RS_FPMUL0 | Secuencial |
| 22 | RS_Branch0 | Secuencial |
| 23 | AGU_Arbiter | Combinacional |
| 24 | AGU_0 | Combinacional |
| 25 | AGU_1 | Combinacional |
| 26 | IntALU_Arbiter | Combinacional |
| 27 | IntALU | Combinacional |
| 28 | FPALU_Arbiter | Combinacional |
| 29 | FPALU | Secuencial (pipelined) |
| 30 | IntMUL_Arbiter | Combinacional |
| 31 | IntMUL | Secuencial |
| 32 | FPMUL_Arbiter | Combinacional |
| 33 | FPMUL | Secuencial |
| 34 | Branch_Arbiter | Combinacional |
| 35 | Branch_Executor | Combinacional |
| 36 | CDB_Arbiter | Combinacional |
| 37 | CDB_A | Combinacional (bus) |
| 38 | CDB_B | Combinacional (bus) |
| 39 | Memory_Arbiter | Secuencial |
| 40 | D_Cache | Secuencial |
| 41 | Commit_Unit | Combinacional/Secuencial |
