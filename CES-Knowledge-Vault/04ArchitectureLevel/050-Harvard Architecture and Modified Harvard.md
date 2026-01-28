---
Theme: Harvard Architecture and Modified Harvard
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Microarchitecture, Fundamentals]
---

## 📚 Idea/Concept

Harvard architecture uses physically separate memories and buses for instructions and data, eliminating the Von Neumann bottleneck but at increased hardware cost.

### Pure Harvard Architecture

Origin: Designed by Howard Aiken (1937), built at Harvard (1944)

Key characteristic: Separate memories and buses for instructions and data

Advantages:
- Simultaneous access to instructions and data
- Higher throughput by eliminating the Von Neumann bottleneck
- Ideal for systems requiring high memory parallelism (e.g., DSPs, military systems)

Disadvantages:
- Greater physical space consumption
- Increased power consumption
- Higher complexity due to duplicated hardware (two memories and buses)

### Modified Harvard Architecture

Motivation: Reduce cost and complexity of pure Harvard while retaining benefits

Key characteristics:
- Single unified address space for instructions and data
- Separate caches (I-Cache and D-Cache) at CPU level
- Unified main memory (logically Von Neumann)
- Allows accessing instructions as data when necessary

### Architecture Comparison

| Feature | Von Neumann | Harvard | Modified Harvard |
|---------|-------------|---------|------------------|
| Address Space | Single | Double (I and D) | Single |
| Main Memory | Unified | Two physical memories | Unified |
| CPU Access Paths | Single | Independent I/D routes | Separate cache routes |
| Memory Parallelism | Limited (bottleneck) | Concurrent I/O and data | Concurrent I/D cache |
| Cost & Complexity | Low | High (duplicated HW) | Intermediate |

## 📌 Key Points (Optional)
- Most modern processors use Modified Harvard: unified main memory with split L1 caches.
- Pure Harvard is still used in specialized DSPs and embedded systems.

## 🖼️ Recommended Image
- Side-by-side diagrams of Von Neumann, Harvard, and Modified Harvard showing memory organization.

## 🔗 Connections
- [[049-Von Neumann Architecture]]
- [[087-Cache Memory Organization and Parameters]]
- [[086-Memory Hierarchy Principles and Locality]]
- [[065-Pipeline MIPS Five Stage Design]]