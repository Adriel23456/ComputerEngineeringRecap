---
Theme: Inter Process Communication IPC
Date created: 2026-01-31 12:00
tags: [KernelLevel, Process, IPC, Communication]
---

## 📚 Idea/Concept

Inter-Process Communication (IPC) enables processes to exchange data and coordinate activities. Since processes have isolated address spaces, the OS must provide mechanisms for controlled communication.

### Why IPC is Needed

Processes are isolated by design:
- Cannot directly read/write each other's memory
- Protection is essential for stability
- But cooperation is often necessary

Use cases:
- Producer-consumer patterns
- Client-server communication
- Parallel computation coordination
- Data sharing between related processes

### Two Fundamental IPC Models

```
┌─────────────────────────────────────────────────────────────┐
│           Shared Memory                                      │
│                                                              │
│   Process A          Shared Region          Process B       │
│  ┌─────────┐        ┌──────────┐          ┌─────────┐      │
│  │         │───────→│   Data   │←─────────│         │      │
│  │         │←───────│          │─────────→│         │      │
│  └─────────┘        └──────────┘          └─────────┘      │
│                                                              │
│  - Fast (no kernel involvement for data transfer)           │
│  - Requires synchronization (race conditions possible)      │
│  - OS sets up region, processes manage access               │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│           Message Passing                                    │
│                                                              │
│   Process A                                   Process B     │
│  ┌─────────┐                                ┌─────────┐    │
│  │         │─── send(B, msg) ──────────────→│         │    │
│  │         │                                │         │    │
│  │         │←── send(A, reply) ─────────────│         │    │
│  └─────────┘         ↑                      └─────────┘    │
│                      │                                      │
│               Kernel manages                                │
│               message delivery                              │
│                                                              │
│  - Safer (kernel controls transfer)                         │
│  - Slower (system calls required)                           │
│  - No synchronization needed for transfer itself           │
└─────────────────────────────────────────────────────────────┘
```

### Shared Memory Details

Setup and usage:
```c
// Process A creates shared memory
int shm_id = shmget(KEY, SIZE, IPC_CREAT | 0666);
char *shared = shmat(shm_id, NULL, 0);

// Process B attaches to same memory
int shm_id = shmget(KEY, SIZE, 0);
char *shared = shmat(shm_id, NULL, 0);

// Both can now read/write 'shared'
// BUT must coordinate access!
```

Characteristics:
- Region accessible by multiple processes
- Requires accepting coordination restrictions
- Fast for large data transfers
- Must handle synchronization manually

### Message Passing Details

Two fundamental operations:
- `send(destination, message)`
- `receive(source, message)`

Variations:
| Aspect | Options |
|--------|---------|
| Naming | Direct (to process) / Indirect (to mailbox) |
| Synchronization | Blocking / Non-blocking |
| Buffering | Zero capacity / Bounded / Unbounded |

### Direct vs Indirect Communication

```
Direct:                          Indirect (Mailbox):
┌───┐  send(P2, msg)  ┌───┐     ┌───┐  send(M, msg)  ┌───┐  recv(M)  ┌───┐
│P1 │────────────────→│P2 │     │P1 │───────────────→│ M │←──────────│P2 │
└───┘                 └───┘     └───┘                └───┘           └───┘
                                                    Mailbox
Must know recipient           Decoupled: sender doesn't
                              need to know receiver
```

### Synchronization Modes

| Mode | Send | Receive |
|------|------|---------|
| Blocking (sync) | Waits until received | Waits until message arrives |
| Non-blocking (async) | Returns immediately | Returns null if no message |
| Mixed | Various combinations possible |

### Quantum's Role in IPC

The quantum affects IPC timing:
- Process may block mid-communication
- Context switches during message transfer
- Quantum typically 1-10ms

### Common IPC Mechanisms

| Mechanism | Type | Use Case |
|-----------|------|----------|
| Pipes | Message | Parent-child, streaming |
| Named Pipes (FIFO) | Message | Unrelated processes |
| Message Queues | Message | Structured messages |
| Shared Memory | Shared | High-performance data sharing |
| Semaphores | Synchronization | Coordination primitive |
| Signals | Notification | Async event notification |
| Sockets | Message | Network + local communication |

## 📌 Key Points (Optional)
- Shared memory is fast but requires explicit synchronization.
- Message passing is safer but involves kernel overhead.
- Both models solve the fundamental isolation problem differently.

## 🖼️ Recommended Image
- Side-by-side comparison of shared memory vs message passing models.

## 🔗 Connections
- [[127-Process Concept and Program vs Process]]
- [[133-Race Conditions and Critical Regions]]
- [[134-Synchronization Solutions and Semaphores]]