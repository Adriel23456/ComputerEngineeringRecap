---
Theme: GPU Architecture and SIMT Model
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, GPU, DLP]
---

## 📚 Idea/Concept

Graphics Processing Units (GPUs) are massively parallel processors originally designed for graphics rendering but now widely used for general-purpose computing (GPGPU), employing the SIMT (Single Instruction, Multiple Threads) execution model.

### GPU vs CPU Philosophy

CPU Design:
- Optimized for latency (fast single-thread)
- Large caches to hide memory latency
- Complex control (OoO, speculation, branch prediction)
- Few powerful cores (4-64)

GPU Design:
- Optimized for throughput (many threads)
- Small caches, hide latency with threads
- Simple control (in-order, massive parallelism)
- Many simple cores (thousands)

### GPU Architecture Overview

```
┌─────────────────────────────────────────────────────┐
│                        GPU                          │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐  │
│  │   SM    │ │   SM    │ │   SM    │ │   SM    │  │
│  │ ┌─────┐ │ │ ┌─────┐ │ │ ┌─────┐ │ │ ┌─────┐ │  │
│  │ │Cores│ │ │ │Cores│ │ │ │Cores│ │ │ │Cores│ │  │
│  │ └─────┘ │ │ └─────┘ │ │ └─────┘ │ │ └─────┘ │  │
│  │ ┌─────┐ │ │ ┌─────┐ │ │ ┌─────┐ │ │ ┌─────┐ │  │
│  │ │SHMem│ │ │ │SHMem│ │ │ │SHMem│ │ │ │SHMem│ │  │
│  │ └─────┘ │ │ └─────┘ │ │ └─────┘ │ │ └─────┘ │  │
│  └────┬────┘ └────┬────┘ └────┬────┘ └────┬────┘  │
│       └───────────┴───────────┴───────────┘       │
│                        │                          │
│                  ┌─────┴─────┐                    │
│                  │  L2 Cache │                    │
│                  └─────┬─────┘                    │
│                        │                          │
│                  ┌─────┴─────┐                    │
│                  │  GDDR/HBM │                    │
│                  └───────────┘                    │
└─────────────────────────────────────────────────────┘
```

### Streaming Multiprocessor (SM)

NVIDIA SM Contains:
- Multiple CUDA cores (32-128)
- Warp schedulers (2-4)
- Register file (64K-256K registers)
- Shared memory (48-164 KB)
- L1 cache / texture cache
- Special Function Units (SFUs)
- Tensor cores (recent GPUs)

### SIMT Execution Model

SIMT (Single Instruction, Multiple Threads):
- Groups of threads execute same instruction
- Each thread has own registers and PC
- Threads grouped into warps (NVIDIA) or wavefronts (AMD)
- Warp size: 32 threads (NVIDIA), 64 threads (AMD)

Warp Execution:
```
Instruction: ADD R1, R2, R3

Thread 0:  R1₀ = R2₀ + R3₀
Thread 1:  R1₁ = R2₁ + R3₁
Thread 2:  R1₂ = R2₂ + R3₂
...
Thread 31: R1₃₁ = R2₃₁ + R3₃₁
```

All 32 threads execute ADD simultaneously (SIMD-style).

### Thread Hierarchy

```
Grid
├── Block 0
│   ├── Warp 0 (threads 0-31)
│   ├── Warp 1 (threads 32-63)
│   └── ...
├── Block 1
│   ├── Warp 0
│   └── ...
└── ...
```

- Grid: All threads for one kernel launch
- Block: Threads that can cooperate (shared memory, sync)
- Warp: Hardware scheduling unit (SIMT execution)

### Warp Divergence

Problem: Branches cause thread divergence

```c
if (threadIdx.x < 16)
    A();  // Threads 0-15
else
    B();  // Threads 16-31
```

Execution:
1. Execute A() with threads 16-31 masked
2. Execute B() with threads 0-15 masked
3. Reconverge after if-else

Impact: Both paths executed serially → 50% efficiency

### Memory Hierarchy

| Memory | Scope | Latency | Bandwidth |
|--------|-------|---------|-----------|
| Registers | Thread | 0 cycles | Highest |
| Shared Memory | Block | ~20 cycles | Very High |
| L1 Cache | SM | ~30 cycles | High |
| L2 Cache | Device | ~200 cycles | Medium |
| Global Memory | Device | ~400 cycles | Lower |

### Latency Hiding

GPU strategy: Use many threads to hide memory latency

While warp 0 waits for memory:
- Schedule warp 1
- Schedule warp 2
- ...
- Warp 0 data arrives, resume

Occupancy:
$$\text{Occupancy} = \frac{\text{Active Warps}}{\text{Max Warps per SM}}$$

Higher occupancy → better latency hiding (usually).

### Comparison: GPU vs CPU

| Aspect | CPU | GPU |
|--------|-----|-----|
| Cores | 4-64 | 1000-10000 |
| Clock | 3-5 GHz | 1-2 GHz |
| Cache | Large (MB) | Small (KB per core) |
| Memory BW | 50-100 GB/s | 500-2000 GB/s |
| Control | Complex | Simple |
| Best for | Latency | Throughput |

### GPU Generations (NVIDIA)

| Architecture | Year | Key Features |
|--------------|------|--------------|
| Tesla | 2006 | First CUDA GPU |
| Fermi | 2010 | L1/L2 cache, ECC |
| Kepler | 2012 | Dynamic parallelism |
| Maxwell | 2014 | Power efficiency |
| Pascal | 2016 | NVLink, HBM2 |
| Volta | 2017 | Tensor cores |
| Turing | 2018 | RT cores |
| Ampere | 2020 | Improved tensor cores |
| Hopper | 2022 | Transformer engine |

## 📌 Key Points (Optional)
- GPUs achieve throughput through massive parallelism and latency hiding.
- SIMT allows flexible threading with SIMD efficiency for coherent execution.

## 🖼️ Recommended Image
- Diagram showing GPU SM architecture with CUDA cores, shared memory, and warp execution.

## 🔗 Connections
- [[107-SIMD and Vector Architecture Fundamentals]]
- [[052-Flynns Taxonomy and Parallel Architectures]]
- [[112-CUDA Programming Model]]
- [[097-Thread Level Parallelism TLP Fundamentals]]
- [[109-Arithmetic Intensity and Roofline Model]]