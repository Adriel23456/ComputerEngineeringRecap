---
Theme: Addressing Modes and Effective Address Calculation
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, ISA, Fundamentals]
---

## 📚 Idea/Concept

Addressing modes define the methods by which instructions specify the location of operands, ranging from immediate values embedded in instructions to complex memory address calculations.

### Why Addressing Modes Matter

Different operand locations:
- Constants (immediates)
- Registers
- Memory locations (various methods)

Trade-offs:
- Flexibility vs instruction encoding complexity
- Memory traffic vs register pressure
- Code density vs execution speed

### Common Addressing Modes

| Mode | Operand Location | Example | Use Case |
|------|-----------------|---------|----------|
| Immediate | In instruction | ADD R1, #5 | Constants |
| Register | In register | ADD R1, R2 | Fast access |
| Direct | At fixed address | LOAD R1, [1000] | Global variables |
| Indirect | Address in register | LOAD R1, [R2] | Pointers |
| Displacement | Base + offset | LOAD R1, [R2+100] | Struct fields, arrays |
| Indexed | Base + index | LOAD R1, [R2+R3] | Array access |
| Scaled Index | Base + index×scale | LOAD R1, [R2+R3*4] | Array of structs |
| Auto-increment | Register, then increment | LOAD R1, [R2]+ | Sequential access |
| Auto-decrement | Decrement, then register | LOAD R1, -[R2] | Stack operations |
| PC-relative | PC + offset | BRANCH [PC+100] | Position-independent |

### Effective Address Calculation

The Effective Address (EA) is the actual memory address accessed:

| Mode | EA Calculation |
|------|---------------|
| Direct | EA = Address_in_instruction |
| Register Indirect | EA = [Register] |
| Displacement | EA = [Base_Register] + Displacement |
| Indexed | EA = [Base_Register] + [Index_Register] |
| Scaled Indexed | EA = [Base] + [Index] × Scale + Displacement |

### x86 Complex Addressing

Full x86 addressing mode:
$$EA = Base + (Index \times Scale) + Displacement$$

Where:
- Base: Any general register
- Index: Any register except ESP
- Scale: 1, 2, 4, or 8
- Displacement: 8-bit or 32-bit constant

Example:
```nasm
MOV EAX, [EBX + ECX*4 + 100]  ; Array of ints: arr[i] with base at EBX+100
```

### RISC Addressing (ARM, RISC-V)

Simplified modes for pipeline efficiency:

ARM:
```assembly
LDR R0, [R1]           ; Register indirect
LDR R0, [R1, #4]       ; Immediate offset
LDR R0, [R1, R2]       ; Register offset
LDR R0, [R1, R2, LSL #2] ; Scaled register
LDR R0, [R1], #4       ; Post-indexed (auto-increment)
LDR R0, [R1, #4]!      ; Pre-indexed (update base)
```

RISC-V (minimal):
```assembly
LW  x1, 0(x2)          ; Base + 12-bit immediate only
```

### Addressing Mode Comparison

| Architecture | Modes | Philosophy |
|--------------|-------|------------|
| x86 | Many (20+) | CISC: Powerful instructions |
| ARM | Moderate (8-10) | Balanced flexibility |
| MIPS | Few (3-4) | RISC: Simple decoding |
| RISC-V | Minimal (1-2) | Ultra-simple pipeline |

### Address Calculation Hardware

Dedicated Address Generation Unit (AGU):
```
┌─────────────────────────────────────────┐
│           Address Generation Unit        │
│  ┌──────┐   ┌──────┐   ┌──────────┐    │
│  │ Base │ + │Index │ × │  Scale   │    │
│  └──┬───┘   └──┬───┘   └────┬─────┘    │
│     │          │            │          │
│     └────────┬─┴────────────┘          │
│              │                         │
│         ┌────▼────┐                    │
│         │  Adder  │ ← Displacement     │
│         └────┬────┘                    │
│              │                         │
│         Effective Address              │
└─────────────────────────────────────────┘
```

### Impact on Pipeline

Simple modes (register, immediate):
- Address known at decode
- No additional calculation cycle

Complex modes (scaled indexed):
- May require dedicated AGU
- Could add pipeline stage or use ALU

### Code Examples

Array access A[i]:
```c
// C code
x = A[i];

// x86: Complex addressing
MOV EAX, [EBX + ESI*4]  ; One instruction

// RISC-V: Multiple instructions
SLLI t0, t1, 2          ; i * 4
ADD  t0, t0, t2         ; base + offset
LW   t3, 0(t0)          ; load
```

Structure field access:
```c
// struct { int a; int b; } *ptr;
// x = ptr->b;

// x86
MOV EAX, [EBX+4]        ; Displacement mode

// RISC-V
LW  x1, 4(x2)           ; Same syntax, simple mode
```

### PC-Relative Addressing

Used for:
- Branch targets
- Position-independent code (PIC)
- Shared libraries

```assembly
; Branch to label (PC + offset)
BEQ label     ; if equal, PC = PC + offset

; Load from PC-relative address (ARM)
LDR R0, =variable  ; Pseudo-instruction, assembler calculates offset
```

Advantage: Code can run at any address (relocatable).

## 📌 Key Points (Optional)
- CISC architectures offer many modes; RISC architectures prefer simplicity.
- Complex addressing can reduce instruction count but complicates hardware.

## 🖼️ Recommended Image
- Diagram showing different addressing modes with memory/register illustrations.

## 🔗 Connections
- [[054-ISA Instruction Set Architecture Fundamentals]]
- [[060-CISC vs RISC Design Philosophy]]
- [[072-RISC-V Architecture and Design Philosophy]]
- [[065-Pipeline MIPS Five Stage Design]]
- [[045-ALU Architecture Operations Flags and Control Signals]]