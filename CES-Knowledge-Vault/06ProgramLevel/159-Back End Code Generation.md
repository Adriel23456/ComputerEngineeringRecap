---
Theme: Back End Code Generation
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, BackEnd, CodeGen]
---

## 📚 Idea/Concept

The Back End of a compiler transforms optimized IR into target machine code. This involves instruction selection, register allocation, and instruction scheduling to generate efficient executable code for the specific target architecture.

### Back End Phases
```
Optimized IR
     ↓
┌─────────────────────┐
│ Instruction         │  Map IR operations to machine instructions
│ Selection           │
└─────────────────────┘
     ↓
┌─────────────────────┐
│ Register            │  Assign IR temporaries to physical registers
│ Allocation          │
└─────────────────────┘
     ↓
┌─────────────────────┐
│ Instruction         │  Reorder instructions for performance
│ Scheduling          │
└─────────────────────┘
     ↓
┌─────────────────────┐
│ Code                │  Emit assembly or machine code
│ Emission            │
└─────────────────────┘
```

### Instruction Selection

Map IR operations to target machine instructions:

**Simple Macro Expansion**:
```
IR: t1 = a + b
x86: MOV EAX, [a]
     ADD EAX, [b]
     MOV [t1], EAX
```

**Tree Pattern Matching**:
Match IR trees to instruction patterns for better code.
```
IR Tree:          Pattern Match:
   ADD            Intel LEA instruction
  /   \           LEA EAX, [EBX + ECX*4]
 x    MUL         (when scale is 1, 2, 4, or 8)
     /   \
    y     4
```

**CISC vs RISC Considerations**:
- CISC: Complex instructions, memory operands
- RISC: Simple instructions, load/store architecture

### Register Allocation

Assign IR temporaries to physical registers:

**Goals**:
- Maximize register usage
- Minimize memory accesses (spilling)
- Handle calling conventions

**Graph Coloring Algorithm**:
1. Build interference graph (variables live at same time conflict)
2. Color graph with K colors (K = number of registers)
3. If not colorable, spill variables to memory
```
Interference Graph:
    a --- b
    |     |
    c --- d --- e

If 3 registers available (K=3):
  a → R1
  b → R2
  c → R2
  d → R3
  e → R1
```

**Spilling**:
When more variables than registers:
```asm
; Spill variable x to stack
MOV [ESP-4], EAX    ; Store
; ... use register for something else ...
MOV EAX, [ESP-4]    ; Reload
```

### Instruction Scheduling

Reorder instructions to minimize pipeline stalls:

**Dependency Types**:
- **RAW** (Read After Write): True dependency
- **WAR** (Write After Read): Anti-dependency
- **WAW** (Write After Write): Output dependency

**List Scheduling**:
```
Ready list: instructions with all dependencies satisfied
Repeat:
  Select highest priority ready instruction
  Schedule it
  Update ready list
Until all scheduled
```

**Example**:
```asm
; Before scheduling (stalls on dependency)
LOAD  R1, [A]       ; 3 cycle latency
ADD   R2, R1, #1    ; Must wait for R1
LOAD  R3, [B]
ADD   R4, R3, #1

; After scheduling (hide latency)
LOAD  R1, [A]
LOAD  R3, [B]       ; Execute while waiting for R1
ADD   R2, R1, #1    ; R1 now ready
ADD   R4, R3, #1    ; R3 now ready
```

### Calling Conventions

Rules for function calls:

**x86-64 System V (Linux)**:
- Integer args: RDI, RSI, RDX, RCX, R8, R9
- Floating args: XMM0-XMM7
- Return value: RAX (integer), XMM0 (float)
- Caller-saved: RAX, RCX, RDX, R8-R11
- Callee-saved: RBX, RBP, R12-R15

**Prologue/Epilogue**:
```asm
; Function prologue
push    rbp
mov     rbp, rsp
sub     rsp, 32         ; Local variables

; Function body
...

; Function epilogue
leave                   ; mov rsp, rbp; pop rbp
ret
```

### Code Emission

Generate final assembly or object code:

**Assembly Output**:
```asm
.text
.globl  factorial
factorial:
    push    rbp
    mov     rbp, rsp
    cmp     edi, 1
    jle     .base_case
    mov     eax, edi
    dec     edi
    call    factorial
    imul    eax, edi
    jmp     .done
.base_case:
    mov     eax, 1
.done:
    pop     rbp
    ret
```

**Object File Sections**:
- `.text`: Executable code
- `.data`: Initialized data
- `.bss`: Uninitialized data
- `.rodata`: Read-only data

### GCC Back End (RTL)

GCC uses Register Transfer Language (RTL) as its low-level IR:
```
(insn 5 4 6 2 (set (reg:SI 0 ax)
        (plus:SI (reg:SI 1 dx)
                 (reg:SI 2 cx))) "test.c":3 -1
     (nil))
```

## 📌 Key Points (Optional)
- Instruction selection maps IR to target machine instructions
- Register allocation assigns temporaries to physical registers
- Instruction scheduling hides latencies and avoids stalls
- Calling conventions ensure interoperability between functions

## 🖼️ Recommended Image
- Diagram showing the transformation from IR through the back end phases to assembly

## 🔗 Connections
- [[150-Compiler Fundamentals and Structure]]
- [[158-Middle End Optimization]]
- [[160-GCC Compiler Architecture]]