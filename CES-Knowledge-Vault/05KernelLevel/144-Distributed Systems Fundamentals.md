---
Theme: Distributed Systems Fundamentals
Date created: 2026-01-31 12:00
tags: [KernelLevel, DistributedSystems, Networks]
---

## 📚 Idea/Concept

A distributed system is a collection of autonomous computers connected by a network that cooperate to provide the illusion of a single coherent system. The network is the fundamental limiting factor.

### Core Definition

Three guiding principles:
1. **Physical separation**: Nodes on different machines; no shared memory or global clock
2. **Network first**: Latency, loss, duplication, reordering, partitions affect everything
3. **Unity illusion**: Protocols and coordination mask distribution from users

### Distributed vs Parallel Processing

```
┌─────────────────────────────────────────────────────────────┐
│  DISTRIBUTED = Real parallelism on DIFFERENT machines      │
│  - Communication via network                                │
│  - Partial failures possible                                │
│  - Inconsistencies, need explicit coordination             │
│  - Example: Web services, databases across data centers    │
├─────────────────────────────────────────────────────────────┤
│  PARALLEL = Division of work on SAME machine               │
│  - Shared memory, low latency                              │
│  - All-or-nothing failure                                  │
│  - Locks, CAS for synchronization                          │
│  - Example: Multi-threaded application, GPU computing      │
└─────────────────────────────────────────────────────────────┘

Key rule: If it runs on different machines → DISTRIBUTED
          If it runs on same machine → PARALLEL
          (Both can coexist: each cluster node runs parallel code)
```

### The Network as Limiting Factor

"Networks are the main constraint in distributed processing."

Network problems:
- **Latency**: Milliseconds vs nanoseconds (local memory)
- **Loss**: Packets may never arrive
- **Duplication**: Same packet arrives multiple times
- **Reordering**: Packets arrive out of order
- **Partitions**: Network splits, nodes can't communicate

### Key Advantages of Distribution

1. **Availability 24/7**: Replicas, failover, load balancing
2. **Resource sharing**: GPUs, storage, databases
3. **Incremental scaling**: Add nodes without replacing everything
4. **Proximity**: Serve users from nearby locations

### Communication Protocols

**What is a protocol?**
Rules agreed by computers for communication:
- **Syntax**: Format, fields, encoding
- **Semantics**: Meaning of messages, error codes
- **Timing**: Timeouts, ordering, pacing

**TCP vs UDP:**

| Aspect | TCP | UDP |
|--------|-----|-----|
| Connection | Connection-oriented | Connectionless |
| Reliability | Guaranteed delivery, ordering | Best effort |
| Overhead | Higher (acks, retransmits) | Lower |
| Use case | Integrity matters | Latency matters |
| Examples | HTTP, SSH, databases | DNS, VoIP, gaming |

### Communication Methods

**1. Message Passing:**
```
No shared memory; explicit send/receive

Process A                    Process B
    │                            │
    │── send(B, message) ───────→│
    │                            │
    │←── send(A, reply) ─────────│
    │                            │

Requires: Correlation IDs, message IDs, sequence numbers
```

**2. RPC (Remote Procedure Call):**
```
Make remote calls look like local calls

Process A                         Process B
    │                                 │
    │ result = P(x)                  │
    │     │                          │
    │     ↓                          │
    │ [Stub: pack x, send]───────────→ [Stub: unpack, call P]
    │                                │     │
    │                                │     ↓
    │                                │   Execute P(x)
    │                                │     │
    │ [Stub: unpack result]←─────────│ [Stub: pack result, send]
    │     │                          │
    │     ↓                          │
    │ use result                     │
```

Critical: Process A NEVER "connects directly" to B. Always through network + module (RPC/MPI/queues/sockets).

### Delivery Semantics

| Semantic | Meaning | Implementation |
|----------|---------|----------------|
| **At-most-once** | 0 or 1 execution | Unique IDs, reply cache, deduplication |
| **At-least-once** | 1 or more executions | Retries, idempotent operations |
| **Exactly-once** | Exactly 1 execution | At-least-once + transactional dedup |

Handling duplicates (N copies arrive):
- Use message IDs for deduplication
- Idempotent operations (repeating has same effect)
- Reply cache (return cached response for duplicate request)

### Mutual Exclusion in Distributed Systems

No shared memory → Can't use semaphores directly!

**Centralized Algorithm:**
```
Central coordinator controls all resources
- Simple, consistent decisions
- Single point of failure (SPOF)
- Bottleneck under load

Flow: Request → Coordinator → Grant → Release
```

**Distributed Algorithms:**
- Ricart-Agrawala (logical timestamps)
- Token-based (passing token grants access)
- More complex but no SPOF

### Deadlock in Distributed Systems

Same four Coffman conditions apply, but detection/handling is harder:

**Treatments:**
| Approach | Description |
|----------|-------------|
| Centralized | One coordinator detects global deadlock |
| Hierarchical | Coordinators per domain, escalate |
| Distributed | Probe messages detect cycles |
| Policies | Wait-die, wound-wait, timeouts |

### The Complete Path

Everything goes through the network:
```
Client → UI → Logic → Socket Stack → OS → NIC → 
    [Network: routing, queues, congestion] →
NIC → OS → Socket Stack → Server Logic → Database
```

## 📌 Key Points (Optional)
- Distributed ≠ Parallel: distributed means different machines, network involved.
- Network is the limiting factor; design for failures by default.
- No process "talks directly" to another; always through network modules.
- Without idempotency + deduplication + timeouts, data will be lost or duplicated.

## 🖼️ Recommended Image
- Diagram showing client-server communication through network layers.

## 🔗 Connections
- [[131-Inter Process Communication IPC]]
- [[135-Deadlock Fundamentals and Coffman Conditions]]
- [[134-Synchronization Solutions and Semaphores]]