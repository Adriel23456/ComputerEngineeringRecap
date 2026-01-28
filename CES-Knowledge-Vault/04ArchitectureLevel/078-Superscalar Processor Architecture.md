---
Theme: Superscalar Processor Architecture
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ILP, Microarchitecture]
---

## 📚 Idea/Concept

Superscalar processors can issue and execute multiple instructions per clock cycle by replicating functional units and using sophisticated dispatch logic, achieving IPC greater than 1.

### Definition

A superscalar processor:
- Fetches multiple instructions per cycle
- Decodes multiple instructions per cycle
- Issues multiple instructions per cycle
- Has multiple functional units executing in parallel

### Superscalar vs Scalar vs Superpipeline

| Type | Issue Width | Pipeline | IPC Target |
|------|-------------|----------|------------|
| Scalar | 1 | N stages | 1 |
| Superpipeline | 1 | 2N stages (deeper) | 1 (higher freq) |
| Superscalar | M | N stages | M |

### Typical Superscalar Organization

```
        ┌─────────────────────────────────────┐
        │          Instruction Cache          │
        └─────────────────────────────────────┘
                         │
                    Fetch 4-8 instructions
                         │
        ┌─────────────────────────────────────┐
        │       Decode/Rename (4-wide)        │
        └─────────────────────────────────────┘
                         │
        ┌─────────────────────────────────────┐
        │    Dispatch Buffer / Issue Queue    │
        └─────────────────────────────────────┘
              │      │      │      │
           ┌──┴──┐┌──┴──┐┌──┴──┐┌──┴──┐
           │ ALU ││ ALU ││ FPU ││LS U │
           └─────┘└─────┘└─────┘└─────┘
                         │
        ┌─────────────────────────────────────┐
        │          Reorder Buffer             │
        └─────────────────────────────────────┘
                         │
                      Commit
```

### Key Components

Multiple Functional Units:
- 2-4 integer ALUs
- 1-2 FP units
- 1-2 load/store units
- 1 branch unit

Wide Fetch:
- Fetch 4-8 instructions per cycle
- Branch prediction for continuous fetch
- Instruction buffer/queue

Parallel Decode:
- Decode multiple instructions simultaneously
- Identify dependencies
- Perform register renaming

Issue Logic:
- Select instructions to dispatch
- Check operand availability
- Respect functional unit constraints

### Issue Policies

In-order Issue:
- Issue instructions in program order
- Stall if dependent instruction can't issue
- Simpler but leaves FUs idle

Out-of-order Issue:
- Issue any ready instruction
- Requires dependency tracking
- Better FU utilization

### Dispatch Window

Issue queue holds decoded, renamed instructions:
- Instructions wait for operands
- When ready, compete for FU
- Selection logic picks which instructions issue

Typical sizes: 32-96 entries

### Challenges

Dependency Detection:
- Must check N×N dependencies for N-wide issue
- Quadratic complexity growth

Branch Handling:
- Multiple branches in flight
- Complex prediction and recovery

Register Renaming:
- Must rename N instructions per cycle
- Large physical register file needed

Bypass Network:
- Results must forward to all FUs
- Crossbar complexity grows with width

Power Consumption:
- Wide issue = high power
- Diminishing returns past 4-6 wide

### Real-World Examples

| Processor | Issue Width | Year |
|-----------|-------------|------|
| Intel P6 | 3 | 1995 |
| AMD K7 | 3 | 1999 |
| Intel Core | 4 | 2006 |
| Apple M1 | 8 | 2020 |
| AMD Zen 4 | 6 | 2022 |

### Performance Limits

Actual IPC << Issue Width due to:
- Data dependencies
- Control dependencies (branches)
- Memory latency
- Limited ILP in programs

Typical achieved: 1.5-3 IPC on 4-6 wide designs

## 📌 Key Points (Optional)
- Superscalar relies heavily on OoO execution and speculation to fill issue slots.
- Diminishing returns make very wide (>8) issue impractical for general code.

## 🖼️ Recommended Image
- Block diagram showing multiple parallel pipelines with shared front-end and ROB.

## 🔗 Connections
- [[064-Instruction Level Parallelism ILP Fundamentals]]
- [[073-Out of Order Execution Fundamentals]]
- [[076-Tomasulo Algorithm]]
- [[079-VLIW Architecture and Static Scheduling]]
- [[071-Scalar and Superpipeline Processors]]