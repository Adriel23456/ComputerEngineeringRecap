---
Theme: Software Pipelining
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, ILP, Optimization]
---

## 📚 Idea/Concept

Software pipelining is a loop optimization technique that overlaps iterations of a loop, creating a schedule where different stages of consecutive iterations execute concurrently, achieving steady-state parallelism.

### Concept Overview

Traditional loop execution:
```
Iteration 1: [Stage A][Stage B][Stage C]
Iteration 2:                            [Stage A][Stage B][Stage C]
Iteration 3:                                                        [Stage A]...
```

Software pipelined:
```
Prologue:     [A1]
              [B1][A2]
Steady-state: [C1][B2][A3]
              [C2][B3][A4]
              [C3][B4][A5]
              ...
Epilogue:     [C_n-1][B_n]
              [C_n]
```

### Key Concepts

Initiation Interval (II):
- Cycles between starting consecutive iterations
- Goal: Minimize II (maximize throughput)

Kernel (Steady State):
- Repeating pattern during main execution
- All pipeline stages active
- New iteration starts every II cycles

Prologue:
- Fill the pipeline
- Partial iterations at start

Epilogue:
- Drain the pipeline
- Partial iterations at end

### Example: DAXPY Loop

Original code:
```assembly
Loop: LD   F0, 0(R1)      ; Load X[i]
      MULD F2, F0, F4     ; a * X[i]
      LD   F6, 0(R2)      ; Load Y[i]
      ADDD F6, F2, F6     ; a*X[i] + Y[i]
      SD   F6, 0(R2)      ; Store Y[i]
      ADDI R1, R1, #8     ; Increment X pointer
      ADDI R2, R2, #8     ; Increment Y pointer
      BNE  R1, R3, Loop   ; Loop if not done
```

Latencies:
- LD: 2 cycles
- MULD: 4 cycles
- ADDD: 2 cycles

### Software Pipelined Version

With II = 2 (ideal):
```
Cycle | Iteration i | Iteration i+1 | Iteration i+2
------|-------------|---------------|---------------
  1   | LD X[i]     |               |
  2   | LD Y[i]     | LD X[i+1]     |
  3   | MULD        | LD Y[i+1]     | LD X[i+2]
  4   | (wait)      | MULD          | LD Y[i+2]
  5   | ADDD        | (wait)        | MULD
  6   | SD          | ADDD          | (wait)
  7   | (done)      | SD            | ADDD
  8   |             | (done)        | SD
```

### Modulo Scheduling

Algorithm for software pipelining:

1. Build dependence graph with latencies
2. Compute Minimum Initiation Interval (MII):
   - Resource MII: Based on functional unit constraints
   - Recurrence MII: Based on loop-carried dependencies
   - MII = max(Resource MII, Recurrence MII)

3. Try to schedule at II = MII
4. If fail, increment II and retry

### Resource MII Calculation

$$ResMII = \max_i \left\lceil \frac{uses_i}{units_i} \right\rceil$$

Example:
- 2 loads, 1 load unit → ⌈2/1⌉ = 2
- 1 multiply, 1 mult unit → ⌈1/1⌉ = 1
- 1 add, 1 add unit → ⌈1/1⌉ = 1

Resource MII = max(2, 1, 1) = 2

### Recurrence MII Calculation

For loop-carried dependency:
$$RecMII = \left\lceil \frac{latency}{distance} \right\rceil$$

Example: A[i] = A[i-1] + B[i]
- Latency of A computation: 5 cycles
- Distance: 1 iteration
- RecMII = ⌈5/1⌉ = 5

### Modulo Reservation Table

Track resource usage modulo II:

| Cycle mod II | Load | Mult | Add | Store |
|--------------|------|------|-----|-------|
| 0 | X | X | | X |
| 1 | X | | X | |

All iterations use same pattern, offset by II.

### Register Allocation

Challenge: Multiple iterations in flight need different registers

Solutions:
- Rotating register files (hardware support)
- Modulo variable expansion (software)
- Unroll enough to cover lifetime

Rotating registers (IA-64):
```
LD   R[32], ...     ; R32 for current iteration
                    ; After branch, R32 becomes R33
                    ; New R32 available for next iteration
```

### Prologue and Epilogue Generation

Prologue:
```
; Cycle 0: Start iteration 0
LD   F0, 0(R1)
; Cycle 1: Continue iter 0, start iter 1  
LD   F6, 0(R2)
LD   F10, 8(R1)
; Cycle 2: Now at steady state
```

Epilogue:
- Continue executing without starting new iterations
- Handle remaining partial iterations

### Loop Count Considerations

If loop count N < pipeline depth:
- May not reach steady state
- Prologue/epilogue dominate
- Consider not pipelining short loops

### Comparison with Loop Unrolling

| Aspect | Loop Unrolling | Software Pipelining |
|--------|---------------|---------------------|
| Code size | Larger (replicated) | Moderate |
| Iterations | Reduced | Same |
| Parallelism | Within unrolled body | Across iterations |
| Register pressure | Higher | Controlled by II |
| Loop-carried deps | Exposed | Scheduled around |

### Hardware Support

Helpful features:
- Rotating registers (IA-64)
- Predicated execution
- Branch prediction for loop exit
- Modulo scheduling support

### Practical Considerations

Best suited for:
- Loops with predictable iteration count
- Regular memory access patterns
- Moderate loop-carried dependencies
- VLIW architectures

Challenges:
- Complex code generation
- Increased register pressure
- Code size for prologue/epilogue
- Dynamic trip counts

## 📌 Key Points (Optional)
- Software pipelining achieves throughput of one iteration per II cycles.
- Effective for loops where II can be kept small.

## 🖼️ Recommended Image
- Timeline showing overlapped iterations in prologue, steady-state, and epilogue.

## 🔗 Connections
- [[082-Loop Unrolling and Software ILP Techniques]]
- [[079-VLIW Architecture and Static Scheduling]]
- [[080-Code Scheduling for VLIW]]
- [[064-Instruction Level Parallelism ILP Fundamentals]]
- [[081-Basic Block and Control Flow Analysis]]