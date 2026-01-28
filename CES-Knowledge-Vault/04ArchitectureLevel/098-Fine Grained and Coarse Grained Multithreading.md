---
Theme: Fine Grained and Coarse Grained Multithreading
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, TLP, Multithreading]
---

## 📚 Idea/Concept

Fine-grained and coarse-grained multithreading are two approaches to exploiting thread-level parallelism within a single processor core, differing in how frequently and why they switch between threads.

### Fine-Grained Multithreading (Interleaved)

Definition:
- Processor switches between threads every cycle (or every few cycles)
- Round-robin or priority-based thread selection
- Multiple threads interleaved in pipeline simultaneously

Operation:
```
Cycle:  1   2   3   4   5   6   7   8
Thread: A   B   C   D   A   B   C   D
```

Characteristics:
- Pipeline contains instructions from multiple threads
- Each thread progresses slowly but steadily
- Excellent latency hiding
- No single thread gets full pipeline bandwidth

Advantages:
- Hides all single-cycle stalls automatically
- High throughput for multi-threaded workloads
- Simple hardware (no complex stall detection)
- Good for memory-bound applications

Disadvantages:
- Individual thread performance degraded
- Fast threads slowed by sharing cycles
- Requires multiple thread contexts
- Not ideal for latency-sensitive single threads

Examples:
- SPARC T1-T5 (Oracle): 4-8 threads per core
- Tera MTA: Extreme fine-grained (128 threads)
- GPUs (NVIDIA): Warp-level interleaving

### Coarse-Grained Multithreading (Blocked)

Definition:
- Processor executes one thread until a costly event occurs
- Switches thread only on long-latency stalls
- Single thread dominates pipeline at any time

Switch Triggers:
- L2/L3 cache miss
- TLB miss
- Long-latency ALU operations
- I/O operations
- Quantum expiration
- Thread priority change

Operation:
```
Cycle:    1   2   3   4   5   6   7   8   9  10  11  12
Thread:   A   A   A   A  [miss]  B   B   B   B   B   B
                         ↑
                    Switch on L2 miss
```

Characteristics:
- Pipeline contains only one thread's instructions
- Full bandwidth to executing thread
- Switch cost: pipeline flush/refill

Advantages:
- Single thread gets maximum performance
- Lower hardware complexity than SMT
- Good for workloads with occasional long stalls

Disadvantages:
- Pipeline flush on switch (wasted cycles)
- Short stalls not hidden
- Less effective latency hiding than fine-grained

Examples:
- Intel Montecito (Itanium 2): 2 threads per core
- IBM RS64 IV

### Switch Cost Comparison

| Approach | Switch Trigger | Switch Cost | Latency Hiding |
|----------|---------------|-------------|----------------|
| Fine-Grained | Every cycle | Zero | Excellent |
| Coarse-Grained | Long stall | Pipeline depth | Moderate |

### Pipeline State Comparison

Fine-Grained (4 threads, 5-stage pipeline):
```
Stage:    IF    ID    EX   MEM   WB
Thread:    D     C     B    A     D
```

Coarse-Grained (same pipeline):
```
Stage:    IF    ID    EX   MEM   WB
Thread:    A     A     A    A     A
```

### Hardware Requirements

Fine-Grained:
- Multiple PC registers (one per thread)
- Partitioned or replicated register files
- Thread ID tracking through pipeline
- Fast thread selection logic

Coarse-Grained:
- Multiple thread contexts (PC, registers)
- Stall detection logic
- Context switch mechanism
- Optional prefetch buffer for next thread

### Thread Selection Policies

Fine-Grained:
- Round-robin: Fair, simple
- Priority-based: Favors important threads
- Skip stalled: Only select ready threads

Coarse-Grained:
- Urgency-based: Dynamic priority from system events
- Timeout-based: Switch after N cycles
- Event-driven: Switch on specific stall types

### Example: UltraSPARC T1 (Fine-Grained)

Configuration:
- 8 cores, 4 threads per core (32 total)
- Switch threads every cycle
- Small caches (I$: 16KB, D$: 8KB)
- Cache misses tolerated by thread switching

Design philosophy:
- Don't wait for memory—switch threads
- Small caches acceptable because latency hidden
- Optimized for throughput, not single-thread speed

## 📌 Key Points (Optional)
- Fine-grained trades individual thread speed for throughput.
- Coarse-grained preserves single-thread performance but hides only long latencies.

## 🖼️ Recommended Image
- Timeline diagram comparing thread execution patterns in both approaches.

## 🔗 Connections
- [[097-Thread Level Parallelism TLP Fundamentals]]
- [[099-Simultaneous Multithreading SMT]]
- [[065-Pipeline MIPS Five Stage Design]]
- [[086-Memory Hierarchy Principles and Locality]]