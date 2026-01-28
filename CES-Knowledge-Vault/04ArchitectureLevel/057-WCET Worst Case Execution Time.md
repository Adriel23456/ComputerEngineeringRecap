---
Theme: WCET Worst Case Execution Time
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, RealTime, Performance]
---

## 📚 Idea/Concept

WCET (Worst-Case Execution Time) represents the maximum time an algorithm or code segment takes to execute under the worst possible conditions, critical for real-time systems where deadlines must be guaranteed.

### Definition

For a program with N basic blocks Bi, each with maximum execution time ci and executed xi times:

$$WCET = \sum_{i=1}^{N} c_i \cdot x_i$$

### Basic Block Concept

A basic block is an uninterrupted sequence of instructions:
- No internal jumps or branches
- Single entry point, single exit point
- 100% sequential control flow within the block
- Facilitates static timing analysis

### Analysis Methods

Static Analysis:
- Analyzes code without execution
- Conservative upper bounds
- Considers all possible paths
- Requires knowledge of hardware timing

Dynamic Analysis:
- Measures execution with test inputs
- May miss worst-case scenarios
- Useful for validation

Hybrid Approaches:
- Combine static bounds with measured data
- Trade-off between precision and safety

### Challenges for WCET Analysis

Modern processors complicate WCET:
- Cache behavior (hits vs misses)
- Branch prediction (correct vs mispredicted)
- Pipeline effects (stalls, hazards)
- Out-of-order execution
- Memory access patterns

### Real-Time System Classes

| Class | WCET Requirement | Example |
|-------|-----------------|---------|
| Hard Real-Time | Must never exceed | Airbag deployment |
| Firm Real-Time | Rare exceeds acceptable | Video streaming |
| Soft Real-Time | Statistical bounds | User interfaces |

## 📌 Key Points (Optional)
- WCET analysis rejects dynamic branching in favor of static analyzability.
- Modern speculative execution makes tight WCET bounds increasingly difficult.

## 🖼️ Recommended Image
- Control flow graph showing basic blocks with timing annotations.

## 🔗 Connections
- [[043-Performance Metrics CPI IPC and Execution Time]]
- [[065-Pipeline MIPS Five Stage Design]]
- [[085-Speculative Execution and Branch Prediction]]
- [[056-Reliability and Maintainability Metrics]]