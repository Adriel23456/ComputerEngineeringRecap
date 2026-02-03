---
Theme: Context Switch and Process Control Block
Date created: 2026-01-31 12:00
tags: [KernelLevel, Process, PCB, ContextSwitch]
---

## 📚 Idea/Concept

Context switching is the mechanism by which the OS saves the state of one process and loads the state of another, enabling multiprogramming. The Process Control Block (PCB) stores all information needed to resume a process.

### Why Context Switch Matters

Switching processes isn't "just picking another and running it":
- Must save ENTIRE current state
- Must load ENTIRE new state
- Involves multiple data structures
- Has real performance cost

### Process Control Block (PCB)

The PCB is THE critical data structure for process management:

```
┌─────────────────────────────────────────────────────────────┐
│                  Process Control Block                       │
├─────────────────────────────────────────────────────────────┤
│  Process Identification                                      │
│  ├─ Process ID (PID)                                        │
│  ├─ Parent Process ID (PPID)                                │
│  └─ User ID, Group ID                                       │
├─────────────────────────────────────────────────────────────┤
│  Processor State (Context)                                   │
│  ├─ Program Counter (PC)                                    │
│  ├─ Stack Pointer (SP)                                      │
│  ├─ General Purpose Registers                               │
│  ├─ Condition Codes / Flags                                 │
│  └─ FPU/SIMD Registers                                      │
├─────────────────────────────────────────────────────────────┤
│  Process State                                               │
│  └─ Running / Ready / Blocked / etc.                        │
├─────────────────────────────────────────────────────────────┤
│  Scheduling Information                                      │
│  ├─ Priority                                                │
│  ├─ Scheduling queue pointers                               │
│  └─ CPU time used, time slice remaining                     │
├─────────────────────────────────────────────────────────────┤
│  Memory Management                                           │
│  ├─ Page table base pointer                                 │
│  ├─ Memory limits (base/limit registers)                    │
│  └─ Segment table pointers                                  │
├─────────────────────────────────────────────────────────────┤
│  I/O and File Information                                   │
│  ├─ Open file descriptors table                             │
│  ├─ Current working directory                               │
│  └─ I/O status, pending operations                          │
├─────────────────────────────────────────────────────────────┤
│  Accounting Information                                      │
│  ├─ CPU time consumed                                       │
│  ├─ Time limits                                             │
│  └─ Process start time                                      │
└─────────────────────────────────────────────────────────────┘
```

### Key PCB Facts

- **One PCB per process** in the system
- **One PCB table per OS** (array or linked list of all PCBs)
- PCB created when process created
- PCB destroyed when process terminates

### Context Switch Steps

```
Process A running → Timer interrupt → Process B runs

┌─────────────────────────────────────────────────────────────┐
│ 1. Interrupt/Trap occurs                                    │
│    - Timer, I/O completion, system call                     │
├─────────────────────────────────────────────────────────────┤
│ 2. Save Process A's context                                 │
│    - Push registers to kernel stack                         │
│    - Save PC, SP, flags                                     │
│    - Update PCB_A with saved state                          │
│    - Change PCB_A state: Running → Ready/Blocked            │
├─────────────────────────────────────────────────────────────┤
│ 3. Scheduler decision                                       │
│    - Select next process (Process B)                        │
│    - Based on scheduling algorithm                          │
├─────────────────────────────────────────────────────────────┤
│ 4. Load Process B's context                                 │
│    - Load PCB_B values into registers                       │
│    - Restore PC, SP, flags                                  │
│    - Switch page tables (memory mapping)                    │
│    - Change PCB_B state: Ready → Running                    │
├─────────────────────────────────────────────────────────────┤
│ 5. Resume Process B                                         │
│    - Return from interrupt                                  │
│    - Process B continues from where it stopped              │
└─────────────────────────────────────────────────────────────┘
```

### Context Switch Overhead

What must be saved/restored:
- All CPU registers (dozens on modern CPUs)
- Floating point state
- Memory management state (TLB flush may be needed)
- Cache effects (cold cache after switch)

Typical cost: 1-10 microseconds direct + cache effects

### Visual Timeline

```
Time →
        ┌──────────────┬─────────┬──────────────┐
Process │  Process A   │ Context │  Process B   │
        │  executing   │ Switch  │  executing   │
        └──────────────┴─────────┴──────────────┘
                       ↑
                 Overhead time
                 (pure waste)
```

### PCB Table Organization

```
┌──────────────────────────────────────────────────────────┐
│                     PCB Table                             │
├─────┬───────────────────────────────────────────────────┤
│ PID │                    PCB Data                        │
├─────┼───────────────────────────────────────────────────┤
│  1  │ [init/systemd: PC=x, SP=y, State=Sleeping, ...]   │
├─────┼───────────────────────────────────────────────────┤
│  2  │ [kthreadd: PC=a, SP=b, State=Sleeping, ...]       │
├─────┼───────────────────────────────────────────────────┤
│ ... │ ...                                                │
├─────┼───────────────────────────────────────────────────┤
│ 1234│ [user_app: PC=m, SP=n, State=Running, ...]        │
└─────┴───────────────────────────────────────────────────┘
```

## 📌 Key Points (Optional)
- The PCB contains everything needed to pause and resume a process.
- Context switch overhead is real and affects system performance.
- One PCB per process, one PCB table per operating system.

## 🖼️ Recommended Image
- Diagram showing context switch flow between two processes with PCB updates.

## 🔗 Connections
- [[129-Process States FSM Simplified and Complete]]
- [[147-CPU Scheduling Algorithms]]
- [[132-Threads vs Processes]]