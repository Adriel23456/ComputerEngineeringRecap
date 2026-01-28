---
Theme: Instruction Level Parallelism ILP Fundamentals
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ILP, Parallelism]
---

## 📚 Idea/Concept

Instruction-Level Parallelism (ILP) refers to the potential overlap in execution of instructions within a single thread, exploited by hardware and compiler techniques to improve single-thread performance.

### Definition

ILP is a programming and implementation technique that overlaps execution of multiple instructions to reduce total execution time on a single processor.

### Approaches to Exploit ILP

Hardware (Dynamic):
- Decisions made at runtime
- Examples: Dynamic pipeline, Out-of-Order Execution
- Recovery mechanism: Pipeline flush
- Advantage: Adapts to runtime conditions

Software (Static):
- Optimizations at compile time
- Examples: VLIW, static scheduling
- Recovery mechanism: NOPs
- Advantage: Simpler hardware

### Principal ILP Techniques

1. Pipelining:
   - Divide instruction execution into overlapped stages
   - IF → ID → EX → MEM → WB

2. Out-of-Order Execution (OoOE):
   - Dynamically reorder instructions
   - Execute instructions whose dependencies are resolved
   - Keep functional units busy

3. VLIW (Very Long Instruction Word):
   - Compiler bundles independent operations
   - Multiple operations per instruction word
   - Static scheduling

4. Superscalar:
   - Multiple functional units
   - Dispatch multiple instructions per cycle
   - IPC > 1 possible

5. Speculation:
   - Execute instructions before conditions confirmed
   - Branch prediction enables speculative execution
   - Rollback if speculation wrong

6. Register Renaming:
   - Eliminate false dependencies (WAR/WAW)
   - Map architectural to physical registers

### Basic Block Limitation

Basic Block: Uninterrupted instruction sequence (no internal branches)

Typical basic block: 4-7 instructions
Limited ILP within single basic block
Techniques needed to exploit ILP across blocks:
- Loop unrolling
- Trace scheduling
- Speculation

### ILP vs Other Parallelism

| Aspect | ILP | TLP |
|--------|-----|-----|
| Granularity | Instructions | Threads |
| Management | Hardware/Compiler | OS/Programmer |
| Visibility | Transparent | Explicit |
| Scaling | Limited by dependencies | Limited by parallelism |

## 📌 Key Points (Optional)
- ILP hit a wall around 2005, driving the shift to multi-core (TLP).
- Practical ILP extraction is limited by true data dependencies and control flow.

## 🖼️ Recommended Image
- Diagram showing ILP techniques as overlapping instruction execution streams.

## 🔗 Connections
- [[053-Parallelism Levels BLP ILP DLP TLP]]
- [[065-Pipeline MIPS Five Stage Design]]
- [[073-Out of Order Execution Fundamentals]]
- [[079-VLIW Architecture and Static Scheduling]]
- [[078-Superscalar Processor Architecture]]