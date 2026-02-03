---
Theme: Synchronization Solutions and Semaphores
Date created: 2026-01-31 12:00
tags: [KernelLevel, Synchronization, Semaphores, MutualExclusion]
---

## 📚 Idea/Concept

Multiple solutions exist for achieving mutual exclusion, ranging from simple (but flawed) approaches to robust mechanisms like semaphores. Understanding the progression helps appreciate why semaphores became the standard.

### Solution 1: Disable Interrupts

```c
// Enter critical region
disable_interrupts();

// Critical section
// ... access shared resource ...

// Leave critical region
enable_interrupts();
```

Problems:
- Only works on single CPU (other cores unaffected)
- Dangerous: user process controlling interrupts
- Delays important system functions
- Not practical for user-level code

Use case: Briefly inside kernel code only.

### Solution 2: Lock Variables (Software)

```c
int lock = 0;  // Shared lock

// Process trying to enter
while (lock == 1) { /* busy wait */ }
lock = 1;          // Take lock
// Critical section
lock = 0;          // Release lock
```

Problem: Race condition on the lock itself!
```
Process A                Process B
while(lock==1){}        while(lock==1){}
  // lock is 0            // lock is 0
lock = 1                lock = 1
  // Both in critical region!
```

### Solution 3: Strict Alternation

```c
int turn = 0;  // Whose turn

// Process 0                  // Process 1
while (turn != 0) { }         while (turn != 1) { }
// Critical section           // Critical section
turn = 1;                     turn = 0;
```

Problem: Violates progress condition
- If Process 0 doesn't want to enter, Process 1 is blocked
- Forced alternation even when unnecessary

### Solution 4: Peterson's Algorithm

```c
int flag[2] = {0, 0};  // Intention to enter
int turn;              // Whose turn when conflict

void enter_critical(int process) {
    int other = 1 - process;
    flag[process] = 1;     // I want to enter
    turn = other;          // Give other priority
    while (flag[other] && turn == other) { }  // Wait
}

void leave_critical(int process) {
    flag[process] = 0;     // No longer interested
}
```

Works correctly for two processes:
- Mutual exclusion: ✓
- Progress: ✓
- Bounded waiting: ✓

Limitation: Only works for exactly 2 processes.

### Semaphores: The Robust Solution

Invented by Dijkstra. A semaphore is an integer variable with two atomic operations:

**Down (P, wait, proberen):**
```
if (semaphore > 0) {
    semaphore--;
    // Continue execution
} else {
    // Block: add process to wait queue
    // Sleep until awakened
}
```

**Up (V, signal, verhogen):**
```
semaphore++;
if (processes waiting) {
    // Wake one waiting process
}
```

Critical property: Both operations are **ATOMIC** (indivisible).

### Semaphore Implementation

```
┌─────────────────────────────────────────────────────────────┐
│                    Semaphore Structure                       │
├─────────────────────────────────────────────────────────────┤
│  Value: integer (resource count or binary 0/1)              │
│  Wait Queue: list of blocked processes                      │
│  Last Operator: ID of last process that operated            │
│  Waiting Count: number of processes in queue                │
└─────────────────────────────────────────────────────────────┘
```

### Using Semaphores for Mutual Exclusion

```c
semaphore mutex = 1;  // Binary semaphore

// Process A                  // Process B
down(&mutex);                 down(&mutex);
// Critical section           // Critical section
up(&mutex);                   up(&mutex);
```

### Counting Semaphores

For resources with multiple instances:

```c
semaphore printers = 3;  // 3 printers available

// Any process wanting a printer:
down(&printers);         // Get printer (or wait)
// Use printer
up(&printers);           // Release printer
```

### Why Kernel Manages Semaphores

The semaphore lock is managed by the OS because:
- Atomicity requires hardware support or disabling interrupts
- Wait queues need kernel data structures
- Wake-up requires scheduler interaction
- Protection from malicious manipulation

### Semaphore Problems

Potential issues:
- **Deadlock**: Incorrect order of down operations
- **Starvation**: Unfair wake-up policy
- **Programming errors**: Forget up(), wrong order
- **No protocol for priority**: Which waiting process gets it?

```
If multiple processes are in DOWN state, 
which one gets awakened when UP occurs?

Semaphore protocol doesn't specify!
OS must decide (FIFO, priority, random...)
```

## 📌 Key Points (Optional)
- Semaphores provide atomic operations that solve the race on locks.
- Down/Up are atomic: either complete fully or don't execute at all.
- The OS kernel manages semaphore state and wait queues.

## 🖼️ Recommended Image
- Diagram showing semaphore value changes with down/up operations and process states.

## 🔗 Connections
- [[133-Race Conditions and Critical Regions]]
- [[135-Deadlock Fundamentals and Coffman Conditions]]
- [[131-Inter Process Communication IPC]]