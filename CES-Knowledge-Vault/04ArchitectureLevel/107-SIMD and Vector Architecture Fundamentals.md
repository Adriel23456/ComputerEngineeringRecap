---
Theme: SIMD and Vector Architecture Fundamentals
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, DLP, SIMD]
---

## 📚 Idea/Concept

SIMD (Single Instruction, Multiple Data) and vector architectures exploit data-level parallelism by applying the same operation to multiple data elements simultaneously, achieving high throughput for regular, data-parallel computations.

### SIMD Concept

Single instruction operates on multiple data elements:
```
Scalar:     A[0] + B[0] = C[0]  (1 operation)

SIMD:       A[0:3] + B[0:3] = C[0:3]  (4 operations, 1 instruction)
            [a0]     [b0]     [c0]
            [a1]  +  [b1]  =  [c1]
            [a2]     [b2]     [c2]
            [a3]     [b3]     [c3]
```

### Vector Architecture

Historical approach (Cray-1, etc.):
- Long vector registers (64-256 elements)
- Vector instructions process entire vectors
- Memory designed for vector access patterns
- Explicit vector length control

Modern SIMD:
- Short vectors in wide registers (128-512 bits)
- Fixed vector width per instruction
- Part of general-purpose ISA extensions

### SIMD Extensions (x86)

| Extension | Year | Register Width | Elements (32-bit) |
|-----------|------|---------------|-------------------|
| MMX | 1997 | 64 bits | 2 |
| SSE | 1999 | 128 bits | 4 |
| AVX | 2011 | 256 bits | 8 |
| AVX-512 | 2016 | 512 bits | 16 |

ARM SIMD:
- NEON: 128-bit vectors
- SVE/SVE2: Scalable Vector Extension (128-2048 bits)

### Vector vs Array Processor

Array Processor:
- Multiple processing elements (PEs)
- Each PE has ALU and local memory
- Lockstep execution
- Example: ILLIAC IV, Connection Machine

Vector Processor:
- Single CPU with vector registers
- Pipelined functional units
- Elements processed sequentially but pipelined
- Example: Cray-1, NEC SX

### VMIPS Example Architecture

Simplified vector extension to MIPS:
- 8 vector registers (V0-V7)
- Each holds 64 64-bit elements
- Vector length register (VLR)
- Vector mask register (VM)

Vector Operations:
```assembly
LV    V1, R1        ; Load vector from address in R1
ADDV  V3, V1, V2    ; V3[i] = V1[i] + V2[i]
MULV  V4, V3, V5    ; V4[i] = V3[i] * V5[i]
SV    V4, R2        ; Store vector to address in R2
```

### DAXPY Example

Double-precision A*X Plus Y: Y = a*X + Y

Scalar code:
```c
for (i = 0; i < n; i++)
    Y[i] = a * X[i] + Y[i];
```

Vector code (VMIPS):
```assembly
      LV    V1, Rx        ; Load X vector
      MULVS V2, V1, F0    ; V2 = a * X (scalar multiply)
      LV    V3, Ry        ; Load Y vector
      ADDV  V4, V2, V3    ; V4 = a*X + Y
      SV    V4, Ry        ; Store result to Y
```

### Performance Model Terms

Convoy:
- Set of vector instructions that can execute together
- No structural or data hazards between them

Chime:
- Time for one convoy to execute
- Approximately 1 chime ≈ n cycles (n = vector length)

Initiation Rate:
- Rate at which new vector operations start
- Limited by structural hazards

Lane:
- Parallel execution unit
- 4 lanes → 4 elements processed per cycle

Example: 4 lanes, 64-element vector
- Elements per cycle: 4
- Cycles per vector operation: 64/4 = 16

### Vector Chaining

Forwarding for vectors:
- Result of one vector op forwarded to next
- Reduces convoy barriers
- Example: MULV result chains to ADDV input

Without chaining:
```
MULV V2, V1, V3   ; Wait for complete result
ADDV V4, V2, V5   ; Then start add
```

With chaining:
```
MULV V2, V1, V3   ; Elements of V2 produced gradually
ADDV V4, V2, V5   ; ADDV starts as V2 elements available
```

### Handling Conditionals

Vector Mask Register:
- Bit vector indicating which elements to process
- Disabled elements not modified

```assembly
      SETLT VM, V1, F0    ; Set mask where V1[i] < 0
      ADDV  V2, V1, V3    ; Only add where mask=1
```

Gather/Scatter:
- Index vector specifies non-contiguous access
- Enables indirect addressing patterns

### SIMD vs Vector Trade-offs

| Aspect | SIMD | Vector |
|--------|------|--------|
| Width | Fixed (128-512b) | Variable (VLR) |
| Code | Width-specific | Length-agnostic |
| Efficiency | Best at width | Any length |
| Hardware | Simpler | More complex |
| Conditionals | Masking | Masking |

### Applications

Well-suited:
- Scientific computing (matrix operations)
- Media processing (image/video/audio)
- Machine learning (tensor operations)
- Graphics rendering
- Signal processing

Poorly suited:
- Irregular data access
- Heavy branching
- Small data sets
- Pointer-chasing algorithms

## 📌 Key Points (Optional)
- SIMD/Vector enables massive throughput for data-parallel workloads.
- Modern CPUs rely heavily on SIMD for performance (multimedia, ML).

## 🖼️ Recommended Image
- Diagram showing SIMD operation on vector registers with parallel ALUs.

## 🔗 Connections
- [[052-Flynns Taxonomy and Parallel Architectures]]
- [[053-Parallelism Levels BLP ILP DLP TLP]]
- [[108-Vector Performance Model Chime Convoy Lane]]
- [[110-GPU Architecture and SIMT Model]]
