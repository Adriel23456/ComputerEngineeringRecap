---
Theme: Snooping Cache Coherence Protocols
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, Coherence]
---

## 📚 Idea/Concept

Snooping protocols maintain cache coherence by having all caches monitor (snoop) a shared bus for memory transactions, taking action when they observe accesses to data they hold.

### Snooping Basics

All caches connected to shared bus:
```
┌───────┐  ┌───────┐  ┌───────┐  ┌───────┐
│ CPU0  │  │ CPU1  │  │ CPU2  │  │ CPU3  │
│ Cache │  │ Cache │  │ Cache │  │ Cache │
└───┬───┘  └───┬───┘  └───┬───┘  └───┬───┘
    │          │          │          │
    ├──────────┴──────────┴──────────┤
    │         Shared Bus             │
    ├────────────────────────────────┤
    │          Main Memory           │
    └────────────────────────────────┘
```

Every cache controller:
1. Monitors all bus transactions
2. Compares addresses to local tags
3. Takes appropriate action based on protocol

### Bus Transactions

Common bus operations:
- BusRd: Read request (want shared copy)
- BusRdX: Read exclusive (want to modify)
- BusUpgr: Upgrade (have shared, want exclusive)
- Flush: Write back dirty data
- FlushOpt: Provide data (intervention)

### Snoop Actions

On observing a bus transaction, cache may:
- Ignore (don't have this block)
- Invalidate local copy
- Provide data (intervention)
- Write back dirty data
- Assert shared signal

### Write-Invalidate Protocol

Most common approach:

On processor write:
1. Obtain exclusive ownership (BusRdX or BusUpgr)
2. Invalidate all other copies
3. Write locally

Other caches on snoop BusRdX:
- Invalidate their copies
- Supply data if dirty (intervention)

Advantage: Subsequent writes are local (no bus traffic)

### Write-Update Protocol

Alternative approach:

On processor write:
1. Broadcast new value on bus
2. All caches with copy update their value

Advantage: Readers always have latest value
Disadvantage: High bus traffic on repeated writes

### MESI Snooping Implementation

State transitions based on:
- Processor requests (PrRd, PrWr)
- Bus snoops (BusRd, BusRdX)

Example: BusRd snoop when in Modified
```
State: Modified
Event: Snoop BusRd from another CPU
Action: 
  - Assert Shared signal
  - Provide data (intervention)
  - Transition to Shared
  - Write back to memory (or not, depending on variant)
```

### Bus Arbitration

Multiple CPUs may want bus simultaneously:
- Arbitration logic grants access
- Priority schemes: Round-robin, priority-based
- Split transactions: Release bus during memory access

### Intervention and Data Forwarding

Without intervention:
```
CPU1 requests X (BusRd)
Memory provides X (slow)
```

With intervention (CPU0 has Modified):
```
CPU1 requests X (BusRd)
CPU0 snoops, has X in Modified
CPU0 provides X directly (fast)
CPU0 writes back to memory
Both have X in Shared
```

Intervention reduces latency and memory traffic.

### Shared Signal

Wire asserted by any cache holding block:
- Determines if block is Exclusive or Shared on read
- Read miss, no shared asserted → Exclusive (E in MESI)
- Read miss, shared asserted → Shared (S in MESI)

### Snooping Protocol Example (MESI)

```
Initial: Memory[X] = 0, all caches Invalid

1. CPU0 Read X:
   - BusRd, no sharers
   - CPU0: Invalid → Exclusive
   - Memory provides data

2. CPU1 Read X:
   - BusRd, CPU0 asserts Shared
   - CPU0: Exclusive → Shared
   - CPU1: Invalid → Shared
   - Memory (or CPU0) provides data

3. CPU0 Write X:
   - BusUpgr (already have data, need exclusive)
   - CPU1: Shared → Invalid
   - CPU0: Shared → Modified

4. CPU2 Read X:
   - BusRd
   - CPU0: Modified → Shared, provides data, writes back
   - CPU2: Invalid → Shared
```

### Snooping Scalability Limits

Bandwidth:
- All transactions on shared bus
- Bus becomes bottleneck with many CPUs

Snoop traffic:
- Every cache must check every transaction
- Power and latency overhead

Practical limit: ~8-32 processors for bus-based snooping

### Snoop Filter / Snoop Directory

Optimization for larger systems:
- Track which caches might have each block
- Only send snoops to relevant caches
- Reduces snoop traffic and power

### Split-Transaction Bus

Non-atomic transactions:
1. Request phase: CPU sends address, releases bus
2. Response phase: Data returned, uses bus again

Allows multiple pending requests:
- Higher bus utilization
- More complex protocol

## 📌 Key Points (Optional)
- Snooping is simple and low-latency for small-scale multiprocessors.
- Bus bandwidth limits scalability; larger systems need directories.

## 🖼️ Recommended Image
- Diagram showing snooping process with bus, caches, and state transitions.

## 🔗 Connections
- [[103-Cache Coherence Problem and Solutions]]
- [[106-MSI MESI and MOESI Protocols]]
- [[105-Directory Based Cache Coherence]]
- [[101-Multiprocessor Types SMP AMP and DSM]]