---
Theme: Amdahls Law and Speedup Limits
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Performance, Parallelism]
---

## 📚 Idea/Concept

Amdahl's Law provides a framework for estimating the maximum performance improvement when only a portion of a computational task can be parallelized or enhanced.

### The Law

For a program with:
- B: fraction that is non-parallelizable (sequential)
- (1-B): fraction that is parallelizable

Execution time with N processing units:

T(N) = B + (1-B)/N

Global Speedup:

S = T(1)/T(N) = 1 / (B + (1-B)/N)

### Maximum Theoretical Speedup

As N → ∞:

S_max = 1/B

This means even infinite parallelism cannot overcome the sequential portion.

### Example Calculation

For B = 0.6 (60% non-parallelizable) with N = 10 processors:

S = 1 / (0.6 + 0.4/10) = 1 / (0.6 + 0.04) = 1 / 0.64 ≈ 1.56

Only 56% improvement despite using 10 processors!

### Practical Exercise

Problem: Achieve 80× speedup with 100 processors. What fraction can be sequential?

80 = 1 / (B + (1-B)/100)
80 = 1 / (B + 0.01 - 0.01B)
80 = 1 / (0.01 + 0.99B)

Solving: B = 0.0025 = 0.25%

Only 0.25% of the program can be sequential to achieve 80× speedup with 100 processors.

### Implications

1. Serial portions are critical: Even small sequential fractions severely limit speedup
2. Diminishing returns: Adding more processors yields progressively smaller gains
3. Optimization priority: Focus on parallelizing the sequential bottleneck first

### Asymmetric Speedup

Modern heterogeneous processors (big.LITTLE) balance:
- Big cores: Execute sequential code faster
- Small cores: Handle parallel workloads efficiently

For each parallelizable fraction F, there's an optimal number of big cores R that maximizes speedup.

## 📌 Key Points (Optional)
- Amdahl's Law sets hard limits on parallel speedup.
- Gustafson's Law offers a more optimistic view for scaled problem sizes.

## 🖼️ Recommended Image
- Graph showing speedup curves for different parallelizable fractions vs processor count.

## 🔗 Connections
- [[043-Performance Metrics CPI IPC and Execution Time]]
- [[053-Parallelism Levels BLP ILP DLP TLP]]
- [[097-Thread Level Parallelism TLP Fundamentals]]
- [[101-Multiprocessor Types SMP AMP and DSM]]