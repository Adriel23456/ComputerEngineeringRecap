---
Theme: Cache Memory Organization and Parameters
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, MemoryHierarchy]
---

## 📚 Idea/Concept

Cache memory is a small, fast SRAM buffer that stores copies of frequently accessed data from main memory, managed automatically by hardware to reduce average memory access time.

### Fundamental Cache Parameters

Cache Size (T_cache):
- Total storage capacity of the cache
- Larger cache → lower miss rate, but higher access time and cost

Block Size (T_block):
- Unit of data transfer between cache and memory
- Also called "cache line"
- Typical sizes: 32, 64, or 128 bytes

Number of Blocks:
$$\text{Number of blocks} = \frac{T_{cache}}{T_{block}}$$

Associativity (n-way):
- Number of locations where a block can be placed
- Affects conflict miss rate and hardware complexity

### Cache Access Terminology

Cache Hit:
- Requested data found in cache
- Access completed at cache speed

Cache Miss:
- Requested data not in cache
- Must fetch from lower level (slower)

Miss Penalty:
- Additional time required to fetch data on a miss
- Includes memory access latency and data transfer time

### Address Decomposition

For an address accessing cache:
```
[    Tag    |   Index   |  Offset  ]
     ↓           ↓          ↓
  Identity    Set/Line    Byte
  check       selection   within block
```

- Offset bits: log₂(block_size)
- Index bits: log₂(number_of_sets)
- Tag bits: remaining address bits

### Cache Organization Types

| Type | Index Bits | Ways | Flexibility | Hardware Cost |
|------|------------|------|-------------|---------------|
| Direct-Mapped | log₂(blocks) | 1 | Lowest | Lowest |
| N-Way Set Associative | log₂(blocks/N) | N | Medium | Medium |
| Fully Associative | 0 | All blocks | Highest | Highest |

### Replacement Policies

When cache is full and new block needed:
- LRU (Least Recently Used): Replace oldest accessed block
- FIFO (First In, First Out): Replace oldest loaded block
- Random: Randomly select victim block
- Pseudo-LRU: Approximation of LRU with less hardware

### Valid and Dirty Bits

Valid Bit (V):
- Indicates if cache line contains valid data
- Initially 0, set to 1 on first load

Dirty Bit (D):
- Indicates if cached data has been modified
- Used for write-back policy
- If dirty, must write back before eviction

## 📌 Key Points (Optional)
- Cache design involves trade-offs between hit time, miss rate, and miss penalty.
- The three Cs of cache misses: Compulsory, Capacity, Conflict.

## 🖼️ Recommended Image
- Diagram showing cache structure with tag array, data array, and address decomposition.

## 🔗 Connections
- [[086-Memory Hierarchy Principles and Locality]]
- [[088-Cache Mapping Direct Associative and Set Associative]]
- [[090-Average Memory Access Time AMAT]]
- [[089-Cache Miss Types Three Cs]]
- [[091-Cache Write Policies]]