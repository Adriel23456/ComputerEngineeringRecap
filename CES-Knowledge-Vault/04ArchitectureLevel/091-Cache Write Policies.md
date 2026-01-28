---
Theme: Cache Write Policies
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, MemoryHierarchy]
---

## 📚 Idea/Concept

Cache write policies determine how and when modified data in cache is synchronized with main memory, balancing performance, consistency, and complexity.

### Write Hit Policies

Write-Through:
- Write data to both cache and main memory simultaneously
- Memory always has current data
- Advantages: Simple, memory always consistent
- Disadvantages: High memory traffic, slower writes
- Often uses write buffer to hide memory latency

Write-Back:
- Write data only to cache, mark block as "dirty"
- Write to memory only when block is evicted
- Advantages: Less memory traffic, faster writes
- Disadvantages: Complex, memory has stale data
- Requires dirty bit per cache line

### Write Miss Policies

Write-Allocate (Fetch on Write):
- On write miss, load block into cache, then write
- Assumes spatial/temporal locality for writes
- Usually paired with write-back

No-Write-Allocate (Write Around):
- On write miss, write directly to memory, don't load to cache
- Avoids cache pollution from write-only data
- Usually paired with write-through

### Common Policy Combinations

| Hit Policy | Miss Policy | Use Case |
|------------|-------------|----------|
| Write-Through | No-Write-Allocate | Simple systems, I/O buffers |
| Write-Back | Write-Allocate | High-performance CPUs |

### Write Buffer

Purpose:
- Decouple CPU from slow memory writes
- Allow CPU to continue while write completes

Operation:
- Store writes in FIFO buffer
- Drain buffer to memory in background
- Stall CPU only if buffer full

Considerations:
- Read must check write buffer for recent writes
- RAW hazard: Read may need data still in buffer

### Detailed Policy Comparison

| Aspect | Write-Through | Write-Back |
|--------|---------------|------------|
| Memory Traffic | High (every write) | Low (only on eviction) |
| Consistency | Always consistent | May have stale data |
| Miss Penalty | Lower (no writeback) | Higher (may need writeback) |
| Hardware | Simpler | Complex (dirty bits, writeback logic) |
| Power | Higher (more bus activity) | Lower |
| Multi-core | Easier coherence | Complex coherence |

### Read-Allocate Policy

Most caches are read-allocate:
- On read miss, always load block to cache
- Standard behavior, exploits locality

Some systems may have no-read-allocate for streaming data.

### Implementation Example

Write-Back with Write-Allocate on write miss:
```
1. Check if block in cache (tag match?)
2. If hit and write:
   - Write to cache line
   - Set dirty bit = 1
3. If miss and write:
   - Select victim line
   - If victim dirty, write back to memory
   - Fetch block from memory
   - Write new data to cache
   - Set dirty bit = 1
```

### Modern Processor Policies

L1 Cache:
- Often write-through to L2 (simplifies coherence)
- Or write-back with fast L2 access

L2/L3 Cache:
- Typically write-back (reduces memory traffic)
- Write-allocate for locality exploitation

## 📌 Key Points (Optional)
- Write-back is preferred for performance; write-through simplifies coherence.
- The choice affects both performance and multiprocessor cache coherence complexity.

## 🖼️ Recommended Image
- Flowchart showing decision process for write-through vs write-back on hit and miss.

## 🔗 Connections
- [[087-Cache Memory Organization and Parameters]]
- [[103-Cache Coherence Problem and Solutions]]
- [[090-Average Memory Access Time AMAT]]
- [[086-Memory Hierarchy Principles and Locality]]