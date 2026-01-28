---
Theme: Arithmetic Intensity and Roofline Model
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Performance, DLP]
---

## 📚 Idea/Concept

The Roofline Model is a visual performance analysis framework that relates achievable performance to arithmetic intensity, helping identify whether a workload is compute-bound or memory-bound.

### Arithmetic Intensity Definition

$$A_i = \frac{\text{Total Floating Point Operations}}{\text{Total Bytes Transferred to/from Memory}}$$

Units: FLOPS/Byte (or FLOP/Byte)

Interpretation:
- High Ai: Many operations per byte → compute-bound
- Low Ai: Few operations per byte → memory-bound

### Roofline Model Concept

Two performance ceilings:
1. Peak Compute: Maximum FLOPS of processor
2. Memory Bandwidth: Maximum bytes/second × Ai

Attainable Performance:
$$P_{attainable} = \min(P_{peak}, BW_{memory} \times A_i)$$

### Roofline Graph

```
Performance
(GFLOPS)
    │
    │     ┌─────────────────── Peak Compute
    │    /
    │   /
    │  /    Roofline
    │ /
    │/
    └────────────────────────────────
           Arithmetic Intensity (FLOP/Byte)
```

- Sloped region: Memory-bound (limited by bandwidth)
- Flat region: Compute-bound (limited by peak FLOPS)
- Ridge point: Transition between regions

### Ridge Point

$$A_{ridge} = \frac{P_{peak}}{BW_{memory}}$$

- Below ridge: Memory-bound
- Above ridge: Compute-bound

Example:
- Peak = 100 GFLOPS
- Bandwidth = 50 GB/s
- Ridge = 100/50 = 2 FLOP/Byte

### Common Kernel Arithmetic Intensities

| Kernel | Ai (FLOP/Byte) | Bound |
|--------|---------------|-------|
| STREAM Copy | 0 | Memory |
| DAXPY | 0.083 | Memory |
| SpMV | 0.25 | Memory |
| Dense Matrix-Vector | 0.25 | Memory |
| Dense Matrix-Matrix | O(n) | Compute |
| FFT | ~1.5 | Varies |
| Stencil (3D 7-point) | 0.875 | Memory |

### DAXPY Analysis

Operation: Y[i] = a * X[i] + Y[i]

Per element:
- Operations: 2 (1 multiply, 1 add)
- Memory: 24 bytes (load X: 8B, load Y: 8B, store Y: 8B)

$$A_i = \frac{2}{24} = 0.083\ \text{FLOP/Byte}$$

With 50 GB/s bandwidth:
$$P_{max} = 50 \times 0.083 = 4.17\ \text{GFLOPS}$$

Even a 100 GFLOPS processor achieves only 4.17 GFLOPS on DAXPY!

### Matrix Multiplication Analysis

Operation: C = A × B (n × n matrices)

Operations: 2n³ (n³ multiplies, n³ adds)
Memory (naive): 3n² × 8 bytes (read A, B, write C once)

$$A_i = \frac{2n^3}{24n^2} = \frac{n}{12}$$

For n = 1000: Ai = 83 FLOP/Byte → Compute-bound!

With blocking/tiling: Can achieve even higher Ai by reusing cached data.

### Extended Roofline Model

Additional ceilings for:
- SIMD width (without vectorization)
- Cache bandwidth (L1, L2, L3)
- Instruction-level parallelism
- Thread-level parallelism

```
Performance
    │
    │     ┌───────────────── Peak (all optimizations)
    │    /┌────────────────── Without SIMD
    │   //┌─────────────────── Without ILP
    │  ///
    │ ///
    │///
    └────────────────────────────────
           Arithmetic Intensity
```

### Using Roofline for Optimization

1. Plot current performance point
2. Identify limiting factor (which ceiling?)
3. Apply appropriate optimization:
   - Below roofline slope: Improve memory access
   - Below compute ceiling: Improve computation

Optimization strategies:
- Memory-bound: Prefetching, blocking, data layout
- Compute-bound: Vectorization, parallelization, algorithm

### Measuring Arithmetic Intensity

Hardware counters:
- FLOP counters (or retire instruction counts)
- Memory controller byte counts
- Cache miss counts × line size

Software analysis:
- Static code analysis
- Profiler annotations
- Algorithm analysis

### Limitations of Roofline

Assumptions:
- Perfect overlap of compute and memory
- Uniform memory access pattern
- No other bottlenecks (latency, branches)

Reality:
- Memory latency matters for small working sets
- Irregular access patterns reduce effective bandwidth
- Branch mispredictions, dependencies affect compute

## 📌 Key Points (Optional)
- Roofline quickly identifies optimization opportunities and bottlenecks.
- Most real-world kernels are memory-bound; optimizing memory access is crucial.

## 🖼️ Recommended Image
- Roofline plot with multiple kernels plotted at their arithmetic intensities.

## 🔗 Connections
- [[108-Vector Performance Model Chime Convoy Lane]]
- [[107-SIMD and Vector Architecture Fundamentals]]
- [[086-Memory Hierarchy Principles and Locality]]
- [[043-Performance Metrics CPI IPC and Execution Time]]
- [[058-Benchmarking Latency and Throughput Metrics]]