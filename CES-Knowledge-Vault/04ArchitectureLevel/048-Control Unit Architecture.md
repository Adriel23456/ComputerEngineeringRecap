---
Theme: Control Unit Architecture
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Microarchitecture, Control]
---

## 📚 Idea/Concept

The Control Unit generates the control signals that orchestrate the operation of all processor components, interpreting instructions and coordinating the datapath to execute them correctly.

### Control Unit Role

Input: Instruction opcode and function fields
Output: Control signals for:
- Register file (read/write enables, register select)
- ALU (operation select)
- Memory (read/write, address source)
- Multiplexers (data path selection)
- Program counter (next PC calculation)

### Control Unit Types

Hardwired Control:
- Combinational logic circuits
- Fast execution
- Difficult to modify
- Used in RISC processors

Microprogrammed Control:
- Control signals stored in control memory
- Microinstructions executed sequentially
- Flexible, easy to modify
- Higher latency
- Used in CISC processors

### Hardwired Control Structure

```
┌─────────────────────────────────────────────────────┐
│                  Instruction Register               │
│     ┌────────┬───────────┬──────────────────┐      │
│     │ Opcode │   Funct   │     Other        │      │
│     └───┬────┴─────┬─────┴──────────────────┘      │
│         │          │                               │
│         ▼          ▼                               │
│    ┌─────────────────────┐                         │
│    │  Combinational      │                         │
│    │  Control Logic      │                         │
│    │  (AND/OR gates,     │                         │
│    │   decoders, etc.)   │                         │
│    └──────────┬──────────┘                         │
│               │                                    │
│    ┌──────────┴──────────────────────────────┐    │
│    │  Control Signals                         │    │
│    │  RegWrite, MemRead, MemWrite, ALUSrc,   │    │
│    │  RegDst, Branch, MemtoReg, ALUOp, ...   │    │
│    └──────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────┘
```

### MIPS Control Signals

| Signal | Purpose | 0 | 1 |
|--------|---------|---|---|
| RegDst | Write register | rt field | rd field |
| RegWrite | Enable register write | No | Yes |
| ALUSrc | ALU second operand | Register | Immediate |
| MemRead | Read from memory | No | Yes |
| MemWrite | Write to memory | No | Yes |
| MemtoReg | Data to register | ALU result | Memory data |
| Branch | Branch instruction | No | Yes |
| Jump | Jump instruction | No | Yes |

### Control Signal Generation (MIPS)

Truth table for main control:

| Instruction | RegDst | ALUSrc | MemtoReg | RegWrite | MemRead | MemWrite | Branch | ALUOp |
|-------------|--------|--------|----------|----------|---------|----------|--------|-------|
| R-type | 1 | 0 | 0 | 1 | 0 | 0 | 0 | 10 |
| lw | 0 | 1 | 1 | 1 | 1 | 0 | 0 | 00 |
| sw | X | 1 | X | 0 | 0 | 1 | 0 | 00 |
| beq | X | 0 | X | 0 | 0 | 0 | 1 | 01 |
| j | X | X | X | 0 | 0 | 0 | 0 | XX |

### ALU Control

Two-level decoding:
1. Main control generates ALUOp (2 bits)
2. ALU control uses ALUOp + funct field

| ALUOp | Funct | ALU Action |
|-------|-------|------------|
| 00 | XXXX | Add (for lw/sw) |
| 01 | XXXX | Subtract (for beq) |
| 10 | 100000 | Add |
| 10 | 100010 | Subtract |
| 10 | 100100 | AND |
| 10 | 100101 | OR |
| 10 | 101010 | SLT |

### Microprogrammed Control

Control memory (microcode ROM):
```
┌─────────────────────────────────────────────────────┐
│                Instruction Register                  │
│                      │                              │
│                      ▼                              │
│              ┌───────────────┐                      │
│              │ Microprogram  │                      │
│              │ Address Logic │                      │
│              └───────┬───────┘                      │
│                      │                              │
│                      ▼                              │
│              ┌───────────────┐                      │
│              │ Control Memory│                      │
│              │  (Microcode)  │                      │
│              └───────┬───────┘                      │
│                      │                              │
│              ┌───────▼───────┐                      │
│              │Microinstruction│                     │
│              │   Register     │                     │
│              └───────┬───────┘                      │
│                      │                              │
│              Control Signals + Next Address         │
└─────────────────────────────────────────────────────┘
```

### Microinstruction Format

Horizontal microcode:
- One bit per control signal
- Wide words (50-100+ bits)
- Fast (direct signal generation)
- Large control memory

Vertical microcode:
- Encoded control fields
- Narrower words
- Requires decoding
- Smaller control memory

### FSM-Based Control (Multi-Cycle)

For multi-cycle processor:
```
         ┌─────────┐
         │  Fetch  │
         └────┬────┘
              ▼
         ┌─────────┐
         │ Decode  │
         └────┬────┘
              │
    ┌─────────┼─────────┬─────────┐
    ▼         ▼         ▼         ▼
┌───────┐ ┌───────┐ ┌───────┐ ┌───────┐
│MemRef │ │R-type │ │Branch │ │ Jump  │
│Execute│ │Execute│ │Complete│ │Complete│
└───┬───┘ └───┬───┘ └───────┘ └───────┘
    │         │
    ▼         ▼
┌───────┐ ┌───────┐
│ Memory│ │R-type │
│ Access│ │ WB    │
└───┬───┘ └───────┘
    │
    ▼
┌───────┐
│MemWB │
└───────┘
```

### Pipeline Control

Control signals travel with instruction through pipeline:
```
IF/ID → ID/EX → EX/MEM → MEM/WB
        Control signals propagate →
```

Each stage uses appropriate subset of control signals.

### Hazard Control

Control unit extended for:
- Stall insertion (pipeline bubbles)
- Forwarding control
- Branch resolution
- Exception handling

### Exception Control

When exception detected:
1. Save PC to EPC register
2. Set Cause register
3. Jump to exception handler
4. Modify control flow

## 📌 Key Points (Optional)
- Control complexity grows with instruction set complexity.
- Hardwired is faster; microprogrammed is more flexible.

## 🖼️ Recommended Image
- Block diagram showing control unit inputs (opcode) and output signals.

## 🔗 Connections
- [[045-ALU Architecture Operations Flags and Control Signals]]
- [[065-Pipeline MIPS Five Stage Design]]
- [[070-Hazard Detection Unit]]
- [[051-Single Cycle vs Multi Cycle vs Pipelined Processor]]