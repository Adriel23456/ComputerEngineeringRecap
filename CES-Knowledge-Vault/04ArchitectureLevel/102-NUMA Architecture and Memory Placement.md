---
Theme: NUMA Architecture and Memory Placement
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Multiprocessor, Memory]
---

## 📚 Idea/Concept

NUMA (Non-Uniform Memory Access) is a multiprocessor memory architecture where memory access time depends on the memory location relative to the processor, requiring careful data placement for optimal performance.

### NUMA vs UMA

UMA (Uniform Memory Access):
- All processors access all memory with same latency
- Symmetric Multiprocessing (SMP)
- Limited scalability (bus bottleneck)

NUMA:
- Memory physically distributed across nodes
- Local memory: Fast access
- Remote memory: Slower access (traverses interconnect)
- Scales to many processors

### NUMA Architecture
```
┌─────────────────┐         ┌─────────────────┐
│     Node 0      │         │     Node 1      │
│ ┌─────┐ ┌─────┐ │         │ ┌─────┐ ┌─────┐ │
│ │CPU0 │ │CPU1 │ │         │ │CPU2 │ │CPU3 │ │
│ └──┬──┘ └──┬──┘ │         │ └──┬──┘ └──┬──┘ │
│    └───┬───┘    │         │    └───┬───┘    │
│   ┌────┴────┐   │         │   ┌────┴────┐   │
│   │ Memory  │   │◀───────▶│   │ Memory  │   │
│   │ (Local) │   │Interconnect │ (Local) │   │
│   └─────────┘   │         │   └─────────┘   │
└─────────────────┘         └─────────────────┘
     ~100 ns                     ~100 ns
         └──────── ~150-200 ns ────────┘
              (remote access)
```

### NUMA Ratio

$$NUMA\ Ratio = \frac{Remote\ Access\ Latency}{Local\ Access\ Latency}$$

Typical values: 1.5x to 3x

Example:
- Local: 100 ns
- Remote: 200 ns
- NUMA ratio: 2.0

### Memory Placement Policies

First-Touch:
- Page allocated on node where first accessed
- Default policy in most OS
- Works well if accessor continues using data

Interleave:
- Pages distributed round-robin across nodes
- Averages out latency
- Good for shared data structures

Local Allocation:
- Always allocate on requesting CPU's node
- Best for private data

Bind:
- Explicitly bind memory to specific node
- Programmer control

### NUMA-Aware Programming

Linux tools:
```bash
# Show NUMA topology
numactl --hardware

# Run process on specific node
numactl --cpunodebind=0 --membind=0 ./program

# Interleave memory across nodes
numactl --interleave=all ./program
```

### Data Placement Strategy

Optimal placement:
- Keep data close to computing threads
- Minimize remote accesses
- Co-locate related data

Example - Matrix multiplication:
```
Node 0: Rows 0-N/2 of A, Columns 0-N/2 of B
Node 1: Rows N/2-N of A, Columns N/2-N of B

Each node computes its portion with local data.
```

### NUMA Effects on Performance

Poor placement consequences:
- Increased latency (1.5-3x per access)
- Interconnect congestion
- Reduced bandwidth

Performance impact:
- Memory-bound apps: 20-50% slowdown
- Compute-bound apps: Minimal impact

### Modern NUMA Systems

AMD EPYC:
- Multiple CCDs (Core Complex Dies) per socket
- NUMA even within single socket
- Memory channels distributed across CCDs

Intel Xeon:
- Sub-NUMA clustering (SNC)
- Can expose NUMA domains within socket
- UPI interconnect between sockets

### Detection and Optimization

Detect NUMA issues:
- Performance counters (remote vs local accesses)
- Linux `perf` tool
- Memory bandwidth monitoring

Optimize:
- Thread pinning (bind threads to cores)
- Memory binding (bind allocations to nodes)
- Data structure alignment to NUMA boundaries

### OS NUMA Support

Linux:
- NUMA-aware scheduler
- Memory migration (move pages to local node)
- AutoNUMA (automatic page migration)

Page migration trade-off:
- Cost: Copy page + TLB shootdown
- Benefit: Future local accesses

## 📌 Key Points (Optional)
- NUMA enables scaling but requires locality-aware programming.
- First-touch policy works well when threads stay with their data.

## 🖼️ Recommended Image
- Diagram showing NUMA nodes with local/remote access latency differences.

## 🔗 Connections
- [[101-Multiprocessor Types SMP AMP and DSM]]
- [[086-Memory Hierarchy Principles and Locality]]
- [[103-Cache Coherence Problem and Solutions]]
- [[097-Thread Level Parallelism TLP Fundamentals]]