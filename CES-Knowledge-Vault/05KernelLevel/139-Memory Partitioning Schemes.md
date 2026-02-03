---
Theme: Memory Partitioning Schemes
Date created: 2026-01-31 12:00
tags: [KernelLevel, Memory, Partitioning, Fragmentation]
---

## 📚 Idea/Concept

Memory partitioning divides RAM into regions for processes. Different schemes trade off simplicity, utilization, and fragmentation differently.

### Fixed-Size Partitioning

Memory divided into predetermined partitions:

```
Variant 1: Equal-sized           Variant 2: Multiple sizes
┌──────────────────┐            ┌──────────────────┐
│  Partition 1     │ 2MB        │  Partition 1     │ 1MB
│  (2MB)           │            │  (1MB)           │
├──────────────────┤            ├──────────────────┤
│  Partition 2     │ 2MB        │  Partition 2     │ 2MB
│  (2MB)           │            │  (2MB)           │
├──────────────────┤            ├──────────────────┤
│  Partition 3     │ 2MB        │  Partition 3     │ 4MB
│  (2MB)           │            │  (4MB)           │
├──────────────────┤            ├──────────────────┤
│  Partition 4     │ 2MB        │  Partition 4     │ 8MB
│  (2MB)           │            │  (8MB)           │
└──────────────────┘            └──────────────────┘
```

Problems:
- Program larger than partition → Cannot run
- Program smaller than partition → **Internal fragmentation** (wasted space inside)

Queue strategies:
- **One queue per partition**: Process goes to smallest fitting partition
- **Single queue**: OS chooses best partition for each process

### Dynamic Partitioning

Partitions created on demand, sized exactly for each process:

```
Initial:                    After allocations:
┌──────────────────┐       ┌──────────────────┐
│                  │       │  Process A (3MB) │
│                  │       ├──────────────────┤
│    Free (16MB)   │       │  Process B (5MB) │
│                  │       ├──────────────────┤
│                  │       │  Process C (2MB) │
│                  │       ├──────────────────┤
└──────────────────┘       │    Free (6MB)    │
                           └──────────────────┘

After B terminates:
┌──────────────────┐
│  Process A (3MB) │
├──────────────────┤
│    Hole (5MB)    │ ← External fragmentation!
├──────────────────┤
│  Process C (2MB) │
├──────────────────┤
│    Free (6MB)    │
└──────────────────┘
```

### Fragmentation Types

```
┌─────────────────────────────────────────────────────────────┐
│  INTERNAL FRAGMENTATION                                      │
│  - Wasted space INSIDE allocated partition                  │
│  - Example: 18MB program in 20MB partition → 2MB wasted     │
│  - Caused by: Fixed partitioning                            │
├─────────────────────────────────────────────────────────────┤
│  EXTERNAL FRAGMENTATION                                      │
│  - Wasted space BETWEEN allocated regions                   │
│  - Total free > needed, but not contiguous                  │
│  - Example: 3 holes of 2MB each, need 5MB → Can't fit       │
│  - Caused by: Dynamic partitioning                          │
└─────────────────────────────────────────────────────────────┘
```

### Placement Algorithms

For dynamic partitioning, where to place new process?

| Algorithm | Strategy | Result |
|-----------|----------|--------|
| **First Fit** | First hole that fits | Fast, reasonable fragmentation |
| **Best Fit** | Smallest hole that fits | Minimizes leftover, creates tiny holes |
| **Worst Fit** | Largest hole | Leaves larger useful holes |

```
Memory: [3MB hole] [Used] [5MB hole] [Used] [10MB hole]
Request: 4MB

First Fit: Uses 5MB hole → Leaves 1MB hole
Best Fit:  Uses 5MB hole → Leaves 1MB hole
Worst Fit: Uses 10MB hole → Leaves 6MB hole (more useful)
```

Generally, **Worst Fit** is preferred when future reuse matters—larger remaining holes are more versatile.

### Compaction

Solution to external fragmentation:
```
Before compaction:              After compaction:
┌──────────────────┐           ┌──────────────────┐
│  Process A       │           │  Process A       │
├──────────────────┤           ├──────────────────┤
│    Hole          │           │  Process C       │
├──────────────────┤    →      ├──────────────────┤
│  Process C       │           │  Process D       │
├──────────────────┤           ├──────────────────┤
│    Hole          │           │                  │
├──────────────────┤           │  Contiguous Free │
│  Process D       │           │                  │
├──────────────────┤           │                  │
│    Hole          │           │                  │
└──────────────────┘           └──────────────────┘
```

Cost: Must relocate processes, update all addresses, expensive I/O.

### Buddy System

Allocator using power-of-2 block sizes:

```
Request 70KB (needs 128KB block):

Start: 1024KB block
Split: 512KB | 512KB
Split: 256KB | 256KB | 512KB
Split: 128KB | 128KB | 256KB | 512KB
        ↑
    Allocate this

On free: If "buddy" (adjacent same-size block) is free, merge
```

Advantages:
- O(1) split and merge
- Simple free list per size class
- Foundation for modern page frame allocators

Used today in Linux for managing physical page frames.

## 📌 Key Points (Optional)
- Fixed partitioning causes internal fragmentation; dynamic causes external.
- Buddy system enables efficient splitting and merging with power-of-2 sizes.
- Compaction solves external fragmentation but is expensive.

## 🖼️ Recommended Image
- Comparison of fixed vs dynamic partitioning with fragmentation examples.

## 🔗 Connections
- [[138-Memory Management Fundamentals]]
- [[140-Virtual Memory and Paging]]
