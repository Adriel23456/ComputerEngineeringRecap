# Tomasulo Microarchitecture — Complete Component Logic Specification (Part 1)

## Components 1–22: Fetch, Decode, Issue, Buffers, Reservation Stations, AGU

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
        // Flush does NOT directly write PC; the PC_MUX handles redirect.
        // But Flush cancels any stall so PC can accept the redirect.
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
        // But if miss_pending was for a flushed instruction, we let it complete silently.

    // Handle RAM response
    if RAMtoIC_Ready_i == 1:
        if miss_pending == 1:
            // Install the returned line into the cache
            set ← extract_set(miss_addr)
            way ← select_lru_way(set)
            tag_array[set][way] ← extract_tag(miss_addr)
            data_array[set][way] ← RAMtoIC_Data_i   // 512 bits = 64 bytes
            valid[set][way] ← 1
            update_lru(set, way)
            miss_pending ← 0
        else if prefetch_pending == 1:
            // Install prefetched line
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
    set ← addr[10:6]          // 32 sets → 5 bits
    tag ← addr[63:11]
    offset ← addr[5:3]        // Which 8-byte instruction in the 64-byte line
    
    hit ← 0
    for w in 0..3:
        if valid[set][w] == 1 AND tag_array[set][w] == tag:
            hit ← 1
            InstrF_o ← data_array[set][w][ offset * 8 .. offset * 8 + 7 ]  // 8 bytes = 64 bits
            update_lru(set, w)
    
    if hit == 1:
        InstReady_o ← 1
        ICtoRAM_Req_o ← 0
        
        // Initiate next-line prefetch (if not already prefetching)
        next_line_addr ← (addr & ~63) + 64
        if not in_cache(next_line_addr) AND prefetch_pending == 0:
            prefetch_pending ← 1
            prefetch_addr ← next_line_addr
            ICtoRAM_Req_o ← 1
            ICtoRAM_Addr_o ← next_line_addr
    else:
        InstReady_o ← 0
        InstrF_o ← 0  // Invalid
        if miss_pending == 0:
            miss_pending ← 1
            miss_addr ← addr & ~63  // Align to cache line
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
    
    if ic_req_active == 1 AND dc_req_active == 0:  // I-Cache only runs when D-Cache idle
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
- Every RS/Buffer receives Op_in, but only the one whose Alloc_i=1 captures it.

---

## COMPONENT 7: Extends (Immediate Extender)

### Purpose
Extends the 32-bit immediate field to 64 bits. For integer operations, it performs two's complement sign extension. For floating-point operations, it converts IEEE 754 single-precision (32-bit float) to double-precision (64-bit double).

### Pseudocode

```
ALWAYS (combinational):
    IsFP ← IsFPALU_i OR IsFPMUL_i
    
    if IsFP == 1:
        // IEEE 754 float32 → float64 conversion
        // Extract float32 fields:
        sign     ← Imm_in_i[31]
        exponent ← Imm_in_i[30:23]   // 8 bits, biased by 127
        mantissa ← Imm_in_i[22:0]    // 23 bits
        
        // Convert to float64 fields:
        // Double bias = 1023, Single bias = 127, difference = 896
        if exponent == 0x00:
            // Zero or denormalized → double zero/denorm
            double_exp ← 0x000
            double_man ← mantissa << 29   // 23 bits → 52 bits (pad with zeros)
        else if exponent == 0xFF:
            // Inf or NaN
            double_exp ← 0x7FF
            double_man ← mantissa << 29
        else:
            // Normalized
            double_exp ← exponent - 127 + 1023  // Re-bias
            double_man ← mantissa << 29
        
        ImmExt_o ← (sign << 63) | (double_exp << 52) | double_man
    else:
        // Two's complement sign extension from 32 to 64 bits
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
        // Integer ALU (3-reg)
        0x00..0x03, 0x06..0x0D: IsIntALU ← 1
        // Integer ALU (imm)
        0x0E..0x11, 0x14..0x1B: IsIntALU ← 1; UseImm_int ← 1
        // INC/DEC (treated as ALU imm with implicit #1)
        0x1C, 0x1D: IsIntALU ← 1; UseImm_int ← 1
        // Integer MUL/DIV (3-reg)
        0x04, 0x05: IsIntMUL ← 1
        // Integer MUL/DIV (imm)
        0x12, 0x13: IsIntMUL ← 1; UseImm_int ← 1
        // FP ALU (3-reg)
        0x1E, 0x1F, 0x22: IsFPALU_int ← 1
        // FP ALU (imm)
        0x23, 0x24, 0x27: IsFPALU_int ← 1; UseImm_int ← 1
        // FP Unary (2-reg) — goes to FPALU except FSQRT
        0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30: IsFPALU_int ← 1
        // FP Unary (imm) — goes to FPALU except FSQRTI
        0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A: IsFPALU_int ← 1; UseImm_int ← 1
        // FP MUL/DIV (3-reg)
        0x20, 0x21: IsFPMUL_int ← 1
        // FP MUL/DIV (imm)
        0x25, 0x26: IsFPMUL_int ← 1; UseImm_int ← 1
        // FSQRT (2-reg) — goes to FPMUL unit
        0x28: IsFPMUL_int ← 1
        // FSQRTI (imm) — goes to FPMUL unit
        0x52: IsFPMUL_int ← 1; UseImm_int ← 1
        // MOV/MVN (2-reg) — IntALU
        0x31, 0x32: IsIntALU ← 1
        // MOVI/MVNI (imm) — IntALU
        0x33, 0x34: IsIntALU ← 1; UseImm_int ← 1
        // FMOVI/FMVNI (imm) — IntALU (it's just a move, no FP compute)
        0x35, 0x36: IsIntALU ← 1; UseImm_int ← 1
        // Integer CMP (2-reg) — IntALU, no Rd write
        0x37..0x3A: IsIntALU ← 1; IsCMPonly ← 1
        // Integer CMP (imm) — IntALU, no Rd write
        0x3B..0x3E: IsIntALU ← 1; IsCMPonly ← 1; UseImm_int ← 1
        // Float CMP (2-reg) — FPALU, no Rd write
        0x3F..0x41: IsFPALU_int ← 1; IsCMPonly ← 1
        // Float CMP (imm) — FPALU, no Rd write
        0x42..0x44: IsFPALU_int ← 1; IsCMPonly ← 1; UseImm_int ← 1
        // Branches
        0x45..0x4B: IsBranch ← 1
        // Special
        0x4C: IsSWI ← 1
        0x4D: IsNOP ← 1
        // Memory
        0x4E, 0x50: IsLoad ← 1    // LDR, LDRB
        0x4F, 0x51: IsStore ← 1   // STR, STRB
    
    // ====================================================================
    // STEP 2: DERIVE CONTROL SIGNALS
    // ====================================================================
    
    // ModifiesFlags: does this instruction produce N,Z,C,V?
    ModifiesFlags_int ← IsIntALU OR IsIntMUL   // All ALU/MUL ops modify flags
                         // EXCEPT MOV/MVN/MOVI/MVNI/FMOVI/FMVNI
    if op in {0x31, 0x32, 0x33, 0x34, 0x35, 0x36}: ModifiesFlags_int ← 0
    // CMP/FCMP also modify flags (already covered since IsIntALU/IsFPALU is set)
    if IsCMPonly == 1: ModifiesFlags_int ← 1
    // FCMP variants modify flags
    if op in {0x3F..0x44}: ModifiesFlags_int ← 1
    
    // NeedsFlags: does this instruction REQUIRE flags as an input operand?
    NeedsFlags_int ← 0
    if op in {0x02, 0x03, 0x10, 0x11}: NeedsFlags_int ← 1   // ADC, SBC, ADCI, SBCI
    if IsBranch AND op != 0x45: NeedsFlags_int ← 1            // All conditional branches
    
    // WritesRd: does this instruction write a result to register Rd?
    WritesRd_int ← NOT IsCMPonly AND NOT IsStore AND NOT IsBranch AND NOT IsNOP AND NOT IsSWI
    
    // IsUnconditional
    IsUnconditional ← (op == 0x45)  // B (unconditional branch)
    
    // Output signals to Extends
    IsFPALU_o ← IsFPALU_int
    IsFPMUL_o ← IsFPMUL_int
    
    // Output signals to RS/Buffers (broadcast, captured only by allocated unit)
    ModifiesFlags_o ← ModifiesFlags_int
    NeedsFlags_o    ← NeedsFlags_int
    UseImm_o        ← UseImm_int
    IsCMPonly_o     ← IsCMPonly
    
    // TagWrEn: enable register rename ONLY if instruction writes to Rd
    TagWrEn_o ← WritesRd_int AND (InstReady_i == 1) AND NOT StallIF_o_pending
    
    // FlagsTagWrEn: enable flags rename ONLY if instruction modifies flags
    FlagsTagWrEn_o ← ModifiesFlags_int AND (InstReady_i == 1) AND NOT StallIF_o_pending
    
    // AllocType: 3-bit type code for ROB
    if IsIntALU:     AllocType_o ← 000
    if IsIntMUL:     AllocType_o ← 001
    if IsFPALU_int:  AllocType_o ← 010
    if IsFPMUL_int:  AllocType_o ← 011
    if IsLoad:       AllocType_o ← 100
    if IsStore:      AllocType_o ← 101
    if IsBranch:     AllocType_o ← 110
    if IsCMPonly:    AllocType_o ← 111  // Overrides the unit type
    if IsNOP:        AllocType_o ← 000  // NOP uses ALU type but WritesRd=0
    
    // AllocPredicted: for branches, static predict-not-taken
    AllocPredicted_o ← 0  // Always predict not-taken
    
    // ====================================================================
    // STEP 3: RESOURCE ALLOCATION (first-free policy)
    // ====================================================================
    
    // Default: no allocation
    StallIF_o ← 0
    ROBAlloc_o ← 0
    AllocSB0_o ← 0; AllocSB1_o ← 0
    AllocLB0_o ← 0; AllocLB1_o ← 0; AllocLB2_o ← 0
    AllocRSIntALU0_o ← 0; AllocRSIntALU1_o ← 0
    AllocRSFPALU0_o ← 0; AllocRSIntMUL0_o ← 0; AllocRSFPMUL0_o ← 0
    AllocRSBranch0_o ← 0
    
    // Guard: no instruction to issue
    if InstReady_i == 0 OR ROBFull_i == 1:
        StallIF_o ← 1
        RETURN
    
    // NOP/SWI: allocate ROB entry but no RS/Buffer needed
    if IsNOP OR IsSWI:
        ROBAlloc_o ← 1
        AllocSourceStation_o ← 0xF  // Special: no RS/Buffer
        RETURN
    
    // Find a free RS/Buffer for the instruction type
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
        StallIF_o ← 1   // No free RS/Buffer → stall
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
// ====================================================================
// COMBINATIONAL READ (every cycle, no clock needed)
// ====================================================================
ALWAYS (combinational):
    // Port 1: read Rn
    RD1_Value_o   ← REG[Rn_in_i].Value
    RD1_Qi_o      ← REG[Rn_in_i].Qi
    RD1_QiValid_o ← REG[Rn_in_i].Qi_valid
    
    // Port 2: read Rm
    RD2_Value_o   ← REG[Rm_in_i].Value
    RD2_Qi_o      ← REG[Rm_in_i].Qi
    RD2_QiValid_o ← REG[Rm_in_i].Qi_valid
    
    // Segmentation limits (always available)
    UPPER_o ← REG[0xA].Value   // UPPER_REG
    LOWER_o ← REG[0xB].Value   // LOWER_REG

// ====================================================================
// SEQUENTIAL WRITES (rising edge of CLK)
// ====================================================================
RISING_EDGE(CLK):
    
    // FLUSH: clear all rename tags (restore to committed state)
    if Flush_i == 1:
        for i in 0..11:
            REG[i].Qi_valid ← 0
        RETURN   // Flush overrides everything else this cycle
    
    // COMMIT WRITE: instruction has committed, write its result
    if CommitWrEn_i == 1:
        REG[CommitWrAddr_i].Value ← CommitWrData_i
        // Clear the rename tag ONLY if it still points to this ROB entry
        // (A newer instruction may have re-renamed this register)
        if REG[CommitWrAddr_i].Qi == CommitROBIdx_i AND REG[CommitWrAddr_i].Qi_valid == 1:
            REG[CommitWrAddr_i].Qi_valid ← 0
    
    // RENAME: tag the destination register with the new ROB index
    // This happens AFTER commit write in priority (same cycle, commit first)
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

    // --- FLUSH ---
    if Flush_i == 1:
        for i in 0..31: entries[i].Busy ← 0
        Tail ← Head
        Count ← 0
        RETURN
    
    // --- ALLOCATE (new instruction issued) ---
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
- **Dual CDB**: Both CDB_A and CDB_B can update DIFFERENT ROB entries in the SAME cycle. This means two instructions can complete simultaneously. If both happen to target the same ROB entry (impossible in correct hardware since each instruction has a unique ROB index), the hardware would need arbitration — but by design, each instruction gets a unique ROB index, so this cannot happen.
- **ROB Read Ports**: These provide forwarding during Issue. When an instruction is being issued and its operand register has Qi_valid=1, the Issue logic reads ROB[Qi] to check if the value is already Ready. If yes, the value is forwarded immediately. If no, the RS will wait for CDB.

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
    
    // COMMIT: update architectural flags
    if FlagsCommitEn_i == 1:
        Flags_arch ← FlagsCommitValue_i
        // Clear rename tag if it points to the committing instruction
        if Flags_Qi == CommitROBIdx_i AND Flags_Qi_valid == 1:
            Flags_Qi_valid ← 0
    
    // RENAME: a new flag-modifying instruction was issued
    if FlagsTagWrEn_i == 1:
        Flags_Qi       ← ROBTail_i
        Flags_Qi_valid ← 1
```

### Key behavior
- If multiple instructions modify flags, only the LATEST one's tag is stored. When an earlier one commits its flags, the tag is NOT cleared (it still points to the newer one). When the LAST one commits, the tag IS cleared.
- RS that need flags (ADC, SBC, conditional branches) capture the FlagsQi at issue time and then snoop the CDB for that specific ROB tag to get the speculative flags.

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
StCompleteNotified(1b)   — already sent StoreComplete to ROB
```

### Pseudocode (SB0; SB1 identical with prefix change)

```
RISING_EDGE(CLK):
    if Flush_i == 1:
        Busy ← 0
        RETURN
    
    if SB0_Free_i == 1:
        Busy ← 0
        RETURN
    
    // --- ALLOCATE ---
    if Alloc_i == 1:
        Busy ← 1
        Op ← Op_in_i
        Offset ← ImmExt_i
        ROBTag ← ROBTail_i
        AddressReady ← 0
        SegFault ← 0
        StCompleteNotified ← 0
        
        // Resolve BASE operand (Rn — read port 1)
        if RD1_QiValid_i == 0:
            // Register is up-to-date, use value directly
            BaseValue ← RD1_Value_i
            BaseTagValid ← 0
        else:
            // Register has pending producer — check ROB forwarding
            if ROBReadReady1_i == 1:
                BaseValue ← ROBReadValue1_i
                BaseTagValid ← 0
            else:
                // Must wait for CDB
                BaseTag ← RD1_Qi_i
                BaseTagValid ← 1
        
        // Resolve DATA operand (Rm — read port 2)
        // For STR: Rm is the data source register (bits [47:44])
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
        // Snoop CDB_A
        if CDBA_Valid_i == 1:
            if BaseTagValid == 1 AND BaseTag == CDBA_ROBTag_i:
                BaseValue ← CDBA_Value_i
                BaseTagValid ← 0
            if DataTagValid == 1 AND DataTag == CDBA_ROBTag_i:
                DataValue ← CDBA_Value_i
                DataTagValid ← 0
        
        // Snoop CDB_B
        if CDBB_Valid_i == 1:
            if BaseTagValid == 1 AND BaseTag == CDBB_ROBTag_i:
                BaseValue ← CDBB_Value_i
                BaseTagValid ← 0
            if DataTagValid == 1 AND DataTag == CDBB_ROBTag_i:
                DataValue ← CDBB_Value_i
                DataTagValid ← 0
    
    // --- AGU DONE ---
    if SB0_AGUDone_i == 1:
        Address ← SB0_AGUAddress_i
        AddressReady ← 1
        SegFault ← SB0_AGUSegFault_i

// ====================================================================
// COMBINATIONAL OUTPUTS
// ====================================================================
ALWAYS (combinational):
    Busy_o ← Busy
    
    // AGU Request: ask for address calculation when base is ready and address not yet computed
    SB0_AGUReq_o    ← Busy AND (BaseTagValid == 0) AND (AddressReady == 0)
    SB0_AGUBase_o   ← BaseValue
    SB0_AGUOffset_o ← Offset
    SB0_AGUROBTag_o ← ROBTag
    
    // Store Complete: notify ROB when BOTH address and data are resolved
    // Only notify ONCE (use StCompleteNotified flag)
    complete ← Busy AND AddressReady AND (DataTagValid == 0) AND (StCompleteNotified == 0)
    StComplete_Valid_o  ← complete
    StComplete_ROBTag_o ← ROBTag
    StComplete_Addr_o   ← Address
    StComplete_Data_o   ← DataValue
    
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
MemRequested(1b)     — memory request sent to Memory_Arbiter
MemDone_int(1b)      — data received from memory
LoadedData(64b)      — data from memory
CDBRequested(1b)     — CDB publish request sent
```

### Pseudocode (LB0; LB1, LB2 identical with prefix change)

```
RISING_EDGE(CLK):
    if Flush_i == 1:
        Busy ← 0; RETURN
    if LB0_Free_i == 1:
        Busy ← 0; RETURN
    
    // --- ALLOCATE ---
    if Alloc_i == 1:
        Busy ← 1
        Op ← Op_in_i
        Offset ← ImmExt_i
        DestROBTag ← ROBTail_i
        AddressReady ← 0
        SegFault ← 0
        MemRequested ← 0
        MemDone_int ← 0
        CDBRequested ← 0
        
        // Resolve BASE operand (Rn) — same logic as SB
        if RD1_QiValid_i == 0:
            BaseValue ← RD1_Value_i; BaseTagValid ← 0
        else:
            if ROBReadReady1_i == 1:
                BaseValue ← ROBReadValue1_i; BaseTagValid ← 0
            else:
                BaseTag ← RD1_Qi_i; BaseTagValid ← 1
        
        // Loads do NOT have a data operand (no Rm dependency for data)
    
    // --- CDB SNOOP (for base operand only) ---
    if Busy == 1:
        if CDBA_Valid_i == 1 AND BaseTagValid == 1 AND BaseTag == CDBA_ROBTag_i:
            BaseValue ← CDBA_Value_i; BaseTagValid ← 0
        if CDBB_Valid_i == 1 AND BaseTagValid == 1 AND BaseTag == CDBB_ROBTag_i:
            BaseValue ← CDBB_Value_i; BaseTagValid ← 0
    
    // --- AGU DONE ---
    if LB0_AGUDone_i == 1:
        Address ← LB0_AGUAddress_i; AddressReady ← 1; SegFault ← LB0_AGUSegFault_i
    
    // --- MEMORY DONE ---
    if LB0_MemDone_i == 1:
        LoadedData ← LB0_MemData_i
        MemDone_int ← 1
    
    // --- CDB STALL ---
    if LB0_CDBStall_i == 1:
        // CDB Arbiter denied us this cycle, keep trying next cycle
        CDBRequested ← 0  // Will re-request next cycle

ALWAYS (combinational):
    Busy_o ← Busy
    
    // AGU Request
    LB0_AGUReq_o    ← Busy AND (BaseTagValid == 0) AND (AddressReady == 0)
    LB0_AGUBase_o   ← BaseValue
    LB0_AGUOffset_o ← Offset
    LB0_AGUROBTag_o ← DestROBTag
    
    // Memory Request
    LB0_MemReq_o    ← Busy AND AddressReady AND (SegFault == 0) AND (MemRequested == 0)
    LB0_MemAddr_o   ← Address
    LB0_MemOp_o     ← Op
    LB0_MemROBTag_o ← DestROBTag
    // Note: when Memory_Arbiter accepts, LB internally sets MemRequested ← 1
    
    // CDB Request (once data is loaded)
    if SegFault == 1:
        // Report segfault as exception via CDB
        LB0_CDBReq_o       ← Busy AND AddressReady AND (CDBRequested == 0)
        LB0_CDBValue_o      ← 0
        LB0_CDBException_o  ← 0x2  // seg_fault code
    else:
        LB0_CDBReq_o       ← Busy AND MemDone_int AND (CDBRequested == 0)
        LB0_CDBValue_o      ← LoadedData
        LB0_CDBException_o  ← 0x0
    LB0_CDBROBTag_o ← DestROBTag
```

### Key behavior
- **Loads DO publish to CDB** (unlike stores). The loaded value needs to reach the ROB and any RS waiting for it.
- **Loads progress through 3 phases**: (1) Wait for base → (2) AGU computes address → (3) Memory access → CDB publish.
- **Loads don't block Issue** either. They sit in their LB and progress independently.

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
    if RSIAU0_Free_i == 1:
        Busy ← 0; RETURN
    
    // --- ALLOCATE ---
    if Alloc_i == 1:
        Busy ← 1
        Op ← Op_in_i
        DestROBTag ← ROBTail_i
        UseImm ← UseImm_i
        ModifiesFlags ← ModifiesFlags_i
        NeedsFlags ← NeedsFlags_i
        Dispatched ← 0
        
        // === OPERAND 1 (Vj) — always from Rn ===
        if RD1_QiValid_i == 0:
            Vj ← RD1_Value_i; QjValid ← 0   // Ready!
        else:
            if ROBReadReady1_i == 1:
                Vj ← ROBReadValue1_i; QjValid ← 0   // Forwarded from ROB
            else:
                Qj ← RD1_Qi_i; QjValid ← 1          // Wait for CDB
        
        // === OPERAND 2 (Vk) ===
        if UseImm_i == 1:
            // Immediate operand — always ready
            Vk ← ImmExt_i; QkValid ← 0
        else:
            // Register Rm
            if RD2_QiValid_i == 0:
                Vk ← RD2_Value_i; QkValid ← 0
            else:
                if ROBReadReady2_i == 1:
                    Vk ← ROBReadValue2_i; QkValid ← 0
                else:
                    Qk ← RD2_Qi_i; QkValid ← 1
        
        // === FLAGS DEPENDENCY ===
        if NeedsFlags_i == 1:
            if FlagsQiValid_i == 0:
                // Flags are up-to-date (committed)
                FlagsValue ← FlagsArch_i; FlagsQiValid ← 0
            else:
                // Flags pending — must wait for specific ROB entry
                FlagsQi ← FlagsQi_i; FlagsQiValid ← 1
        else:
            FlagsQiValid ← 0
        
        // SPECIAL CASES based on Op:
        // For CMP/CMN/TST/TEQ: both operands are Rn and Rm (use read port 1 and 2)
        // For MOV Rd, Rm: Vj is unused, Vk comes from Rm (read port 2)
        //   Actually in the ISA encoding: MOV Rd, Rm puts Rm in bits[47:44]
        //   So Vj is not needed. We set QjValid ← 0, Vj ← dont_care.
        if Op in {0x31, 0x32}:  // MOV, MVN
            QjValid ← 0  // Only need Rm (read from Vk via port 2)
        
        // For INC/DEC: the assembler encodes Rn=Rd, Imm=1, UseImm=1
        // So Vj=REG[Rd], Vk=1 — already handled by UseImm path
    
    // --- CDB SNOOP (every cycle while Busy and not yet dispatched) ---
    if Busy == 1:
        for each CDB in {A, B}:
            if CDBX_Valid == 1:
                // Check operand 1
                if QjValid == 1 AND Qj == CDBX_ROBTag:
                    Vj ← CDBX_Value; QjValid ← 0
                // Check operand 2
                if QkValid == 1 AND Qk == CDBX_ROBTag:
                    Vk ← CDBX_Value; QkValid ← 0
                // Check flags dependency
                if FlagsQiValid == 1 AND NeedsFlags == 1 AND FlagsQi == CDBX_ROBTag:
                    if CDBX_FlagsValid == 1:
                        FlagsValue ← CDBX_Flags; FlagsQiValid ← 0

ALWAYS (combinational):
    Busy_o ← Busy
    
    // Ready condition: all operands AND flags (if needed) are resolved
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
    
    // When granted, mark as dispatched
    if RSIAU0_Grant_i == 1:
        Dispatched ← 1  // latched next cycle
```

### Key behavior for Tomasulo
- **This is the heart of Tomasulo's algorithm**: the RS independently waits for its operands by snooping the CDB. No centralized scoreboard is needed.
- **Multiple RS can be waiting simultaneously** for different operands. When a CDB broadcast matches, the operand is captured in the SAME CYCLE — this is the "bypass" that eliminates bubbles.
- **Dual CDB advantage**: If RS_IntALU0 is waiting for operand Qj=ROB[5] and Qk=ROB[7], and CDB_A publishes ROB[5] while CDB_B publishes ROB[7] in the SAME cycle, BOTH operands are captured simultaneously. The RS becomes ready next cycle.

---

## COMPONENT 19: RS_FPALU0 (FP ALU Reservation Station)

### Structure and logic
Identical to RS_IntALU0 with these differences:
- Prefix: `RSFPAU0_` instead of `RSIAU0_`
- Connected to FPALU_Arbiter instead of IntALU_Arbiter
- **NeedsFlags**: Always 0 for FP operations (no ADC/SBC equivalent in FP)
- **ModifiesFlags**: Only 1 for FCMP/FCMN/FCMPS and their immediate variants
- **FP Unary ops (FNEG, FABS, etc.)**: Only use ONE operand (Rm). The RS sets QjValid ← 0 (Vj unused), and gets operand from Vk (read port 2 for Rm).

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
- Additional internal bit: `Executing` — set to 1 when dispatched, cleared when the MUL unit finishes.

### Key difference from ALU stations

```
// Ready condition includes NOT Executing:
Ready ← Busy AND (QjValid == 0) AND (QkValid == 0) AND (Executing == 0)
         AND ((NeedsFlags == 0) OR (FlagsQiValid == 0))

// When granted:
if RSIMUL0_Grant_i == 1:
    Executing ← 1

// When MUL unit publishes result on CDB (the RS recognizes this because
// the CDB ROBTag matches its DestROBTag):
// Actually, the RS doesn't need to detect this — it stays Executing
// until freed by Commit_Unit.
// The Executing flag prevents re-dispatch while the MUL unit is working.
```

This is critical because MUL/DIV are multi-cycle (4-16 cycles). Without the Executing flag, the arbiter would try to re-dispatch the same instruction every cycle.

---

## COMPONENT 21: RS_FPMUL0 (FP MUL/DIV/SQRT Reservation Station)

### Structure and logic
Identical to RS_IntMUL0 (has Executing bit) with prefix `RSFPMUL0_`, connected to FPMUL_Arbiter.

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
    if RSBr0_Free_i == 1: Busy ← 0; RETURN
    
    // --- ALLOCATE ---
    if Alloc_i == 1:
        Busy ← 1
        Op ← Op_in_i
        ROBTag ← ROBTail_i
        
        // Compute branch target: PC + sign-extended offset
        // The assembler encodes: offset = (target_PC - current_PC) * 8
        // So offset is already in bytes. Target = PC + offset.
        TargetPC ← PCCurrent_i + ImmExt_i
        
        PredictedTaken ← 0  // Static predict not-taken
        
        // Flags dependency
        if Op == 0x45:  // B (unconditional)
            FlagsQiValid ← 0  // No flags needed
        else:  // Conditional branches
            if FlagsQiValid_i == 0:
                FlagsValue ← FlagsArch_i; FlagsQiValid ← 0
            else:
                FlagsQi ← FlagsQi_i; FlagsQiValid ← 1
    
    // --- CDB SNOOP (only for flags) ---
    if Busy == 1:
        for each CDB in {A, B}:
            if CDBX_Valid == 1 AND CDBX_FlagsValid == 1:
                if FlagsQiValid == 1 AND FlagsQi == CDBX_ROBTag:
                    FlagsValue ← CDBX_Flags; FlagsQiValid ← 0

ALWAYS (combinational):
    Busy_o ← Busy
    
    // Ready: unconditional is always ready; conditional needs flags
    if Op == 0x45:  // B unconditional
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
- **Conditional branches** must wait until the flag-producing instruction publishes on CDB. If flags arrive via CDB in cycle N, the branch becomes ready in cycle N (same-cycle capture) and can be dispatched to Branch_Executor in cycle N+1.

---

## END OF PART 1

Part 2 will cover:
- Components 23-25: AGU_Arbiter, AGU_0, AGU_1
- Components 26-27: IntALU_Arbiter, IntALU
- Components 28-29: FPALU_Arbiter, FPALU
- Components 30-31: IntMUL_Arbiter, IntMUL
- Components 32-33: FPMUL_Arbiter, FPMUL
- Components 34-35: Branch_Arbiter, Branch_Executor
- Components 36-38: CDB_Arbiter, CDB_A, CDB_B
- Components 39-40: Memory_Arbiter, D_Cache
- Component 41: Commit_Unit
- End-to-end instruction trace example with the real ISA
# Tomasulo Microarchitecture — Complete Component Logic Specification (Part 2)

## Components 23–41: AGUs, Execution Units, Arbiters, CDB, Memory, Commit

---

## COMPONENT 23: AGU_Arbiter (Address Generation Unit Arbiter)

### Purpose
Coordinates access from 5 requestors (SB0, SB1, LB0, LB1, LB2) to 2 AGU units (AGU_0, AGU_1). Each cycle it can dispatch UP TO 2 address calculations simultaneously. It also routes results BACK from the AGUs to the correct requestor.

### Internal State

```
// Track which requestor was dispatched to which AGU
AGU0_CurrentSource (3b)   — 000=SB0, 001=SB1, 010=LB0, 011=LB1, 100=LB2
AGU1_CurrentSource (3b)
```

### Pseudocode

```
ALWAYS (combinational):
    // Default: no dispatches
    AGU0_Start_o ← 0
    AGU1_Start_o ← 0
    SB0_AGUDone_o ← 0; SB1_AGUDone_o ← 0
    LB0_AGUDone_o ← 0; LB1_AGUDone_o ← 0; LB2_AGUDone_o ← 0

    // ================================================================
    // PHASE 1: ROUTE RESULTS BACK FROM AGUs TO REQUESTORS
    // ================================================================
    // AGU_0 finished — route result back to whoever sent it
    if AGU0_Done_i == 1:
        CASE AGU0_SourceID_i OF:
            000: SB0_AGUDone_o ← 1; SB0_AGUAddress_o ← AGU0_Address_i; SB0_AGUSegFault_o ← AGU0_SegFault_i
            001: SB1_AGUDone_o ← 1; SB1_AGUAddress_o ← AGU0_Address_i; SB1_AGUSegFault_o ← AGU0_SegFault_i
            010: LB0_AGUDone_o ← 1; LB0_AGUAddress_o ← AGU0_Address_i; LB0_AGUSegFault_o ← AGU0_SegFault_i
            011: LB1_AGUDone_o ← 1; LB1_AGUAddress_o ← AGU0_Address_i; LB1_AGUSegFault_o ← AGU0_SegFault_i
            100: LB2_AGUDone_o ← 1; LB2_AGUAddress_o ← AGU0_Address_i; LB2_AGUSegFault_o ← AGU0_SegFault_i

    // AGU_1 finished — same routing logic
    if AGU1_Done_i == 1:
        CASE AGU1_SourceID_i OF:
            000: SB0_AGUDone_o ← 1; SB0_AGUAddress_o ← AGU1_Address_i; SB0_AGUSegFault_o ← AGU1_SegFault_i
            001: SB1_AGUDone_o ← 1; SB1_AGUAddress_o ← AGU1_Address_i; SB1_AGUSegFault_o ← AGU1_SegFault_i
            010: LB0_AGUDone_o ← 1; LB0_AGUAddress_o ← AGU1_Address_i; LB0_AGUSegFault_o ← AGU1_SegFault_i
            011: LB1_AGUDone_o ← 1; LB1_AGUAddress_o ← AGU1_Address_i; LB1_AGUSegFault_o ← AGU1_SegFault_i
            100: LB2_AGUDone_o ← 1; LB2_AGUAddress_o ← AGU1_Address_i; LB2_AGUSegFault_o ← AGU1_SegFault_i

    // ================================================================
    // PHASE 2: DISPATCH NEW REQUESTS TO FREE AGUs
    // ================================================================
    // Priority: SB0 > SB1 > LB0 > LB1 > LB2
    // Collect pending requests into an ordered list
    pending ← []
    if SB0_AGUReq_i == 1: pending.append({id=000, base=SB0_AGUBase_i, offset=SB0_AGUOffset_i})
    if SB1_AGUReq_i == 1: pending.append({id=001, base=SB1_AGUBase_i, offset=SB1_AGUOffset_i})
    if LB0_AGUReq_i == 1: pending.append({id=010, base=LB0_AGUBase_i, offset=LB0_AGUOffset_i})
    if LB1_AGUReq_i == 1: pending.append({id=011, base=LB1_AGUBase_i, offset=LB1_AGUOffset_i})
    if LB2_AGUReq_i == 1: pending.append({id=100, base=LB2_AGUBase_i, offset=LB2_AGUOffset_i})

    // Dispatch first-priority to AGU_0 (if AGU_0 is free)
    if pending.size() >= 1 AND AGU0_Free_i == 1:
        p ← pending[0]
        AGU0_Start_o     ← 1
        AGU0_Base_o      ← p.base
        AGU0_Offset_o    ← p.offset
        AGU0_SourceID_o  ← p.id
        pending.remove(0)

    // Dispatch second-priority to AGU_1 (if AGU_1 is free)
    if pending.size() >= 1 AND AGU1_Free_i == 1:
        p ← pending[0]
        AGU1_Start_o     ← 1
        AGU1_Base_o      ← p.base
        AGU1_Offset_o    ← p.offset
        AGU1_SourceID_o  ← p.id
```

### Key behavior
- **Two AGUs = two address calculations per cycle**. A store and a load can compute their addresses simultaneously, or two loads, etc.
- **Stores get priority** over loads. This minimizes the chance that a store's address computation is delayed, which would delay its StoreComplete signal and potentially hold up commit.
- **The SourceID tag** tracks which requestor sent the job so the result is routed back correctly. This is a 3-bit tag carried through the AGU pipeline.

---

## COMPONENTS 24-25: AGU_0, AGU_1 (Address Generation Units)

### Purpose
Computes effective address = Base + Offset, then checks against segmentation bounds (LOWER ≤ Address ≤ UPPER). Pure combinational — result is available in the SAME cycle the inputs are presented.

### Pseudocode (AGU_0; AGU_1 identical with prefix change)

```
ALWAYS (combinational):
    AGU0_Free_o ← 1   // Always free (combinational, no pipeline)
    AGU0_Done_o ← 0

    if AGU0_Start_i == 1:
        // Compute effective address
        Address ← AGU0_Base_i + AGU0_Offset_i

        // Segmentation check
        if (Address < LOWER_i) OR (Address > UPPER_i):
            SegFault ← 1
        else:
            SegFault ← 0

        AGU0_Done_o      ← 1
        AGU0_Address_o   ← Address
        AGU0_SegFault_o  ← SegFault
        AGU0_SourceID_o  ← AGU0_SourceID_i   // Pass-through for routing
```

### Key behavior
- **Combinational**: no pipeline stages, no stalls. Input arrives, output is ready the same cycle.
- **Segmentation check** compares against REG[13] (LOWER_REG) and REG[14] (UPPER_REG) from the Register_File. If the address falls outside this range, a SegFault exception is raised.
- **SegFault does NOT halt execution** — it's captured in the requesting SB/LB and eventually propagated to the ROB via StoreComplete or CDB. The exception is handled at Commit time.
- **AGU0_Free_o is always 1** because the unit is combinational. The arbiter can always send a new request the next cycle.

---

## COMPONENT 26: IntALU_Arbiter

### Purpose
Selects between RS_IntALU0 and RS_IntALU1 to grant access to the single IntALU execution unit. Only one can execute per cycle.

### Pseudocode

```
ALWAYS (combinational):
    RSIAU0_Grant_o ← 0
    RSIAU1_Grant_o ← 0
    IntALU_Start_o ← 0

    // Priority: RS_IntALU0 > RS_IntALU1
    if IntALU_Free_i == 1:
        if RSIAU0_Req_i == 1:
            RSIAU0_Grant_o     ← 1
            IntALU_Start_o     ← 1
            IntALU_Op_o        ← RSIAU0_Op_i
            IntALU_A_o         ← RSIAU0_Vj_i
            IntALU_B_o         ← RSIAU0_Vk_i
            IntALU_DestROBTag_o← RSIAU0_DestROBTag_i
            IntALU_ModFlags_o  ← RSIAU0_ModFlags_i
            IntALU_NeedsFlags_o← RSIAU0_NeedsFlags_i
            IntALU_FlagsIn_o   ← RSIAU0_FlagsValue_i
        else if RSIAU1_Req_i == 1:
            RSIAU1_Grant_o     ← 1
            IntALU_Start_o     ← 1
            IntALU_Op_o        ← RSIAU1_Op_i
            IntALU_A_o         ← RSIAU1_Vj_i
            IntALU_B_o         ← RSIAU1_Vk_i
            IntALU_DestROBTag_o← RSIAU1_DestROBTag_i
            IntALU_ModFlags_o  ← RSIAU1_ModFlags_i
            IntALU_NeedsFlags_o← RSIAU1_NeedsFlags_i
            IntALU_FlagsIn_o   ← RSIAU1_FlagsValue_i
```

### Key behavior
- **Single-cycle arbitration**: one grant per cycle, strict priority RS0 > RS1.
- Having TWO RS feeding ONE ALU means: even if RS0 is occupied waiting for operands, RS1 can still accept a new instruction at issue time. Then when RS0's operands arrive and RS1 is still waiting, RS0 gets dispatched immediately. This decouples issue from execution.

---

## COMPONENT 27: IntALU (Integer ALU)

### Purpose
Performs all integer arithmetic, logic, shift, comparison, and move operations in a SINGLE CYCLE. Purely combinational — result is available the same cycle the inputs are presented.

### Pseudocode

```
ALWAYS (combinational):
    IntALU_Free_o          ← NOT IntALU_CDBStall_i  // Free if CDB is not stalling us
    IntALU_CDBReq_o        ← 0
    IntALU_CDBFlagsValid_o ← 0

    if IntALU_Start_i == 1:
        op ← IntALU_Op_i
        A  ← IntALU_A_i       // Vj (operand 1, from Rn)
        B  ← IntALU_B_i       // Vk (operand 2, from Rm or Imm)
        cin ← 0
        if IntALU_NeedsFlags_i == 1:
            cin ← IntALU_FlagsIn_i[1]   // Carry flag is bit[1] of N,Z,C,V

        // ============================================================
        // COMPUTE RESULT
        // ============================================================
        CASE op OF:
            // --- Arithmetic ---
            0x00, 0x0E: Result ← A + B                          // ADD, ADDI
            0x01, 0x0F: Result ← A - B                          // SUB, SUBI
            0x02, 0x10: Result ← A + B + cin                    // ADC, ADCI
            0x03, 0x11: Result ← A - B - (NOT cin)              // SBC, SBCI
            0x1C:       Result ← A + 1                          // INC
            0x1D:       Result ← A - 1                          // DEC

            // --- Logic ---
            0x06, 0x14: Result ← A AND B                        // AND, ANDI
            0x07, 0x15: Result ← A OR B                         // ORR, ORRI
            0x08, 0x16: Result ← A XOR B                        // EOR, EORI
            0x09, 0x17: Result ← A AND (NOT B)                  // BIC, BICI

            // --- Shifts ---
            0x0A, 0x18: Result ← A << (B mod 64)               // LSL, LSLI
            0x0B, 0x19: Result ← A >> (B mod 64) (logical)     // LSR, LSRI
            0x0C, 0x1A: Result ← A >>> (B mod 64) (arithmetic) // ASR, ASRI
            0x0D, 0x1B: Result ← rotate_right(A, B mod 64)     // ROR, RORI

            // --- Move ---
            0x31:       Result ← B                              // MOV Rd, Rm → passthrough Rm
            0x32:       Result ← NOT B                          // MVN Rd, Rm → bitwise NOT
            0x33:       Result ← B                              // MOVI Rd, #imm → passthrough imm
            0x34:       Result ← NOT B                          // MVNI Rd, #imm
            0x35:       Result ← B                              // FMOVI Rd, #imm (just a move)
            0x36:       Result ← NOT B                          // FMVNI Rd, #imm

            // --- Compare (result discarded, only flags matter) ---
            0x37, 0x3B: temp ← A - B                           // CMP, CMPI (subtract)
            0x38, 0x3C: temp ← A + B                           // CMN, CMNI (add)
            0x39, 0x3D: temp ← A AND B                         // TST, TSTI (bitwise AND)
            0x3A, 0x3E: temp ← A XOR B                         // TEQ, TEQI (bitwise XOR)
                        Result ← 0  // CMP/CMN/TST/TEQ don't write a register

        // ============================================================
        // COMPUTE FLAGS (N, Z, C, V)
        // ============================================================
        if IntALU_ModFlags_i == 1:
            // N: Negative (bit 63 of result)
            N ← Result[63]   // or temp[63] for CMP/CMN/TST/TEQ

            // Z: Zero
            Z ← (Result == 0) ? 1 : 0   // or (temp == 0) for comparisons

            // C: Carry (depends on operation)
            if op is ADD/ADDI/ADC/ADCI/INC/CMN/CMNI:
                C ← carry_out of addition
            else if op is SUB/SUBI/SBC/SBCI/DEC/CMP/CMPI:
                C ← borrow_out of subtraction (inverted borrow)
            else if op is shift:
                C ← last bit shifted out
            else:
                C ← 0

            // V: Overflow (signed overflow)
            if op is ADD/SUB/ADC/SBC/INC/DEC/CMP/CMN (and their imm variants):
                V ← signed_overflow(A, B, Result)
            else:
                V ← 0

            Flags ← {N, Z, C, V}
            IntALU_CDBFlagsValid_o ← 1
            IntALU_CDBFlags_o      ← Flags
        else:
            IntALU_CDBFlagsValid_o ← 0

        // ============================================================
        // OUTPUT TO CDB
        // ============================================================
        IntALU_CDBReq_o       ← 1
        IntALU_CDBROBTag_o    ← IntALU_DestROBTag_i
        IntALU_CDBValue_o     ← Result
        IntALU_CDBException_o ← 0x0   // No exceptions in IntALU

        // If CDB_Arbiter stalls us (IntALU_CDBStall_i == 1), we hold the result
        // and keep requesting. IntALU_Free_o will be 0, preventing the arbiter
        // from dispatching a new instruction to us.
```

### Key behavior
- **Single-cycle latency**: the ALU is purely combinational. In the same cycle an RS dispatches operands, the result is available at the CDB_Arbiter input.
- **CDB stall handling**: if the CDB_Arbiter denies the publish (both CDB slots taken by higher-priority units), the IntALU holds its result and sets IntALU_Free_o = 0. This prevents the IntALU_Arbiter from dispatching a new instruction.
- **For CMP/CMN/TST/TEQ**: the Result value is meaningless (it goes to ROB but DestReg=0xF so it's never written). Only the Flags matter.
- **MOV encoding detail**: per the assembler, `MOV Rd, Rm` encodes Rm in bits[47:44]. The RS captures Rm via read port 2 into Vk. Vj is unused (QjValid forced to 0). The ALU receives A=don't_care, B=Rm_value and outputs Result = B.

---

## COMPONENT 28: FPALU_Arbiter

### Purpose
Selects from RS_FPALU0 to grant access to the FPALU. Only one RS exists, so this is a simple pass-through when the FPALU is free.

### Pseudocode

```
ALWAYS (combinational):
    RSFPAU0_Grant_o ← 0
    FPALU_Start_o   ← 0

    if FPALU_Free_i == 1 AND RSFPAU0_Req_i == 1:
        RSFPAU0_Grant_o     ← 1
        FPALU_Start_o       ← 1
        FPALU_Op_o          ← RSFPAU0_Op_i
        FPALU_A_o           ← RSFPAU0_Vj_i
        FPALU_B_o           ← RSFPAU0_Vk_i
        FPALU_DestROBTag_o  ← RSFPAU0_DestROBTag_i
        FPALU_ModFlags_o    ← RSFPAU0_ModFlags_i
        FPALU_NeedsFlags_o  ← RSFPAU0_NeedsFlags_i
        FPALU_FlagsIn_o     ← RSFPAU0_FlagsValue_i
```

### Why a separate arbiter for just one RS?
Consistency: every execution unit has an arbiter. If a future design adds RS_FPALU1, only the arbiter changes. The FPALU itself stays untouched.

---

## COMPONENT 29: FPALU (Floating-Point ALU)

### Purpose
Performs floating-point addition, subtraction, comparisons, and unary operations (FNEG, FABS, conversions). 3-stage pipeline — the unit can accept a new instruction every cycle while previous ones progress through the pipeline.

### Internal State

```
// 3-stage pipeline registers
stage1.active, stage1.op, stage1.A, stage1.B, stage1.ROBTag, stage1.ModFlags
stage2.active, stage2.partial_result, stage2.ROBTag, stage2.ModFlags, stage2.op
stage3.active, stage3.result, stage3.ROBTag, stage3.flags, stage3.flagsValid, stage3.exception

output_hold.active, output_hold.{result, ROBTag, flags, flagsValid, exception}
```

### Pseudocode

```
RISING_EDGE(CLK):

    // ================================================================
    // STAGE 3 → OUTPUT (result ready to publish)
    // ================================================================
    if stage3.active == 1:
        if FPALU_CDBStall_i == 0:
            // CDB accepted our result — clear stage 3
            stage3.active ← 0
        else:
            // CDB busy — hold result, stall pipeline
            output_hold.active ← 1
            output_hold.result ← stage3.result
            output_hold.ROBTag ← stage3.ROBTag
            // ... (copy all fields)

    // ================================================================
    // STAGE 2 → STAGE 3 (normalize / round)
    // ================================================================
    if stage2.active == 1 AND (stage3.active == 0 OR stage3 just cleared):
        stage3.active ← 1
        stage3.result ← normalize_and_round(stage2.partial_result)
        stage3.ROBTag ← stage2.ROBTag

        // Generate flags for FCMP variants
        if stage2.op in {0x3F, 0x40, 0x41, 0x42, 0x43, 0x44}:
            A_dbl ← interpret_as_double(stage2.A)
            B_dbl ← interpret_as_double(stage2.B)
            isNaN ← is_NaN(A_dbl) OR is_NaN(B_dbl)
            Z ← (A_dbl == B_dbl) ? 1 : 0
            N ← (A_dbl < B_dbl) ? 1 : 0
            C ← (A_dbl >= B_dbl) ? 1 : 0
            V ← isNaN ? 1 : 0
            stage3.flags ← {N, Z, C, V}
            stage3.flagsValid ← 1
        else:
            stage3.flagsValid ← 0

        stage3.exception ← check_fp_exception(stage2.partial_result)
        stage2.active ← 0

    // ================================================================
    // STAGE 1 → STAGE 2 (alignment / operation)
    // ================================================================
    if stage1.active == 1 AND stage2.active == 0:
        stage2.active ← 1
        stage2.op ← stage1.op

        CASE stage1.op OF:
            // --- FP Addition/Subtraction ---
            0x1E, 0x23: partial ← fp_add(stage1.A, stage1.B)       // FADD, FADDI
            0x1F, 0x24: partial ← fp_sub(stage1.A, stage1.B)       // FSUB, FSUBI

            // --- FCOPYSIGN ---
            0x22, 0x27: partial ← copy_sign(stage1.A, stage1.B)    // FCOPYSIGN(I)

            // --- Unary Operations ---
            0x29, 0x53: partial ← negate(stage1.B)                 // FNEG(I)
            0x2A, 0x54: partial ← abs(stage1.B)                    // FABS(I)
            0x2B, 0x55: partial ← convert_double_to_int(stage1.B)  // CDTI(I)
            0x2C, 0x56: partial ← convert_int_to_double(stage1.B)  // CDTD(I)
            0x2D, 0x57: partial ← round_nearest(stage1.B)          // RTNR(I)
            0x2E, 0x58: partial ← round_toward_zero(stage1.B)      // RTZ(I)
            0x2F, 0x59: partial ← round_toward_positive(stage1.B)  // RTP(I)
            0x30, 0x5A: partial ← round_nearest_even(stage1.B)     // RTNE(I)

            // --- FP Compare (result is dummy, flags are generated in stage 3) ---
            0x3F..0x44: partial ← 0  // Result doesn't matter for CMP

        stage2.partial_result ← partial
        stage2.A ← stage1.A; stage2.B ← stage1.B  // Carry for FCMP flag generation
        stage2.ROBTag ← stage1.ROBTag
        stage2.ModFlags ← stage1.ModFlags
        stage1.active ← 0

    // ================================================================
    // INPUT → STAGE 1 (latch operands)
    // ================================================================
    if FPALU_Start_i == 1 AND stage1.active == 0:
        stage1.active   ← 1
        stage1.op       ← FPALU_Op_i
        stage1.A        ← FPALU_A_i
        stage1.B        ← FPALU_B_i
        stage1.ROBTag   ← FPALU_DestROBTag_i
        stage1.ModFlags ← FPALU_ModFlags_i

ALWAYS (combinational):
    // Free if stage 1 is empty (can accept new input)
    FPALU_Free_o ← (stage1.active == 0) AND (output_hold.active == 0)

    // CDB outputs
    if stage3.active == 1 OR output_hold.active == 1:
        src ← output_hold if output_hold.active else stage3
        FPALU_CDBReq_o       ← 1
        FPALU_CDBROBTag_o    ← src.ROBTag
        FPALU_CDBValue_o     ← src.result
        FPALU_CDBException_o ← src.exception
        FPALU_CDBFlags_o     ← src.flags
        FPALU_CDBFlagsValid_o← src.flagsValid
    else:
        FPALU_CDBReq_o ← 0
```

### Key behavior — Pipelining
- **3-cycle latency**: an instruction enters stage 1 at cycle T, and its result is ready at cycle T+3 (output of stage 3).
- **Throughput of 1 per cycle**: since stage 1 accepts a new instruction every cycle while earlier instructions progress through stages 2 and 3, the FPALU can have 3 instructions in-flight simultaneously.
- **CDB stall backpressure**: if the CDB_Arbiter denies publish, stage 3 holds its result, stage 2 stalls, stage 1 stalls, and FPALU_Free_o goes to 0 — the arbiter won't dispatch more work.
- **FCMP** generates flags (N, Z, C, V) based on IEEE 754 comparison rules: Z for equality, N for less-than, C for greater-or-equal, V for unordered (NaN).

---

## COMPONENT 30: IntMUL_Arbiter

### Purpose
Selects from RS_IntMUL0 (single RS) to the IntMUL execution unit.

### Pseudocode

```
ALWAYS (combinational):
    RSIMUL0_Grant_o ← 0
    IntMUL_Start_o  ← 0

    if IntMUL_Free_i == 1 AND RSIMUL0_Req_i == 1:
        RSIMUL0_Grant_o     ← 1
        IntMUL_Start_o      ← 1
        IntMUL_Op_o         ← RSIMUL0_Op_i
        IntMUL_A_o          ← RSIMUL0_Vj_i
        IntMUL_B_o          ← RSIMUL0_Vk_i
        IntMUL_DestROBTag_o ← RSIMUL0_DestROBTag_i
        IntMUL_ModFlags_o   ← RSIMUL0_ModFlags_i
        IntMUL_NeedsFlags_o ← RSIMUL0_NeedsFlags_i
        IntMUL_FlagsIn_o    ← RSIMUL0_FlagsValue_i
```

---

## COMPONENT 31: IntMUL (Integer Multiplier/Divider)

### Purpose
Performs integer multiplication (4 cycles) and division (16 cycles). NOT pipelined — the unit is busy for the full duration of the operation and cannot accept new instructions until done.

### Internal State

```
busy                 — operation in progress
cycles_remaining     — countdown to completion
result(64b), ROBTag(5b), flags(4b), flagsValid(1b), exception(4b)
op(8b), A(64b), B(64b)
```

### Pseudocode

```
RISING_EDGE(CLK):
    if busy == 1:
        cycles_remaining ← cycles_remaining - 1
        if cycles_remaining == 0:
            // Compute result (done after N cycles)
            CASE op OF:
                0x04, 0x12: result ← A * B              // MUL, MULI
                0x05, 0x13:                              // DIV, DIVI
                    if B == 0:
                        exception ← 0x3                  // Division by zero
                        result ← 0
                    else:
                        result ← A / B (signed integer)

            // Compute flags
            N ← result[63]
            Z ← (result == 0) ? 1 : 0
            C ← 0  // MUL/DIV don't generate meaningful carry
            V ← overflow detection for MUL (upper 64 bits nonzero)
            flags ← {N, Z, C, V}
            flagsValid ← 1

    // Accept new instruction when idle
    if busy == 0 AND IntMUL_Start_i == 1:
        busy ← 1
        op ← IntMUL_Op_i
        A  ← IntMUL_A_i
        B  ← IntMUL_B_i
        ROBTag ← IntMUL_DestROBTag_i

        if op in {0x04, 0x12}:  // MUL, MULI
            cycles_remaining ← 4
        else:                    // DIV, DIVI
            cycles_remaining ← 16

ALWAYS (combinational):
    // Free when not busy AND not waiting on CDB stall
    IntMUL_Free_o ← (busy == 0) OR (busy == 1 AND cycles_remaining == 0 AND IntMUL_CDBStall_i == 0)
    // Actually more precisely:
    IntMUL_Free_o ← (busy == 0)

    // CDB request when computation is done
    if busy == 1 AND cycles_remaining == 0:
        IntMUL_CDBReq_o       ← 1
        IntMUL_CDBROBTag_o    ← ROBTag
        IntMUL_CDBValue_o     ← result
        IntMUL_CDBException_o ← exception
        IntMUL_CDBFlags_o     ← flags
        IntMUL_CDBFlagsValid_o← flagsValid

        if IntMUL_CDBStall_i == 0:
            busy ← 0   // CDB accepted, we're free next cycle
        // else: hold result, keep requesting
    else:
        IntMUL_CDBReq_o ← 0
```

### Key behavior
- **Blocking execution**: while a MUL is in progress, NO new MUL can start. The RS_IntMUL0 holds its instruction until Free_o goes high.
- **However, Issue is NOT blocked**: even though the MUL unit is occupied for 4–16 cycles, the pipeline keeps issuing other instruction types (ALU, FP, Load, Store, Branch) every cycle. Only a SECOND MUL instruction would stall at Issue — and only if RS_IntMUL0 is still occupied.

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
        busy ← 1
        op ← FPMUL_Op_i
        A ← FPMUL_A_i
        B ← FPMUL_B_i
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
                    if is_zero(B):
                        exception ← 0x4  // FP division by zero
                        result ← fp_infinity(sign_of(A) XOR sign_of(B))
                    else:
                        result ← fp_divide(A, B)
                0x28, 0x52:
                    if is_negative(B) AND NOT is_zero(B):
                        exception ← 0x5  // FP invalid operation (sqrt of negative)
                        result ← fp_NaN()
                    else:
                        result ← fp_sqrt(B)

            // FPMUL never generates flags (ModifiesFlags always 0)
            flagsValid ← 0

    // CDB output logic identical to IntMUL
```

---

## COMPONENT 34: Branch_Arbiter

### Purpose
Passes through from RS_Branch0 to Branch_Executor (single RS, single executor).

### Pseudocode

```
ALWAYS (combinational):
    RSBr0_Grant_o ← 0
    BrEx_Start_o  ← 0

    // Branch_Executor is always free (combinational)
    if RSBr0_Req_i == 1:
        RSBr0_Grant_o      ← 1
        BrEx_Start_o       ← 1
        BrEx_Op_o          ← RSBr0_Op_i
        BrEx_TargetPC_o    ← RSBr0_TargetPC_i
        BrEx_FlagsValue_o  ← RSBr0_FlagsValue_i
        BrEx_PredTaken_o   ← RSBr0_PredTaken_i
        BrEx_ROBTag_o      ← RSBr0_ROBTag_i
```

---

## COMPONENT 35: Branch_Executor

### Purpose
Evaluates the branch condition against the flags and determines if the branch should be taken. Compares the actual outcome against the prediction to detect mispredictions. Results go DIRECTLY to ROB (NOT via CDB).

### Pseudocode

```
ALWAYS (combinational):
    BrExResult_Valid_o ← 0

    if BrEx_Start_i == 1:
        flags ← BrEx_FlagsValue_i
        N ← flags[3]; Z ← flags[2]; C ← flags[1]; V ← flags[0]

        // Evaluate condition
        CASE BrEx_Op_i OF:
            0x45: taken ← 1                           // B   (unconditional)
            0x46: taken ← (Z == 1)                     // BEQ (equal: Z=1)
            0x47: taken ← (Z == 0)                     // BNE (not equal: Z=0)
            0x48: taken ← (N != V)                     // BLT (less than: N≠V)
            0x49: taken ← (Z == 0) AND (N == V)        // BGT (greater than: Z=0 and N=V)
            0x4A: taken ← (V == 1)                     // BUN (unordered: V=1)
            0x4B: taken ← (V == 0)                     // BORD (ordered: V=0)

        // Detect misprediction
        predicted ← BrEx_PredTaken_i    // Always 0 (predict not-taken)
        mispredict ← (taken != predicted)

        // Output to ROB
        BrExResult_Valid_o      ← 1
        BrExResult_ROBTag_o     ← BrEx_ROBTag_i
        BrExResult_Taken_o      ← taken
        BrExResult_Target_o     ← BrEx_TargetPC_i
        BrExResult_Mispredict_o ← mispredict
```

### Key behavior
- **No CDB needed**: branch results go directly to the ROB. Branches don't produce register values, so there's nothing to broadcast. This means branches NEVER compete for CDB bandwidth.
- **Misprediction detection**: since we always predict not-taken, any taken branch is a misprediction. The ROB stores the Mispredict flag and the correct target. At commit time, the Commit_Unit triggers a flush and redirects the PC.
- **Misprediction penalty**: all instructions issued after the mispredicted branch (speculatively fetched from the fall-through path) must be flushed. The pipeline restarts from BranchTarget.

---

## COMPONENT 36: CDB_Arbiter (Common Data Bus Arbiter)

### Purpose
Selects up to 2 out of 7 solicitants per cycle and routes them to CDB_A and CDB_B respectively. This is the core throughput multiplier of the dual-CDB design.

### Pseudocode

```
ALWAYS (combinational):
    // Default: no grants, all stalled
    ArbToCDBA_Valid_o ← 0
    ArbToCDBB_Valid_o ← 0
    IntALU_CDBStall_o ← 0; FPALU_CDBStall_o ← 0
    IntMUL_CDBStall_o ← 0; FPMUL_CDBStall_o ← 0
    LB0_CDBStall_o ← 0; LB1_CDBStall_o ← 0; LB2_CDBStall_o ← 0

    // Collect all active requests (priority-ordered)
    // Priority: IntALU > FPALU > IntMUL > FPMUL > LB0 > LB1 > LB2
    candidates ← []
    if IntALU_CDBReq_i == 1: candidates.append(IntALU)
    if FPALU_CDBReq_i  == 1: candidates.append(FPALU)
    if IntMUL_CDBReq_i == 1: candidates.append(IntMUL)
    if FPMUL_CDBReq_i  == 1: candidates.append(FPMUL)
    if LB0_CDBReq_i    == 1: candidates.append(LB0)
    if LB1_CDBReq_i    == 1: candidates.append(LB1)
    if LB2_CDBReq_i    == 1: candidates.append(LB2)

    // ================================================================
    // WINNER 1 → CDB_A
    // ================================================================
    if candidates.size() >= 1:
        winner_A ← candidates[0]
        ArbToCDBA_Valid_o     ← 1
        ArbToCDBA_ROBTag_o    ← winner_A.CDBROBTag
        ArbToCDBA_Value_o     ← winner_A.CDBValue
        ArbToCDBA_Exception_o ← winner_A.CDBException
        // For LBx: force Flags=0000, FlagsValid=0
        if winner_A is LBx:
            ArbToCDBA_Flags_o      ← 0000
            ArbToCDBA_FlagsValid_o ← 0
        else:
            ArbToCDBA_Flags_o      ← winner_A.CDBFlags
            ArbToCDBA_FlagsValid_o ← winner_A.CDBFlagsValid
        candidates.remove(0)

    // ================================================================
    // WINNER 2 → CDB_B
    // ================================================================
    if candidates.size() >= 1:
        winner_B ← candidates[0]
        ArbToCDBB_Valid_o     ← 1
        ArbToCDBB_ROBTag_o    ← winner_B.CDBROBTag
        ArbToCDBB_Value_o     ← winner_B.CDBValue
        ArbToCDBB_Exception_o ← winner_B.CDBException
        if winner_B is LBx:
            ArbToCDBB_Flags_o      ← 0000
            ArbToCDBB_FlagsValid_o ← 0
        else:
            ArbToCDBB_Flags_o      ← winner_B.CDBFlags
            ArbToCDBB_FlagsValid_o ← winner_B.CDBFlagsValid
        candidates.remove(0)

    // ================================================================
    // STALL THE LOSERS (everyone not selected)
    // ================================================================
    for each remaining in candidates:
        remaining.CDBStall ← 1
```

### Key behavior — Dual CDB Throughput
- **Two results per cycle**: this is crucial for Tomasulo performance. In a single-CDB design, even if IntALU and FPALU both finish the same cycle, one must wait. With dual CDB, BOTH publish simultaneously.
- **Priority ensures low-latency units go first**: IntALU has highest priority because it's single-cycle — if it can't publish, it blocks a new instruction from entering the ALU. Multi-cycle units (IntMUL, FPMUL) can afford to wait one more cycle since they're already multi-cycle.
- **Load buffers have lowest priority** because loads already have long latency (memory access). One extra cycle of CDB wait is a small fraction of their total time.

---

## COMPONENTS 37-38: CDB_A, CDB_B (Common Data Buses)

### Purpose
Pure wire routing: takes the selected result from CDB_Arbiter and broadcasts it to ALL consumers (ROB, all RS, all Buffers, Flags_Unit). These components represent the physical bus.

### Pseudocode (CDB_A; CDB_B identical with CDBA→CDBB)

```
ALWAYS (combinational):
    // Pure pass-through — wire routing
    CDBA_Valid_o      ← ArbToCDBA_Valid_i
    CDBA_ROBTag_o     ← ArbToCDBA_ROBTag_i
    CDBA_Value_o      ← ArbToCDBA_Value_i
    CDBA_Exception_o  ← ArbToCDBA_Exception_i
    CDBA_Flags_o      ← ArbToCDBA_Flags_i
    CDBA_FlagsValid_o ← ArbToCDBA_FlagsValid_i

    // These outputs fan out to 13 components:
    // ROB, SB0, SB1, LB0, LB1, LB2,
    // RS_IntALU0, RS_IntALU1, RS_FPALU0, RS_IntMUL0, RS_FPMUL0,
    // RS_Branch0, Flags_Unit
    //
    // Each consumer independently compares CDBA_ROBTag against its
    // own pending tags (Qj, Qk, FlagsQi) and captures the value
    // if there's a match.
```

### Why make CDB_A and CDB_B explicit components?
- They represent a real physical resource: a shared bus with limited capacity.
- Making them explicit components ensures every consumer's input is traced to a single source (the bus), not directly to the arbiter.
- In hardware, the bus would be a set of wires with drivers and fan-out buffers.

---

## COMPONENT 39: Memory_Arbiter

### Purpose
Coordinates access to the single D-Cache port from multiple requestors: committed stores (from Commit_Unit) and load buffer memory requests (LB0, LB1, LB2). Store commits have highest priority to avoid commit stalls.

### Internal State

```
current_op          — which operation is in progress
current_source      — who requested (0=StoreCommit, 1=LB0, 2=LB1, 3=LB2)
waiting_for_dcache  — D-Cache access in progress
```

### Pseudocode

```
RISING_EDGE(CLK):
    // Default outputs
    StoreCommit_Done_o ← 0
    LB0_MemDone_o ← 0; LB1_MemDone_o ← 0; LB2_MemDone_o ← 0
    DC_Req_o ← 0

    // ================================================================
    // HANDLE D-CACHE RESPONSE
    // ================================================================
    if waiting_for_dcache == 1 AND DC_Done_i == 1:
        CASE current_source OF:
            0:  // Store commit completed
                StoreCommit_Done_o ← 1
            1:  // LB0 load completed
                LB0_MemDone_o ← 1
                LB0_MemData_o ← DC_RData_i
            2:  // LB1 load completed
                LB1_MemDone_o ← 1
                LB1_MemData_o ← DC_RData_i
            3:  // LB2 load completed
                LB2_MemDone_o ← 1
                LB2_MemData_o ← DC_RData_i
        waiting_for_dcache ← 0

    // ================================================================
    // ACCEPT NEW REQUEST (if D-Cache is free)
    // ================================================================
    if waiting_for_dcache == 0 AND DC_Ready_i == 1:
        // Priority: StoreCommit > LB0 > LB1 > LB2
        if StoreCommit_Req_i == 1:
            DC_Req_o    ← 1
            DC_RW_o     ← 1       // Write
            DC_Addr_o   ← StoreCommit_Addr_i
            DC_WData_o  ← StoreCommit_Data_i
            DC_Size_o   ← decode_size(StoreCommit_Op_i)  // STR=64b, STRB=8b
            current_source ← 0
            waiting_for_dcache ← 1

        else if LB0_MemReq_i == 1:
            DC_Req_o    ← 1
            DC_RW_o     ← 0       // Read
            DC_Addr_o   ← LB0_MemAddr_i
            DC_Size_o   ← decode_size(LB0_MemOp_i)  // LDR=64b, LDRB=8b
            current_source ← 1
            waiting_for_dcache ← 1

        else if LB1_MemReq_i == 1:
            DC_Req_o    ← 1
            DC_RW_o     ← 0
            DC_Addr_o   ← LB1_MemAddr_i
            DC_Size_o   ← decode_size(LB1_MemOp_i)
            current_source ← 2
            waiting_for_dcache ← 1

        else if LB2_MemReq_i == 1:
            DC_Req_o    ← 1
            DC_RW_o     ← 0
            DC_Addr_o   ← LB2_MemAddr_i
            DC_Size_o   ← decode_size(LB2_MemOp_i)
            current_source ← 3
            waiting_for_dcache ← 1

// Size decode helper (internal)
function decode_size(op):
    if op in {0x4E, 0x4F}: return 0b11  // LDR/STR → 8 bytes (64-bit)
    if op in {0x50, 0x51}: return 0b00  // LDRB/STRB → 1 byte
```

### Key behavior
- **Store commits have absolute priority** because the Commit_Unit is blocked until StoreCommit_Done arrives. Delaying commit means the ROB can't free entries, which can cause the entire pipeline to stall.
- **Loads are speculative** — they can afford to wait a few extra cycles. They're already behind the store in program order anyway.

---

## COMPONENT 40: D_Cache (Data Cache)

### Purpose
L1 data cache. Handles load and store requests with cache hit/miss semantics. Write-back, write-allocate policy.

### Internal State

```
tag_array[32][4]        — 4-way associative tags
data_array[32][4][64B]  — 64-byte cache lines
valid[32][4], dirty[32][4]
lru_state[32]
miss_pending, writeback_pending
miss_addr, writeback_addr, writeback_data
request_latched         — current request being processed
```

### Pseudocode

```
RISING_EDGE(CLK):
    DC_Done_o  ← 0
    DC_Ready_o ← 1   // Default: ready for new request

    // ================================================================
    // HANDLE RAM RESPONSE (miss fill or writeback complete)
    // ================================================================
    if writeback_pending == 1 AND RAMtoDC_Ready_i == 1:
        writeback_pending ← 0
        // Now fetch the missed line
        DCtoRAM_RdReq_o ← 1
        DCtoRAM_Addr_o  ← miss_addr
        miss_pending ← 1
        DC_Ready_o ← 0

    if miss_pending == 1 AND RAMtoDC_Ready_i == 1:
        // Install new line
        set ← extract_set(miss_addr)
        way ← lru_way  // already selected
        tag_array[set][way] ← extract_tag(miss_addr)
        data_array[set][way] ← RAMtoDC_Data_i
        valid[set][way] ← 1
        dirty[set][way] ← 0
        miss_pending ← 0
        // Retry the original request — it will now hit

    // ================================================================
    // HANDLE NEW REQUEST
    // ================================================================
    if DC_Req_i == 1 AND DC_Ready_o == 1:
        addr ← DC_Addr_i
        set  ← addr[10:6]
        tag  ← addr[63:11]

        // Check for hit
        hit ← 0; hit_way ← 0
        for w in 0..3:
            if valid[set][w] == 1 AND tag_array[set][w] == tag:
                hit ← 1; hit_way ← w

        if hit == 1:
            if DC_RW_i == 0:  // READ
                raw_line ← data_array[set][hit_way]
                byte_offset ← addr[5:0]
                CASE DC_Size_i OF:
                    0b11: DC_RData_o ← raw_line[byte_offset*8 +: 64]   // 8 bytes
                    0b00: DC_RData_o ← zero_extend(raw_line[byte_offset*8 +: 8])  // 1 byte
                DC_Done_o ← 1
            else:  // WRITE
                byte_offset ← addr[5:0]
                CASE DC_Size_i OF:
                    0b11: data_array[set][hit_way][byte_offset*8 +: 64] ← DC_WData_i
                    0b00: data_array[set][hit_way][byte_offset*8 +: 8]  ← DC_WData_i[7:0]
                dirty[set][hit_way] ← 1
                DC_Done_o ← 1
            update_lru(set, hit_way)

        else:  // MISS
            DC_Ready_o ← 0  // Stall new requests
            // Select victim
            victim_way ← select_lru_way(set)
            lru_way ← victim_way

            // Writeback if dirty
            if valid[set][victim_way] == 1 AND dirty[set][victim_way] == 1:
                DCtoRAM_WrReq_o  ← 1
                DCtoRAM_Addr_o   ← reconstruct_addr(tag_array[set][victim_way], set)
                DCtoRAM_WrData_o ← data_array[set][victim_way]
                writeback_pending ← 1
            else:
                // Fetch directly
                DCtoRAM_RdReq_o ← 1
                DCtoRAM_Addr_o  ← addr & ~63  // Line-aligned
                miss_pending ← 1

            miss_addr ← addr & ~63
```

### Key behavior
- **Write-back**: dirty lines are only written to RAM when evicted. This reduces RAM traffic.
- **Write-allocate**: on a write miss, the line is fetched from RAM first, then the write is applied. This ensures the cache always has a complete line.
- **Hit latency**: 1 cycle (DC_Done_o asserted same cycle as request).
- **Miss penalty**: 50-100 cycles (RAM latency) plus possible writeback.

---

## COMPONENT 41: Commit_Unit

### Purpose
The final stage of Tomasulo's pipeline. It retires instructions IN PROGRAM ORDER from the ROB head. Only committed results become architecturally visible (written to Register_File, written to memory). This is what makes the architecture precise: mispredictions and exceptions can be cleanly handled because speculative state is confined to ROB/RS.

### Internal State

```
store_commit_in_progress (1b) — waiting for StoreCommit_Done from Memory_Arbiter
```

### Pseudocode

```
ALWAYS (combinational):
    // Default all outputs to 0
    CommitPop_o         ← 0
    CommitWrEn_o        ← 0
    FlagsCommitEn_o     ← 0
    StoreCommit_Req_o   ← 0
    BranchRedirect_o    ← 0
    Flush_o             ← 0
    FreeSB0_o..FreeRSBr0_o ← 0
    ExceptionSignal_o   ← 0

    // ================================================================
    // GUARD: nothing to commit
    // ================================================================
    if ROBHeadBusy_i == 0:
        RETURN   // ROB is empty

    if ROBHeadReady_i == 0:
        RETURN   // Head instruction hasn't finished executing

    // ================================================================
    // EXCEPTION CHECK (takes priority over everything)
    // ================================================================
    if ROBHeadException_i != 0x0:
        ExceptionSignal_o ← 1
        ExceptionCode_o   ← ROBHeadException_i
        ExceptionPC_o     ← ROBHeadPC_i

        // Flush entire pipeline
        Flush_o    ← 1
        CommitPop_o ← 1
        activate_free(ROBHeadSourceStation_i)  // Free the RS/Buffer
        RETURN

    // ================================================================
    // COMMIT BASED ON INSTRUCTION TYPE
    // ================================================================

    type ← ROBHeadType_i

    // ------------------------------------------------------------------
    // ALU / MUL / FPALU / FPMUL / LOAD (types 000, 001, 010, 011, 100)
    // ------------------------------------------------------------------
    if type in {000, 001, 010, 011, 100}:
        // Write result to register file (if this instruction writes to Rd)
        if ROBHeadDestReg_i != 0xF:
            CommitWrEn_o   ← 1
            CommitWrAddr_o ← ROBHeadDestReg_i
            CommitWrData_o ← ROBHeadValue_i
            CommitROBIdx_o ← ROBHead_i

        // Commit flags (if this instruction produced flags)
        if ROBHeadFlagsValid_i == 1:
            FlagsCommitEn_o    ← 1
            FlagsCommitValue_o ← ROBHeadFlagsResult_i
            CommitROBIdx_o     ← ROBHead_i   // shared with register commit

        // Free the RS/Buffer
        activate_free(ROBHeadSourceStation_i)

        // Pop ROB head
        CommitPop_o ← 1

    // ------------------------------------------------------------------
    // STORE (type 101)
    // ------------------------------------------------------------------
    else if type == 101:
        if ROBHeadStoreReady_i == 0:
            RETURN  // Store address/data not resolved yet — wait

        if store_commit_in_progress == 0:
            // Initiate memory write
            StoreCommit_Req_o  ← 1
            StoreCommit_Addr_o ← ROBHeadStoreAddr_i
            StoreCommit_Data_o ← ROBHeadStoreData_i
            StoreCommit_Op_o   ← ROBHeadType_i  // used to determine STR vs STRB
                                                  // Actually we need the Op... see note below
            store_commit_in_progress ← 1

        if StoreCommit_Done_i == 1:
            // Memory write completed
            store_commit_in_progress ← 0
            activate_free(ROBHeadSourceStation_i)
            CommitPop_o ← 1

        // NOTE: While waiting for StoreCommit_Done, the Commit_Unit is STALLED.
        // No other instructions can commit. This is by design — in-order commit
        // guarantees that the store's memory effect is visible before later
        // instructions commit.

    // ------------------------------------------------------------------
    // BRANCH (type 110)
    // ------------------------------------------------------------------
    else if type == 110:
        if ROBHeadMispredict_i == 1:
            // MISPREDICTION: redirect PC and flush pipeline
            BranchRedirect_o ← 1
            BranchTarget_o   ← ROBHeadBranchTarget_i
            Flush_o          ← 1

        // Whether or not mispredicted, commit the branch
        activate_free(ROBHeadSourceStation_i)
        CommitPop_o ← 1

    // ------------------------------------------------------------------
    // CMP / FLAGS-ONLY (type 111)
    // ------------------------------------------------------------------
    else if type == 111:
        // CMP/CMN/TST/TEQ/FCMP: only commit flags, no register write
        if ROBHeadFlagsValid_i == 1:
            FlagsCommitEn_o    ← 1
            FlagsCommitValue_o ← ROBHeadFlagsResult_i
            CommitROBIdx_o     ← ROBHead_i

        activate_free(ROBHeadSourceStation_i)
        CommitPop_o ← 1


// ================================================================
// HELPER: Activate the correct Free signal based on SourceStation
// ================================================================
function activate_free(station_id):
    CASE station_id OF:
        0000: FreeSB0_o     ← 1
        0001: FreeSB1_o     ← 1
        0010: FreeLB0_o     ← 1
        0011: FreeLB1_o     ← 1
        0100: FreeLB2_o     ← 1
        0101: FreeRSIAU0_o  ← 1
        0110: FreeRSIAU1_o  ← 1
        0111: FreeRSFPAU0_o ← 1
        1000: FreeRSIMUL0_o ← 1
        1001: FreeRSFPMUL0_o← 1
        1010: FreeRSBr0_o   ← 1
        1111: // NOP/SWI — no RS/Buffer to free
```

### Key behavior for in-order commit
- **Only the ROB HEAD can commit** — this guarantees in-order retirement regardless of out-of-order execution.
- **Store writes are deferred to commit**: a store instruction's effect on memory is ONLY visible after commit. This prevents speculative memory corruption. If a branch before the store was mispredicted, the store is discarded during flush.
- **Misprediction recovery**: When a mispredicted branch reaches the head, the Commit_Unit asserts Flush_o. This signal reaches ALL pipeline components simultaneously:
  - Register_File clears all Qi_valid tags (restores committed state)
  - ROB clears all entries (Tail ← Head)
  - All RS/Buffers clear their Busy bits
  - Flags_Unit clears Qi_valid
  - PC_MUX receives BranchTarget and redirects the fetch
- **Exception handling** follows the same flush mechanism but additionally signals the trap handler.
- **One commit per cycle**: even with dual CDB allowing 2 completions per cycle, only ONE instruction commits per cycle (the ROB head). This is a bottleneck in sustained throughput but is necessary for precise exceptions.

---

# END-TO-END TRACE: Tomasulo in Action

## Sample Program (Real ISA Assembly)

```asm
; Program demonstrating OOO execution with Tomasulo
        MOVI  REG0, #100         ; PC=0x00: REG0 ← 100
        MOVI  REG1, #200         ; PC=0x08: REG1 ← 200
        ADD   REG2, REG0, REG1   ; PC=0x10: REG2 ← REG0 + REG1 (data dependency on REG0, REG1)
        MULI  REG3, REG2, #3     ; PC=0x18: REG3 ← REG2 * 3 (data dependency on REG2)
        SUBI  REG4, REG0, #50    ; PC=0x20: REG4 ← REG0 - 50 (independent of REG2/REG3)
        CMP   REG4, REG1         ; PC=0x28: flags ← compare(REG4, REG1)
        BEQ   done               ; PC=0x30: if Z==1, jump to 'done'
        ADDI  REG5, REG3, #10    ; PC=0x38: REG5 ← REG3 + 10 (speculative, depends on REG3)
done:   STR   REG5, [REG0, #0]   ; PC=0x40: MEM[REG0+0] ← REG5
```

### Encoding (from Assembler.cpp rules)

| PC | Instruction | Op | Rd | Rn | Rm | Imm | Hex encoding |
|----|-------------|----|----|----|-----|------|-------------|
| 0x00 | MOVI REG0, #100 | 0x33 | 0 | 0 | - | 100 | Op=0x33, Rd=0, Rn=0, Imm=100 |
| 0x08 | MOVI REG1, #200 | 0x33 | 1 | 1 | - | 200 | Op=0x33, Rd=1, Rn=1, Imm=200 |
| 0x10 | ADD REG2, REG0, REG1 | 0x00 | 2 | 0 | 1 | - | Op=0x00, Rd=2, Rn=0, Rm=1 |
| 0x18 | MULI REG3, REG2, #3 | 0x12 | 3 | 2 | - | 3 | Op=0x12, Rd=3, Rn=2, Imm=3 |
| 0x20 | SUBI REG4, REG0, #50 | 0x0F | 4 | 0 | - | 50 | Op=0x0F, Rd=4, Rn=0, Imm=50 |
| 0x28 | CMP REG4, REG1 | 0x37 | - | 4 | 1 | - | Op=0x37, Rn=4, Rm=1 |
| 0x30 | BEQ done | 0x46 | - | - | - | +16 | Op=0x46, Imm=16 (2 instr × 8 bytes) |
| 0x38 | ADDI REG5, REG3, #10 | 0x0E | 5 | 3 | - | 10 | Op=0x0E, Rd=5, Rn=3, Imm=10 |
| 0x40 | STR REG5, [REG0, #0] | 0x4F | - | 0 | 5 | 0 | Op=0x4F, Rn=0, Rm=5, Imm=0 |

### Cycle-by-Cycle Trace

**Initial state**: All registers = 0, ROB empty, all RS/Buffers free.

---

#### CYCLE 1: Issue MOVI REG0, #100
```
PC = 0x00
I_Cache → InstrF = {Op=0x33, Rd=0, Rn=0, Rm=0, Imm=100}
Instruction_Decoder → Op_in=0x33, Dest_in=0, Rn_in=0, Rm_in=0, Imm_in=100

Control_Unit:
  Op=0x33 → IsIntALU=1, UseImm=1, WritesRd=1
  ModifiesFlags=0 (MOVI doesn't set flags)
  Allocate RS_IntALU0 → AllocRSIntALU0_o=1, AllocSourceStation=0101
  ROBAlloc_o=1, TagWrEn_o=1

Register_File:
  Read port 1: REG[0] → Value=0, Qi_valid=0 → RD1_Value=0, RD1_QiValid=0
  Rename: REG[0].Qi ← ROBTail(0), REG[0].Qi_valid ← 1

ROB:
  Allocate entry [0]: Type=000(ALU), DestReg=0, PC=0x00, SourceStation=0101
  Tail → 1

Extends: IsFP=0 → ImmExt = sign_extend(100) = 0x0000_0000_0000_0064

RS_IntALU0:
  Alloc_i=1 → captures:
    Op=0x33, DestROBTag=0, UseImm=1
    Vj=0 (REG[0] value, but MOVI ignores Vj internally — MOV just passes Vk)
    QjValid=0 (ready)
    Vk=0x64 (from ImmExt), QkValid=0 (ready, it's immediate)
    NeedsFlags=0
    
  Ready immediately! RSIAU0_Req=1
```

**State after cycle 1**:
- REG[0]: Value=0, Qi=0, Qi_valid=1 (renamed to ROB[0])
- ROB[0]: Busy=1, Ready=0, MOVI REG0
- RS_IntALU0: Busy=1, ready to dispatch

---

#### CYCLE 2: Issue MOVI REG1, #200 | Execute MOVI REG0

**ISSUE** (MOVI REG1, #200):
```
PC = 0x08
Control_Unit → IsIntALU=1, UseImm=1
Allocate RS_IntALU1 (RS_IntALU0 is busy) → AllocSourceStation=0110

Register_File:
  Read port 1: REG[1] → Value=0, Qi_valid=0
  Rename: REG[1].Qi ← ROBTail(1), REG[1].Qi_valid ← 1

ROB[1]: Type=000, DestReg=1, SourceStation=0110
Tail → 2

RS_IntALU1: Op=0x33, Vk=0xC8(200), ready immediately
```

**EXECUTE** (MOVI REG0):
```
IntALU_Arbiter: RS_IntALU0 ready → grant RS_IntALU0
IntALU: Op=0x33, A=0, B=0x64 → Result=0x64, ModFlags=0, FlagsValid=0
IntALU_CDBReq=1, ROBTag=0, Value=0x64
```

**CDB_Arbiter**: Only IntALU requesting → CDB_A gets IntALU result
```
CDB_A: Valid=1, ROBTag=0, Value=0x64, FlagsValid=0
```

**CDB SNOOP**:
```
ROB[0]: matches ROBTag=0 → Ready=1, Value=0x64
RS_IntALU1: Qj, Qk both already valid → no match needed
All other RS/Buffers: empty, no matches
```

**State after cycle 2**:
- REG[0]: Value=0, Qi=0, Qi_valid=1
- REG[1]: Value=0, Qi=1, Qi_valid=1
- ROB[0]: Ready=1, Value=0x64 ✓
- ROB[1]: Busy=1, Ready=0
- RS_IntALU0: still Busy (not freed yet — freed at commit)
- RS_IntALU1: Busy=1, ready to dispatch

---

#### CYCLE 3: Issue ADD REG2,REG0,REG1 | Execute MOVI REG1 | Commit MOVI REG0

**COMMIT** (ROB[0] = MOVI REG0):
```
Commit_Unit: ROBHeadBusy=1, ROBHeadReady=1, Type=000, DestReg=0
  CommitWrEn=1, CommitWrAddr=0, CommitWrData=0x64
  Register_File: REG[0].Value ← 0x64
    REG[0].Qi==0 AND Qi_valid==1 → clear: REG[0].Qi_valid ← 0
  FlagsCommitEn=0 (no flags)
  FreeRSIAU0=1 → RS_IntALU0.Busy ← 0
  CommitPop=1 → ROB Head advances to 1
```

**ISSUE** (ADD REG2, REG0, REG1):
```
PC = 0x10
Control_Unit → IsIntALU=1, UseImm=0, ModifiesFlags=1
Allocate RS_IntALU0 (just freed!) → AllocSourceStation=0101

Register_File:
  Read port 1: REG[0] → Value=0x64, Qi_valid=0 (cleared by commit this cycle? 
    IMPORTANT: commit happens on clock edge, but Issue reads combinationally.
    If commit and issue are in the same cycle, the commit write happens first
    on the rising edge, and the rename happens after. So REG[0] is now clean.)
    Actually, let's be precise: at the rising edge, commit writes clear Qi_valid.
    Then combinationally, Issue reads REG[0]: Qi_valid=0, Value=0x64. 
    Then at the NEXT rising edge, rename sets REG[2].Qi = ROBTail(2).
    
    So: RD1_Value=0x64, RD1_QiValid=0 → Vj=0x64, QjValid=0 ✓

  Read port 2: REG[1] → Value=0, Qi=1, Qi_valid=1
    ROB forwarding: ROB[1].Ready? → let's check CDB this cycle...

  Rename: REG[2].Qi ← ROBTail(2), REG[2].Qi_valid ← 1

Flags_Unit: FlagsTagWrEn=1 (ADD modifies flags) → Flags_Qi ← 2, Flags_Qi_valid ← 1
```

**EXECUTE** (MOVI REG1):
```
IntALU_Arbiter: RS_IntALU1 ready → grant
IntALU: Op=0x33, B=0xC8 → Result=0xC8
CDB_A: Valid=1, ROBTag=1, Value=0xC8
```

**CDB SNOOP**:
```
ROB[1]: ROBTag=1 → Ready=1, Value=0xC8
RS_IntALU0 (just allocated for ADD):
  QjValid=0 (already resolved)
  QkValid=1, Qk=1 (waiting for REG[1])
  CDB_A has ROBTag=1 → MATCH! Vk ← 0xC8, QkValid ← 0
  → RS_IntALU0 becomes READY this cycle!
```

**State after cycle 3**:
- REG[0]: Value=0x64, Qi_valid=0 ✓ (committed)
- REG[1]: Value=0, Qi=1, Qi_valid=1 (result in ROB[1], not yet committed)
- REG[2]: Value=0, Qi=2, Qi_valid=1 (renamed to ROB[2])
- ROB[1]: Ready=1, Value=0xC8
- ROB[2]: Busy=1, Ready=0 (ADD)
- RS_IntALU0: Busy, Op=ADD, Vj=0x64, Vk=0xC8, READY
- RS_IntALU1: Busy (holding MOVI REG1, awaiting free at commit)
- Flags_Unit: Qi=2, Qi_valid=1

---

#### CYCLE 4: Issue MULI REG3,REG2,#3 | Execute ADD | Commit MOVI REG1

**COMMIT** (ROB[1] = MOVI REG1):
```
REG[1].Value ← 0xC8, Qi_valid ← 0
FreeRSIAU1=1
Head → 2
```

**ISSUE** (MULI REG3, REG2, #3):
```
Control_Unit → IsIntMUL=1, UseImm=1, ModifiesFlags=1
Allocate RS_IntMUL0 → AllocSourceStation=1000

Register_File:
  Read port 1: REG[2] → Qi=2, Qi_valid=1
    ROB[2].Ready? → Not yet (ADD is executing THIS cycle)
    → Qj=2, QjValid=1 (must wait)
  
Rename: REG[3].Qi ← ROBTail(3), REG[3].Qi_valid ← 1
Flags_Unit: Qi ← 3 (MULI modifies flags)

RS_IntMUL0: Op=0x12, Qj=2(wait for REG2), Vk=3(imm,ready), NOT ready
```

**EXECUTE** (ADD REG2, REG0, REG1):
```
IntALU_Arbiter: RS_IntALU0 ready → grant
IntALU: Op=0x00, A=0x64(100), B=0xC8(200) → Result=0x12C(300)
  ModFlags=1: N=0, Z=0, C=0, V=0
CDB_A: Valid=1, ROBTag=2, Value=0x12C, Flags={0,0,0,0}, FlagsValid=1
```

**CDB SNOOP**:
```
ROB[2]: Ready=1, Value=0x12C, FlagsResult={0,0,0,0}, FlagsValid=1
RS_IntMUL0: QjValid=1, Qj=2 → MATCH! Vj ← 0x12C, QjValid ← 0 → NOW READY!
Flags_Unit: Flags_Qi=2? No, it was just updated to 3 (MULI). No match for old value.
  But RS_Branch0 (empty), RS_IntALU* (empty) — no one snooping for flags tag 2.
```

**State after cycle 4**:
- REG[2]: Qi=2, Qi_valid=1 (not committed yet)
- REG[3]: Qi=3, Qi_valid=1
- ROB[2]: Ready=1, Value=300, Flags={0,0,0,0}
- ROB[3]: Busy=1, Ready=0 (MULI)
- RS_IntMUL0: READY (Vj=300, Vk=3)
- Flags_Unit: Qi=3, Qi_valid=1

---

#### CYCLE 5: Issue SUBI REG4,REG0,#50 | Dispatch MULI to IntMUL | Commit ADD

**COMMIT** (ROB[2] = ADD REG2):
```
REG[2].Value ← 0x12C(300), Qi_valid ← 0
FlagsCommitEn=1, FlagsCommitValue={0,0,0,0}
  Flags_Unit: Flags_arch ← {0,0,0,0}
  Flags_Qi=3, CommitROBIdx=2 → 3≠2, DON'T clear Qi_valid (newer MULI still pending)
FreeRSIAU0=1
Head → 3
```

**ISSUE** (SUBI REG4, REG0, #50):
```
Control_Unit → IsIntALU=1, UseImm=1, ModifiesFlags=1
Allocate RS_IntALU0 → AllocSourceStation=0101

Register_File:
  Read port 1: REG[0] → Value=0x64, Qi_valid=0 → Vj=0x64, QjValid=0 ✓
  Rename: REG[4].Qi ← ROBTail(4), Qi_valid ← 1
  
Flags_Unit: Qi ← 4 (SUBI modifies flags too)

RS_IntALU0: Op=0x0F, Vj=0x64(100), Vk=0x32(50), READY immediately
```

**EXECUTE** (MULI dispatched to IntMUL):
```
IntMUL_Arbiter: RS_IntMUL0 ready → grant
IntMUL: begins MUL 300 × 3, cycles_remaining ← 4
RS_IntMUL0: Executing ← 1
```

**ALSO EXECUTE** (SUBI — can use IntALU since it was just freed):
```
IntALU_Arbiter: RS_IntALU0 ready → grant
IntALU: Op=0x0F, A=100, B=50 → Result=50(0x32)
  ModFlags=1: N=0, Z=0, C=1(no borrow), V=0
CDB_A: IntALU → ROBTag=4, Value=0x32, Flags={0,0,1,0}, FlagsValid=1
```

*Note: MULI is still computing (3 cycles left). IntALU and IntMUL operate IN PARALLEL!*

**CDB SNOOP**:
```
ROB[4]: Ready=1, Value=0x32, Flags={0,0,1,0}, FlagsValid=1
```

**State after cycle 5**:
- ROB[3]: MULI, Busy=1, Ready=0 (3 cycles left in IntMUL)
- ROB[4]: SUBI, Ready=1, Value=50, Flags={0,0,1,0}
- IntMUL: busy, 3 cycles remaining
- Flags_Unit: Qi=4, Qi_valid=1

---

#### CYCLE 6: Issue CMP REG4,REG1 | IntMUL cycle 2 of 4

**ISSUE** (CMP REG4, REG1):
```
Control_Unit → IsIntALU=1, IsCMPonly=1, ModifiesFlags=1, WritesRd=0

Register_File:
  Read port 1 (Rn=REG4): Qi=4, Qi_valid=1
    ROB[4].Ready=1 → Forward! Vj=0x32(50), QjValid=0
  Read port 2 (Rm=REG1): Qi_valid=0, Value=0xC8(200) → Vk=0xC8, QkValid=0
  TagWrEn=0 (CMP doesn't write Rd)

ROB[5]: Type=111(CMP), DestReg=0xF
Flags_Unit: Qi ← 5

RS_IntALU1 (RS0 is still holding SUBI until freed): 
  Op=0x37, Vj=50, Vk=200, READY
```

**EXECUTE**: RS_IntALU1 dispatches CMP to IntALU:
```
IntALU: Op=0x37, A=50, B=200 → temp = 50-200 = -150
  N=1(negative), Z=0, C=0(borrow), V=0
CDB_A: ROBTag=5, Value=0, Flags={1,0,0,0}, FlagsValid=1
```

**CDB SNOOP**:
```
ROB[5]: Ready=1, Flags={1,0,0,0}, FlagsValid=1
```

---

#### CYCLE 7: Issue BEQ done | IntMUL cycle 3 of 4 | Commit ROB[3]? No — not ready

**COMMIT**: ROB Head=3 (MULI) → Ready=0 → CANNOT COMMIT (waiting for IntMUL)

**ISSUE** (BEQ done):
```
Control_Unit → IsBranch=1, NeedsFlags=1

Flags_Unit:
  FlagsQi=5, FlagsQiValid=1
  But ROB[5] already has FlagsValid=1 on the CDB from last cycle...
  Actually, the RS captures at Issue time:
    FlagsQi=5, FlagsQiValid=1 (must wait for CMP's flags)
    BUT CDB_A published ROB tag 5 with flags LAST cycle.
    The Flags_Unit itself: does Flags_Qi still == 5?
    Let's check: CMP committed? No (ROB head is still at 3).
    But the CDB result already updated ROB[5].Ready=1.
    For the RS_Branch0: it captures FlagsQi=5 from Flags_Unit.
    Then it checks CDB this cycle — CDB has no tag 5 this cycle.
    So RS_Branch0 must wait. BUT... ROB[5] IS ready.
    
    Wait — let me reconsider. The CDB published tag 5 in cycle 6.
    At the START of cycle 7, the RS_Branch0 is being allocated.
    During allocation, it reads FlagsQi=5, FlagsQiValid=1.
    It doesn't have the flags yet because it was just allocated.
    CDB is not broadcasting tag 5 this cycle.
    So RS_Branch0: FlagsQi=5, FlagsQiValid=1 → NOT ready.
    
    However! We need a mechanism. The ROB already captured the
    result. But the RS cannot read from ROB for flags forwarding
    in our current design. So the branch must wait until the
    instruction that produces flags (CMP, tag 5) is committed
    and Flags_arch is updated... OR until a new CDB broadcast
    happens to match.
    
    ACTUALLY: Let's reconsider the design. The CDB broadcast
    happened in cycle 6. The RS_Branch0 wasn't allocated until
    cycle 7. So it MISSED the broadcast. The flags are "trapped"
    in ROB[5] but not in the architectural Flags_arch (not committed yet).
    
    SOLUTION: The RS_Branch0 stays waiting. Eventually:
    - ROB[3] (MULI) finishes → commits → ROB[4] commits → ROB[5] commits
    - At ROB[5] commit, FlagsCommitEn=1, Flags_arch ← {1,0,0,0}
    - But RS_Branch0 won't see this via CDB...
    
    BETTER SOLUTION (already in our design): RS_Branch0 captures
    FlagsQi=5 and snoops CDB. Since the CDB already broadcasted
    tag 5 in cycle 6, and RS wasn't allocated yet, the RS missed it.
    
    But wait — this is a real issue in Tomasulo implementations.
    The standard solution is: at Issue time, if FlagsQiValid=1,
    check if ROB[FlagsQi].Ready. If yes, forward the flags from ROB.
    
    We DO have ROB read ports! ROBReadReady1/2 forward register values.
    But we don't have a dedicated ROB read port for FLAGS forwarding.
    
    Let me note this as an ARCHITECTURAL OBSERVATION: in our current
    design, if the flag producer has already written to CDB before
    the flag consumer is issued, the consumer must wait until the
    producer COMMITS (updating Flags_arch). This adds latency.
    
    An optimization would be to add a "ROBReadFlags" port.
    
    For now, let's proceed with the current design.

RS_Branch0: Op=0x46(BEQ), TargetPC=0x00+16+0x30=0x40, FlagsQi=5, NOT ready
```

---

#### CYCLE 8: Issue ADDI REG5,REG3,#10 | IntMUL cycle 4 of 4 (finishes!)

**ISSUE** (ADDI REG5, REG3, #10) — SPECULATIVE (after the BEQ):
```
Control_Unit → IsIntALU=1, UseImm=1, ModifiesFlags=1
Allocate RS_IntALU0 → AllocSourceStation=0101

Register_File:
  Read port 1: REG[3] → Qi=3, Qi_valid=1
    ROB[3].Ready? → IntMUL finishes THIS cycle (cycle 8 = cycle 5 start + 4 - 1)

RS_IntALU0: Qj=3 (wait for MULI result)
Rename: REG[5].Qi ← ROBTail(7)
```

**IntMUL COMPLETES**:
```
IntMUL: cycles_remaining reaches 0 → result = 300 × 3 = 900 (0x384)
  N=0, Z=0, C=0, V=0
IntMUL_CDBReq=1

CDB_Arbiter: IntALU not requesting, FPALU not requesting, IntMUL requesting
  CDB_A → IntMUL: ROBTag=3, Value=0x384, Flags={0,0,0,0}, FlagsValid=1
```

**CDB SNOOP**:
```
ROB[3]: Ready=1, Value=0x384
RS_IntALU0: QjValid=1, Qj=3 → MATCH! Vj ← 0x384, QjValid ← 0 → READY
RS_Branch0: FlagsQi=5, CDB has tag 3 → no match (5≠3)
```

---

#### CYCLE 9: Execute ADDI REG5 | Commit MULI REG3

**COMMIT** (ROB[3] = MULI REG3):
```
REG[3].Value ← 0x384(900)
FlagsCommitEn=1, Flags_arch ← {0,0,0,0}
  Flags_Unit: Qi=5 (set when CMP was issued), CommitROBIdx=3 → 5≠3 → DON'T clear
FreeRSIMUL0=1
Head → 4
```

**EXECUTE** (ADDI REG5):
```
IntALU: Op=0x0E, A=0x384(900), B=10 → Result=910(0x38E)
  ModFlags=1: N=0, Z=0, C=0, V=0
CDB_A: ROBTag=7, Value=0x38E, Flags={0,0,0,0}, FlagsValid=1
```

**CDB SNOOP**:
```
ROB[7]: Ready=1, Value=0x38E
RS_Branch0: FlagsQi=5, tag 7 → no match
```

---

#### CYCLE 10: Commit SUBI REG4

**COMMIT** (ROB[4] = SUBI REG4):
```
REG[4].Value ← 0x32(50)
FlagsCommitEn=1, Flags_arch ← {0,0,1,0}
  Flags_Unit: Qi=5, CommitROBIdx=4 → 5≠4 → don't clear
Head → 5
```

---

#### CYCLE 11: Commit CMP (ROB[5])

**COMMIT** (ROB[5] = CMP, type=111):
```
FlagsCommitEn=1, FlagsCommitValue={1,0,0,0}
  Flags_Unit: Flags_arch ← {1,0,0,0}
  Flags_Qi=5, CommitROBIdx=5 → MATCH! Flags_Qi_valid ← 0
Head → 6
```

Now Flags_Unit: Flags_arch={1,0,0,0}, Flags_Qi_valid=0

**RS_Branch0**: At Issue it captured FlagsQi=5, FlagsQiValid=1.
Since Flags_Qi_valid is now 0 (cleared at commit), but the RS checks its OWN
internal FlagsQiValid — it hasn't received a CDB match for tag 5.
The CDB broadcast for tag 5 happened back in cycle 6, before RS_Branch0 existed.

**IMPORTANT OBSERVATION**: The RS_Branch0 is stuck. It's waiting for CDB to broadcast
ROBTag=5 with FlagsValid=1, but that already happened and won't happen again.

**DESIGN FIX NEEDED**: We need ONE of these solutions:
1. **Add ROB flags read port**: at Issue, if ROB[FlagsQi].Ready AND FlagsValid, forward flags.
2. **Re-broadcast on commit**: when flags are committed, broadcast to RS.
3. **Direct Flags_arch read**: RS reads Flags_arch when Flags_Qi_valid drops to 0.

**Solution 3 is simplest and already partially in our design**: RS_Branch0 can CHECK
each cycle whether FlagsQiValid_o from Flags_Unit has gone to 0. If it has, AND the
RS's internal FlagsQi matched the old producer, then the architectural flags ARE the
correct value (because the producer committed). The RS can then capture FlagsArch_o.

**REVISED RS_Branch0 snoop logic** (addition to existing CDB snoop):
```
// Each cycle, if my flags are still pending:
if FlagsQiValid_internal == 1:
    // Check CDB (existing logic)
    ...
    // ALSO check: has the architectural flag register been updated?
    // If Flags_Unit.FlagsQiValid_o == 0, it means ALL pending flag
    // producers have committed. The current FlagsArch is authoritative.
    if FlagsQiValid_i == 0:   // from Flags_Unit output
        FlagsValue ← FlagsArch_i
        FlagsQiValid_internal ← 0  // resolved!
```

With this fix, in cycle 11 (after CMP commits):
- Flags_Unit outputs FlagsQiValid_o=0
- RS_Branch0 sees FlagsQiValid_i=0 → captures FlagsArch={1,0,0,0}
- RS_Branch0 becomes READY

---

#### CYCLE 12: Execute BEQ | Commit BEQ starts

**Branch_Executor**:
```
Op=0x46(BEQ), Flags={1,0,0,0}: N=1, Z=0, C=0, V=0
BEQ checks Z==1 → Z=0 → taken=0
Predicted=0 → mispredict = (0 ≠ 0) = 0  → CORRECT PREDICTION!
BrExResult: Valid=1, ROBTag=6, Taken=0, Mispredict=0
ROB[6].Ready ← 1, Mispredict ← 0
```

---

#### CYCLE 13: Commit BEQ (ROB[6])

```
Commit_Unit: Type=110(BRANCH), Mispredict=0 → no flush!
FreeRSBr0=1
CommitPop=1
Head → 7
```

---

#### CYCLE 14: Commit ADDI REG5 (ROB[7]) — was speculative, but branch was correct!

```
REG[5].Value ← 0x38E(910)
Head → 8
```

---

#### CYCLE 15+: Issue & Execute STR REG5, [REG0, #0]

```
Issue: Op=0x4F(STR), IsStore=1
  Allocate SB0 → AllocSourceStation=0000
  
  Register_File:
    Read port 1 (Rn=REG0, base): Value=0x64(100), Qi_valid=0 → BaseValue=100
    Read port 2 (Rm=REG5, data): Value=0x38E(910), Qi_valid=0 → DataValue=910

  SB0: BaseValue=100, BaseTagValid=0, Offset=0, DataValue=910, DataTagValid=0
    → AGUReq immediately (base ready)
    
AGU: Address = 100 + 0 = 100. SegCheck: LOWER ≤ 100 ≤ UPPER? (depends on limits)
  Assuming valid: Address=100, SegFault=0

SB0 → StComplete: Valid=1, Addr=100, Data=910 → ROB[8].StoreReady=1, Ready=1

Commit: Type=STORE, StoreReady=1
  StoreCommit_Req → Memory_Arbiter → D_Cache write
  Wait for Done → CommitPop
```

---

## Summary: Key Tomasulo Observations from this Trace

1. **Issue never stalls for data dependencies**: MULI was issued in cycle 4 even though REG2 wasn't ready. The RS_IntMUL0 captured tag Qj=2 and waited independently.

2. **Out-of-order execution**: SUBI (cycle 5) executed BEFORE MULI (cycle 8) even though MULI was issued first. This is because SUBI's operands were immediately available while MULI was waiting in the IntMUL pipeline.

3. **CDB bypass enables zero-bubble forwarding**: When ADD's result appeared on CDB in cycle 4, RS_IntMUL0 captured it in the SAME cycle and became ready immediately.

4. **Register renaming eliminates WAW hazards**: REG0 was written by MOVI in cycle 1, then later REG4 (a different register). But if the same register were reused, the Qi tags would ensure correct ordering — each instruction gets its own ROB tag.

5. **Speculative execution past branches**: ADDI REG5 was issued in cycle 8 (after BEQ) speculatively. Since the branch prediction was correct (not taken), the speculative work was valid and committed normally.

6. **Commit is strictly in-order**: even though SUBI and CMP finished before MULI, they couldn't commit until MULI committed first (ROB head must advance sequentially).

7. **Dual CDB**: in this trace only one CDB was typically active per cycle (small program). But with more instructions in flight (e.g., both IntALU and a completing Load), both CDB_A and CDB_B would be used simultaneously.

---

## DESIGN NOTE: Flags Forwarding Gap

As identified in the trace (cycle 7-11), our current design has a gap: if a flag-consuming instruction is issued AFTER the flag-producing instruction has already published on CDB, the consumer misses the broadcast and must wait until the producer commits.

**Recommended fix** (applied to RS_Branch0 and all RS that use NeedsFlags):

Add this logic to the CDB snoop section of every RS that tracks flags:

```
// ADDITIONAL CHECK each cycle (beyond CDB snoop):
// If Flags_Unit reports no pending producer (FlagsQiValid_i == 0),
// then the architectural flags are authoritative and can be used.
if FlagsQiValid_internal == 1 AND FlagsQiValid_i == 0:
    FlagsValue ← FlagsArch_i
    FlagsQiValid_internal ← 0
```

This does NOT change the cable connections (FlagsArch_i and FlagsQiValid_i are already wired to all RS that need flags). It only adds internal logic to the RS.

**Alternative (better but requires new cables)**: Add a ROB flags read port:
- During Issue, if FlagsQiValid=1, read ROB[FlagsQi].FlagsValid and ROB[FlagsQi].FlagsResult
- If FlagsValid=1, capture the flags immediately at Issue time

This would require adding `ROBReadFlagsValid_o`, `ROBReadFlagsResult_o` outputs from ROB and corresponding inputs to all flag-consuming RS. This is the more performant solution.
