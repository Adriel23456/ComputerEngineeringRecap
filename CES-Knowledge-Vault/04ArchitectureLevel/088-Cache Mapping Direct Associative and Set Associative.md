---
Theme: Cache Mapping Direct Associative and Set Associative
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, MemoryHierarchy]
---

## 📚 Idea/Concept

Cache mapping determines how memory blocks are assigned to cache locations, balancing flexibility against hardware complexity and access time.

### Direct-Mapped Cache

Mapping Rule:
$$B_{cache} = B_{mem} \mod NB_{cache}$$

Where:
- B_mem: Memory block number
- B_cache: Cache block location
- NB_cache: Number of cache blocks

Characteristics:
- Each memory block maps to exactly one cache line
- Simple hardware: one comparator needed
- Fast access: no search required
- High conflict miss rate: blocks compete for same line

Example (4-line cache):
```
Memory Block:  0  1  2  3  4  5  6  7  8  9  10 11 ...
Cache Line:    0  1  2  3  0  1  2  3  0  1  2  3 ...
```

### Fully Associative Cache

Mapping Rule:
- Any memory block can go in any cache line
- No index bits needed (all address bits except offset are tag)

Characteristics:
- Maximum flexibility: no conflict misses
- Complex hardware: must compare all tags simultaneously
- Higher power consumption
- Typically used only for small caches (TLB, victim cache)

### Set-Associative Cache

Mapping Rule:
$$S_{cache} = B_{mem} \mod NS_{cache}$$

Where:
- S_cache: Set number in cache
- NS_cache: Number of sets = NB_cache / associativity

Characteristics:
- Compromise between direct-mapped and fully associative
- Each set contains N ways (N-way set associative)
- Any block in a set can be replaced
- Common configurations: 2-way, 4-way, 8-way, 16-way

Example (8-line, 2-way cache = 4 sets):
```
Memory Block:  0  1  2  3  4  5  6  7  8  ...
Set Number:    0  1  2  3  0  1  2  3  0  ...
Within set:    Either way 0 or way 1 (2 choices)
```

### Comparison Table

| Feature | Direct-Mapped | 2-Way | 4-Way | Fully Assoc |
|---------|---------------|-------|-------|-------------|
| Comparators | 1 | 2 | 4 | N |
| Conflict Misses | High | Medium | Low | None |
| Hit Time | Fastest | Fast | Medium | Slowest |
| Hardware Cost | Lowest | Low | Medium | Highest |

### Address Field Sizes

For cache with:
- C = total cache size
- B = block size  
- N = associativity

| Field | Bits |
|-------|------|
| Offset | log₂(B) |
| Index | log₂(C / (B × N)) |
| Tag | Address bits - Offset - Index |

### Practical Example

32 KB cache, 64-byte blocks, 4-way associative, 32-bit addresses:

- Blocks = 32KB / 64B = 512 blocks
- Sets = 512 / 4 = 128 sets
- Offset = log₂(64) = 6 bits
- Index = log₂(128) = 7 bits
- Tag = 32 - 6 - 7 = 19 bits

## 📌 Key Points (Optional)
- Increasing associativity reduces conflict misses but increases hit time.
- 8-way associative approaches fully associative miss rate for most workloads.

## 🖼️ Recommended Image
- Side-by-side diagrams showing direct-mapped, set-associative, and fully associative mapping.

## 🔗 Connections
- [[087-Cache Memory Organization and Parameters]]
- [[089-Cache Miss Types Three Cs]]
- [[090-Average Memory Access Time AMAT]]
- [[086-Memory Hierarchy Principles and Locality]]
