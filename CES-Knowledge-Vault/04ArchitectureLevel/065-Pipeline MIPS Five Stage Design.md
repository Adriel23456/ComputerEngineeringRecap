---
Theme: Pipeline MIPS Five Stage Design
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Microarchitecture, Pipeline]
---

## 📚 Idea/Concept

The classic MIPS 5-stage pipeline divides instruction execution into five sequential stages, enabling instruction overlap to achieve near one-instruction-per-cycle throughput under ideal conditions.

### The Five Stages

| Stage | Abbreviation | Function |
|-------|--------------|----------|
| 1 | IF | Instruction Fetch: Read instruction at PC, update PC |
| 2 | ID | Instruction Decode: Decode opcode, read registers |
| 3 | EX | Execute: ALU operation, address calculation |
| 4 | MEM | Memory Access: Load/Store operations |
| 5 | WB | Write Back: Write result to register file |

### Pipeline Operation

Ideal execution timeline (no hazards):

| Cycle | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|-------|---|---|---|---|---|---|---|---|---|
| Instr i | IF | ID | EX | MEM | WB | | | | |
| Instr i+1 | | IF | ID | EX | MEM | WB | | | |
| Instr i+2 | | | IF | ID | EX | MEM | WB | | |
| Instr i+3 | | | | IF | ID | EX | MEM | WB | |
| Instr i+4 | | | | | IF | ID | EX | MEM | WB |

### Theoretical Speedup

Without pipeline: Each instruction takes 5 cycles
With pipeline: After filling, one instruction completes per cycle

$$Speedup_{max} = \frac{T_{unpipelined}}{T_{pipelined}} \approx N_{stages}$$

For 5-stage pipeline: Speedup ≈ 5× (ideal)

### Pipeline Assumptions

1. All instructions pass through all stages
2. Each stage has dedicated hardware (no resource sharing)
3. Equal stage delays (balanced pipeline)
4. Instructions are independent (no hazards)
5. Stages are perfectly isolatable in time

### Pipeline Registers

Inter-stage registers store intermediate results:
- IF/ID: Holds fetched instruction
- ID/EX: Holds decoded values and control signals
- EX/MEM: Holds ALU result and branch target
- MEM/WB: Holds memory data or ALU result

### Advantages

- Increased instruction throughput
- Better hardware utilization
- Deterministic timing per stage

### Disadvantages

- Increased hardware complexity
- Higher latency per individual instruction
- Slowest stage determines clock frequency
- Pipeline hazards cause stalls

## 📌 Key Points (Optional)
- Real-world pipelines rarely achieve ideal speedup due to hazards.
- Modern processors use 10-20+ stage pipelines for higher frequencies.

## 🖼️ Recommended Image
- Pipeline diagram showing five stages with instructions flowing through over time.

## 🔗 Connections
- [[066-Pipeline Hazards and Classifications]]
- [[067-Data Hazards RAW WAR WAW]]
- [[068-Control Hazards and Branch Prediction]]
- [[064-Instruction Level Parallelism ILP Fundamentals]]