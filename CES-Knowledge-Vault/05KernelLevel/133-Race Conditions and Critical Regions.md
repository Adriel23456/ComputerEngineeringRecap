---
Theme: Race Conditions and Critical Regions
Date created: 2026-01-31 12:00
tags: [KernelLevel, Synchronization, Concurrency, RaceCondition]
---

## 📚 Idea/Concept

A race condition occurs when multiple processes or threads access shared data and the final result depends on the order/timing of execution. Critical regions are code sections that access shared resources and must be protected.

### Race Condition Definition

When two or more processes/threads:
- Read and write shared data
- The outcome depends on execution order
- Results are non-deterministic

### Classic Example: Counter Increment

```c
// Shared variable
int counter = 0;

// Thread A                    // Thread B
counter++;                     counter++;

// Expected: counter = 2
// Possible: counter = 1 (race condition!)
```

Why it fails:
```
counter++ is actually three operations:
1. LOAD  counter → register
2. ADD   register + 1
3. STORE register → counter

Interleaving that causes bug:
┌────────────────────┬────────────────────┐
│     Thread A       │     Thread B       │
├────────────────────┼────────────────────┤
│ LOAD counter (0)   │                    │
│                    │ LOAD counter (0)   │
│ ADD (reg = 1)      │                    │
│                    │ ADD (reg = 1)      │
│ STORE counter (1)  │                    │
│                    │ STORE counter (1)  │
└────────────────────┴────────────────────┘
Result: counter = 1 (lost update!)
```

### Critical Region

Definition: Part of the program where shared memory/resources are accessed.

If critical regions never overlap, no race conditions occur.

```
Process A:                     Process B:
┌──────────────────────┐      ┌──────────────────────┐
│   Non-critical       │      │   Non-critical       │
├──────────────────────┤      ├──────────────────────┤
│ ████ CRITICAL ████   │      │                      │
│ ████ REGION   ████   │      │                      │ ← B must wait
├──────────────────────┤      ├──────────────────────┤
│   Non-critical       │      │ ████ CRITICAL ████   │
│                      │      │ ████ REGION   ████   │
└──────────────────────┘      └──────────────────────┘
```

### Mutual Exclusion

Solution: Ensure that no two processes are in their critical region simultaneously for the same resource.

Important clarification:
- Mutual exclusion means no two entities in the SAME critical region
- NOT that processes can't execute at the same time in general
- It's about protecting the shared resource

### Four Conditions for a Good Solution

An efficient critical region solution must satisfy:

1. **Mutual Exclusion**: No two processes simultaneously in critical region

2. **Progress**: If no one is in critical region, a waiting process must be able to enter

3. **No Starvation**: No process waits forever (bounded waiting)

4. **Independence**: Solution must not depend on CPU speeds or number of processors

```
┌───────────────────────────────────────────────────────────┐
│         Good Solution Properties                          │
├───────────────────────────────────────────────────────────┤
│  ✓ Mutual exclusion enforced                             │
│  ✓ Process outside CR doesn't block others               │
│  ✓ No process waits indefinitely                         │
│  ✓ Works regardless of speeds/CPUs                       │
└───────────────────────────────────────────────────────────┘
```

### Why Race Conditions Are Hard to Debug

Symptoms of race conditions:
- Program works with `printf` statements, fails without them
- Works in debug mode, fails in release
- Fails only under heavy load
- Different results on different runs
- "Heisenbug" - disappears when you look for it

The timing changes caused by:
- Debug output (slower)
- Different optimization levels
- System load variations
- Multi-core vs single-core

### Shared Resource Examples

What needs protection:
- Global variables
- Shared memory regions
- Files being written
- Database records
- Hardware registers
- Any mutable shared state

## 📌 Key Points (Optional)
- Race conditions arise when shared data access isn't coordinated.
- Critical regions must be protected with mutual exclusion.
- A good solution ensures safety without causing starvation.

## 🖼️ Recommended Image
- Timeline showing interleaved execution leading to race condition.

## 🔗 Connections
- [[134-Synchronization Solutions and Semaphores]]
- [[131-Inter Process Communication IPC]]
- [[132-Threads vs Processes]]
- [[135-Deadlock Fundamentals and Coffman Conditions]]