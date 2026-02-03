---
Theme: Deadlock Fundamentals and Coffman Conditions
Date created: 2026-01-31 12:00
tags: [KernelLevel, Deadlock, Synchronization, Concurrency]
---

## 📚 Idea/Concept

Deadlock occurs when a set of processes is blocked, each waiting for a resource held by another process in the set. No process can proceed, and the system is stuck.

### Intuitive Example

Two processes need Printer and Scanner:
```
Process A:                    Process B:
1. Acquires Printer          1. Acquires Scanner
2. Requests Scanner          2. Requests Printer
   (Scanner held by B)          (Printer held by A)
   → BLOCKED                    → BLOCKED

Neither can proceed → DEADLOCK
```

### Formal Definition

A set of processes is in deadlock if:
- Each process waits for an event
- That event can only be caused by another process in the set
- Typically: waiting for resource release

### Resource Types

**Preemptible Resources:**
- Can be taken away without damage
- Examples: CPU, memory (can be saved/restored)

**Non-Preemptible Resources:**
- Cannot be taken mid-use without corruption
- Examples: Printer mid-page, database lock, tape drive
- These cause deadlocks

Resource usage cycle:
```
Request → Use (Critical Region) → Release
```

### Coffman Conditions

Four conditions that must ALL hold for deadlock to exist:

```
┌─────────────────────────────────────────────────────────────┐
│              Coffman Conditions (ALL required)              │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  1. MUTUAL EXCLUSION                                        │
│     At least one resource is non-shareable                  │
│     Only one process can use it at a time                   │
│                                                              │
│  2. HOLD AND WAIT                                           │
│     Process holds resource(s) while waiting for more        │
│     Won't release what it has until it gets what it needs   │
│                                                              │
│  3. NO PREEMPTION                                           │
│     Resources cannot be forcibly taken away                 │
│     Only voluntary release by holding process               │
│                                                              │
│  4. CIRCULAR WAIT                                           │
│     Circular chain of processes exists                      │
│     P1→P2→P3→...→Pn→P1                                      │
│     Each waiting for resource held by next in chain         │
│                                                              │
└─────────────────────────────────────────────────────────────┘

Key insight: Break ANY ONE condition → No deadlock possible
```

### Resource Allocation Graph (RAG)

Visual modeling tool:
- **Circles**: Processes (P₁, P₂, ...)
- **Squares**: Resources (R₁, R₂, ...)
- **Arrow R→P**: Resource assigned to process
- **Arrow P→R**: Process waiting for resource

```
Deadlock Example:

    P1 ────→ R1 ────→ P2
     ↑                 │
     │                 ↓
    R2 ←───────────────┘

P1 holds R2, wants R1
P2 holds R1, wants R2
Cycle detected → DEADLOCK
```

### Why Deadlock Is Serious

- Resources trapped, unavailable
- System performance degrades
- May affect critical services
- Often requires manual intervention

### Deadlock Handling Strategies

Four approaches:

```
┌─────────────────────────────────────────────────────────────┐
│   Strategy        │  Description                            │
├───────────────────┼─────────────────────────────────────────┤
│   1. Ignore       │  "Ostrich algorithm" - pretend it       │
│      (Ostrich)    │  won't happen, deal with it manually    │
├───────────────────┼─────────────────────────────────────────┤
│   2. Detection    │  Let deadlock occur, detect it,         │
│      + Recovery   │  then recover (kill processes/rollback) │
├───────────────────┼─────────────────────────────────────────┤
│   3. Avoidance    │  Dynamically grant resources only       │
│                   │  if safe state maintained (Banker's)    │
├───────────────────┼─────────────────────────────────────────┤
│   4. Prevention   │  Design system to break one Coffman     │
│                   │  condition structurally                 │
└───────────────────┴─────────────────────────────────────────┘
```

### Detection with Matrices

For systems with multiple resource types:

```
Vectors/Matrices:
E = Existing resources (total capacity per type)
A = Available resources (currently free)
C = Current allocation (what each process holds)
R = Request matrix (what each process still needs)

Invariant: A + Σ(C rows) = E

Detection: Find if there exists a safe sequence
           where all processes can finish
```

Example detection:
```
E = [4, 2, 3, 1]  (4 of R1, 2 of R2, 3 of R3, 1 of R4)
A = [2, 1, 0, 0]  (currently available)

Process | Currently Holds | Still Needs
   P1   |   [0,0,1,0]    |  [2,0,0,1]
   P2   |   [2,0,0,1]    |  [1,0,1,0]
   P3   |   [0,1,2,0]    |  [2,1,0,0]

Can we find order where all complete?
```

### Real-World Symptoms

Signs of deadlock in the wild:
- Blue screens, hangs
- Services that "freeze" with certain operation combinations
- Code that works with printf, fails without
- Programs that work with 2 threads, fail with 3

## 📌 Key Points (Optional)
- All four Coffman conditions must hold for deadlock.
- Breaking any one condition prevents deadlock.
- Synchronization solves races but can introduce deadlock if not careful.

## 🖼️ Recommended Image
- Resource Allocation Graph showing circular wait condition.

## 🔗 Connections
- [[136-Deadlock Avoidance and Bankers Algorithm]]
- [[137-Deadlock Prevention Strategies]]
- [[134-Synchronization Solutions and Semaphores]]
- [[133-Race Conditions and Critical Regions]]