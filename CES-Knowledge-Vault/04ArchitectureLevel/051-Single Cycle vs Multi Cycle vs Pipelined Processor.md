---
Theme: Single Cycle vs Multi Cycle vs Pipelined Processor
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Microarchitecture, ProcessorDesign]
---

## 📚 Idea/Concept

Processor implementations differ in how they execute instructions over clock cycles, ranging from single-cycle (one instruction per long cycle) to pipelined (multiple instructions overlapped), each with distinct performance and complexity trade-offs.

### Single-Cycle Processor

Concept:
- Every instruction completes in exactly one clock cycle
- All hardware active simultaneously
- Clock period = longest instruction latency

Datapath:
```
┌────┐   ┌────────┐   ┌───────┐   ┌─────┐   ┌────────┐   ┌───────┐
│ PC │──▶│  Instr │──▶│ Reg   │──▶│ ALU │──▶│  Data  │──▶│ Reg   │
│    │   │  Mem   │   │ File  │   │     │   │  Mem   │   │ Write │
└────┘   └────────┘   └───────┘   └─────┘   └────────┘   └───────┘
  │                                                           │
  └───────────────────────────────────────────────────────────┘
                     All in ONE cycle
```

Timing Example:
| Component | Delay |
|-----------|-------|
| Instruction Memory | 200 ps |
| Register Read | 100 ps |
| ALU | 200 ps |
| Data Memory | 200 ps |
| Register Write | 100 ps |

Longest path (lw): 200 + 100 + 200 + 200 + 100 = 800 ps
Clock period = 800 ps (1.25 GHz)

Advantages:
- Simple control (combinational)
- Easy to understand and design
- No hazards

Disadvantages:
- Clock speed limited by slowest instruction
- Hardware underutilized
- Inefficient for fast instructions

### Multi-Cycle Processor

Concept:
- Each instruction takes variable number of cycles
- Hardware shared across cycles
- Clock period = longest single stage

Execution phases:
1. Instruction Fetch (IF)
2. Instruction Decode / Register Read (ID)
3. Execution / Address Calculation (EX)
4. Memory Access (MEM)
5. Write Back (WB)

Instruction Cycle Counts:
| Instruction | Cycles |
|-------------|--------|
| R-type | 4 |
| lw | 5 |
| sw | 4 |
| beq | 3 |
| j | 3 |

Timing Example:
- Each stage: 200 ps
- Clock period: 200 ps (5 GHz)
- lw instruction: 5 × 200 = 1000 ps
- R-type: 4 × 200 = 800 ps

Advantages:
- Faster clock
- Different instructions take different times
- Hardware reuse (one ALU for all operations)

Disadvantages:
- Complex control (FSM)
- Still one instruction at a time
- Registers needed between stages

### Pipelined Processor

Concept:
- Overlap execution of multiple instructions
- Each stage works on different instruction
- Throughput: ~1 instruction per cycle (ideal)

Pipeline stages (MIPS 5-stage):
```
Time →
Cycle:    1    2    3    4    5    6    7    8    9
Instr 1: [IF] [ID] [EX] [MEM][WB]
Instr 2:      [IF] [ID] [EX] [MEM][WB]
Instr 3:           [IF] [ID] [EX] [MEM][WB]
Instr 4:                [IF] [ID] [EX] [MEM][WB]
Instr 5:                     [IF] [ID] [EX] [MEM][WB]
```

Timing:
- Clock period: 200 ps (same as multi-cycle stage)
- Throughput: 1 instruction per 200 ps (after filling)
- Latency: 5 × 200 = 1000 ps per instruction

Advantages:
- High throughput (instructions per second)
- Good hardware utilization
- Foundation for modern processors

Disadvantages:
- Pipeline hazards require handling
- More complex control
- Increased latency per instruction

### Performance Comparison

For n instructions:

| Processor | Execution Time |
|-----------|---------------|
| Single-Cycle | n × T_longest |
| Multi-Cycle | Σ(cycles_i × T_stage) |
| Pipelined | (n + stages - 1) × T_stage |

Example (1000 instructions, 20% loads, 80% R-type):

Single-Cycle (800 ps clock):
- Time = 1000 × 800 ps = 800,000 ps

Multi-Cycle (200 ps clock):
- Time = (200 × 5 + 800 × 4) × 200 ps = 4,200 × 200 = 840,000 ps

Pipelined (200 ps clock):
- Time = (1000 + 4) × 200 ps = 200,800 ps

Pipelined is ~4x faster!

### Hardware Comparison

| Resource | Single | Multi | Pipeline |
|----------|--------|-------|----------|
| ALUs | 1-3 | 1 | 1+ |
| Memories | 2 (I + D) | 1 | 2 (I + D) |
| Registers | Few | More | Pipeline regs |
| Control | Simple | FSM | Pipelined |
| Clock | Slow | Fast | Fast |

### Evolution

Historical progression:
```
Single-Cycle → Multi-Cycle → Pipelined → Superscalar → OoO
    (1950s)      (1960s)      (1970s+)    (1990s)    (2000s)
```

Modern processors are:
- Deeply pipelined (10-20 stages)
- Superscalar (multiple pipes)
- Out-of-order execution
- Speculative

### Design Trade-offs

| Factor | Single | Multi | Pipeline |
|--------|--------|-------|----------|
| CPI | 1 | Variable | ~1 (ideal) |
| Clock Speed | Slow | Medium | Fast |
| Throughput | Low | Medium | High |
| Latency | Low | Medium | Higher |
| Complexity | Low | Medium | High |
| Power | Medium | Low-Medium | Higher |

## 📌 Key Points (Optional)
- Pipelining achieves throughput close to one instruction per cycle.
- Modern processors combine pipelining with superscalar and OoO execution.

## 🖼️ Recommended Image
- Three diagrams comparing instruction flow in single, multi-cycle, and pipelined execution.

## 🔗 Connections
- [[065-Pipeline MIPS Five Stage Design]]
- [[048-Control Unit Architecture]]
- [[066-Pipeline Hazards and Classifications]]
- [[078-Superscalar Processor Architecture]]
- [[073-Out of Order Execution Fundamentals]]