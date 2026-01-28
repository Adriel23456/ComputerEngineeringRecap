---
Theme: VLIW Architecture and Static Scheduling
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ILP, VLIW]
---

## 📚 Idea/Concept

VLIW (Very Long Instruction Word) architectures exploit ILP through compiler-scheduled instruction bundles, moving complexity from hardware to software for simpler, lower-power designs.

### Core Philosophy

Superscalar: Hardware discovers parallelism at runtime
VLIW: Compiler discovers parallelism at compile time

Result: Simpler hardware, compiler does the hard work

### VLIW Instruction Format

One "instruction" contains multiple operations:
```
┌────────┬────────┬────────┬────────┬────────┬────────┐
│ Int Op1│ Int Op2│ Mem Op1│ Mem Op2│ FP Op1 │ FP Op2 │
└────────┴────────┴────────┴────────┴────────┴────────┘
         64-128+ bits wide
```

Each slot maps to a specific functional unit.

### Key Characteristics

- Multiple operations per instruction word
- Fixed operation slots for specific FU types
- Compiler determines parallel operations
- No runtime dependency checking
- No out-of-order execution
- In-order issue and completion

### Typical VLIW Configuration

| Unit Type | Count | Latency |
|-----------|-------|---------|
| Integer ALU | 2 | 1 cycle |
| Load/Store | 2 | 3 cycles |
| Floating Point | 2 | 4 cycles |

Total instruction width: 6 operations × ~20 bits = 120+ bits

### Compiler Responsibilities

1. Dependency Analysis:
   - Identify RAW, WAR, WAW dependencies
   - Build dependency graph

2. Scheduling:
   - Assign operations to time slots
   - Respect latencies and dependencies
   - Fill slots with parallel operations

3. NOP Insertion:
   - Insert NOPs when no parallel work available
   - Maintain correct timing

### Dependency Management

Unlike superscalar (hardware handles), VLIW compiler must:
```
Cycle 1: [MUL A,B,C | NOP | NOP | NOP]     ; MUL has 2-cycle latency
Cycle 2: [NOP | NOP | NOP | NOP]           ; Wait for MUL
Cycle 3: [ADD X,A,D | NOP | NOP | NOP]     ; Now can use A
```

### Operation Latencies

| Producer | Consumer | Latency |
|----------|----------|---------|
| FP ALU | FP ALU | 3 cycles |
| FP ALU | Store | 2 cycles |
| Load | FP ALU | 1 cycle |
| Load | Store | 0 cycles |
| Integer | Integer | 0 cycles |

### VLIW vs Superscalar

| Aspect | Superscalar | VLIW |
|--------|-------------|------|
| Scheduling | Dynamic (HW) | Static (compiler) |
| Dependency Check | Runtime | Compile time |
| Hardware | Complex | Simple |
| Power | Higher | Lower |
| Code Size | Normal | Larger (NOPs) |
| Binary Compatibility | Good | Poor |
| Compiler Complexity | Normal | Very high |

### VLIW Advantages

1. Simpler hardware:
   - No dependency detection logic
   - No dynamic scheduling
   - No speculation hardware

2. Lower power consumption:
   - Less control logic
   - Deterministic execution

3. Predictable timing:
   - Good for real-time systems
   - WCET analysis easier

### VLIW Disadvantages

1. Code bloat:
   - NOPs fill empty slots
   - Can be 2-3× larger

2. Poor binary compatibility:
   - Different implementations need recompilation
   - Latencies baked into binary

3. Limited ILP visibility:
   - Compiler can't see runtime events
   - Cache misses, branches unpredictable

4. Branch handling:
   - No speculation (typically)
   - Must use predication or software techniques

### Real-World VLIW Examples

- Intel Itanium (IA-64): EPIC variant
- TI C6000 DSP series
- Transmeta Crusoe
- Various DSPs and embedded processors

## 📌 Key Points (Optional)
- VLIW shifts complexity to compiler, enabling simpler, lower-power hardware.
- Best suited for regular, predictable workloads (DSP, embedded).

## 🖼️ Recommended Image
- Diagram showing VLIW instruction format with multiple operation slots.

## 🔗 Connections
- [[064-Instruction Level Parallelism ILP Fundamentals]]
- [[080-Code Scheduling for VLIW]]
- [[078-Superscalar Processor Architecture]]
- [[082-Loop Unrolling and Software ILP Techniques]]
- [[084-Predicated Execution]]