---
Theme: Process States FSM Simplified and Complete
Date created: 2026-01-31 12:00
tags: [KernelLevel, Process, States, FSM]
---

## 📚 Idea/Concept

Processes transition through different states during their lifetime. Understanding these states and transitions is crucial for comprehending how the OS manages execution.

### Simplified Model (3 States)

```
                    ┌─────────────────────────────────┐
                    │                                 │
                    ▼                                 │
              ┌──────────┐                           │
         ┌───→│  Ready   │←──────────────────┐      │
         │    └────┬─────┘                   │      │
         │         │                         │      │
         │    (3)  │ Scheduler selects       │      │
         │         ▼                         │      │
         │    ┌──────────┐              (2)  │      │
         │    │ Running  │───────────────────┘      │
         │    └────┬─────┘    Quantum expires       │
         │         │                                │
    (4)  │    (1)  │ Blocks for I/O or resource    │
  Input  │         ▼                                │
available│    ┌──────────┐                          │
         └────┤ Blocked  │                          │
              └──────────┘                          │

Transitions:
(1) Process blocks waiting for input/resource
(2) Scheduler preempts (quantum expired)
(3) Scheduler selects this process
(4) Input/resource becomes available
```

State definitions:
- **Running**: Currently executing on CPU
- **Ready**: Waiting for CPU time (runnable)
- **Blocked**: Waiting for external event (I/O, resource)

### Complete Model (5 States)

```
                           ┌───────────────────────────────────────┐
                           │                                       │
           (1)             ▼              (5)                      │
        ┌──────┐      ┌─────────┐      ┌───────────┐              │
NULL ──→│ New  │─────→│  Ready  │─────→│ Running   │──────────────┤
        └──────┘  (2) └────┬────┘  (3) └─────┬─────┘     (4)      │
                           ↑                 │                     │
                           │            (6)  │ (7)                 │
                      (8)  │                 ▼                     ▼
                      ┌────┴────┐      ┌──────────┐         ┌──────────┐
                      │ Blocked │←─────┤  Exit/   │         │ Terminated│
                      └─────────┘      │ Zombie   │         └──────────┘
                                       └──────────┘
```

### State Transition Descriptions

| # | Transition | Cause |
|---|-----------|-------|
| 1 | NULL → New | Process creation requested |
| 2 | New → Ready | OS prepares PCB, tables, resources |
| 3 | Ready → Running | Scheduler selects process |
| 4 | Running → Terminated | Process completes or is killed |
| 5 | Running → Ready | Quantum expires (preemption) |
| 6 | Running → Blocked | Requests resource/waits for I/O |
| 7 | Running → Exit | Fatal error or voluntary exit |
| 8 | Blocked → Ready | Awaited event occurs |

### Detailed Transition Analysis

**New → Ready:**
- OS prepares: PC, tables, addresses, registers
- Typically limited by max process policy
- Question: What if no limit? → Resource exhaustion

**Ready → Running:**
- Scheduler selects based on algorithm
- Context loaded from PCB
- CPU begins executing

**Running → Ready:**
- Quantum (time slice) expires
- Higher priority process arrives
- Voluntary yield

**Running → Blocked:**
- System call requests I/O or resource
- Must wait for external event
- Common cause of transition

**Blocked → Ready:**
- I/O completes, resource available
- Process can compete for CPU again

**Running → Terminated:**
- Normal completion
- Fatal error
- Killed by another process
- Parent terminates child

### Quantum Concept

Quantum (time slice):
- Maximum time a process can run before preemption
- Typically 1-100 milliseconds
- Critical for responsiveness

```
Quantum too large → Poor interactivity, processes hog CPU
Quantum too small → Excessive context switch overhead
Quantum → ∞      → No preemption, system freezes on loops
Quantum → 0      → Nothing but context switches
```

### Ready Queue and Multiple Blocked Queues

```
                    ┌─────────────┐
                    │    CPU      │
                    └──────┬──────┘
                           │
            ┌──────────────┴──────────────┐
            ▼                              ▼
     ┌─────────────┐              ┌─────────────────┐
     │ Ready Queue │              │ Blocked Queues  │
     │ ┌─┐┌─┐┌─┐┌─┐│              │ ┌────────────┐  │
     │ │P││P││P││P││              │ │ Disk I/O   │  │
     │ └─┘└─┘└─┘└─┘│              │ └────────────┘  │
     └─────────────┘              │ ┌────────────┐  │
                                  │ │ Network    │  │
                                  │ └────────────┘  │
                                  │ ┌────────────┐  │
                                  │ │ Keyboard   │  │
                                  │ └────────────┘  │
                                  └─────────────────┘
```

## 📌 Key Points (Optional)
- The 3-state model captures essential transitions; 5-state adds lifecycle endpoints.
- Quantum determines the balance between responsiveness and overhead.

## 🖼️ Recommended Image
- State diagram showing all transitions with labeled arrows.

## 🔗 Connections
- [[127-Process Concept and Program vs Process]]
- [[130-Context Switch and Process Control Block]]
- [[147-CPU Scheduling Algorithms]]