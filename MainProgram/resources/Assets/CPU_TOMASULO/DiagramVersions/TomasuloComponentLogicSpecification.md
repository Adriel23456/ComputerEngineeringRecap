# Tomasulo Microarchitecture — Complete Component Logic Specification

## Components 1–22: Fetch, Decode, Issue, Buffers, Reservation Stations

---

## GLOBAL CONVENTIONS

- **CLK**: All sequential components update on the RISING EDGE of CLK.
- **Combinational outputs**: Available WITHIN the same cycle (no clock needed).
- **Single-issue pipeline**: One instruction issued per cycle (when not stalled).
- **Dual CDB**: Two results can be broadcast simultaneously per cycle.
- **In-order Issue, Out-of-order Execute, In-order Commit**.
- **Flush**: Restores ALL speculative state. Every sequential component resets to committed state.
- **ISA**: 64-bit instructions. Op[63:56], Rd[55:52], Rn[51:48], Rm[47:44], Imm[43:12].

### Opcode Categories (from the ISA assembler):

```
Category 0 — Integer ALU (3-reg):  ADD(0x00), SUB(0x01), ADC(0x02), SBC(0x03),
                                    AND(0x06), ORR(0x07), EOR(0x08), BIC(0x09),
                                    LSL(0x0A), LSR(0x0B), ASR(0x0C), ROR(0x0D)

Category 1 — Integer MUL/DIV:      MUL(0x04), DIV(0x05)

Category 2 — Integer ALU (imm):    ADDI(0x0E), SUBI(0x0F), ADCI(0x10), SBCI(0x11),
                                    ANDI(0x14), ORRI(0x15), EORI(0x16), BICI(0x17),
                                    LSLI(0x18), LSRI(0x19), ASRI(0x1A), RORI(0x1B)

Category 3 — Integer MUL/DIV(imm): MULI(0x12), DIVI(0x13)

Category 4 — INC/DEC:              INC(0x1C), DEC(0x1D)

Category 5 — FP ALU (3-reg):       FADD(0x1E), FSUB(0x1F), FCOPYSIGN(0x22)

Category 6 — FP MUL/DIV (3-reg):   FMUL(0x20), FDIV(0x21)

Category 7 — FP ALU (imm):         FADDI(0x23), FSUBI(0x24), FCOPYSIGNI(0x27)

Category 8 — FP MUL/DIV (imm):     FMULI(0x25), FDIVI(0x26)

Category 9 — FP Unary (2-reg):     FSQRT(0x28), FNEG(0x29), FABS(0x2A),
                                    CDTI(0x2B), CDTD(0x2C), RTNR(0x2D),
                                    RTZ(0x2E), RTP(0x2F), RTNE(0x30)

Category 10 — FP Unary (imm):      FSQRTI(0x52), FNEGI(0x53), FABSI(0x54),
                                    CDTII(0x55), CDTDI(0x56), RTNRI(0x57),
                                    RTZI(0x58), RTPI(0x59), RTNEI(0x5A)

Category 11 — MOV (2-reg):         MOV(0x31), MVN(0x32)

Category 12 — MOV (imm):           MOVI(0x33), MVNI(0x34), FMOVI(0x35), FMVNI(0x36)

Category 13 — CMP (2-reg):         CMP(0x37), CMN(0x38), TST(0x39), TEQ(0x3A)

Category 14 — CMP (imm):           CMPI(0x3B), CMNI(0x3C), TSTI(0x3D), TEQI(0x3E)

Category 15 — FCMP (2-reg):        FCMP(0x3F), FCMN(0x40), FCMPS(0x41)

Category 16 — FCMP (imm):          FCMPI(0x42), FCMNI(0x43), FCMPSI(0x44)

Category 17 — Branch:              B(0x45), BEQ(0x46), BNE(0x47), BLT(0x48),
                                    BGT(0x49), BUN(0x4A), BORD(0x4B)

Category 18 — Special:             SWI(0x4C), NOP(0x4D)

Category 19 — Memory:              LDR(0x4E), STR(0x4F), LDRB(0x50), STRB(0x51)
```

### Execution Unit Assignment:

| Unit | Opcodes |
|------|---------|
| IntALU | Cat 0, 2, 4, 11(MOV/MVN), 12(MOVI/MVNI), 13, 14 |
| IntMUL | Cat 1, 3 |
| FPALU | Cat 5, 7, 9(FNEG/FABS/CDTI/CDTD/RTNR/RTZ/RTP/RTNE), 10(FNEGI/FABSI/...), 15, 16 |
| FPMUL | Cat 6, 8, 9(FSQRT only), 10(FSQRTI only) |
| Branch | Cat 17 |
| AGU (Load) | LDR(0x4E), LDRB(0x50) |
| AGU (Store) | STR(0x4F), STRB(0x51) |
| None | NOP, SWI |

### Which instructions modify flags:

```
ModifiesFlags = 1 for:
  ALL integer ALU ops: ADD, SUB, ADC, SBC, AND, ORR, EOR, BIC, LSL, LSR, ASR, ROR
  ALL integer ALU imm: ADDI, SUBI, ADCI, SBCI, ANDI, ORRI, EORI, BICI, LSLI, LSRI, ASRI, RORI
  INC, DEC
  MUL, DIV, MULI, DIVI  (overflow/zero flags)
  CMP, CMN, TST, TEQ, CMPI, CMNI, TSTI, TEQI
  FCMP, FCMN, FCMPS, FCMPI, FCMNI, FCMPSI
  
ModifiesFlags = 0 for:
  MOV, MVN, MOVI, MVNI, FMOVI, FMVNI
  FADD, FSUB, FMUL, FDIV, FSQRT, FNEG, FABS, etc. (except FCMP variants)
  LDR, STR, LDRB, STRB
  B, BEQ, BNE, BLT, BGT, BUN, BORD
  NOP, SWI
```

### Which instructions NEED flags as input:

```
NeedsFlags = 1 for:
  ADC, SBC, ADCI, SBCI  (need Carry flag)
  BEQ, BNE, BLT, BGT, BUN, BORD  (need N,Z,C,V)

NeedsFlags = 0 for:
  B (unconditional, always taken)
  Everything else
```

### Which instructions write to Rd:

```
WritesRd = 1 for:
  ALL arithmetic/logic ops (ADD, SUB, MUL, DIV, AND, ORR, etc. and their imm variants)
  INC, DEC
  ALL FP ops (FADD, FSUB, FMUL, FDIV, FSQRT, etc. and their imm variants)
  MOV, MVN, MOVI, MVNI, FMOVI, FMVNI
  LDR, LDRB
  
WritesRd = 0 for:
  CMP, CMN, TST, TEQ (and imm variants) — only modify flags
  FCMP, FCMN, FCMPS (and imm variants) — only modify flags
  STR, STRB — write to memory, not to register
  B, BEQ, BNE, etc. — branch, no register write
  NOP, SWI — no effect on registers
```

---

## COMPONENT 1: PC_C (Program Counter)

### Purpose
Holds the address of the NEXT instruction to fetch. Updated every cycle unless stalled.

### Pseudocode

```
RISING_EDGE(CLK):
    if RESET == 1:
        register ← 0x0000_0000_0000_0000
    else if Flush_i == 1:
        // Flush cancels any stall so PC can accept the redirect.
        register ← PCNext_i
    else if StallIF_i == 0:
        register ← PCNext_i
    else:
        // Stall: hold current value, do not advance
        pass

ALWAYS (combinational):
    PCCurrent_o ← register
```

### Key behavior
- During a stall (StallIF_i=1), the PC does NOT advance. The same instruction remains at the I-Cache input, and the issue stage retries next cycle.
- During a flush (misprediction), BranchRedirect_i=1 causes PC_MUX to select BranchTarget, which flows into PCNext_i. The PC accepts this value regardless of stall, because Flush clears the stall condition.

---

## COMPONENT 2: PC_MUX

### Purpose
Selects between sequential execution (PC+8) and branch redirection.

### Pseudocode

```
ALWAYS (combinational):
    if BranchRedirect_i == 1:
        PCNext_o ← BranchTarget_i
    else:
        PCNext_o ← PCPlus8_i
```

### Key behavior
- BranchRedirect is only asserted by Commit_Unit when a mispredicted branch reaches the ROB head. This is the ONLY mechanism for changing control flow — we use a "predict not-taken" static strategy, so branches always fetch the next sequential instruction until proven wrong.

---

## COMPONENT 3: PC_Adder

### Purpose
Computes the address of the next sequential instruction.

### Pseudocode

```
ALWAYS (combinational):
    PCPlus8_o ← PCCurrent_i + 8
```

### Key behavior
- Each instruction is 64 bits = 8 bytes. So the next instruction is always at PC + 8.
- This value is available combinationally in the SAME cycle as PCCurrent_o changes.

---

## COMPONENT 4: I_Cache (Instruction Cache)

### Purpose
Provides the 64-bit instruction word from the address given by the PC. On a hit, the instruction is available in the same cycle. On a miss, the pipeline stalls while the line is fetched from RAM.

### Internal State
```
tag_array[32][4]        — 4-way set-associative tags
data_array[32][4][64B]  — cache line data (64 bytes per line = 8 instructions per line)
valid[32][4]            — valid bits
lru_state[32]           — LRU tracking per set
prefetch_pending        — flag: a prefetch request is in flight
prefetch_addr           — address being prefetched
miss_pending            — flag: a demand miss is in flight
miss_addr               — address of demand miss
```

### Pseudocode

```
RISING_EDGE(CLK):
    if Flush_i == 1:
        prefetch_pending ← 0   // Cancel any in-flight prefetch
        // Do NOT cancel demand miss — it will complete and we'll use the line anyway.

    // Handle RAM response
    if RAMtoIC_Ready_i == 1:
        if miss_pending == 1:
            set ← extract_set(miss_addr)
            way ← select_lru_way(set)
            tag_array[set][way] ← extract_tag(miss_addr)
            data_array[set][way] ← RAMtoIC_Data_i
            valid[set][way] ← 1
            update_lru(set, way)
            miss_pending ← 0
        else if prefetch_pending == 1:
            set ← extract_set(prefetch_addr)
            if not already_in_cache(prefetch_addr):
                way ← select_lru_way(set)
                tag_array[set][way] ← extract_tag(prefetch_addr)
                data_array[set][way] ← RAMtoIC_Data_i
                valid[set][way] ← 1
                update_lru(set, way)
            prefetch_pending ← 0

ALWAYS (combinational):
    addr ← PCCurrent_i
    set ← addr[10:6]
    tag ← addr[63:11]
    offset ← addr[5:3]
    
    hit ← 0
    for w in 0..3:
        if valid[set][w] == 1 AND tag_array[set][w] == tag:
            hit ← 1
            InstrF_o ← data_array[set][w][ offset * 8 .. offset * 8 + 7 ]
            update_lru(set, w)
    
    if hit == 1:
        InstReady_o ← 1
        ICtoRAM_Req_o ← 0
        // Next-line prefetch
        next_line_addr ← (addr & ~63) + 64
        if not in_cache(next_line_addr) AND prefetch_pending == 0:
            prefetch_pending ← 1
            prefetch_addr ← next_line_addr
            ICtoRAM_Req_o ← 1
            ICtoRAM_Addr_o ← next_line_addr
    else:
        InstReady_o ← 0
        InstrF_o ← 0
        if miss_pending == 0:
            miss_pending ← 1
            miss_addr ← addr & ~63
            ICtoRAM_Req_o ← 1
            ICtoRAM_Addr_o ← miss_addr
```

### Key behavior
- **Hit latency**: 0 extra cycles (instruction available in the same cycle the PC is presented).
- **Miss latency**: 50-100 cycles (RAM latency). During this time, InstReady_o = 0, which causes Control_Unit to assert StallIF_o = 1, freezing the PC.
- **Next-line prefetch**: On a hit, the cache proactively fetches the next cache line to reduce future miss rates for sequential code.

---

## COMPONENT 5: RAM (Main Memory)

### Purpose
Backs both I-Cache and D-Cache. Handles read/write requests with multi-cycle latency.

### Internal State
```
memory[50MB]             — byte-addressable storage
ic_req_active            — I-Cache request in progress
dc_req_active            — D-Cache request in progress
ic_counter, dc_counter   — cycle counters for latency simulation
LATENCY = 50             — cycles per access (configurable)
```

### Pseudocode

```
RISING_EDGE(CLK):
    RAMtoIC_Ready_o ← 0
    RAMtoDC_Ready_o ← 0

    // Prioritize D-Cache over I-Cache (to avoid blocking commits)
    if dc_req_active == 1:
        dc_counter ← dc_counter - 1
        if dc_counter == 0:
            if dc_is_write:
                memory[dc_addr .. dc_addr+63] ← DCtoRAM_WrData_i
            else:
                RAMtoDC_Data_o ← memory[dc_addr .. dc_addr+63]
            RAMtoDC_Ready_o ← 1
            dc_req_active ← 0
    
    if ic_req_active == 1 AND dc_req_active == 0:
        ic_counter ← ic_counter - 1
        if ic_counter == 0:
            RAMtoIC_Data_o ← memory[ic_addr .. ic_addr+63]
            RAMtoIC_Ready_o ← 1
            ic_req_active ← 0

    // Accept new requests
    if DCtoRAM_RdReq_i == 1 AND dc_req_active == 0:
        dc_req_active ← 1
        dc_addr ← DCtoRAM_Addr_i
        dc_is_write ← 0
        dc_counter ← LATENCY
    else if DCtoRAM_WrReq_i == 1 AND dc_req_active == 0:
        dc_req_active ← 1
        dc_addr ← DCtoRAM_Addr_i
        dc_is_write ← 1
        dc_counter ← LATENCY
    
    if ICtoRAM_Req_i == 1 AND ic_req_active == 0:
        ic_req_active ← 1
        ic_addr ← ICtoRAM_Addr_i
        ic_counter ← LATENCY
```

---

## COMPONENT 6: Instruction_Decoder

### Purpose
Splits the 64-bit instruction word into its constituent fields. Pure combinational wire slicing.

### Pseudocode

```
ALWAYS (combinational):
    Op_in_o   ← InstrF_i[63:56]   // 8 bits
    Dest_in_o ← InstrF_i[55:52]   // 4 bits
    Rn_in_o   ← InstrF_i[51:48]   // 4 bits
    Rm_in_o   ← InstrF_i[47:44]   // 4 bits
    Imm_in_o  ← InstrF_i[43:12]   // 32 bits
```

### Key behavior
- This is literally wire routing. No logic gates needed beyond bit selection.
- The outputs fan out to Control_Unit, Register_File, ROB, Extends, and all RS/Buffers.
- Every RS/Buffer receives Op_in, but only the one whose Alloc=1 captures it.

---

## COMPONENT 7: Extends (Immediate Extender)

### Purpose
Extends the 32-bit immediate field to 64 bits. For integer operations, it performs two's complement sign extension. For floating-point operations, it converts IEEE 754 single-precision (32-bit float) to double-precision (64-bit double).

### Pseudocode

```
ALWAYS (combinational):
    IsFP ← IsFPALU_i OR IsFPMUL_i
    
    if IsFP == 1:
        sign     ← Imm_in_i[31]
        exponent ← Imm_in_i[30:23]
        mantissa ← Imm_in_i[22:0]
        
        if exponent == 0x00:
            double_exp ← 0x000
            double_man ← mantissa << 29
        else if exponent == 0xFF:
            double_exp ← 0x7FF
            double_man ← mantissa << 29
        else:
            double_exp ← exponent - 127 + 1023
            double_man ← mantissa << 29
        
        ImmExt_o ← (sign << 63) | (double_exp << 52) | double_man
    else:
        if Imm_in_i[31] == 1:
            ImmExt_o ← 0xFFFFFFFF_00000000 | Imm_in_i
        else:
            ImmExt_o ← 0x00000000_00000000 | Imm_in_i
```

---

## COMPONENT 8: Control_Unit

### Purpose
The brain of the Issue stage. It decodes the opcode to determine instruction type, checks resource availability, allocates an RS/Buffer + ROB entry, and drives all control signals for register renaming and tag writing.

### Internal Signals (NOT cables)

```
IsIntALU, IsIntMUL, IsFPALU_int, IsFPMUL_int
IsLoad, IsStore, IsBranch, IsNOP, IsSWI
IsCMPonly, IsUnconditional
ModifiesFlags_int, NeedsFlags_int, UseImm_int, WritesRd_int
```

### Pseudocode

```
ALWAYS (combinational):

    // ====================================================================
    // STEP 1: DECODE (determine instruction class from opcode)
    // ====================================================================
    
    op ← Op_in_i
    
    // Default all internal signals to 0
    IsIntALU ← 0; IsIntMUL ← 0; IsFPALU_int ← 0; IsFPMUL_int ← 0
    IsLoad ← 0; IsStore ← 0; IsBranch ← 0; IsNOP ← 0; IsSWI ← 0
    
    CASE op OF:
        0x00..0x03, 0x06..0x0D: IsIntALU ← 1
        0x0E..0x11, 0x14..0x1B: IsIntALU ← 1; UseImm_int ← 1
        0x1C, 0x1D: IsIntALU ← 1; UseImm_int ← 1
        0x04, 0x05: IsIntMUL ← 1
        0x12, 0x13: IsIntMUL ← 1; UseImm_int ← 1
        0x1E, 0x1F, 0x22: IsFPALU_int ← 1
        0x23, 0x24, 0x27: IsFPALU_int ← 1; UseImm_int ← 1
        0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30: IsFPALU_int ← 1
        0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A: IsFPALU_int ← 1; UseImm_int ← 1
        0x20, 0x21: IsFPMUL_int ← 1
        0x25, 0x26: IsFPMUL_int ← 1; UseImm_int ← 1
        0x28: IsFPMUL_int ← 1
        0x52: IsFPMUL_int ← 1; UseImm_int ← 1
        0x31, 0x32: IsIntALU ← 1
        0x33, 0x34: IsIntALU ← 1; UseImm_int ← 1
        0x35, 0x36: IsIntALU ← 1; UseImm_int ← 1
        0x37..0x3A: IsIntALU ← 1; IsCMPonly ← 1
        0x3B..0x3E: IsIntALU ← 1; IsCMPonly ← 1; UseImm_int ← 1
        0x3F..0x41: IsFPALU_int ← 1; IsCMPonly ← 1
        0x42..0x44: IsFPALU_int ← 1; IsCMPonly ← 1; UseImm_int ← 1
        0x45..0x4B: IsBranch ← 1
        0x4C: IsSWI ← 1
        0x4D: IsNOP ← 1
        0x4E, 0x50: IsLoad ← 1
        0x4F, 0x51: IsStore ← 1
    
    // ====================================================================
    // STEP 2: DERIVE CONTROL SIGNALS
    // ====================================================================
    
    ModifiesFlags_int ← IsIntALU OR IsIntMUL
    if op in {0x31, 0x32, 0x33, 0x34, 0x35, 0x36}: ModifiesFlags_int ← 0
    if IsCMPonly == 1: ModifiesFlags_int ← 1
    if op in {0x3F..0x44}: ModifiesFlags_int ← 1
    
    NeedsFlags_int ← 0
    if op in {0x02, 0x03, 0x10, 0x11}: NeedsFlags_int ← 1
    if IsBranch AND op != 0x45: NeedsFlags_int ← 1
    
    WritesRd_int ← NOT IsCMPonly AND NOT IsStore AND NOT IsBranch AND NOT IsNOP AND NOT IsSWI
    IsUnconditional ← (op == 0x45)
    
    IsFPALU_o ← IsFPALU_int
    IsFPMUL_o ← IsFPMUL_int
    ModifiesFlags_o ← ModifiesFlags_int
    NeedsFlags_o    ← NeedsFlags_int
    UseImm_o        ← UseImm_int
    IsCMPonly_o     ← IsCMPonly
    
    TagWrEn_o ← WritesRd_int AND (InstReady_i == 1) AND NOT StallIF_o_pending
    FlagsTagWrEn_o ← ModifiesFlags_int AND (InstReady_i == 1) AND NOT StallIF_o_pending
    
    // AllocType: 3-bit type code for ROB
    if IsIntALU:     AllocType_o ← 000
    if IsIntMUL:     AllocType_o ← 001
    if IsFPALU_int:  AllocType_o ← 010
    if IsFPMUL_int:  AllocType_o ← 011
    if IsLoad:       AllocType_o ← 100
    if IsStore:      AllocType_o ← 101
    if IsBranch:     AllocType_o ← 110
    if IsCMPonly:    AllocType_o ← 111
    if IsNOP:        AllocType_o ← 000
    
    AllocPredicted_o ← 0  // Always predict not-taken
    
    // ====================================================================
    // STEP 3: RESOURCE ALLOCATION (first-free policy)
    // ====================================================================
    
    StallIF_o ← 0
    ROBAlloc_o ← 0
    AllocSB0_o ← 0; AllocSB1_o ← 0
    AllocLB0_o ← 0; AllocLB1_o ← 0; AllocLB2_o ← 0
    AllocRSIntALU0_o ← 0; AllocRSIntALU1_o ← 0
    AllocRSFPALU0_o ← 0; AllocRSIntMUL0_o ← 0; AllocRSFPMUL0_o ← 0
    AllocRSBranch0_o ← 0
    
    if InstReady_i == 0 OR ROBFull_i == 1:
        StallIF_o ← 1
        RETURN
    
    if IsNOP OR IsSWI:
        ROBAlloc_o ← 1
        AllocSourceStation_o ← 0xF
        RETURN
    
    allocated ← false
    
    if IsIntALU:
        if RS_IntALU0_Busy_i == 0:
            AllocRSIntALU0_o ← 1; AllocSourceStation_o ← 0101; allocated ← true
        else if RS_IntALU1_Busy_i == 0:
            AllocRSIntALU1_o ← 1; AllocSourceStation_o ← 0110; allocated ← true
    
    else if IsIntMUL:
        if RS_IntMUL0_Busy_i == 0:
            AllocRSIntMUL0_o ← 1; AllocSourceStation_o ← 1000; allocated ← true
    
    else if IsFPALU_int:
        if RS_FPALU0_Busy_i == 0:
            AllocRSFPALU0_o ← 1; AllocSourceStation_o ← 0111; allocated ← true
    
    else if IsFPMUL_int:
        if RS_FPMUL0_Busy_i == 0:
            AllocRSFPMUL0_o ← 1; AllocSourceStation_o ← 1001; allocated ← true
    
    else if IsLoad:
        if LB0_Busy_i == 0:
            AllocLB0_o ← 1; AllocSourceStation_o ← 0010; allocated ← true
        else if LB1_Busy_i == 0:
            AllocLB1_o ← 1; AllocSourceStation_o ← 0011; allocated ← true
        else if LB2_Busy_i == 0:
            AllocLB2_o ← 1; AllocSourceStation_o ← 0100; allocated ← true
    
    else if IsStore:
        if SB0_Busy_i == 0:
            AllocSB0_o ← 1; AllocSourceStation_o ← 0000; allocated ← true
        else if SB1_Busy_i == 0:
            AllocSB1_o ← 1; AllocSourceStation_o ← 0001; allocated ← true
    
    else if IsBranch:
        if RS_Branch0_Busy_i == 0:
            AllocRSBranch0_o ← 1; AllocSourceStation_o ← 1010; allocated ← true
    
    if NOT allocated:
        StallIF_o ← 1
        ROBAlloc_o ← 0
    else:
        ROBAlloc_o ← 1
        StallIF_o ← 0
```

### Key behavior regarding Tomasulo and Issue stalls
- **Issue only stalls for structural hazards** (ROB full, or no free RS/Buffer of the correct type). It NEVER stalls for data dependencies — those are handled by the RS/Buffers themselves via tag waiting and CDB snooping.
- This is the CORE of Tomasulo: as long as there's a free station, the instruction is issued, and its operands are either captured immediately (if ready) or noted as pending (tag + Qi_valid). The station then waits independently.

---

## COMPONENT 9: Register_File

### Purpose
Holds the 12 architectural registers (REG0-REG8, SIDS, UPPER_REG, LOWER_REG) with their current values AND their rename tags (Qi, Qi_valid). Provides two read ports for operands and dedicated outputs for segmentation limits.

### Internal State

```
REG[12]:
    .Value     (64b)
    .Qi        (5b)    — ROB index of the instruction that will produce the next value
    .Qi_valid  (1b)    — 1 = Value is stale, waiting for ROB[Qi] to produce result
```

### Pseudocode

```
ALWAYS (combinational):
    RD1_Value_o   ← REG[Rn_in_i].Value
    RD1_Qi_o      ← REG[Rn_in_i].Qi
    RD1_QiValid_o ← REG[Rn_in_i].Qi_valid
    
    RD2_Value_o   ← REG[Rm_in_i].Value
    RD2_Qi_o      ← REG[Rm_in_i].Qi
    RD2_QiValid_o ← REG[Rm_in_i].Qi_valid
    
    UPPER_o ← REG[0xA].Value
    LOWER_o ← REG[0xB].Value

RISING_EDGE(CLK):
    if Flush_i == 1:
        for i in 0..11:
            REG[i].Qi_valid ← 0
        RETURN
    
    if CommitWrEn_i == 1:
        REG[CommitWrAddr_i].Value ← CommitWrData_i
        if REG[CommitWrAddr_i].Qi == CommitROBIdx_i AND REG[CommitWrAddr_i].Qi_valid == 1:
            REG[CommitWrAddr_i].Qi_valid ← 0
    
    if TagWrEn_i == 1:
        REG[Dest_in_i].Qi       ← ROBTail_i
        REG[Dest_in_i].Qi_valid ← 1
```

### Key behavior for Register Renaming
- **Renaming** is how Tomasulo eliminates WAW and WAR hazards. When `ADD REG3, REG1, REG2` is issued, REG3 is renamed: its Qi is set to the ROB tail index, and Qi_valid=1. Any subsequent instruction reading REG3 will see Qi_valid=1 and know to wait for ROB[Qi] instead of using the stale Value.
- **Commit clears the tag** only if the tag still points to the committing instruction. If a NEWER instruction has already re-renamed the register, the tag is NOT cleared (it still points to the newer instruction).
- **Flush clears ALL tags** because all speculative instructions are discarded. The Values remain correct because they were written by previous commits.

---

## COMPONENT 10: ROB (Reorder Buffer)

### Purpose
Maintains program order for in-order commit. Every issued instruction gets a ROB entry. When an instruction completes (via CDB, Branch Executor, or Store Complete), its ROB entry is marked Ready. The Commit_Unit only commits from the HEAD.

### Internal State

```
entries[32]:
    .Busy, .Ready, .Type(3b), .DestReg(4b), .Value(64b)
    .Exception(4b), .PC(64b)
    .FlagsResult(4b), .FlagsValid(1b), .ModifiesFlags(1b)
    .Predicted(1b), .BranchTaken(1b), .BranchTarget(64b), .Mispredict(1b)
    .StoreAddr(64b), .StoreData(64b), .StoreReady(1b)
    .SourceStation(4b)

Head(5b), Tail(5b), Count(6b)
```

### Pseudocode

```
// ====================================================================
// COMBINATIONAL OUTPUTS (always)
// ====================================================================
ALWAYS (combinational):
    ROBTail_o ← Tail
    ROBFull_o ← (Count == 32)
    ROBHead_o ← Head
    
    // Read ports for operand forwarding during Issue
    ROBReadValue1_o ← entries[RD1_Qi_i].Value
    ROBReadReady1_o ← entries[RD1_Qi_i].Ready
    ROBReadValue2_o ← entries[RD2_Qi_i].Value
    ROBReadReady2_o ← entries[RD2_Qi_i].Ready
    
    // ================================================================
    // FLAGS READ PORT (for flags forwarding during Issue)
    // ================================================================
    // FlagsQi_i comes from Flags_Unit — it is the ROB index of the
    // LATEST pending flag-producing instruction. The ROB uses this
    // as a read address to check if that entry has already completed
    // and captured its flags result.
    ROBReadFlagsReady_o  ← entries[FlagsQi_i].Ready AND entries[FlagsQi_i].FlagsValid
    ROBReadFlagsResult_o ← entries[FlagsQi_i].FlagsResult
    
    // Head entry outputs for Commit_Unit
    ROBHeadBusy_o          ← entries[Head].Busy
    ROBHeadReady_o         ← entries[Head].Ready
    ROBHeadType_o          ← entries[Head].Type
    ROBHeadDestReg_o       ← entries[Head].DestReg
    ROBHeadValue_o         ← entries[Head].Value
    ROBHeadException_o     ← entries[Head].Exception
    ROBHeadPC_o            ← entries[Head].PC
    ROBHeadFlagsResult_o   ← entries[Head].FlagsResult
    ROBHeadFlagsValid_o    ← entries[Head].FlagsValid
    ROBHeadMispredict_o    ← entries[Head].Mispredict
    ROBHeadBranchTarget_o  ← entries[Head].BranchTarget
    ROBHeadStoreAddr_o     ← entries[Head].StoreAddr
    ROBHeadStoreData_o     ← entries[Head].StoreData
    ROBHeadStoreReady_o    ← entries[Head].StoreReady
    ROBHeadSourceStation_o ← entries[Head].SourceStation

// ====================================================================
// SEQUENTIAL UPDATES (rising edge CLK)
// ====================================================================
RISING_EDGE(CLK):

    if Flush_i == 1:
        for i in 0..31: entries[i].Busy ← 0
        Tail ← Head
        Count ← 0
        RETURN
    
    // --- ALLOCATE ---
    if ROBAlloc_i == 1:
        e ← entries[Tail]
        e.Busy           ← 1
        e.Ready          ← 0
        e.Type           ← AllocType_i
        e.DestReg        ← if IsCMPonly_i == 1 then 0xF else Dest_in_i
        e.PC             ← PCCurrent_i
        e.Predicted      ← AllocPredicted_i
        e.ModifiesFlags  ← ModifiesFlags_i
        e.SourceStation  ← AllocSourceStation_i
        e.Value          ← 0
        e.Exception      ← 0x0
        e.FlagsValid     ← 0
        e.StoreReady     ← 0
        e.Mispredict     ← 0
        Tail ← (Tail + 1) mod 32
        Count ← Count + 1
    
    // --- CDB_A UPDATE ---
    if CDBA_Valid_i == 1:
        idx ← CDBA_ROBTag_i
        entries[idx].Ready     ← 1
        entries[idx].Value     ← CDBA_Value_i
        entries[idx].Exception ← CDBA_Exception_i
        if CDBA_FlagsValid_i == 1:
            entries[idx].FlagsResult ← CDBA_Flags_i
            entries[idx].FlagsValid  ← 1
    
    // --- CDB_B UPDATE ---
    if CDBB_Valid_i == 1:
        idx ← CDBB_ROBTag_i
        entries[idx].Ready     ← 1
        entries[idx].Value     ← CDBB_Value_i
        entries[idx].Exception ← CDBB_Exception_i
        if CDBB_FlagsValid_i == 1:
            entries[idx].FlagsResult ← CDBB_Flags_i
            entries[idx].FlagsValid  ← 1
    
    // --- BRANCH RESULT ---
    if BrExResult_Valid_i == 1:
        idx ← BrExResult_ROBTag_i
        entries[idx].Ready        ← 1
        entries[idx].BranchTaken  ← BrExResult_Taken_i
        entries[idx].BranchTarget ← BrExResult_Target_i
        entries[idx].Mispredict   ← BrExResult_Mispredict_i
    
    // --- STORE COMPLETE (SB0) ---
    if SB0_StComplete_Valid_i == 1:
        idx ← SB0_StComplete_ROBTag_i
        entries[idx].StoreAddr  ← SB0_StComplete_Addr_i
        entries[idx].StoreData  ← SB0_StComplete_Data_i
        entries[idx].StoreReady ← 1
        entries[idx].Ready      ← 1
    
    // --- STORE COMPLETE (SB1) ---
    if SB1_StComplete_Valid_i == 1:
        idx ← SB1_StComplete_ROBTag_i
        entries[idx].StoreAddr  ← SB1_StComplete_Addr_i
        entries[idx].StoreData  ← SB1_StComplete_Data_i
        entries[idx].StoreReady ← 1
        entries[idx].Ready      ← 1
    
    // --- COMMIT POP ---
    if CommitPop_i == 1:
        entries[Head].Busy ← 0
        Head ← (Head + 1) mod 32
        Count ← Count - 1
```

### Key behavior
- **Dual CDB**: Both CDB_A and CDB_B can update DIFFERENT ROB entries in the SAME cycle. By design, each instruction gets a unique ROB index, so no collision.
- **ROB Read Ports for operands**: When an instruction is being issued and its operand register has Qi_valid=1, the Issue logic reads ROB[Qi] to check if the value is already Ready. If yes, the value is forwarded immediately. If no, the RS will wait for CDB.
- **ROB Flags Read Port**: Works identically to the operand read ports but for flags. FlagsQi_i (from Flags_Unit) is the read address. If ROB[FlagsQi].Ready AND FlagsValid, the flags are forwarded immediately at Issue time. This eliminates the gap where a flag consumer issued after the producer's CDB broadcast would have to wait until commit.

---

## COMPONENT 11: Flags_Unit

### Purpose
Maintains the architectural flags (N, Z, C, V) and the rename tag for speculative flag producers. Only one rename tag is needed because flags form a single "register" that gets overwritten by each flag-modifying instruction.

### Internal State

```
Flags_arch     (4b)   — Committed N,Z,C,V
Flags_Qi       (5b)   — ROB index of the LATEST pending flag-producing instruction
Flags_Qi_valid (1b)   — 1 = there is a pending flag producer
```

### Pseudocode

```
ALWAYS (combinational):
    FlagsArch_o    ← Flags_arch
    FlagsQi_o      ← Flags_Qi
    FlagsQiValid_o ← Flags_Qi_valid

RISING_EDGE(CLK):
    if Flush_i == 1:
        Flags_Qi_valid ← 0
        RETURN
    
    if FlagsCommitEn_i == 1:
        Flags_arch ← FlagsCommitValue_i
        if Flags_Qi == CommitROBIdx_i AND Flags_Qi_valid == 1:
            Flags_Qi_valid ← 0
    
    if FlagsTagWrEn_i == 1:
        Flags_Qi       ← ROBTail_i
        Flags_Qi_valid ← 1
```

### Key behavior
- If multiple instructions modify flags, only the LATEST one's tag is stored. When an earlier one commits its flags, the tag is NOT cleared (it still points to the newer one). When the LAST one commits, the tag IS cleared.
- RS that need flags (ADC, SBC, conditional branches) capture the FlagsQi at issue time and then either get immediate forwarding from ROB, snoop the CDB for a match, or fall back to architectural flags.

---

## COMPONENTS 12-13: SB0, SB1 (Store Buffers)

### Purpose
Hold store instructions (STR, STRB) while they wait for:
1. Base address register operand to become ready.
2. Data register operand to become ready.
3. AGU to compute the effective address.
After address and data are both resolved, the buffer notifies the ROB via StoreComplete. The actual memory write happens only at COMMIT time.

### Internal State

```
Busy, Op(8b)
BaseValue(64b), BaseTag(5b), BaseTagValid(1b)
Offset(64b)
Address(64b), AddressReady(1b)
DataValue(64b), DataTag(5b), DataTagValid(1b)
ROBTag(5b)
SegFault(1b)
StCompleteNotified(1b)
```

### Pseudocode (SB0; SB1 identical with prefix change)

```
RISING_EDGE(CLK):
    if Flush_i == 1:
        Busy ← 0; RETURN
    
    if FreeSB0_i == 1:
        Busy ← 0; RETURN
    
    // --- ALLOCATE ---
    if AllocSB0_i == 1:
        Busy ← 1
        Op ← Op_in_i
        Offset ← ImmExt_i
        ROBTag ← ROBTail_i
        AddressReady ← 0
        SegFault ← 0
        StCompleteNotified ← 0
        
        // Resolve BASE operand (Rn — read port 1)
        if RD1_QiValid_i == 0:
            BaseValue ← RD1_Value_i
            BaseTagValid ← 0
        else:
            if ROBReadReady1_i == 1:
                BaseValue ← ROBReadValue1_i
                BaseTagValid ← 0
            else:
                BaseTag ← RD1_Qi_i
                BaseTagValid ← 1
        
        // Resolve DATA operand (Rm — read port 2)
        if RD2_QiValid_i == 0:
            DataValue ← RD2_Value_i
            DataTagValid ← 0
        else:
            if ROBReadReady2_i == 1:
                DataValue ← ROBReadValue2_i
                DataTagValid ← 0
            else:
                DataTag ← RD2_Qi_i
                DataTagValid ← 1
    
    // --- CDB SNOOP (every cycle while Busy) ---
    if Busy == 1:
        if CDBA_Valid_i == 1:
            if BaseTagValid == 1 AND BaseTag == CDBA_ROBTag_i:
                BaseValue ← CDBA_Value_i; BaseTagValid ← 0
            if DataTagValid == 1 AND DataTag == CDBA_ROBTag_i:
                DataValue ← CDBA_Value_i; DataTagValid ← 0
        if CDBB_Valid_i == 1:
            if BaseTagValid == 1 AND BaseTag == CDBB_ROBTag_i:
                BaseValue ← CDBB_Value_i; BaseTagValid ← 0
            if DataTagValid == 1 AND DataTag == CDBB_ROBTag_i:
                DataValue ← CDBB_Value_i; DataTagValid ← 0
    
    // --- AGU DONE ---
    if SB0_AGUDone_i == 1:
        Address ← SB0_AGUAddress_i
        AddressReady ← 1
        SegFault ← SB0_AGUSegFault_i

ALWAYS (combinational):
    SB0_Busy_o ← Busy
    
    SB0_AGUReq_o    ← Busy AND (BaseTagValid == 0) AND (AddressReady == 0)
    SB0_AGUBase_o   ← BaseValue
    SB0_AGUOffset_o ← Offset
    SB0_AGUROBTag_o ← ROBTag
    
    complete ← Busy AND AddressReady AND (DataTagValid == 0) AND (StCompleteNotified == 0)
    SB0_StComplete_Valid_o  ← complete
    SB0_StComplete_ROBTag_o ← ROBTag
    SB0_StComplete_Addr_o   ← Address
    SB0_StComplete_Data_o   ← DataValue
    
    if complete == 1:
        StCompleteNotified ← 1  // Latch on next clock edge
```

### Key behavior
- **Stores NEVER publish to CDB**. They don't produce a register value. They only notify the ROB that address+data are ready.
- **The actual memory write happens at COMMIT** (via Commit_Unit → Memory_Arbiter → D_Cache). This prevents speculative memory corruption.
- **Stores do NOT block Issue**. Even if a store's base or data isn't ready, the instruction is already parked in the SB and the pipeline continues issuing new instructions.

---

## COMPONENTS 14-16: LB0, LB1, LB2 (Load Buffers)

### Purpose
Hold load instructions (LDR, LDRB) while waiting for the base address, then request memory access, then publish the loaded value to CDB.

### Internal State

```
Busy, Op(8b)
BaseValue(64b), BaseTag(5b), BaseTagValid(1b)
Offset(64b)
Address(64b), AddressReady(1b)
DestROBTag(5b)
SegFault(1b)
MemRequested(1b), MemDone_int(1b), LoadedData(64b)
CDBRequested(1b)
```

### Pseudocode (LB0; LB1, LB2 identical with prefix change)

```
RISING_EDGE(CLK):
    if Flush_i == 1:
        Busy ← 0; RETURN
    if FreeLB0_i == 1:
        Busy ← 0; RETURN
    
    // --- ALLOCATE ---
    if AllocLB0_i == 1:
        Busy ← 1
        Op ← Op_in_i
        Offset ← ImmExt_i
        DestROBTag ← ROBTail_i
        AddressReady ← 0
        SegFault ← 0
        MemRequested ← 0
        MemDone_int ← 0
        CDBRequested ← 0
        
        if RD1_QiValid_i == 0:
            BaseValue ← RD1_Value_i; BaseTagValid ← 0
        else:
            if ROBReadReady1_i == 1:
                BaseValue ← ROBReadValue1_i; BaseTagValid ← 0
            else:
                BaseTag ← RD1_Qi_i; BaseTagValid ← 1
    
    // --- CDB SNOOP (for base operand only) ---
    if Busy == 1:
        if CDBA_Valid_i == 1 AND BaseTagValid == 1 AND BaseTag == CDBA_ROBTag_i:
            BaseValue ← CDBA_Value_i; BaseTagValid ← 0
        if CDBB_Valid_i == 1 AND BaseTagValid == 1 AND BaseTag == CDBB_ROBTag_i:
            BaseValue ← CDBB_Value_i; BaseTagValid ← 0
    
    if LB0_AGUDone_i == 1:
        Address ← LB0_AGUAddress_i; AddressReady ← 1; SegFault ← LB0_AGUSegFault_i
    
    if LB0_MemDone_i == 1:
        LoadedData ← LB0_MemData_i; MemDone_int ← 1
    
    if LB0_CDBStall_i == 1:
        CDBRequested ← 0

ALWAYS (combinational):
    LB0_Busy_o ← Busy
    
    LB0_AGUReq_o    ← Busy AND (BaseTagValid == 0) AND (AddressReady == 0)
    LB0_AGUBase_o   ← BaseValue
    LB0_AGUOffset_o ← Offset
    LB0_AGUROBTag_o ← DestROBTag
    
    LB0_MemReq_o    ← Busy AND AddressReady AND (SegFault == 0) AND (MemRequested == 0)
    LB0_MemAddr_o   ← Address
    LB0_MemOp_o     ← Op
    LB0_MemROBTag_o ← DestROBTag
    
    if SegFault == 1:
        LB0_CDBReq_o       ← Busy AND AddressReady AND (CDBRequested == 0)
        LB0_CDBValue_o      ← 0
        LB0_CDBException_o  ← 0x2
    else:
        LB0_CDBReq_o       ← Busy AND MemDone_int AND (CDBRequested == 0)
        LB0_CDBValue_o      ← LoadedData
        LB0_CDBException_o  ← 0x0
    LB0_CDBROBTag_o ← DestROBTag
```

### Key behavior
- **Loads DO publish to CDB** (unlike stores). The loaded value needs to reach the ROB and any RS waiting for it.
- **Loads progress through 3 phases**: (1) Wait for base → (2) AGU computes address → (3) Memory access → CDB publish.

---

## COMPONENTS 17-18: RS_IntALU0, RS_IntALU1 (Integer ALU Reservation Stations)

### Purpose
Hold integer ALU instructions while waiting for operands (and optionally flags). When all inputs are ready, request the IntALU execution unit via the arbiter.

### Internal State

```
Busy, Op(8b)
Vj(64b), Qj(5b), QjValid(1b)      — operand 1 (Rn)
Vk(64b), Qk(5b), QkValid(1b)      — operand 2 (Rm or Imm)
DestROBTag(5b)
UseImm(1b)
ModifiesFlags(1b), NeedsFlags(1b)
FlagsQi(5b), FlagsQiValid(1b)      — tag of flag producer
FlagsValue(4b)                      — captured flags (for ADC/SBC)
Dispatched(1b)                      — sent to ALU, waiting for result
```

### Pseudocode (RS_IntALU0; RS_IntALU1 identical with prefix change)

```
RISING_EDGE(CLK):
    if Flush_i == 1:
        Busy ← 0; RETURN
    if FreeRSIAU0_i == 1:
        Busy ← 0; RETURN
    
    // --- ALLOCATE ---
    if AllocRSIntALU0_i == 1:
        Busy ← 1
        Op ← Op_in_i
        DestROBTag ← ROBTail_i
        UseImm ← UseImm_i
        ModifiesFlags ← ModifiesFlags_i
        NeedsFlags ← NeedsFlags_i
        Dispatched ← 0
        
        // === OPERAND 1 (Vj) — always from Rn ===
        if RD1_QiValid_i == 0:
            Vj ← RD1_Value_i; QjValid ← 0
        else:
            if ROBReadReady1_i == 1:
                Vj ← ROBReadValue1_i; QjValid ← 0
            else:
                Qj ← RD1_Qi_i; QjValid ← 1
        
        // === OPERAND 2 (Vk) ===
        if UseImm_i == 1:
            Vk ← ImmExt_i; QkValid ← 0
        else:
            if RD2_QiValid_i == 0:
                Vk ← RD2_Value_i; QkValid ← 0
            else:
                if ROBReadReady2_i == 1:
                    Vk ← ROBReadValue2_i; QkValid ← 0
                else:
                    Qk ← RD2_Qi_i; QkValid ← 1
        
        // === FLAGS DEPENDENCY (3-tier resolution) ===
        if NeedsFlags_i == 1:
            if FlagsQiValid_i == 0:
                // TIER 1: No pending producer — use architectural flags
                FlagsValue ← FlagsArch_i; FlagsQiValid ← 0
            else:
                // There IS a pending producer. Check ROB read port.
                if ROBReadFlagsReady_i == 1:
                    // TIER 2: Producer already completed — forward from ROB
                    FlagsValue ← ROBReadFlagsResult_i; FlagsQiValid ← 0
                else:
                    // TIER 3: Producer still executing — wait for CDB
                    FlagsQi ← FlagsQi_i; FlagsQiValid ← 1
        else:
            FlagsQiValid ← 0
        
        // SPECIAL CASES:
        if Op in {0x31, 0x32}:  // MOV, MVN
            QjValid ← 0  // Only need Rm (read from Vk via port 2)
    
    // --- CDB SNOOP (every cycle while Busy and not yet dispatched) ---
    if Busy == 1:
        for each CDB in {A, B}:
            if CDBX_Valid == 1:
                if QjValid == 1 AND Qj == CDBX_ROBTag:
                    Vj ← CDBX_Value; QjValid ← 0
                if QkValid == 1 AND Qk == CDBX_ROBTag:
                    Vk ← CDBX_Value; QkValid ← 0
                if FlagsQiValid == 1 AND NeedsFlags == 1 AND FlagsQi == CDBX_ROBTag:
                    if CDBX_FlagsValid == 1:
                        FlagsValue ← CDBX_Flags; FlagsQiValid ← 0
        
        // FALLBACK: If Flags_Unit reports no pending producer anymore,
        // the architectural flags are authoritative.
        if FlagsQiValid == 1 AND FlagsQiValid_i == 0:
            FlagsValue ← FlagsArch_i
            FlagsQiValid ← 0

ALWAYS (combinational):
    RS_IntALU0_Busy_o ← Busy
    
    Ready ← Busy AND (QjValid == 0) AND (QkValid == 0) AND (Dispatched == 0)
             AND ((NeedsFlags == 0) OR (FlagsQiValid == 0))
    
    RSIAU0_Req_o          ← Ready
    RSIAU0_Op_o           ← Op
    RSIAU0_Vj_o           ← Vj
    RSIAU0_Vk_o           ← Vk
    RSIAU0_DestROBTag_o   ← DestROBTag
    RSIAU0_ModFlags_o     ← ModifiesFlags
    RSIAU0_NeedsFlags_o   ← NeedsFlags
    RSIAU0_FlagsValue_o   ← FlagsValue
    
    if RSIAU0_Grant_i == 1:
        Dispatched ← 1
```

### Key behavior — 3-Tier Flags Resolution

Flags dependencies are resolved through three mechanisms, checked in order of priority:

1. **TIER 1 — Architectural flags (FlagsQiValid_i == 0)**: No pending flag producer exists. The committed flags in Flags_Unit are the correct value. Captured immediately at Issue time.

2. **TIER 2 — ROB flags read port (ROBReadFlagsReady_i == 1)**: A flag producer exists but has already completed execution and published its flags to the ROB via CDB. The ROB forwards the speculative flags immediately at Issue time. **This is the new fix that eliminates the forwarding gap.**

3. **TIER 3 — CDB snoop (wait for broadcast)**: The flag producer is still executing. The RS records FlagsQi and waits. When the producer publishes on CDB, the RS captures the flags in the same cycle.

4. **FALLBACK — Architectural flags (secondary)**: If the RS is waiting for a CDB broadcast (Tier 3) but the Flags_Unit's FlagsQiValid_o drops to 0 (meaning the producer committed while the RS was waiting), the RS captures the now-updated architectural flags.

This eliminates the scenario where a flag consumer issued AFTER the producer's CDB broadcast would have to wait until commit — Tier 2 catches this case immediately.

---

## COMPONENT 19: RS_FPALU0 (FP ALU Reservation Station)

### Structure and logic
Identical to RS_IntALU0 with these differences:
- Prefix: `RSFPAU0_` instead of `RSIAU0_`
- Connected to FPALU_Arbiter instead of IntALU_Arbiter
- **NeedsFlags**: Always 0 for FP operations (no ADC/SBC equivalent in FP), EXCEPT for FCMP variants which modify flags
- **ModifiesFlags**: Only 1 for FCMP/FCMN/FCMPS and their immediate variants
- **Flags resolution**: Uses the same 3-tier mechanism (Arch → ROB read → CDB snoop + fallback) as RS_IntALU0, though in practice NeedsFlags is rarely 1 for FPALU instructions

### Special handling for unary FP ops

```
// During ALLOCATE for unary FP ops (0x29, 0x2A, 0x2B..0x30):
QjValid ← 0   // No first operand
// Vk comes from Rm (via RD2), which is the only operand
// For imm variants (0x53..0x5A): Vk ← ImmExt, QkValid ← 0
```

---

## COMPONENT 20: RS_IntMUL0 (Integer MUL Reservation Station)

### Structure and logic
Identical to RS_IntALU0 with additions:
- Prefix: `RSIMUL0_` instead of `RSIAU0_`
- Connected to IntMUL_Arbiter instead of IntALU_Arbiter
- Additional internal bit: `Executing` — set to 1 when dispatched, cleared when freed.
- **Flags resolution**: Same 3-tier mechanism as RS_IntALU0.

### Key difference from ALU stations

```
Ready ← Busy AND (QjValid == 0) AND (QkValid == 0) AND (Executing == 0)
         AND ((NeedsFlags == 0) OR (FlagsQiValid == 0))

if RSIMUL0_Grant_i == 1:
    Executing ← 1
```

This is critical because MUL/DIV are multi-cycle (4-16 cycles). Without the Executing flag, the arbiter would try to re-dispatch the same instruction every cycle.

---

## COMPONENT 21: RS_FPMUL0 (FP MUL/DIV/SQRT Reservation Station)

### Structure and logic
Identical to RS_IntMUL0 (has Executing bit) with prefix `RSFPMUL0_`, connected to FPMUL_Arbiter.
Uses the same 3-tier flags resolution mechanism.

### Special handling for FSQRT

```
// FSQRT uses only ONE operand: Rd ← sqrt(Rm)
// During ALLOCATE for FSQRT (0x28):
QjValid ← 0  // No Vj
// Vk comes from Rm (via RD2)
// For FSQRTI (0x52): Vk ← ImmExt, QkValid ← 0
```

---

## COMPONENT 22: RS_Branch0 (Branch Reservation Station)

### Purpose
Holds a branch instruction while waiting for flags (conditional branches) or immediately ready (unconditional B). Does NOT need register operands — branch target is computed from PC + immediate offset.

### Internal State

```
Busy, Op(8b)
TargetPC(64b)          — pre-computed branch target address
PredictedTaken(1b)     — static prediction used (always 0 = not-taken)
ROBTag(5b)
FlagsQi(5b), FlagsQiValid(1b)
FlagsValue(4b)
```

### Pseudocode

```
RISING_EDGE(CLK):
    if Flush_i == 1: Busy ← 0; RETURN
    if FreeRSBr0_i == 1: Busy ← 0; RETURN
    
    // --- ALLOCATE ---
    if AllocRSBranch0_i == 1:
        Busy ← 1
        Op ← Op_in_i
        ROBTag ← ROBTail_i
        TargetPC ← PCCurrent_i + ImmExt_i
        PredictedTaken ← 0
        
        // === FLAGS DEPENDENCY (3-tier resolution) ===
        if Op == 0x45:  // B (unconditional)
            FlagsQiValid ← 0
        else:  // Conditional branches — need flags
            if FlagsQiValid_i == 0:
                // TIER 1: No pending producer — use architectural flags
                FlagsValue ← FlagsArch_i; FlagsQiValid ← 0
            else:
                if ROBReadFlagsReady_i == 1:
                    // TIER 2: Producer completed — forward from ROB
                    FlagsValue ← ROBReadFlagsResult_i; FlagsQiValid ← 0
                else:
                    // TIER 3: Producer still executing — wait for CDB
                    FlagsQi ← FlagsQi_i; FlagsQiValid ← 1
    
    // --- CDB SNOOP (only for flags) ---
    if Busy == 1:
        for each CDB in {A, B}:
            if CDBX_Valid == 1 AND CDBX_FlagsValid == 1:
                if FlagsQiValid == 1 AND FlagsQi == CDBX_ROBTag:
                    FlagsValue ← CDBX_Flags; FlagsQiValid ← 0
        
        // FALLBACK: architectural flags became authoritative
        if FlagsQiValid == 1 AND FlagsQiValid_i == 0:
            FlagsValue ← FlagsArch_i
            FlagsQiValid ← 0

ALWAYS (combinational):
    RS_Branch0_Busy_o ← Busy
    
    if Op == 0x45:
        Ready ← Busy
    else:
        Ready ← Busy AND (FlagsQiValid == 0)
    
    RSBr0_Req_o        ← Ready
    RSBr0_Op_o         ← Op
    RSBr0_TargetPC_o   ← TargetPC
    RSBr0_FlagsValue_o ← FlagsValue
    RSBr0_PredTaken_o  ← PredictedTaken
    RSBr0_ROBTag_o     ← ROBTag
```

### Key behavior
- **Branches don't use register operands** in this ISA. The target address is PC-relative (from the immediate field). The only data dependency is on FLAGS for conditional branches.
- **Unconditional B** is always immediately ready and can execute the same cycle it's dispatched.
- **3-Tier flags resolution** (same as all flag-consuming RS): Arch → ROB read → CDB snoop + fallback. This ensures a conditional branch issued after the flag producer's CDB broadcast can still get the flags immediately via the ROB read port (Tier 2), rather than waiting for commit.

---

## COMPONENT 23: AGU_Arbiter (Address Generation Unit Arbiter)

### Purpose
Coordinates access from 5 requestors (SB0, SB1, LB0, LB1, LB2) to 2 AGU units (AGU_0, AGU_1). Each cycle it can dispatch UP TO 2 address calculations simultaneously. It also routes results BACK from the AGUs to the correct requestor.

### Pseudocode

```
ALWAYS (combinational):
    AGU0_Start_o ← 0; AGU1_Start_o ← 0
    SB0_AGUDone_o ← 0; SB1_AGUDone_o ← 0
    LB0_AGUDone_o ← 0; LB1_AGUDone_o ← 0; LB2_AGUDone_o ← 0

    // PHASE 1: ROUTE RESULTS BACK FROM AGUs TO REQUESTORS
    if AGU0_Done_i == 1:
        CASE AGU0_SourceID_i OF:
            000: SB0_AGUDone_o ← 1; SB0_AGUAddress_o ← AGU0_Address_i; SB0_AGUSegFault_o ← AGU0_SegFault_i
            001: SB1_AGUDone_o ← 1; SB1_AGUAddress_o ← AGU0_Address_i; SB1_AGUSegFault_o ← AGU0_SegFault_i
            010: LB0_AGUDone_o ← 1; LB0_AGUAddress_o ← AGU0_Address_i; LB0_AGUSegFault_o ← AGU0_SegFault_i
            011: LB1_AGUDone_o ← 1; LB1_AGUAddress_o ← AGU0_Address_i; LB1_AGUSegFault_o ← AGU0_SegFault_i
            100: LB2_AGUDone_o ← 1; LB2_AGUAddress_o ← AGU0_Address_i; LB2_AGUSegFault_o ← AGU0_SegFault_i

    if AGU1_Done_i == 1:
        CASE AGU1_SourceID_i OF:
            000: SB0_AGUDone_o ← 1; SB0_AGUAddress_o ← AGU1_Address_i; SB0_AGUSegFault_o ← AGU1_SegFault_i
            001: SB1_AGUDone_o ← 1; SB1_AGUAddress_o ← AGU1_Address_i; SB1_AGUSegFault_o ← AGU1_SegFault_i
            010: LB0_AGUDone_o ← 1; LB0_AGUAddress_o ← AGU1_Address_i; LB0_AGUSegFault_o ← AGU1_SegFault_i
            011: LB1_AGUDone_o ← 1; LB1_AGUAddress_o ← AGU1_Address_i; LB1_AGUSegFault_o ← AGU1_SegFault_i
            100: LB2_AGUDone_o ← 1; LB2_AGUAddress_o ← AGU1_Address_i; LB2_AGUSegFault_o ← AGU1_SegFault_i

    // PHASE 2: DISPATCH NEW REQUESTS TO FREE AGUs
    // Priority: SB0 > SB1 > LB0 > LB1 > LB2
    pending ← []
    if SB0_AGUReq_i == 1: pending.append({id=000, base=SB0_AGUBase_i, offset=SB0_AGUOffset_i})
    if SB1_AGUReq_i == 1: pending.append({id=001, base=SB1_AGUBase_i, offset=SB1_AGUOffset_i})
    if LB0_AGUReq_i == 1: pending.append({id=010, base=LB0_AGUBase_i, offset=LB0_AGUOffset_i})
    if LB1_AGUReq_i == 1: pending.append({id=011, base=LB1_AGUBase_i, offset=LB1_AGUOffset_i})
    if LB2_AGUReq_i == 1: pending.append({id=100, base=LB2_AGUBase_i, offset=LB2_AGUOffset_i})

    if pending.size() >= 1 AND AGU0_Free_i == 1:
        p ← pending[0]
        AGU0_Start_o ← 1; AGU0_Base_o ← p.base; AGU0_Offset_o ← p.offset; AGU0_SourceID_o ← p.id
        pending.remove(0)

    if pending.size() >= 1 AND AGU1_Free_i == 1:
        p ← pending[0]
        AGU1_Start_o ← 1; AGU1_Base_o ← p.base; AGU1_Offset_o ← p.offset; AGU1_SourceID_o ← p.id
```

### Key behavior
- **Two AGUs = two address calculations per cycle**.
- **Stores get priority** over loads to minimize commit stalls.
- **The SourceID tag** tracks which requestor sent the job so the result is routed back correctly.

---

## COMPONENTS 24-25: AGU_0, AGU_1 (Address Generation Units)

### Purpose
Computes effective address = Base + Offset, then checks against segmentation bounds (LOWER ≤ Address ≤ UPPER). Pure combinational — result is available in the SAME cycle.

### Pseudocode (AGU_0; AGU_1 identical with prefix change)

```
ALWAYS (combinational):
    AGU0_Free_o ← 1   // Always free (combinational, no pipeline)
    AGU0_Done_o ← 0

    if AGU0_Start_i == 1:
        Address ← AGU0_Base_i + AGU0_Offset_i

        if (Address < LOWER_i) OR (Address > UPPER_i):
            SegFault ← 1
        else:
            SegFault ← 0

        AGU0_Done_o      ← 1
        AGU0_Address_o   ← Address
        AGU0_SegFault_o  ← SegFault
        AGU0_SourceID_o  ← AGU0_SourceID_i
```

### Key behavior
- **Combinational**: no pipeline stages. Input arrives, output is ready the same cycle.
- **SegFault does NOT halt execution** — it's captured in the requesting SB/LB and eventually propagated to the ROB. The exception is handled at Commit time.

---

## COMPONENT 26: IntALU_Arbiter

### Purpose
Selects between RS_IntALU0 and RS_IntALU1 to grant access to the single IntALU execution unit.

### Pseudocode

```
ALWAYS (combinational):
    RSIAU0_Grant_o ← 0; RSIAU1_Grant_o ← 0; IntALU_Start_o ← 0

    if IntALU_Free_i == 1:
        if RSIAU0_Req_i == 1:
            RSIAU0_Grant_o ← 1; IntALU_Start_o ← 1
            IntALU_Op_o ← RSIAU0_Op_i; IntALU_A_o ← RSIAU0_Vj_i; IntALU_B_o ← RSIAU0_Vk_i
            IntALU_DestROBTag_o ← RSIAU0_DestROBTag_i; IntALU_ModFlags_o ← RSIAU0_ModFlags_i
            IntALU_NeedsFlags_o ← RSIAU0_NeedsFlags_i; IntALU_FlagsIn_o ← RSIAU0_FlagsValue_i
        else if RSIAU1_Req_i == 1:
            RSIAU1_Grant_o ← 1; IntALU_Start_o ← 1
            IntALU_Op_o ← RSIAU1_Op_i; IntALU_A_o ← RSIAU1_Vj_i; IntALU_B_o ← RSIAU1_Vk_i
            IntALU_DestROBTag_o ← RSIAU1_DestROBTag_i; IntALU_ModFlags_o ← RSIAU1_ModFlags_i
            IntALU_NeedsFlags_o ← RSIAU1_NeedsFlags_i; IntALU_FlagsIn_o ← RSIAU1_FlagsValue_i
```

### Key behavior
- **Single-cycle arbitration**: one grant per cycle, strict priority RS0 > RS1.
- Having TWO RS feeding ONE ALU decouples issue from execution.

---

## COMPONENT 27: IntALU (Integer ALU)

### Purpose
Performs all integer arithmetic, logic, shift, comparison, and move operations in a SINGLE CYCLE. Purely combinational.

### Pseudocode

```
ALWAYS (combinational):
    IntALU_Free_o ← NOT IntALU_CDBStall_i
    IntALU_CDBReq_o ← 0
    IntALU_CDBFlagsValid_o ← 0

    if IntALU_Start_i == 1:
        op ← IntALU_Op_i
        A  ← IntALU_A_i; B ← IntALU_B_i
        cin ← 0
        if IntALU_NeedsFlags_i == 1:
            cin ← IntALU_FlagsIn_i[1]   // Carry flag is bit[1] of N,Z,C,V

        CASE op OF:
            0x00, 0x0E: Result ← A + B                          // ADD, ADDI
            0x01, 0x0F: Result ← A - B                          // SUB, SUBI
            0x02, 0x10: Result ← A + B + cin                    // ADC, ADCI
            0x03, 0x11: Result ← A - B - (NOT cin)              // SBC, SBCI
            0x1C:       Result ← A + 1                          // INC
            0x1D:       Result ← A - 1                          // DEC
            0x06, 0x14: Result ← A AND B                        // AND, ANDI
            0x07, 0x15: Result ← A OR B                         // ORR, ORRI
            0x08, 0x16: Result ← A XOR B                        // EOR, EORI
            0x09, 0x17: Result ← A AND (NOT B)                  // BIC, BICI
            0x0A, 0x18: Result ← A << (B mod 64)               // LSL, LSLI
            0x0B, 0x19: Result ← A >> (B mod 64) (logical)     // LSR, LSRI
            0x0C, 0x1A: Result ← A >>> (B mod 64) (arithmetic) // ASR, ASRI
            0x0D, 0x1B: Result ← rotate_right(A, B mod 64)     // ROR, RORI
            0x31:       Result ← B                              // MOV
            0x32:       Result ← NOT B                          // MVN
            0x33:       Result ← B                              // MOVI
            0x34:       Result ← NOT B                          // MVNI
            0x35:       Result ← B                              // FMOVI
            0x36:       Result ← NOT B                          // FMVNI
            0x37, 0x3B: temp ← A - B; Result ← 0               // CMP, CMPI
            0x38, 0x3C: temp ← A + B; Result ← 0               // CMN, CMNI
            0x39, 0x3D: temp ← A AND B; Result ← 0             // TST, TSTI
            0x3A, 0x3E: temp ← A XOR B; Result ← 0             // TEQ, TEQI

        if IntALU_ModFlags_i == 1:
            N ← Result[63]   // (or temp[63] for CMP/CMN/TST/TEQ)
            Z ← (Result == 0) ? 1 : 0
            // C: carry_out for add/inc/cmn, borrow_out(inverted) for sub/dec/cmp, last-bit-out for shifts
            // V: signed overflow for add/sub/adc/sbc/inc/dec/cmp/cmn
            Flags ← {N, Z, C, V}
            IntALU_CDBFlagsValid_o ← 1
            IntALU_CDBFlags_o      ← Flags
        else:
            IntALU_CDBFlagsValid_o ← 0

        IntALU_CDBReq_o       ← 1
        IntALU_CDBROBTag_o    ← IntALU_DestROBTag_i
        IntALU_CDBValue_o     ← Result
        IntALU_CDBException_o ← 0x0
```

### Key behavior
- **Single-cycle latency**: purely combinational.
- **CDB stall handling**: if CDB_Arbiter denies publish, IntALU holds result and sets Free_o = 0.
- **For CMP/CMN/TST/TEQ**: Result is meaningless (DestReg=0xF). Only Flags matter.

---

## COMPONENT 28: FPALU_Arbiter

### Purpose
Selects from RS_FPALU0 to grant access to the FPALU. Simple pass-through when free.

### Pseudocode

```
ALWAYS (combinational):
    RSFPAU0_Grant_o ← 0; FPALU_Start_o ← 0

    if FPALU_Free_i == 1 AND RSFPAU0_Req_i == 1:
        RSFPAU0_Grant_o ← 1; FPALU_Start_o ← 1
        FPALU_Op_o ← RSFPAU0_Op_i; FPALU_A_o ← RSFPAU0_Vj_i; FPALU_B_o ← RSFPAU0_Vk_i
        FPALU_DestROBTag_o ← RSFPAU0_DestROBTag_i; FPALU_ModFlags_o ← RSFPAU0_ModFlags_i
        FPALU_NeedsFlags_o ← RSFPAU0_NeedsFlags_i; FPALU_FlagsIn_o ← RSFPAU0_FlagsValue_i
```

---

## COMPONENT 29: FPALU (Floating-Point ALU)

### Purpose
Performs floating-point addition, subtraction, comparisons, and unary operations. 3-stage pipeline — the unit can accept a new instruction every cycle while previous ones progress through the pipeline.

### Internal State

```
stage1.active, stage1.op, stage1.A, stage1.B, stage1.ROBTag, stage1.ModFlags
stage2.active, stage2.partial_result, stage2.ROBTag, stage2.ModFlags, stage2.op, stage2.A, stage2.B
stage3.active, stage3.result, stage3.ROBTag, stage3.flags, stage3.flagsValid, stage3.exception

output_hold.active, output_hold.{result, ROBTag, flags, flagsValid, exception}
```

### Pseudocode

```
RISING_EDGE(CLK):

    // STAGE 3 → OUTPUT
    if stage3.active == 1:
        if FPALU_CDBStall_i == 0:
            stage3.active ← 0
        else:
            output_hold ← stage3  // Hold, stall pipeline

    // STAGE 2 → STAGE 3
    if stage2.active == 1 AND (stage3.active == 0 OR stage3 just cleared):
        stage3.active ← 1
        stage3.result ← normalize_and_round(stage2.partial_result)
        stage3.ROBTag ← stage2.ROBTag
        if stage2.op in {0x3F..0x44}:  // FCMP variants
            // Generate flags: N=less, Z=equal, C=greater_or_equal, V=unordered(NaN)
            stage3.flags ← compute_fcmp_flags(stage2.A, stage2.B)
            stage3.flagsValid ← 1
        else:
            stage3.flagsValid ← 0
        stage3.exception ← check_fp_exception(stage2.partial_result)
        stage2.active ← 0

    // STAGE 1 → STAGE 2
    if stage1.active == 1 AND stage2.active == 0:
        stage2.active ← 1; stage2.op ← stage1.op
        CASE stage1.op OF:
            0x1E, 0x23: partial ← fp_add(stage1.A, stage1.B)
            0x1F, 0x24: partial ← fp_sub(stage1.A, stage1.B)
            0x22, 0x27: partial ← copy_sign(stage1.A, stage1.B)
            0x29, 0x53: partial ← negate(stage1.B)
            0x2A, 0x54: partial ← abs(stage1.B)
            0x2B, 0x55: partial ← convert_double_to_int(stage1.B)
            0x2C, 0x56: partial ← convert_int_to_double(stage1.B)
            0x2D, 0x57: partial ← round_nearest(stage1.B)
            0x2E, 0x58: partial ← round_toward_zero(stage1.B)
            0x2F, 0x59: partial ← round_toward_positive(stage1.B)
            0x30, 0x5A: partial ← round_nearest_even(stage1.B)
            0x3F..0x44: partial ← 0  // FCMP: result dummy
        stage2.partial_result ← partial
        stage2.A ← stage1.A; stage2.B ← stage1.B
        stage2.ROBTag ← stage1.ROBTag; stage2.ModFlags ← stage1.ModFlags
        stage1.active ← 0

    // INPUT → STAGE 1
    if FPALU_Start_i == 1 AND stage1.active == 0:
        stage1 ← {active=1, op=FPALU_Op_i, A=FPALU_A_i, B=FPALU_B_i,
                   ROBTag=FPALU_DestROBTag_i, ModFlags=FPALU_ModFlags_i}

ALWAYS (combinational):
    FPALU_Free_o ← (stage1.active == 0) AND (output_hold.active == 0)

    if stage3.active == 1 OR output_hold.active == 1:
        src ← output_hold if output_hold.active else stage3
        FPALU_CDBReq_o ← 1
        FPALU_CDBROBTag_o ← src.ROBTag; FPALU_CDBValue_o ← src.result
        FPALU_CDBException_o ← src.exception; FPALU_CDBFlags_o ← src.flags
        FPALU_CDBFlagsValid_o ← src.flagsValid
    else:
        FPALU_CDBReq_o ← 0
```

### Key behavior — Pipelining
- **3-cycle latency**, throughput of 1 per cycle.
- **CDB stall backpressure** propagates from stage 3 → stage 2 → stage 1 → Free_o = 0.

---

## COMPONENT 30: IntMUL_Arbiter

Identical structure to FPALU_Arbiter. Prefix `RSFPAU0_` → `RSIMUL0_`, `FPALU_` → `IntMUL_`.

---

## COMPONENT 31: IntMUL (Integer Multiplier/Divider)

### Purpose
Performs integer multiplication (4 cycles) and division (16 cycles). NOT pipelined.

### Internal State

```
busy, cycles_remaining
result(64b), ROBTag(5b), flags(4b), flagsValid(1b), exception(4b)
op(8b), A(64b), B(64b)
```

### Pseudocode

```
RISING_EDGE(CLK):
    if busy == 1:
        cycles_remaining ← cycles_remaining - 1
        if cycles_remaining == 0:
            CASE op OF:
                0x04, 0x12: result ← A * B
                0x05, 0x13:
                    if B == 0: exception ← 0x3; result ← 0
                    else: result ← A / B (signed)
            N ← result[63]; Z ← (result == 0); C ← 0; V ← overflow
            flags ← {N, Z, C, V}; flagsValid ← 1

    if busy == 0 AND IntMUL_Start_i == 1:
        busy ← 1; op ← IntMUL_Op_i; A ← IntMUL_A_i; B ← IntMUL_B_i
        ROBTag ← IntMUL_DestROBTag_i
        if op in {0x04, 0x12}: cycles_remaining ← 4
        else: cycles_remaining ← 16

ALWAYS (combinational):
    IntMUL_Free_o ← (busy == 0)

    if busy == 1 AND cycles_remaining == 0:
        IntMUL_CDBReq_o ← 1; IntMUL_CDBROBTag_o ← ROBTag
        IntMUL_CDBValue_o ← result; IntMUL_CDBException_o ← exception
        IntMUL_CDBFlags_o ← flags; IntMUL_CDBFlagsValid_o ← flagsValid
        if IntMUL_CDBStall_i == 0: busy ← 0
    else:
        IntMUL_CDBReq_o ← 0
```

### Key behavior
- **Blocking execution**: while a MUL/DIV is in progress, NO new operation can start.
- **Issue is NOT blocked**: only a SECOND MUL instruction would stall at Issue.

---

## COMPONENT 32: FPMUL_Arbiter

Identical structure to IntMUL_Arbiter. Prefix `RSIMUL0_` → `RSFPMUL0_`, `IntMUL_` → `FPMUL_`.

---

## COMPONENT 33: FPMUL (FP Multiplier/Divider/Sqrt)

### Purpose
Performs FMUL (5 cycles), FDIV (20 cycles), FSQRT (25 cycles). NOT pipelined.

### Pseudocode

```
// Identical structure to IntMUL with different operations and latencies:
RISING_EDGE(CLK):
    if busy == 0 AND FPMUL_Start_i == 1:
        busy ← 1; op ← FPMUL_Op_i; A ← FPMUL_A_i; B ← FPMUL_B_i
        ROBTag ← FPMUL_DestROBTag_i
        CASE op OF:
            0x20, 0x25: cycles_remaining ← 5    // FMUL, FMULI
            0x21, 0x26: cycles_remaining ← 20   // FDIV, FDIVI
            0x28, 0x52: cycles_remaining ← 25   // FSQRT, FSQRTI

    if busy == 1:
        cycles_remaining ← cycles_remaining - 1
        if cycles_remaining == 0:
            CASE op OF:
                0x20, 0x25: result ← fp_multiply(A, B)
                0x21, 0x26:
                    if is_zero(B): exception ← 0x4; result ← fp_infinity(...)
                    else: result ← fp_divide(A, B)
                0x28, 0x52:
                    if is_negative(B) AND NOT is_zero(B):
                        exception ← 0x5; result ← fp_NaN()
                    else: result ← fp_sqrt(B)
            flagsValid ← 0  // FPMUL never generates flags

    // CDB output logic identical to IntMUL
```

---

## COMPONENT 34: Branch_Arbiter

### Purpose
Passes through from RS_Branch0 to Branch_Executor (single RS, single executor).

### Pseudocode

```
ALWAYS (combinational):
    RSBr0_Grant_o ← 0; BrEx_Start_o ← 0

    if RSBr0_Req_i == 1:
        RSBr0_Grant_o ← 1; BrEx_Start_o ← 1
        BrEx_Op_o ← RSBr0_Op_i; BrEx_TargetPC_o ← RSBr0_TargetPC_i
        BrEx_FlagsValue_o ← RSBr0_FlagsValue_i; BrEx_PredTaken_o ← RSBr0_PredTaken_i
        BrEx_ROBTag_o ← RSBr0_ROBTag_i
```

---

## COMPONENT 35: Branch_Executor

### Purpose
Evaluates the branch condition against the flags and detects mispredictions. Results go DIRECTLY to ROB (NOT via CDB).

### Pseudocode

```
ALWAYS (combinational):
    BrExResult_Valid_o ← 0

    if BrEx_Start_i == 1:
        flags ← BrEx_FlagsValue_i
        N ← flags[3]; Z ← flags[2]; C ← flags[1]; V ← flags[0]

        CASE BrEx_Op_i OF:
            0x45: taken ← 1                           // B   (unconditional)
            0x46: taken ← (Z == 1)                     // BEQ
            0x47: taken ← (Z == 0)                     // BNE
            0x48: taken ← (N != V)                     // BLT
            0x49: taken ← (Z == 0) AND (N == V)        // BGT
            0x4A: taken ← (V == 1)                     // BUN
            0x4B: taken ← (V == 0)                     // BORD

        predicted ← BrEx_PredTaken_i
        mispredict ← (taken != predicted)

        BrExResult_Valid_o      ← 1
        BrExResult_ROBTag_o     ← BrEx_ROBTag_i
        BrExResult_Taken_o      ← taken
        BrExResult_Target_o     ← BrEx_TargetPC_i
        BrExResult_Mispredict_o ← mispredict
```

### Key behavior
- **No CDB needed**: branches don't produce register values.
- **Misprediction penalty**: all speculatively-fetched instructions after the branch are flushed at commit time.

---

## COMPONENT 36: CDB_Arbiter (Common Data Bus Arbiter)

### Purpose
Selects up to 2 out of 7 requestors per cycle and routes them to CDB_A and CDB_B.

### Pseudocode

```
ALWAYS (combinational):
    ArbToCDBA_Valid_o ← 0; ArbToCDBB_Valid_o ← 0
    // All stalls default to 0

    // Priority: IntALU > FPALU > IntMUL > FPMUL > LB0 > LB1 > LB2
    candidates ← []
    if IntALU_CDBReq_i == 1: candidates.append(IntALU)
    if FPALU_CDBReq_i  == 1: candidates.append(FPALU)
    if IntMUL_CDBReq_i == 1: candidates.append(IntMUL)
    if FPMUL_CDBReq_i  == 1: candidates.append(FPMUL)
    if LB0_CDBReq_i    == 1: candidates.append(LB0)
    if LB1_CDBReq_i    == 1: candidates.append(LB1)
    if LB2_CDBReq_i    == 1: candidates.append(LB2)

    // WINNER 1 → CDB_A
    if candidates.size() >= 1:
        winner_A ← candidates[0]
        ArbToCDBA_Valid_o ← 1
        ArbToCDBA_ROBTag_o ← winner_A.CDBROBTag; ArbToCDBA_Value_o ← winner_A.CDBValue
        ArbToCDBA_Exception_o ← winner_A.CDBException
        if winner_A is LBx:
            ArbToCDBA_Flags_o ← 0000; ArbToCDBA_FlagsValid_o ← 0
        else:
            ArbToCDBA_Flags_o ← winner_A.CDBFlags; ArbToCDBA_FlagsValid_o ← winner_A.CDBFlagsValid
        candidates.remove(0)

    // WINNER 2 → CDB_B
    if candidates.size() >= 1:
        winner_B ← candidates[0]
        ArbToCDBB_Valid_o ← 1
        ArbToCDBB_ROBTag_o ← winner_B.CDBROBTag; ArbToCDBB_Value_o ← winner_B.CDBValue
        ArbToCDBB_Exception_o ← winner_B.CDBException
        if winner_B is LBx:
            ArbToCDBB_Flags_o ← 0000; ArbToCDBB_FlagsValid_o ← 0
        else:
            ArbToCDBB_Flags_o ← winner_B.CDBFlags; ArbToCDBB_FlagsValid_o ← winner_B.CDBFlagsValid
        candidates.remove(0)

    // STALL THE LOSERS
    for each remaining in candidates:
        remaining.CDBStall ← 1
```

### Key behavior — Dual CDB Throughput
- **Two results per cycle**: crucial for Tomasulo performance.
- **Priority ensures low-latency units go first**: IntALU > multi-cycle units > loads.

---

## COMPONENTS 37-38: CDB_A, CDB_B (Common Data Buses)

### Purpose
Pure wire routing: broadcasts the selected result to ALL consumers.

### Pseudocode (CDB_A; CDB_B identical with CDBA→CDBB)

```
ALWAYS (combinational):
    CDBA_Valid_o      ← ArbToCDBA_Valid_i
    CDBA_ROBTag_o     ← ArbToCDBA_ROBTag_i
    CDBA_Value_o      ← ArbToCDBA_Value_i
    CDBA_Exception_o  ← ArbToCDBA_Exception_i
    CDBA_Flags_o      ← ArbToCDBA_Flags_i
    CDBA_FlagsValid_o ← ArbToCDBA_FlagsValid_i

    // Fan-out to 13 components:
    // ROB, SB0, SB1, LB0, LB1, LB2,
    // RS_IntALU0, RS_IntALU1, RS_FPALU0, RS_IntMUL0, RS_FPMUL0,
    // RS_Branch0, Flags_Unit
```

---

## COMPONENT 39: Memory_Arbiter

### Purpose
Coordinates access to the single D-Cache port from committed stores (highest priority) and load buffer memory requests.

### Internal State

```
current_source      — who requested (0=StoreCommit, 1=LB0, 2=LB1, 3=LB2)
waiting_for_dcache  — D-Cache access in progress
```

### Pseudocode

```
RISING_EDGE(CLK):
    StoreCommit_Done_o ← 0
    LB0_MemDone_o ← 0; LB1_MemDone_o ← 0; LB2_MemDone_o ← 0
    DC_Req_o ← 0

    // HANDLE D-CACHE RESPONSE
    if waiting_for_dcache == 1 AND DC_Done_i == 1:
        CASE current_source OF:
            0: StoreCommit_Done_o ← 1
            1: LB0_MemDone_o ← 1; LB0_MemData_o ← DC_RData_i
            2: LB1_MemDone_o ← 1; LB1_MemData_o ← DC_RData_i
            3: LB2_MemDone_o ← 1; LB2_MemData_o ← DC_RData_i
        waiting_for_dcache ← 0

    // ACCEPT NEW REQUEST
    if waiting_for_dcache == 0 AND DC_Ready_i == 1:
        // Priority: StoreCommit > LB0 > LB1 > LB2
        if StoreCommit_Req_i == 1:
            DC_Req_o ← 1; DC_RW_o ← 1; DC_Addr_o ← StoreCommit_Addr_i
            DC_WData_o ← StoreCommit_Data_i; DC_Size_o ← decode_size(StoreCommit_Op_i)
            current_source ← 0; waiting_for_dcache ← 1
        else if LB0_MemReq_i == 1:
            DC_Req_o ← 1; DC_RW_o ← 0; DC_Addr_o ← LB0_MemAddr_i
            DC_Size_o ← decode_size(LB0_MemOp_i); current_source ← 1; waiting_for_dcache ← 1
        else if LB1_MemReq_i == 1:
            DC_Req_o ← 1; DC_RW_o ← 0; DC_Addr_o ← LB1_MemAddr_i
            DC_Size_o ← decode_size(LB1_MemOp_i); current_source ← 2; waiting_for_dcache ← 1
        else if LB2_MemReq_i == 1:
            DC_Req_o ← 1; DC_RW_o ← 0; DC_Addr_o ← LB2_MemAddr_i
            DC_Size_o ← decode_size(LB2_MemOp_i); current_source ← 3; waiting_for_dcache ← 1

function decode_size(op):
    if op in {0x4E, 0x4F}: return 0b11  // LDR/STR → 8 bytes
    if op in {0x50, 0x51}: return 0b00  // LDRB/STRB → 1 byte
```

---

## COMPONENT 40: D_Cache (Data Cache)

### Purpose
L1 data cache. Write-back, write-allocate policy.

### Internal State

```
tag_array[32][4], data_array[32][4][64B], valid[32][4], dirty[32][4], lru_state[32]
miss_pending, writeback_pending, miss_addr, writeback_addr, writeback_data
```

### Pseudocode

```
RISING_EDGE(CLK):
    DC_Done_o ← 0; DC_Ready_o ← 1

    if writeback_pending == 1 AND RAMtoDC_Ready_i == 1:
        writeback_pending ← 0
        DCtoRAM_RdReq_o ← 1; DCtoRAM_Addr_o ← miss_addr
        miss_pending ← 1; DC_Ready_o ← 0

    if miss_pending == 1 AND RAMtoDC_Ready_i == 1:
        set ← extract_set(miss_addr); way ← lru_way
        tag_array[set][way] ← extract_tag(miss_addr)
        data_array[set][way] ← RAMtoDC_Data_i
        valid[set][way] ← 1; dirty[set][way] ← 0
        miss_pending ← 0

    if DC_Req_i == 1 AND DC_Ready_o == 1:
        addr ← DC_Addr_i; set ← addr[10:6]; tag ← addr[63:11]
        hit ← 0; hit_way ← 0
        for w in 0..3:
            if valid[set][w] == 1 AND tag_array[set][w] == tag:
                hit ← 1; hit_way ← w

        if hit == 1:
            if DC_RW_i == 0:  // READ
                byte_offset ← addr[5:0]
                CASE DC_Size_i OF:
                    0b11: DC_RData_o ← data_array[set][hit_way][byte_offset*8 +: 64]
                    0b00: DC_RData_o ← zero_extend(data_array[set][hit_way][byte_offset*8 +: 8])
                DC_Done_o ← 1
            else:  // WRITE
                byte_offset ← addr[5:0]
                CASE DC_Size_i OF:
                    0b11: data_array[set][hit_way][byte_offset*8 +: 64] ← DC_WData_i
                    0b00: data_array[set][hit_way][byte_offset*8 +: 8]  ← DC_WData_i[7:0]
                dirty[set][hit_way] ← 1; DC_Done_o ← 1
            update_lru(set, hit_way)
        else:  // MISS
            DC_Ready_o ← 0
            victim_way ← select_lru_way(set); lru_way ← victim_way
            if valid[set][victim_way] == 1 AND dirty[set][victim_way] == 1:
                DCtoRAM_WrReq_o ← 1
                DCtoRAM_Addr_o ← reconstruct_addr(tag_array[set][victim_way], set)
                DCtoRAM_WrData_o ← data_array[set][victim_way]
                writeback_pending ← 1
            else:
                DCtoRAM_RdReq_o ← 1; DCtoRAM_Addr_o ← addr & ~63
                miss_pending ← 1
            miss_addr ← addr & ~63
```

### Key behavior
- **Write-back**: dirty lines only written to RAM when evicted.
- **Write-allocate**: on write miss, fetch line first, then apply write.
- **Hit latency**: 1 cycle. **Miss penalty**: 50-100 cycles.

---

## COMPONENT 41: Commit_Unit

### Purpose
Retires instructions IN PROGRAM ORDER from the ROB head. Only committed results become architecturally visible. This makes the architecture precise.

### Internal State

```
store_commit_in_progress (1b)
```

### Pseudocode

```
ALWAYS (combinational):
    // Default all outputs to 0
    CommitPop_o ← 0; CommitWrEn_o ← 0; FlagsCommitEn_o ← 0
    StoreCommit_Req_o ← 0; BranchRedirect_o ← 0; Flush_o ← 0
    FreeSB0_o..FreeRSBr0_o ← 0; ExceptionSignal_o ← 0

    if ROBHeadBusy_i == 0: RETURN
    if ROBHeadReady_i == 0: RETURN

    // EXCEPTION CHECK
    if ROBHeadException_i != 0x0:
        ExceptionSignal_o ← 1; ExceptionCode_o ← ROBHeadException_i; ExceptionPC_o ← ROBHeadPC_i
        Flush_o ← 1; CommitPop_o ← 1
        activate_free(ROBHeadSourceStation_i)
        RETURN

    type ← ROBHeadType_i

    // ALU / MUL / FPALU / FPMUL / LOAD (types 000..100)
    if type in {000, 001, 010, 011, 100}:
        if ROBHeadDestReg_i != 0xF:
            CommitWrEn_o ← 1; CommitWrAddr_o ← ROBHeadDestReg_i
            CommitWrData_o ← ROBHeadValue_i; CommitROBIdx_o ← ROBHead_i
        if ROBHeadFlagsValid_i == 1:
            FlagsCommitEn_o ← 1; FlagsCommitValue_o ← ROBHeadFlagsResult_i
            CommitROBIdx_o ← ROBHead_i
        activate_free(ROBHeadSourceStation_i)
        CommitPop_o ← 1

    // STORE (type 101)
    else if type == 101:
        if ROBHeadStoreReady_i == 0: RETURN
        if store_commit_in_progress == 0:
            StoreCommit_Req_o ← 1; StoreCommit_Addr_o ← ROBHeadStoreAddr_i
            StoreCommit_Data_o ← ROBHeadStoreData_i; StoreCommit_Op_o ← ROBHeadType_i
            store_commit_in_progress ← 1
        if StoreCommit_Done_i == 1:
            store_commit_in_progress ← 0
            activate_free(ROBHeadSourceStation_i)
            CommitPop_o ← 1

    // BRANCH (type 110)
    else if type == 110:
        if ROBHeadMispredict_i == 1:
            BranchRedirect_o ← 1; BranchTarget_o ← ROBHeadBranchTarget_i; Flush_o ← 1
        activate_free(ROBHeadSourceStation_i)
        CommitPop_o ← 1

    // CMP / FLAGS-ONLY (type 111)
    else if type == 111:
        if ROBHeadFlagsValid_i == 1:
            FlagsCommitEn_o ← 1; FlagsCommitValue_o ← ROBHeadFlagsResult_i
            CommitROBIdx_o ← ROBHead_i
        activate_free(ROBHeadSourceStation_i)
        CommitPop_o ← 1


function activate_free(station_id):
    CASE station_id OF:
        0000: FreeSB0_o ← 1;      0001: FreeSB1_o ← 1
        0010: FreeLB0_o ← 1;      0011: FreeLB1_o ← 1;      0100: FreeLB2_o ← 1
        0101: FreeRSIAU0_o ← 1;   0110: FreeRSIAU1_o ← 1
        0111: FreeRSFPAU0_o ← 1;  1000: FreeRSIMUL0_o ← 1;  1001: FreeRSFPMUL0_o ← 1
        1010: FreeRSBr0_o ← 1;    1111: // NOP/SWI — nothing
```

### Key behavior for in-order commit
- **Only the ROB HEAD can commit** — guarantees in-order retirement.
- **Store writes deferred to commit** — prevents speculative memory corruption.
- **Misprediction recovery**: Flush_o reaches ALL pipeline components simultaneously, restoring committed state.
- **One commit per cycle**: bottleneck but necessary for precise exceptions.

---

# END-TO-END TRACE: Tomasulo in Action

## Sample Program (Real ISA Assembly)

```asm
        MOVI  REG0, #100         ; PC=0x00
        MOVI  REG1, #200         ; PC=0x08
        ADD   REG2, REG0, REG1   ; PC=0x10
        MULI  REG3, REG2, #3     ; PC=0x18
        SUBI  REG4, REG0, #50    ; PC=0x20
        CMP   REG4, REG1         ; PC=0x28
        BEQ   done               ; PC=0x30
        ADDI  REG5, REG3, #10    ; PC=0x38
done:   STR   REG5, [REG0, #0]   ; PC=0x40
```

**Initial state**: All registers = 0, ROB empty, all RS/Buffers free.

---

#### CYCLE 1: Issue MOVI REG0, #100
```
Control_Unit: Op=0x33 → IsIntALU=1, UseImm=1, ModifiesFlags=0
  Allocate RS_IntALU0 → AllocSourceStation=0101, ROBAlloc=1, TagWrEn=1

Register_File: REG[0] → Value=0, Qi_valid=0. Rename: REG[0].Qi←0, Qi_valid←1
ROB[0]: Type=000, DestReg=0, PC=0x00. Tail→1
RS_IntALU0: Op=0x33, Vj=0, QjValid=0, Vk=0x64(imm), QkValid=0 → READY
```

---

#### CYCLE 2: Issue MOVI REG1, #200 | Execute MOVI REG0
```
ISSUE: RS_IntALU1 ← MOVI REG1. REG[1].Qi←1, Qi_valid←1. ROB[1]. Tail→2.
EXECUTE: IntALU: 0x33, B=0x64 → Result=0x64. CDB_A: tag=0, val=0x64.
SNOOP: ROB[0].Ready←1, Value←0x64.
```

---

#### CYCLE 3: Issue ADD REG2,REG0,REG1 | Execute MOVI REG1 | Commit MOVI REG0
```
COMMIT ROB[0]: REG[0].Value←0x64, Qi_valid←0. FreeRSIAU0. Head→1.

ISSUE ADD: RS_IntALU0 (just freed).
  REG[0]: Qi_valid=0 → Vj=0x64. REG[1]: Qi=1, Qi_valid=1 → Qk=1, QkValid=1.
  Rename REG[2].Qi←2. Flags_Unit: Qi←2 (ADD modifies flags).

EXECUTE MOVI REG1: IntALU → Result=0xC8. CDB_A: tag=1, val=0xC8.
SNOOP: ROB[1].Ready←1. RS_IntALU0: Qk=1 matches → Vk←0xC8, QkValid←0. READY!
```

---

#### CYCLE 4: Issue MULI REG3,REG2,#3 | Execute ADD | Commit MOVI REG1
```
COMMIT ROB[1]: REG[1].Value←0xC8, Qi_valid←0. FreeRSIAU1. Head→2.

ISSUE MULI: RS_IntMUL0.
  REG[2]: Qi=2, Qi_valid=1. ROB[2].Ready? No (ADD executing now).
  → Qj=2, QjValid=1. Vk=3(imm). NOT ready.
  Rename REG[3].Qi←3. Flags_Unit: Qi←3 (MULI modifies flags).

EXECUTE ADD: IntALU: A=100, B=200 → 300. Flags={0,0,0,0}.
  CDB_A: tag=2, val=0x12C, flags={0,0,0,0}, flagsValid=1.
SNOOP: ROB[2].Ready←1. RS_IntMUL0: Qj=2 matches → Vj←300, QjValid←0. READY!
```

---

#### CYCLE 5: Issue SUBI REG4,REG0,#50 | Dispatch MULI | Commit ADD
```
COMMIT ROB[2]: REG[2].Value←300, Qi_valid←0. FlagsCommit={0,0,0,0}.
  Flags_Unit: Qi=3≠2 → don't clear. FreeRSIAU0. Head→3.

ISSUE SUBI: RS_IntALU0. Vj=100(REG0), Vk=50(imm). READY.
  Rename REG[4].Qi←4. Flags_Unit: Qi←4.

DISPATCH: IntMUL starts MUL 300×3, 4 cycles.
EXECUTE SUBI: IntALU: 100-50=50. Flags={0,0,1,0}.
  CDB_A: tag=4, val=0x32, flags={0,0,1,0}, flagsValid=1.
SNOOP: ROB[4].Ready←1, Flags={0,0,1,0}, FlagsValid=1.
```

---

#### CYCLE 6: Issue CMP REG4,REG1 | IntMUL cycle 2/4
```
ISSUE CMP: IsIntALU=1, IsCMPonly=1, WritesRd=0, ModifiesFlags=1.
  REG[4]: Qi=4, Qi_valid=1. ROB[4].Ready=1 → Forward! Vj=50.
  REG[1]: Qi_valid=0, Value=200 → Vk=200.
  TagWrEn=0. Flags_Unit: Qi←5. ROB[5]: Type=111, DestReg=0xF.
  RS_IntALU1: READY.

EXECUTE CMP: IntALU: 50-200=-150. N=1,Z=0,C=0,V=0.
  CDB_A: tag=5, val=0, flags={1,0,0,0}, flagsValid=1.
SNOOP: ROB[5].Ready←1, FlagsResult={1,0,0,0}, FlagsValid←1.
```

---

#### CYCLE 7: Issue BEQ done | IntMUL cycle 3/4

**COMMIT**: ROB Head=3 (MULI) → Ready=0 → CANNOT COMMIT.

```
ISSUE BEQ:
  Flags_Unit: FlagsQi=5, FlagsQiValid=1 (pending producer exists).
  
  === 3-TIER FLAGS RESOLUTION ===
  TIER 1: FlagsQiValid_i=1 → producer IS pending. Skip.
  TIER 2: ROBReadFlagsReady_i? → ROB[5].Ready=1 AND ROB[5].FlagsValid=1 → YES!
    FlagsValue ← ROBReadFlagsResult_i = {1,0,0,0}
    FlagsQiValid ← 0  → RESOLVED IMMEDIATELY!
  
  RS_Branch0: Op=0x46(BEQ), TargetPC=0x30+16=0x40,
    FlagsValue={1,0,0,0}, FlagsQiValid=0 → READY!
```

**This is the key improvement**: In the old design, RS_Branch0 would have been stuck waiting because the CDB broadcast for tag 5 happened in cycle 6 (before RS_Branch0 was allocated). The RS would have had to wait until CMP committed in cycle 11. With the ROB flags read port, the flags are forwarded immediately at Issue time. **Saved 4+ cycles!**

---

#### CYCLE 8: Issue ADDI REG5,REG3,#10 | Execute BEQ | IntMUL finishes!

```
EXECUTE BEQ (dispatched from RS_Branch0):
  Branch_Executor: Op=0x46, Flags={1,0,0,0}: Z=0.
  BEQ checks Z==1 → Z=0 → taken=0.
  Predicted=0 → mispredict = (0≠0) = 0. CORRECT PREDICTION!
  BrExResult: Valid=1, ROBTag=6, Taken=0, Mispredict=0.
  ROB[6].Ready←1, Mispredict←0.

ISSUE ADDI REG5: RS_IntALU0.
  REG[3]: Qi=3, Qi_valid=1. ROB[3].Ready? IntMUL finishes THIS cycle.
  → Qj=3, QjValid=1. Vk=10(imm).

IntMUL COMPLETES: 300×3=900. CDB_A: tag=3, val=0x384, flags={0,0,0,0}.
SNOOP: ROB[3].Ready←1. RS_IntALU0: Qj=3 matches → Vj←900, QjValid←0. READY!
```

---

#### CYCLE 9: Execute ADDI | Commit MULI REG3
```
COMMIT ROB[3]: REG[3].Value←900. FlagsCommit={0,0,0,0}. FreeRSIMUL0. Head→4.
EXECUTE ADDI: 900+10=910. CDB_A: tag=7, val=0x38E.
SNOOP: ROB[7].Ready←1.
```

---

#### CYCLE 10: Commit SUBI REG4
```
COMMIT ROB[4]: REG[4].Value←50. FlagsCommit={0,0,1,0}. Flags_Unit: Qi=5≠4 → don't clear. Head→5.
```

---

#### CYCLE 11: Commit CMP (ROB[5])
```
COMMIT ROB[5]: Type=111. FlagsCommit={1,0,0,0}. Flags_Unit: Qi=5=5 → Qi_valid←0. Head→6.
```

---

#### CYCLE 12: Commit BEQ (ROB[6])
```
COMMIT: Type=110, Mispredict=0 → no flush! FreeRSBr0. Head→7.
```

---

#### CYCLE 13: Commit ADDI REG5 (ROB[7])
```
COMMIT: REG[5].Value←910. Head→8.
```

---

#### CYCLE 14+: Issue & Execute STR REG5, [REG0, #0]
```
Issue: SB0. BaseValue=100(REG0), DataValue=910(REG5). Both ready.
AGU: 100+0=100. StComplete → ROB[8].Ready←1.
Commit: StoreCommit → Memory_Arbiter → D_Cache write. Done.
```

---

## Summary: Key Tomasulo Observations from this Trace

1. **Issue never stalls for data dependencies**: MULI was issued in cycle 4 even though REG2 wasn't ready.

2. **Out-of-order execution**: SUBI (cycle 5) executed BEFORE MULI (cycle 8) even though MULI was issued first.

3. **CDB bypass enables zero-bubble forwarding**: When ADD's result appeared on CDB in cycle 4, RS_IntMUL0 captured it in the SAME cycle.

4. **Register renaming eliminates WAW hazards**: Each instruction gets its own ROB tag, so reuse of registers is safe.

5. **Speculative execution past branches**: ADDI REG5 was issued in cycle 8 speculatively. The prediction was correct, so the speculative work committed normally.

6. **Commit is strictly in-order**: SUBI and CMP finished before MULI but couldn't commit first.

7. **3-Tier Flags Resolution eliminates forwarding gap**: In cycle 7, the BEQ was issued AFTER CMP's CDB broadcast (cycle 6), but the ROB flags read port (Tier 2) forwarded the flags immediately. The branch became ready the SAME cycle it was issued, instead of waiting 4+ cycles for CMP to commit. This is the critical improvement over the previous design.

8. **Dual CDB**: With more instructions in flight, both CDB_A and CDB_B would be used simultaneously, doubling completion throughput.

---

## DESIGN NOTE: Flags Forwarding — APPLIED FIX

The original design had a gap: if a flag-consuming instruction was issued AFTER the flag-producing instruction had already published on CDB, the consumer missed the broadcast and had to wait until the producer committed (updating Flags_arch). In the trace, this would have added 4+ cycles of unnecessary latency for the BEQ.

**Fix applied**: ROB Flags Read Port (3-Tier Flags Resolution)

All flag-consuming RS (RS_IntALU0/1, RS_FPALU0, RS_IntMUL0, RS_FPMUL0, RS_Branch0) now resolve flags through three tiers:

| Tier | Condition | Action | Latency |
|------|-----------|--------|---------|
| 1 | FlagsQiValid_i == 0 | Use FlagsArch_i (committed flags) | 0 cycles |
| 2 | ROBReadFlagsReady_i == 1 | Forward from ROB[FlagsQi] | 0 cycles |
| 3 | Neither ready | Record FlagsQi, wait for CDB | Variable |
| Fallback | FlagsQiValid_i drops to 0 | Use updated FlagsArch_i | Variable |

**New wires added**:
- `ROB.FlagsQi_i` (5b) ← `Flags_Unit.FlagsQi_o` — ROB read address for flags
- `ROB.ROBReadFlagsReady_o` (1b) → All 6 flag-consuming RS
- `ROB.ROBReadFlagsResult_o` (4b) → All 6 flag-consuming RS

**ROB combinational logic added**:
```
ROBReadFlagsReady_o  ← entries[FlagsQi_i].Ready AND entries[FlagsQi_i].FlagsValid
ROBReadFlagsResult_o ← entries[FlagsQi_i].FlagsResult
```

This fix is architecturally complete and requires no changes to the CDB, Commit_Unit, or Flags_Unit logic.