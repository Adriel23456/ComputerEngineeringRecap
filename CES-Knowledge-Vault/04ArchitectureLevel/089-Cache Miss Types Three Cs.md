---
Theme: Cache Miss Types Three Cs
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, Performance]
---

## 📚 Idea/Concept

Cache misses are categorized into three types known as the "Three Cs": Compulsory, Capacity, and Conflict. Understanding these categories guides optimization strategies.

### Compulsory Misses (Cold Misses)

Definition:
- First access to a block that has never been in cache
- Unavoidable on first reference to any data

Characteristics:
- Would occur even with infinite cache
- Independent of cache size or organization
- Proportional to number of unique blocks accessed

Mitigation:
- Prefetching: Load blocks before they're needed
- Larger block sizes: Bring more data per miss (but increases miss penalty)

### Capacity Misses

Definition:
- Misses that occur because cache cannot contain all blocks needed
- Working set exceeds cache size

Characteristics:
- Would occur even with fully associative cache
- Decrease as cache size increases
- Independent of associativity

Mitigation:
- Larger cache
- Better algorithms that improve locality
- Working set optimization

### Conflict Misses (Collision Misses)

Definition:
- Misses that occur because multiple blocks compete for the same cache location
- Block evicted before reuse due to mapping constraints

Characteristics:
- Would not occur with fully associative cache
- Decrease with higher associativity
- Also called "interference misses"

Two Sub-types:
1. Mapping Conflict: Blocks map to same set despite available space elsewhere
2. Replacement Conflict: Imperfect replacement policy evicts useful blocks

Mitigation:
- Higher associativity
- Victim cache
- Better replacement policies

### Fourth C: Coherence Misses

In multiprocessor systems:
- Misses due to cache invalidation by other processors
- Data invalidated by writes from other cores
- Also called "communication misses"

### Miss Classification Method

To classify a miss:
1. Simulate fully associative cache of same size
2. If miss occurs → Compulsory or Capacity
3. If hit in fully associative but miss in actual cache → Conflict

Further distinction:
- Simulate infinite fully associative cache
- Miss there → Compulsory
- Hit there but miss in sized fully associative → Capacity

### Impact by Cache Configuration

| Cache Change | Compulsory | Capacity | Conflict |
|--------------|------------|----------|----------|
| Increase Size | - | ↓ | ↓ |
| Increase Associativity | - | - | ↓ |
| Increase Block Size | ↓ | May ↑ | May ↑ |
| Add Prefetching | ↓ | - | - |
| Add Victim Cache | - | - | ↓ |

### Typical Distribution

For typical workloads:
- Compulsory: ~2-5% of misses
- Capacity: ~20-40% of misses
- Conflict: ~40-60% of misses (in direct-mapped)

With 8-way associativity, conflict misses drop significantly.

## 📌 Key Points (Optional)
- The Three Cs model helps identify which optimizations will be effective.
- Most cache design trade-offs involve balancing these miss types.

## 🖼️ Recommended Image
- Venn diagram or bar chart showing relative contribution of each miss type.

## 🔗 Connections
- [[087-Cache Memory Organization and Parameters]]
- [[088-Cache Mapping Direct Associative and Set Associative]]
- [[090-Average Memory Access Time AMAT]]
- [[092-Victim Cache and Miss Reduction]]