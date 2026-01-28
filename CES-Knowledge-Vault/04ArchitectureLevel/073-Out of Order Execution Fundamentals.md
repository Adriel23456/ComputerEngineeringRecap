---
Theme: Out of Order Execution Fundamentals
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ILP, OoOE]
---

## 📚 Idea/Concept

Out-of-Order Execution (OoOE) is a paradigm where instructions are dispatched and executed in an order different from the program order to maximize functional unit utilization and hide latencies.

### Motivation

In-order execution problem:
```assembly
1. SUB R1, R2, R3     ; Takes 1 cycle
2. ADD R4, R3, R1     ; Depends on R1, must wait
3. ROR R2, R2, #4     ; Independent! But blocked
```

With OoOE: Execute instruction 3 while waiting for instruction 2's dependency

### Key Principles

1. Issue in order: Instructions enter pipeline in program order
2. Execute out of order: Instructions execute when operands ready
3. Complete out of order: Results may finish in any order
4. Commit in order: Results written to architectural state in program order

### OoOE Pipeline Structure

```
┌──────┐   ┌──────┐   ┌──────────┐   ┌──────────┐   ┌──────┐
│  IF  │──▶│  ID  │──▶│ Dispatch │──▶│ Execute  │──▶│ WB   │
└──────┘   └──────┘   │  Buffer  │   │ (OoO)    │   └──────┘
                      └──────────┘   └──────────┘       │
                                                        ▼
                                                  ┌──────────┐
                                                  │ Reorder  │
                                                  │ Buffer   │
                                                  └──────────┘
                                                        │
                                                        ▼
                                                  ┌──────────┐
                                                  │ Commit   │
                                                  │ (in-order)│
                                                  └──────────┘
```

### Dependencies in OoOE

True Dependencies (RAW):
- Cannot be eliminated
- Must wait for producer to complete
- Managed by tracking operand availability

False Dependencies:
- WAR (Anti-dependency): Eliminated by register renaming
- WAW (Output dependency): Eliminated by register renaming

### Register Renaming

Maps architectural registers to larger physical register file:
- Eliminates WAR and WAW hazards
- Allows more parallel execution
- Implemented via rename table (RAT)

Example:
```
Architectural: R1, R2, R3, ...
Physical:      P1, P2, P3, ... P64 (many more)

R1 = R2 + R3  →  P5 = P2 + P3  (R1 maps to P5)
R1 = R4 + R5  →  P8 = P6 + P7  (R1 now maps to P8, no WAW!)
```

### Reorder Buffer (ROB)

Purpose: Maintain program order for commits
Contains:
- Instruction type
- Destination register
- Value (when computed)
- Ready bit
- Exception information

Commit rules:
- Only head of ROB can commit
- Must be completed (ready)
- No exceptions pending

### Scheduling Policies

1. In-order issue, in-order complete (baseline)
2. In-order issue, out-of-order complete
3. Out-of-order issue, out-of-order complete

### Benefits of OoOE

- Hides memory latency
- Better functional unit utilization
- Extracts more ILP
- Tolerates variable latency operations

### Costs of OoOE

- Complex hardware (ROB, rename tables, issue queues)
- Higher power consumption
- Increased design verification effort
- Security vulnerabilities (Spectre, Meltdown)

## 📌 Key Points (Optional)
- OoOE is fundamental to modern high-performance processors.
- In-order commit preserves precise exceptions and program semantics.

## 🖼️ Recommended Image
- Pipeline diagram showing out-of-order execution with ROB and in-order commit.

## 🔗 Connections
- [[064-Instruction Level Parallelism ILP Fundamentals]]
- [[074-Register Renaming and False Dependencies]]
- [[075-Scoreboard Algorithm]]
- [[076-Tomasulo Algorithm]]
- [[077-Reorder Buffer and Precise Exceptions]]