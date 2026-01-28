---
Theme: Datapath Components and Organization
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Microarchitecture, Datapath]
---

## 📚 Idea/Concept

The datapath is the hardware that performs all the data processing operations in a processor, consisting of functional units, registers, buses, and multiplexers interconnected to execute instructions.

### Datapath Overview

Major components:
```
┌─────────────────────────────────────────────────────────────────┐
│                         DATAPATH                                 │
│                                                                 │
│  ┌────┐    ┌────────┐    ┌────────┐    ┌─────┐    ┌────────┐  │
│  │ PC │───▶│  Instr │───▶│Register│───▶│ ALU │───▶│  Data  │  │
│  │    │    │  Mem   │    │  File  │    │     │    │  Mem   │  │
│  └────┘    └────────┘    └────────┘    └─────┘    └────────┘  │
│     │                         │            │           │       │
│     └─────────────────────────┴────────────┴───────────┘       │
│                        Interconnect (Buses, MUXes)             │
└─────────────────────────────────────────────────────────────────┘
```

### Program Counter (PC)

Function:
- Holds address of current/next instruction
- Updated each cycle (PC + 4 for sequential)
- Modified by branches and jumps

Implementation:
```
      ┌───────────┐
      │    PC     │
      │  Register │
      └─────┬─────┘
            │
     ┌──────┴──────┐
     │    MUX      │◀── Branch target
     │  (PC src)   │◀── Jump target
     └──────┬──────┘◀── PC + 4
            │
            ▼
       Next PC
```

### Instruction Memory

Function:
- Stores program instructions
- Addressed by PC
- Read-only during execution

Interface:
- Input: Address (PC)
- Output: 32-bit instruction

### Register File

Function:
- Fast storage for operands
- Multiple read ports (typically 2)
- One write port
- Addressed by register numbers

Structure:
```
        Read Reg 1 ──▶┌──────────────┐
        Read Reg 2 ──▶│              │──▶ Read Data 1
        Write Reg ───▶│  32 × 32-bit │──▶ Read Data 2
        Write Data ──▶│   Registers  │
        RegWrite ────▶│              │
                      └──────────────┘
```

Implementation:
- Decoders for register selection
- Tri-state buffers or MUXes for read
- Write enable gating

### Arithmetic Logic Unit (ALU)

Function:
- Performs arithmetic (add, sub)
- Performs logical operations (and, or, xor)
- Generates status flags

Interface:
```
     A ───────▶┌─────────┐
               │   ALU   │───▶ Result
     B ───────▶│         │───▶ Zero (flag)
               └─────────┘
                    ▲
                    │
               ALU Control
```

### Data Memory

Function:
- Stores program data
- Read (load) and write (store)
- Addressed by ALU result

Interface:
```
     Address ────▶┌──────────────┐
     Write Data ─▶│     Data     │──▶ Read Data
     MemRead ────▶│    Memory    │
     MemWrite ───▶│              │
                  └──────────────┘
```

### Multiplexers (MUX)

Function:
- Select between multiple inputs
- Controlled by control signals
- Critical for datapath flexibility

Common MUXes in MIPS:
| MUX | Selects Between | Control Signal |
|-----|-----------------|----------------|
| ALUSrc | Register / Immediate | ALUSrc |
| RegDst | rt / rd | RegDst |
| MemtoReg | ALU result / Memory data | MemtoReg |
| PCSrc | PC+4 / Branch target | PCSrc |

### Sign Extension Unit

Function:
- Extend 16-bit immediate to 32-bit
- Preserves sign for signed values

```
     16-bit ────▶┌─────────────┐───▶ 32-bit
     immediate   │Sign Extend  │
                 └─────────────┘
                 
     0x8000 → 0xFFFF8000 (negative)
     0x0001 → 0x00000001 (positive)
```

### Adders

Dedicated adders for:
- PC + 4 (next sequential instruction)
- Branch target calculation (PC + 4 + offset×4)

```
         ┌─────────┐
   PC ──▶│    +    │──▶ PC + 4
    4 ──▶│         │
         └─────────┘
```

### Shift Units

Types:
- Shift left 2 (branch offset)
- General shifter (in ALU or separate)

```
     offset ────▶┌──────────────┐───▶ offset × 4
                 │ Shift Left 2 │
                 └──────────────┘
```

### Complete MIPS Datapath

```
                                    ┌───────┐
                              ┌────▶│ +4    │───┐
                              │     └───────┘   │
        ┌──────┐         ┌────┴──┐              │
        │  PC  │────────▶│  MUX  │◀─────────────┤
        └──┬───┘         └───┬───┘              │
           │                 │                  │
           ▼                 │                  │
     ┌──────────┐           │                  │
     │  Instr   │           │     ┌─────────┐  │
     │  Memory  │───────────┼────▶│Shift L 2│──┤
     └────┬─────┘           │     └─────────┘  │
          │                 │          ▲       │
          │    ┌────────────┘          │       │
          │    │                       │       │
          ▼    ▼                       │       │
     ┌─────────────┐    ┌──────────────┴───┐   │
     │  Register   │───▶│       ALU        │───┤
     │    File     │◀───│                  │   │
     └─────────────┘    └────────┬─────────┘   │
          ▲                      │             │
          │                      ▼             │
          │                ┌──────────┐        │
          └────────────────│   Data   │────────┘
                           │  Memory  │
                           └──────────┘
```

### Datapath Timing

Single-cycle timing:
- All operations complete in one clock period
- Clock period ≥ longest path delay

Critical paths:
1. Load: PC → I-Mem → Regs → ALU → D-Mem → Regs
2. R-type: PC → I-Mem → Regs → ALU → Regs

### Bus Architecture

Single bus:
- One shared path
- Sequential transfers
- Simple but slow

Multiple buses:
- Parallel transfers
- Higher performance
- More hardware

### Datapath Width

Affects:
- Register size
- ALU width
- Bus width
- Memory interface

Common widths: 32-bit, 64-bit

## 📌 Key Points (Optional)
- Datapath design determines what operations the processor can perform.
- Control signals configure the datapath for each instruction type.

## 🖼️ Recommended Image
- Complete single-cycle MIPS datapath with all components labeled.

## 🔗 Connections
- [[045-ALU Architecture Operations Flags and Control Signals]]
- [[048-Control Unit Architecture]]
- [[065-Pipeline MIPS Five Stage Design]]
- [[051-Single Cycle vs Multi Cycle vs Pipelined Processor]]
- [[046-Register File Organization]]