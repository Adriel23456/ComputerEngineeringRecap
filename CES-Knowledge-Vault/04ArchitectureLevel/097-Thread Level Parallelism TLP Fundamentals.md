---
Theme: Thread Level Parallelism TLP Fundamentals
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, TLP, Parallelism]
---

## 📚 Idea/Concept

Thread-Level Parallelism (TLP) exploits parallelism by executing multiple threads concurrently, providing a complementary approach to ILP for improving processor throughput, especially when single-thread performance is limited.

### Why TLP Matters

ILP limitations:
- Data dependencies limit single-thread parallelism
- Branch mispredictions cause stalls
- Memory latency difficult to hide completely
- Diminishing returns from wider issue

TLP advantages:
- Multiple independent instruction streams
- Natural latency hiding (one thread waits, another executes)
- Better utilization of hardware resources
- Scales with thread count

### Process vs Thread

Process:
- Independent program instance
- Own address space, resources
- Heavy context switch cost
- Strong isolation

Thread:
- Unit of execution within a process
- Shared code, data, files with other threads
- Own PC, registers, stack
- Lighter context switch
- Cooperative or competitive resource use

Context Switch:
- Process switch: Save/restore full state (expensive)
- Thread switch: Save/restore registers, PC, stack pointer (cheaper)

### Multithreading Approaches

| Approach | Granularity | When Switch | Pipeline State |
|----------|-------------|-------------|----------------|
| Fine-Grained | Every cycle | Regularly | Mixed threads |
| Coarse-Grained | On stall | Cache miss, etc. | Single thread until switch |
| SMT | Same cycle | Never (simultaneous) | Multiple threads |

### Hardware Requirements for Multithreading

Per-thread state:
- Program Counter (PC)
- Register file (or partitioned)
- Stack pointer
- Instruction buffer/queue

Shared resources:
- Functional units
- Caches
- Memory interface
- Branch predictor (may be partitioned)

### Benefits of TLP

1. Latency Tolerance:
   - Memory stall in one thread → execute another
   - Overlap computation with memory access

2. Resource Utilization:
   - Fill functional unit bubbles
   - Reduce idle cycles

3. Throughput Scaling:
   - Linear scaling with parallelizable workloads
   - Amdahl's Law applies

### Thread Sources

Hardware threads:
- Simultaneous Multithreading (SMT)
- Appears as multiple logical CPUs

Software threads:
- OS kernel threads
- User-level threads
- Fiber/coroutine libraries

Thread creation:
- Fork-join models
- Thread pools
- Task-based parallelism

### Terminology

Physical Core: Hardware execution unit
Logical Core: Thread context visible to OS
Hardware Thread: Same as logical core (SMT context)
RISC-V Hart: Hardware Thread in RISC-V terminology

### Modern Implementations

| Processor | Cores | Threads/Core | Total Threads |
|-----------|-------|--------------|---------------|
| Intel Core i9 | 8-24 | 2 (SMT) | 16-32 |
| AMD Ryzen 9 | 12-16 | 2 (SMT) | 24-32 |
| Apple M3 Max | 12-16 | 1 | 12-16 |
| SPARC T5 | 16 | 8 (FG-MT) | 128 |

### TLP vs ILP Trade-offs

| Aspect | ILP | TLP |
|--------|-----|-----|
| Visibility | Transparent | Explicit programming |
| Scaling | Limited by dependencies | Limited by parallelism |
| Hardware | Complex OoO logic | Thread contexts |
| Power | High (speculation) | Variable |
| Latency | Single-thread focused | Throughput focused |

## 📌 Key Points (Optional)
- TLP provides throughput scaling when ILP is exhausted.
- Effective TLP requires parallel software and appropriate hardware support.

## 🖼️ Recommended Image
- Diagram comparing ILP (single thread) vs TLP (multiple threads) resource utilization.

## 🔗 Connections
- [[053-Parallelism Levels BLP ILP DLP TLP]]
- [[098-Fine Grained and Coarse Grained Multithreading]]
- [[099-Simultaneous Multithreading SMT]]
- [[101-Multiprocessor Types SMP AMP and DSM]]
- [[064-Instruction Level Parallelism ILP Fundamentals]]