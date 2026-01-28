---
Theme: Vector Performance Model Chime Convoy Lane
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, DLP, Performance]
---

## 📚 Idea/Concept

The vector performance model provides a framework for analyzing and predicting the execution time of vector programs using concepts of convoys, chimes, lanes, and initiation rates.

### Basic Definitions

Vector Length (n):
- Number of elements in vector operation
- May be set by Vector Length Register (VLR)

Maximum Vector Length (MVL):
- Hardware limit on vector register size
- Typically 64-256 elements

Lane:
- Parallel execution path in vector unit
- Each lane processes one element per cycle
- p lanes → p elements per cycle

### Convoy

Definition:
- Set of vector instructions that can begin execution in the same clock cycle
- No structural hazards between instructions in convoy
- No data hazards (or chaining available)

Convoy Formation Rules:
1. Instructions using same functional unit → different convoys
2. RAW hazard without chaining → different convoys
3. Independent instructions → same convoy

### Chime

Definition:
- Unit of time to execute one convoy
- Approximate measure ignoring overhead
- One chime ≈ n cycles for n-element vector

Execution Time Model:
$$T_{execute} = \text{chimes} \times \frac{n}{\text{lanes}}$$

### Initiation Rate

Definition:
- Rate at which vector unit can start new operations
- Measured in operations per cycle
- Limited by:
  - Functional unit availability
  - Memory bandwidth
  - Structural hazards

### Performance Calculation Example

VMIPS Configuration:
- Vector registers: 64 elements
- Lanes: 4
- Functional units: 1 load/store, 1 multiply, 1 add

DAXPY Code:
```assembly
      LV    V1, Rx        ; Load X
      MULVS V2, V1, F0    ; V2 = a * X
      LV    V3, Ry        ; Load Y
      ADDV  V4, V2, V3    ; V4 = a*X + Y
      SV    V4, Ry        ; Store result
```

Convoy Analysis (with chaining):
```
Convoy 1: LV V1, Rx
Convoy 2: MULVS V2, V1, F0 ; LV V3, Ry (parallel: different FUs)
Convoy 3: ADDV V4, V2, V3
Convoy 4: SV V4, Ry
```

Execution Time:
- 4 convoys × 64 elements / 4 lanes = 64 cycles
- Time per element = 64/64 = 1 cycle

Without chaining:
```
Convoy 1: LV V1, Rx
Convoy 2: MULVS V2, V1, F0
Convoy 3: LV V3, Ry
Convoy 4: ADDV V4, V2, V3
Convoy 5: SV V4, Ry
```
- 5 convoys × 64/4 = 80 cycles

### Overhead Considerations

Startup Time:
- Pipeline fill time for each vector operation
- First result delayed by pipeline depth
- Amortized over vector length

Strip Mining:
- When n > MVL, must process in chunks
- Each chunk has loop overhead
- Total time = ⌈n/MVL⌉ × (convoy_time + overhead)

Memory Latency:
- Vector loads/stores have initial latency
- Subsequent elements pipelined
- Bank conflicts may cause stalls

### Sustained vs Peak Performance

Peak FLOPS:
$$\text{Peak} = \text{lanes} \times \text{FP units} \times \text{frequency}$$

Sustained FLOPS:
- Limited by memory bandwidth
- Limited by convoy structure
- Limited by vector length

### Arithmetic Intensity

$$A_i = \frac{\text{Total FP Operations}}{\text{Total Bytes Transferred}}$$

Operations/byte ratio determines:
- Compute-bound (high Ai): Limited by FLOPS
- Memory-bound (low Ai): Limited by bandwidth

DAXPY Arithmetic Intensity:
- Operations: 2n (1 multiply, 1 add per element)
- Memory: 3n × 8 bytes (load X, load Y, store Y, 64-bit)
- Ai = 2n / 24n = 1/12 ≈ 0.083 FLOPS/byte

Result: DAXPY is memory-bound.

### Performance Metrics Summary

| Metric | Formula | Unit |
|--------|---------|------|
| Cycles per convoy | n / lanes | cycles |
| Total cycles | chimes × n / lanes + overhead | cycles |
| FLOPS | FP ops / time | FLOP/s |
| Bandwidth used | bytes transferred / time | B/s |
| Efficiency | sustained / peak | % |

### Strip Mining Example

n = 1000 elements, MVL = 64

Iterations: ⌈1000/64⌉ = 16

Processing:
```
for (i = 0; i < 1000; i += 64) {
    VL = min(64, 1000 - i);  // Vector length
    // Vector operations on VL elements
}
```

Overhead:
- 16 loop iterations
- Each has setup cost (VL adjustment, address calc)
- Reduces effective performance

## 📌 Key Points (Optional)
- Longer vectors amortize startup costs, improving efficiency.
- Memory bandwidth often limits vector performance more than compute capacity.

## 🖼️ Recommended Image
- Timeline diagram showing convoy execution with lanes and chimes labeled.

## 🔗 Connections
- [[107-SIMD and Vector Architecture Fundamentals]]
- [[109-Arithmetic Intensity and Roofline Model]]
- [[043-Performance Metrics CPI IPC and Execution Time]]
- [[086-Memory Hierarchy Principles and Locality]]