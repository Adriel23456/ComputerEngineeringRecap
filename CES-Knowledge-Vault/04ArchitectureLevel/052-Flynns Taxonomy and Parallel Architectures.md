---
Theme: Flynns Taxonomy and Parallel Architectures
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Parallelism, Classification]
---

## 📚 Idea/Concept

Flynn's Taxonomy classifies computer architectures based on the number of instruction streams and data streams they can process simultaneously.

### The Four Categories

|  | Single Data | Multiple Data |
|--|-------------|---------------|
| Single Instruction | SISD | SIMD |
| Multiple Instruction | MISD | MIMD |

### SISD (Single Instruction, Single Data)

Traditional single-processor architecture:
- Implements pipelining to overlap execution phases
- Exploits ILP through superscalar or VLIW designs
- Does not execute multiple instructions concurrently, but overlaps stages
- Classic Von Neumann machine

### SIMD (Single Instruction, Multiple Data)

One instruction operates on multiple data elements simultaneously:
- Single instruction stream controls multiple processing units
- Two main variants:
  - Array Processors: Process all elements in parallel (massive parallelism)
  - Vector Processors: Process elements in rapid sequence under same instruction

Applications: Graphics, scientific computing, multimedia

### MISD (Multiple Instruction, Single Data)

Multiple instruction streams operate on the same data stream:
- Rare in practice
- Specific applications in:
  - Systems with execution redundancy for error detection
  - Fault-tolerant computing

### MIMD (Multiple Instruction, Multiple Data)

Each processor executes its own instruction stream on its own data:
- Common implementations:
  - Multi-core and multiprocessor systems
  - Clusters and distributed systems
- Main challenges:
  - Memory consistency requires hardware/software solutions
  - Synchronization overhead

### Classification Examples

| Function | Classification |
|----------|---------------|
| [f(x), g(y), h(z)] = [(x+1)/2, sin(y)/y, e^z] | MIMD |
| [h(x,y)] = [e^(x+y)] | SIMD |
| [f(x)] = a0 + x(a1 + x(a2 + a3x)) | MISD |
| [g(x,y,z)] = x/(a0 + y/(a1 + z/a2)) | MIMD |

## 📌 Key Points (Optional)
- Modern CPUs combine multiple paradigms: SIMD extensions (SSE, AVX) within MIMD multi-core.
- Flynn's taxonomy provides the foundational vocabulary for parallel architecture discussion.

## 🖼️ Recommended Image
- 2x2 matrix diagram with visual representation of each architecture type.

## 🔗 Connections
- [[053-Parallelism Levels BLP ILP DLP TLP]]
- [[107-SIMD and Vector Architecture Fundamentals]]
- [[097-Thread Level Parallelism TLP Fundamentals]]
- [[101-Multiprocessor Types SMP AMP and DSM]]