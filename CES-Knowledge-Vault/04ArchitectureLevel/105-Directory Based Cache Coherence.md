---
Theme: Directory Based Cache Coherence
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, Coherence]
---

## 📚 Idea/Concept

Directory-based cache coherence maintains coherence information in a distributed or centralized directory structure, using point-to-point messages instead of broadcasts, enabling scalability to hundreds or thousands of processors.

### Why Directories?

Snooping limitations:
- Requires broadcast to all caches
- Bus bandwidth limits scalability
- Every cache must snoop every transaction

Directory advantages:
- Point-to-point messages (no broadcast)
- Scales to large systems (100+ processors)
- Works with any interconnect topology

### Directory Structure

For each memory block, directory stores:
- State: Uncached, Shared, Exclusive/Modified
- Sharers: Which caches have copies

Directory location options:
- Centralized: One directory for all memory
- Distributed: Directory co-located with memory (home node)

### Distributed Directory Organization

```
┌─────────────────────────────────────────────────────┐
│                   Interconnect                       │
├──────────┬──────────┬──────────┬──────────┬────────┤
│  Node 0  │  Node 1  │  Node 2  │  Node 3  │  ...   │
│ ┌──────┐ │ ┌──────┐ │ ┌──────┐ │ ┌──────┐ │        │
│ │ CPU  │ │ │ CPU  │ │ │ CPU  │ │ │ CPU  │ │        │
│ │Cache │ │ │Cache │ │ │Cache │ │ │Cache │ │        │
│ └──────┘ │ └──────┘ │ └──────┘ │ └──────┘ │        │
│ ┌──────┐ │ ┌──────┐ │ ┌──────┐ │ ┌──────┐ │        │
│ │Memory│ │ │Memory│ │ │Memory│ │ │Memory│ │        │
│ │ Dir  │ │ │ Dir  │ │ │ Dir  │ │ │ Dir  │ │        │
│ └──────┘ │ └──────┘ │ └──────┘ │ └──────┘ │        │
└──────────┴──────────┴──────────┴──────────┴────────┘
```

Each node is "home" for portion of address space.

### Directory Entry Format

Bit vector (full):
```
┌───────┬────────────────────────────┐
│ State │ P0 │ P1 │ P2 │ ... │ Pn-1 │
└───────┴────────────────────────────┘
```

One bit per processor indicating if it has a copy.

For N processors: N bits per entry
For 1024 processors, 64B blocks, 1TB memory:
- Entries: 16 billion
- Directory size: 2 TB (huge!)

### Directory Entry Optimizations

Limited pointer:
- Store only K pointers (e.g., K=4)
- If more sharers, use broadcast or overflow

Coarse bit vector:
- One bit per group of processors
- Less precision, smaller directory

Sparse directory:
- Only track cached blocks
- Use cache-like structure for directory

### Directory States

Uncached (U):
- No cache has copy
- Data only in memory

Shared (S):
- One or more caches have read-only copies
- Memory has valid copy

Exclusive/Modified (E/M):
- Exactly one cache has copy
- May be modified
- Memory may be stale

### Protocol Messages

Request messages (to directory):
- Read-Req: Want shared copy
- Write-Req: Want exclusive copy
- Upgrade-Req: Have shared, want exclusive

Response messages (from directory):
- Data-Reply: Here's the data
- Inv-Req: Please invalidate
- Fetch-Req: Send your data

Completion messages:
- Inv-Ack: I've invalidated
- Data-Reply: Here's my data

### Read Miss Protocol

Requesting node P wants to read block X:

Case 1: Uncached at home
```
1. P → Home: Read-Req(X)
2. Home → P: Data-Reply(X)
3. Directory: U → S, add P to sharers
```

Case 2: Shared at home
```
1. P → Home: Read-Req(X)
2. Home → P: Data-Reply(X)
3. Directory: add P to sharers
```

Case 3: Exclusive at owner Q
```
1. P → Home: Read-Req(X)
2. Home → Q: Fetch-Req(X)
3. Q → Home: Data-Reply(X)
4. Home → P: Data-Reply(X)
5. Directory: E → S, add P to sharers
```

### Write Miss Protocol

Requesting node P wants to write block X:

Case: Shared with sharers Q, R
```
1. P → Home: Write-Req(X)
2. Home → Q: Inv-Req(X)
3. Home → R: Inv-Req(X)
4. Q → Home: Inv-Ack
5. R → Home: Inv-Ack
6. Home → P: Data-Reply(X) with exclusive
7. Directory: S → E, sharers = {P}
```

### Interconnect Considerations

Directory protocols work with:
- Meshes, tori
- Trees, fat trees
- Crossbars
- Any point-to-point network

Message routing:
- Source-based routing
- Table-based routing
- Adaptive routing

### Latency Comparison

Snooping (bus):
- Broadcast to all (1 hop)
- Response from memory or cache (1 hop)
- Total: Low latency, high bandwidth

Directory:
- Request to home (variable hops)
- Possible forward to owner (more hops)
- Response to requester (more hops)
- Total: Higher latency, but scalable

### Scalability Analysis

| System Size | Snooping | Directory |
|-------------|----------|-----------|
| 2-8 CPUs | Excellent | Overkill |
| 8-32 CPUs | Strained | Good |
| 32-256 CPUs | Impractical | Good |
| 256+ CPUs | Impossible | Essential |

### Modern Implementations

AMD Infinity Fabric:
- Directory-based coherence
- Scalable across chiplets and sockets

Intel UPI (Ultra Path Interconnect):
- Snoop-based with directory-assist
- Home snoop and source snoop modes

ARM AMBA CHI:
- Coherent Hub Interface
- Directory and snoop filter support

## 📌 Key Points (Optional)
- Directory protocols enable coherence at scale by avoiding broadcasts.
- The trade-off is increased latency and directory storage overhead.

## 🖼️ Recommended Image
- Diagram showing directory-based protocol message flow for read/write operations.

## 🔗 Connections
- [[103-Cache Coherence Problem and Solutions]]
- [[104-Snooping Cache Coherence Protocols]]
- [[106-MSI MESI and MOESI Protocols]]
- [[101-Multiprocessor Types SMP AMP and DSM]]