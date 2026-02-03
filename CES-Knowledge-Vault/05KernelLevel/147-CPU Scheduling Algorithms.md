---
Theme: CPU Scheduling Algorithms
Date created: 2026-01-31 12:00
tags: [KernelLevel, Scheduling, CPU, Algorithms]
---

## 📚 Idea/Concept

CPU scheduling determines which process runs when, optimizing for different goals depending on system type. The scheduler (dispatcher) is responsible for placing processes on the CPU and performing context switches.

### When Scheduling Occurs

1. Process creation (new process enters ready queue)
2. Process termination (need to pick next)
3. Process blocks on I/O (switch to ready process)
4. I/O completion interrupt (blocked → ready, may preempt)
5. Timer interrupt (quantum expired)

### Scheduling Goals

**All Systems:**
- Fairness: Equal CPU share for similar processes
- Policy enforcement: Priorities respected
- Balance: Keep all resources busy

**Batch Systems:**
- Maximize throughput (jobs/hour)
- Minimize turnaround time
- Maximize CPU utilization

**Interactive Systems:**
- Minimize response time
- Meet user expectations (proportionality)

**Real-Time Systems:**
- Meet deadlines (deterministic)
- Predictable behavior

### Batch Scheduling Algorithms

**FIFO / FCFS (First Come First Served):**
```
Jobs arrive: P1(24ms), P2(3ms), P3(3ms)

Timeline:
│ P1 (24ms)                    │ P2 │ P3 │
0                              24   27   30

Average wait: (0 + 24 + 27) / 3 = 17ms

Non-preemptive, simple, convoy effect (short jobs wait)
```

**SJF (Shortest Job First):**
```
Same jobs, SJF order:
│P2│P3│ P1 (24ms)                    │
0  3  6                              30

Average wait: (6 + 0 + 3) / 3 = 3ms

Optimal IF all jobs known in advance
Non-preemptive, can cause starvation of long jobs
```

**SRTF (Shortest Remaining Time First):**
```
Preemptive version of SJF
New short job can preempt running longer job
Even better average wait time
Still can starve long jobs
```

### Interactive Scheduling Algorithms

**Round Robin (RR):**
```
Quantum = 4ms
Jobs: P1(8ms), P2(4ms), P3(2ms)

│P1│P2│P3│P1│
0  4  8  10 14

Each process gets quantum, then goes to back of queue
Preemptive, fair, no starvation
Performance depends on quantum size:
  - Too large → degenerates to FCFS
  - Too small → excessive context switch overhead
```

**Priority Scheduling:**
```
Each process has priority (lower number = higher priority)

Problem: Starvation of low-priority processes
Solution: Aging (increase priority over time)

Can be preemptive or non-preemptive
```

**Multilevel Queue / Feedback:**
```
Multiple queues with different priorities and quanta:

Queue 0 (highest): quantum = 1
Queue 1: quantum = 2
Queue 2: quantum = 4
Queue 3 (lowest): FCFS

New process starts at Queue 0
If uses full quantum, demoted to lower queue
I/O-bound processes stay high (release CPU quickly)
CPU-bound sink to lower queues
```

### Real-Time Scheduling

**Rate Monotonic (RM):**
```
Static priority: shorter period = higher priority

Process | Period | Execution
   A    |   50   |    20
   B    |  100   |    35

Priority: A > B (A has shorter period)

Schedulability test: Σ(Ci/Pi) ≤ n(2^(1/n) - 1)
For n=2: ≤ 0.828

(20/50) + (35/100) = 0.4 + 0.35 = 0.75 ≤ 0.828 ✓
```

**EDF (Earliest Deadline First):**
```
Dynamic priority: closest deadline = highest priority

Can achieve 100% CPU utilization
Schedulability: Σ(Ci/Pi) ≤ 1

More flexible than RM, handles overload poorly
```

**RM vs EDF:**
| Aspect | Rate Monotonic | EDF |
|--------|---------------|-----|
| Priority | Static | Dynamic |
| Utilization | ≤ ~69% (ln2) | ≤ 100% |
| Simplicity | Simpler | More complex |
| Overload | Predictable | Unpredictable |

### Key Formulas

**CPU Utilization:**
$$U = \frac{\text{CPU busy time}}{\text{Total time}}$$

**Throughput:**
$$\text{Throughput} = \frac{\text{Number of processes completed}}{\text{Time}}$$

**Turnaround Time:**
$$T_{turnaround} = T_{completion} - T_{arrival}$$

**Waiting Time:**
$$T_{wait} = T_{turnaround} - T_{burst}$$

**Response Time:**
$$T_{response} = T_{first\_run} - T_{arrival}$$

### Dispatcher Role

The dispatcher:
1. Switches context (save/restore PCB)
2. Switches to user mode
3. Jumps to correct instruction in user program

Dispatch latency should be minimized.

## 📌 Key Points (Optional)
- FIFO is simplest but suffers convoy effect.
- SJF is optimal for average wait but requires knowing job lengths.
- Round Robin provides fairness through time slicing.
- Real-time systems need RM or EDF for deadline guarantees.

## 🖼️ Recommended Image
- Gantt charts comparing FIFO, SJF, and Round Robin for same workload.

## 🔗 Connections
- [[129-Process States FSM Simplified and Complete]]
- [[130-Context Switch and Process Control Block]]
- [[132-Threads vs Processes]]