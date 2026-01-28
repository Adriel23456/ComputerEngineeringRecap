---
Theme: Cache Coherence Problem and Solutions
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, Multiprocessor]
---

## 📚 Idea/Concept

The cache coherence problem arises in multiprocessor systems where multiple caches may hold copies of the same memory location, and writes by one processor must be visible to all others to maintain a consistent view of memory.

### The Problem

Scenario:
```
Initial: Memory[X] = 0, both caches empty

Time 1: CPU0 reads X → Cache0[X] = 0
Time 2: CPU1 reads X → Cache1[X] = 0
Time 3: CPU0 writes X = 1 → Cache0[X] = 1
Time 4: CPU1 reads X → Cache1[X] = 0 (STALE!)
```

Without coherence, CPU1 sees outdated value.

### Coherence Definition

A memory system is coherent if:
1. Program Order: A read by processor P to location X that follows a write by P to X always returns the written value (if no intervening writes by others)
2. Coherent View: A read by processor P to X that follows a write by processor Q to X returns the written value (if sufficient time elapsed)
3. Write Serialization: Writes to same location are seen in same order by all processors

### Coherence vs Consistency

Coherence:
- Concerns single memory location
- What values can be returned by reads
- When writes become visible

Consistency:
- Concerns ordering across multiple locations
- Sequential consistency, relaxed models
- Memory ordering guarantees

### Solution Approaches

Two main approaches:

1. Snooping Protocols:
   - All caches monitor (snoop) shared bus
   - Broadcast-based communication
   - Good for small-scale SMP (bus-based)

2. Directory Protocols:
   - Centralized or distributed directory tracks sharing
   - Point-to-point communication
   - Scalable to large systems

### Snooping Protocol Basics

Each cache controller:
- Monitors all bus transactions
- Responds to relevant addresses
- Takes action based on protocol state

Bus transactions snooped:
- BusRd: Read request (shared)
- BusRdX: Read with intent to modify (exclusive)
- BusWB: Write-back dirty data

Cache actions:
- Invalidate local copy
- Supply data (intervention)
- Update state

### Directory Protocol Basics

Directory entry per memory block:
- Presence bits: Which caches have copy
- State: Shared, exclusive, uncached

Operations:
- Read miss: Check directory, get data, update sharing
- Write miss: Check directory, invalidate sharers, get exclusive

Advantages:
- No broadcast needed
- Scales to many processors
- Point-to-point messages

Disadvantages:
- Directory storage overhead
- Indirection latency
- More complex protocol

### Write Policies for Coherence

Write-Invalidate:
- On write, invalidate all other copies
- Most common approach
- Subsequent accesses miss and fetch new value

Write-Update (Write-Broadcast):
- On write, update all other copies
- Higher bus traffic
- Good for producer-consumer patterns

### Coherence Protocol States

Basic states (MSI):
- Modified (M): Exclusive, dirty, must write back
- Shared (S): Clean copy, others may have copies
- Invalid (I): Not valid, must fetch on access

Extended states (MESI, MOESI):
- Exclusive (E): Only copy, clean (no write-back needed if evicted)
- Owned (O): Dirty but shared (responsible for write-back)

### Performance Implications

Coherence misses (4th C):
- Invalidation by other processor
- True sharing: Necessary communication
- False sharing: Unrelated data in same cache line

False Sharing Problem:
```c
struct {
    int count_cpu0;  // Both in same cache line!
    int count_cpu1;
} shared;
```
CPUs bounce cache line despite accessing different variables.

Solution: Pad data to cache line boundaries.

### Coherence Traffic

Types of coherence messages:
- Invalidations (write-invalidate)
- Data responses (cache-to-cache transfer)
- Acknowledgments (completion signals)

Optimization techniques:
- Intervention: Supply data from cache instead of memory
- Migratory sharing detection
- Speculative snooping

## 📌 Key Points (Optional)
- Coherence ensures all processors see consistent memory state.
- Snooping works for small systems; directories scale better.

## 🖼️ Recommended Image
- Diagram showing coherence problem scenario and protocol message flow.

## 🔗 Connections
- [[091-Cache Write Policies]]
- [[106-MSI MESI and MOESI Protocols]]
- [[104-Snooping Cache Coherence Protocols]]
- [[105-Directory Based Cache Coherence]]
- [[101-Multiprocessor Types SMP AMP and DSM]]