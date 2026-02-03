---
Theme: Threads vs Processes
Date created: 2026-01-31 12:00
tags: [KernelLevel, Threads, Process, Concurrency]
---

## 📚 Idea/Concept

Threads are "lightweight processes" that enable multiple flows of execution within a single process, sharing memory space while maintaining independent execution state. They are the cousins of processes.

### Thread Definition

A thread has its own:
- Program Counter (PC)
- Stack
- Register set
- Thread state

A thread shares with siblings:
- Code section
- Data section
- Open files
- Heap memory

### Process vs Thread Comparison

```
Single-threaded Process:           Multi-threaded Process:
┌─────────────────────────┐       ┌─────────────────────────┐
│ ┌─────────────────────┐ │       │ ┌─────────────────────┐ │
│ │    Code Section     │ │       │ │    Code Section     │ │
│ └─────────────────────┘ │       │ └─────────────────────┘ │
│ ┌─────────────────────┐ │       │ ┌─────────────────────┐ │
│ │    Data Section     │ │       │ │    Data Section     │ │
│ └─────────────────────┘ │       │ └─────────────────────┘ │
│ ┌─────────────────────┐ │       │ ┌───────┬───────┬─────┐ │
│ │  Registers │ PC     │ │       │ │Thread1│Thread2│Thr3 │ │
│ └─────────────────────┘ │       │ │Regs/PC│Regs/PC│Regs │ │
│ ┌─────────────────────┐ │       │ └───────┴───────┴─────┘ │
│ │       Stack         │ │       │ ┌───────┬───────┬─────┐ │
│ └─────────────────────┘ │       │ │Stack 1│Stack 2│Stk 3│ │
└─────────────────────────┘       │ └───────┴───────┴─────┘ │
                                  └─────────────────────────┘
```

### Why Use Threads?

Advantages over processes:
1. **Natural memory sharing** - No IPC overhead for shared data
2. **Lower creation cost** - Much faster than process creation
3. **Lower context switch cost** - Same address space, no TLB flush
4. **Better multicore utilization** - Real parallelism if cores available

Word processor example:
```
┌─────────────────────────────────────────┐
│           Word Processor Process        │
│                                         │
│  Thread 1: Handle user input            │
│  Thread 2: Spell checking               │
│  Thread 3: Auto-save                    │
│  Thread 4: Formatting/rendering         │
│                                         │
│  All share: Document data, preferences  │
└─────────────────────────────────────────┘
```

### When Is There Real Parallelism?

```
Single Core:                  Multi-Core:
┌────┬────┬────┬────┐        ┌────┬────┬────┬────┐
│ T1 │ T2 │ T1 │ T2 │        │ T1 │ T1 │ T1 │ T1 │ Core 0
└────┴────┴────┴────┘        ├────┼────┼────┼────┤
  Interleaved (illusion)      │ T2 │ T2 │ T2 │ T2 │ Core 1
                              └────┴────┴────┴────┘
                                True parallelism
```

### Thread Implementation Options

**User-Level Threads:**
```
┌─────────────────────────────────────┐
│           User Space                │
│  ┌─────────────────────────────┐   │
│  │    Thread Library           │   │
│  │  ┌────┬────┬────┬────┐     │   │
│  │  │ T1 │ T2 │ T3 │ T4 │     │   │
│  │  └────┴────┴────┴────┘     │   │
│  │    Thread Table             │   │
│  └─────────────────────────────┘   │
├─────────────────────────────────────┤
│           Kernel                    │
│  (Sees only one process)           │
└─────────────────────────────────────┘

Advantages:
- Fast context switch (no kernel)
- Custom scheduling per process
- Works on any OS

Disadvantages:
- Blocking syscall blocks ALL threads
- No true parallelism
- Can't use multiple cores
```

**Kernel-Level Threads:**
```
┌─────────────────────────────────────┐
│           User Space                │
│  ┌────┬────┬────┬────┐             │
│  │ T1 │ T2 │ T3 │ T4 │             │
│  └────┴────┴────┴────┘             │
├─────────────────────────────────────┤
│           Kernel                    │
│  ┌─────────────────────────────┐   │
│  │    Kernel Thread Table       │   │
│  │  ┌────┬────┬────┬────┐      │   │
│  │  │ T1 │ T2 │ T3 │ T4 │      │   │
│  │  └────┴────┴────┴────┘      │   │
│  └─────────────────────────────┘   │
└─────────────────────────────────────┘

Advantages:
- True parallelism on multicore
- One thread blocks, others continue
- Kernel schedules threads

Disadvantages:
- Slower context switch (kernel involved)
- More system call overhead
```

### Critical Distinction

**Scheduling ≠ Synchronizing**
- **Scheduling**: Deciding WHO runs on CPU and WHEN
- **Synchronizing**: Coordinating ACCESS to shared data

The OS must handle priorities between threads. With shared resources, synchronization becomes critical.

## 📌 Key Points (Optional)
- Threads share memory, processes don't (by default).
- Scheduling determines execution order; synchronization prevents data corruption.
- Kernel threads enable true parallelism; user threads are faster but limited.

## 🖼️ Recommended Image
- Diagram showing thread vs process memory layout differences.

## 🔗 Connections
- [[127-Process Concept and Program vs Process]]
- [[133-Race Conditions and Critical Regions]]
- [[147-CPU Scheduling Algorithms]]