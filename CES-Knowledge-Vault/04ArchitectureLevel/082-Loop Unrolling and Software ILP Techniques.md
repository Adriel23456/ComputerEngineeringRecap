---
Theme: Loop Unrolling and Software ILP Techniques
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, ILP, Optimization]
---

## 📚 Idea/Concept

Loop unrolling is a compiler optimization technique that replicates the loop body multiple times to increase instruction-level parallelism, reduce loop overhead, and enable better scheduling.

### Basic Concept

Original Loop:
```c
for (i = 0; i < 100; i++) {
    A[i] = B[i] + C[i];
}
```

Unrolled by Factor 4:
```c
for (i = 0; i < 100; i += 4) {
    A[i]   = B[i]   + C[i];
    A[i+1] = B[i+1] + C[i+1];
    A[i+2] = B[i+2] + C[i+2];
    A[i+3] = B[i+3] + C[i+3];
}
```

### Benefits of Loop Unrolling

1. Reduced Loop Overhead:
   - Fewer branch instructions
   - Fewer index increments and comparisons
   - Less branch prediction pressure

2. Increased ILP:
   - More independent instructions visible to scheduler
   - Better utilization of functional units
   - Enables software pipelining

3. Better Register Utilization:
   - More opportunities for register allocation
   - Reduced memory traffic through register reuse

4. Improved Scheduling:
   - Larger basic blocks for compiler optimization
   - More freedom to reorder instructions

### Assembly Example

Original (MIPS-style):
```assembly
Loop: LD   F0, 0(R1)
      ADD  F4, F0, F2
      SD   F4, 0(R1)
      SUBI R1, R1, #8
      BNE  R1, R2, Loop
```
With stalls: ~10 cycles per iteration

Unrolled (factor 4) and scheduled:
```assembly
Loop: LD   F0, 0(R1)
      LD   F6, -8(R1)
      LD   F10, -16(R1)
      LD   F14, -24(R1)
      ADD  F4, F0, F2
      ADD  F8, F6, F2
      ADD  F12, F10, F2
      ADD  F16, F14, F2
      SD   F4, 0(R1)
      SD   F8, -8(R1)
      SD   F12, -16(R1)
      SD   F16, -24(R1)
      SUBI R1, R1, #32
      BNE  R1, R2, Loop
```
Result: ~3.5 cycles per iteration

### Costs of Loop Unrolling

1. Code Size Increase:
   - Larger binaries
   - Potential instruction cache pressure

2. Register Pressure:
   - More registers needed for renamed variables
   - May cause register spilling

3. Remainder Handling:
   - Need cleanup code for iterations not divisible by unroll factor
   - Adds complexity

### Loop-Carried Dependencies

Dependencies between iterations limit unrolling effectiveness:

No Loop-Carried Dependencies (fully parallelizable):
```c
A[i] = B[i] + C[i]  // Each iteration independent
```

Loop-Carried Dependencies present:
```c
Y[i] = Y[i-5] + Y[i]  // Iteration i depends on i-5
```
Maximum unroll factor = 5 (iterations i through i+4 are independent)

### Register Renaming in Unrolling

To avoid false dependencies after unrolling:
- Use different registers for each unrolled iteration
- Original: F0 → After unrolling: F0, F6, F10, F14

### VLIW Application

Unrolling factor 7 with VLIW-5 architecture:
- Multiple operations packed per instruction
- Can achieve 1.28 cycles per original iteration
- Requires sufficient parallel hardware

## 📌 Key Points (Optional)
- Unrolling trades code size for performance by exposing more parallelism.
- Loop-carried dependencies set the maximum useful unroll factor.

## 🖼️ Recommended Image
- Side-by-side comparison of original loop and unrolled loop execution timeline.

## 🔗 Connections
- [[064-Instruction Level Parallelism ILP Fundamentals]]
- [[079-VLIW Architecture and Static Scheduling]]
- [[080-Code Scheduling for VLIW]]
- [[081-Basic Block and Control Flow Analysis]]
- [[083-Software Pipelining]]