---
Theme: Average Memory Access Time AMAT
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, Performance]
---

## 📚 Idea/Concept

Average Memory Access Time (AMAT) is the key metric for evaluating memory system performance, combining hit time, miss rate, and miss penalty into a single measure.

### AMAT Formula

Basic Formula:
$$AMAT = Hit\ Time + Miss\ Rate \times Miss\ Penalty$$

Alternative Form:
$$AMAT = Hit\ Rate \times Hit\ Time + Miss\ Rate \times Miss\ Time$$

Where:
$$Miss\ Time = Hit\ Time + Miss\ Penalty$$

Relationship:
$$Hit\ Rate + Miss\ Rate = 1$$

### Component Definitions

Hit Time:
- Time to access cache and determine hit/miss
- Includes tag comparison and data retrieval
- Typically 1-4 cycles for L1

Miss Rate:
- Fraction of accesses that miss in cache
- Miss Rate = Misses / Total Accesses

Miss Penalty:
- Additional cycles needed when miss occurs
- Time to fetch block from next level

### Single-Level Cache Example

Given:
- Hit time = 1 cycle
- Miss penalty = 50 cycles
- Miss rate = 5.5% (55 misses per 100 accesses)

Calculation:
$$AMAT = 1 + 0.055 \times 50 = 1 + 2.75 = 3.75\ cycles$$

### Multi-Level Cache AMAT

For L1 and L2 cache hierarchy:
$$AMAT = Hit\ Time_{L1} + Miss\ Rate_{L1} \times (Hit\ Time_{L2} + Miss\ Rate_{L2} \times Miss\ Penalty_{L2})$$

Example:
- L1: Hit time = 4 cycles, Miss rate = 40%
- L2: Hit time = 10 cycles, Miss rate = 15%
- Memory: Access time = 100 cycles

Calculation:
$$AMAT = 4 + 0.40 \times (10 + 0.15 \times 100)$$
$$AMAT = 4 + 0.40 \times (10 + 15)$$
$$AMAT = 4 + 0.40 \times 25 = 4 + 10 = 14\ cycles$$

### Local vs Global Miss Rate

Local Miss Rate:
- Misses in this cache / Accesses to this cache
- What the cache "sees"

Global Miss Rate:
- Misses in this cache / Total CPU memory accesses
- Impact on overall performance

For L2:
$$Global\ Miss\ Rate_{L2} = Miss\ Rate_{L1} \times Local\ Miss\ Rate_{L2}$$

### Improving AMAT

Three approaches:
1. Reduce Hit Time: Smaller cache, simpler organization, pipelining
2. Reduce Miss Rate: Larger cache, higher associativity, better replacement
3. Reduce Miss Penalty: Faster memory, additional cache levels, prefetching

### CPI Impact

Memory contribution to CPI:
$$CPI_{memory} = \frac{Memory\ Accesses}{Instruction} \times Miss\ Rate \times Miss\ Penalty$$

Total CPI:
$$CPI_{total} = CPI_{base} + CPI_{memory}$$

## 📌 Key Points (Optional)
- AMAT is dominated by miss penalty when miss rate is significant.
- Multi-level caches dramatically reduce effective miss penalty.

## 🖼️ Recommended Image
- Flowchart showing AMAT calculation for multi-level cache hierarchy.

## 🔗 Connections
- [[087-Cache Memory Organization and Parameters]]
- [[089-Cache Miss Types Three Cs]]
- [[043-Performance Metrics CPI IPC and Execution Time]]
- [[086-Memory Hierarchy Principles and Locality]]