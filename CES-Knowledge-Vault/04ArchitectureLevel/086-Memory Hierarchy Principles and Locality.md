---
Theme: Memory Hierarchy Principles and Locality
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, MemoryHierarchy, Fundamentals]
---

## 📚 Idea/Concept

The memory hierarchy is a structured organization of storage systems that exploits the principle of locality to bridge the performance gap between fast processors and slow main memory.

### The Memory Wall Problem

The performance gap between CPU and memory has grown exponentially:
- CPU speed increases ~60% per year (historically)
- DRAM latency improves only ~7% per year
- Result: Memory access can cost hundreds of CPU cycles

### Locality Principles

Temporal Locality:
- Recently accessed data is likely to be accessed again soon
- Examples: Loop variables, stack operations, frequently called functions
- Exploitation: Keep recently used data in fast, small caches

Spatial Locality:
- Data near recently accessed addresses is likely to be accessed soon
- Examples: Array traversals, sequential instruction fetch
- Exploitation: Fetch entire blocks/lines, not single bytes

### Memory Hierarchy Levels

| Level | Technology | Typical Size | Access Time | Cost/GB |
|-------|------------|--------------|-------------|---------|
| Registers | SRAM (on-chip) | ~1 KB | < 1 ns | Highest |
| L1 Cache | SRAM | 32-64 KB | 1-2 ns | Very High |
| L2 Cache | SRAM | 256 KB-1 MB | 3-10 ns | High |
| L3 Cache | SRAM | 4-64 MB | 10-20 ns | Medium |
| Main Memory | DRAM | 8-128 GB | 50-100 ns | Low |
| SSD/Flash | Flash | 256 GB-4 TB | 25-100 μs | Very Low |
| HDD | Magnetic | 1-20 TB | 5-10 ms | Lowest |

### Inclusion Property

Strict Inclusion:
- Data in L1 is always also in L2, L2 in L3, etc.
- Simplifies coherence but wastes capacity

Exclusion:
- Data exists in only one level
- Better capacity utilization, more complex management

### Working Set Concept

The working set is the set of memory locations actively used by a program during a time interval:
- If working set fits in cache → high hit rate
- If working set exceeds cache → thrashing (frequent replacements)

## 📌 Key Points (Optional)
- The hierarchy trades off speed, size, and cost at each level.
- Effective caching relies on programs exhibiting good locality behavior.

## 🖼️ Recommended Image
- Pyramid diagram showing memory hierarchy levels with speed/size trade-offs.

## 🔗 Connections
- [[087-Cache Memory Organization and Parameters]]
- [[049-Von Neumann Architecture]]
- [[050-Harvard Architecture and Modified Harvard]]
- [[090-Average Memory Access Time AMAT]]
- [[062-Memory Alignment and Data Structure Optimization]]
