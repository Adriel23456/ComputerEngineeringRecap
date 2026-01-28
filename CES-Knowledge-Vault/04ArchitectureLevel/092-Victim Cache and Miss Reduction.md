---
Theme: Victim Cache and Miss Reduction
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, Optimization]
---

## 📚 Idea/Concept

A victim cache is a small, fully associative cache that stores blocks evicted from the main cache, providing a second chance for blocks that would otherwise cause conflict misses, particularly effective for direct-mapped caches.

### Conflict Miss Problem

Direct-mapped cache limitation:
```
Address A maps to set 0
Address B maps to set 0
Address C maps to set 0

Access pattern: A, B, C, A, B, C, A, B, C...
Result: 100% miss rate (ping-pong eviction)
```

Even though cache has capacity, conflicts cause misses.

### Victim Cache Concept

Small buffer (4-16 entries) holds recently evicted blocks:
```
┌─────────────────────────────────────┐
│          Main Cache                 │
│  (Direct-mapped or low assoc)       │
└──────────────┬──────────────────────┘
               │ Evicted blocks
               ▼
┌─────────────────────────────────────┐
│         Victim Cache                │
│    (Fully associative, 4-16 entries)│
└─────────────────────────────────────┘
```

### Operation

On cache miss:
1. Check main cache → Miss
2. Check victim cache → Hit? 
   - Yes: Swap with main cache line, return data
   - No: Fetch from memory, evict from main to victim

On main cache eviction:
1. Evicted block moves to victim cache
2. If victim cache full, LRU victim block discarded

### Swap Operation

```
Main Cache Line X: Block A
Victim Cache Entry: Block B

If access to B:
1. Swap A and B
2. Main Cache Line X: Block B (hit)
3. Victim Cache Entry: Block A
```

This "swap" gives frequently alternating blocks second chance.

### Effectiveness

Jouppi's results (original 1990 paper):
- 4-entry victim cache
- Reduces conflict misses by 20-95%
- Most effective for direct-mapped L1

Example scenario:
```
Without victim cache:
A, B, A, B, A, B → 6 misses

With victim cache:
A (miss), B (miss, A→victim), A (victim hit, swap)
B (victim hit, swap), A (victim hit, swap), B (victim hit, swap)
→ 2 misses
```

### Implementation Details

Victim cache is checked in parallel with main cache tag:
- No additional latency on main cache hit
- Victim cache provides data on swap

Victim cache fields:
- Tag (full address tag)
- Data (cache line)
- Valid bit
- LRU state

### Design Considerations

Size trade-off:
- Too small: Limited conflict absorption
- Too large: Power, area, latency concerns
- Sweet spot: 4-16 entries

Associativity:
- Always fully associative (small enough)
- No index bits needed
- Compare all tags in parallel

### Selective Victim Cache

Enhancement: Only cache "useful" victims

Prediction:
- Track reuse of victim cache entries
- Evict predicted non-reusable blocks to memory directly
- Keep blocks likely to be reused

### Exclusive vs Inclusive

Exclusive victim cache:
- Block in either main or victim, not both
- Swap operation maintains exclusivity
- No wasted capacity

Inclusive (not typical):
- Would duplicate data
- No advantage for victim cache

### Multi-Level Cache Interaction

Victim cache typically at L1:
```
┌─────────┐
│   L1    │
├─────────┤
│ Victim  │
├─────────┤
│   L2    │
└─────────┘
```

L2 inclusion considerations:
- Victim cache blocks may or may not be in L2
- Depends on L2 inclusion policy

### Other Miss Reduction Techniques

Prefetching:
- Fetch blocks before they're needed
- Hardware or software controlled

Way prediction:
- Predict which way will hit
- Access predicted way first, reduce energy

Pseudo-associativity:
- Check alternate locations on miss
- Column associativity, skewed associativity

### Column Associativity

Alternative to victim cache:
```
Primary location: Set 0
Secondary location: Set 0 with inverted high bit

Miss in primary → Check secondary → Miss → Fetch
```

Simpler than victim cache but less flexible.

### Filter Cache

Related concept:
- Small cache in front of L1
- Filters repeated accesses to same block
- Reduces L1 energy consumption

### Comparing Techniques

| Technique | Reduces | Complexity | Effectiveness |
|-----------|---------|------------|---------------|
| Victim Cache | Conflict | Low | High for DM |
| Higher Assoc | Conflict | Medium | High |
| Larger Cache | Capacity | High | Varies |
| Prefetching | Compulsory | Medium | Varies |

## 📌 Key Points (Optional)
- Victim cache provides most benefit for direct-mapped caches with conflict misses.
- The swap mechanism ensures frequently accessed blocks stay available.

## 🖼️ Recommended Image
- Diagram showing victim cache operation with swap on hit.

## 🔗 Connections
- [[087-Cache Memory Organization and Parameters]]
- [[089-Cache Miss Types Three Cs]]
- [[088-Cache Mapping Direct Associative and Set Associative]]
- [[090-Average Memory Access Time AMAT]]
- [[093-Prefetching Hardware and Software]]