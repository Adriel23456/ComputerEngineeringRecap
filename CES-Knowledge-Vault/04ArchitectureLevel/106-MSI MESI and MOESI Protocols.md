---
Theme: MSI MESI and MOESI Protocols
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, Coherence]
---

## 📚 Idea/Concept

MSI, MESI, and MOESI are snooping-based cache coherence protocols that define cache line states and transitions to maintain consistency across multiple caches in shared-memory multiprocessors.

### MSI Protocol (Basic)

Three States:
- Modified (M): Exclusive ownership, dirty data
- Shared (S): Valid copy, others may have copies
- Invalid (I): Not valid

State Characteristics:
| State | Valid | Exclusive | Dirty |
|-------|-------|-----------|-------|
| M | Yes | Yes | Yes |
| S | Yes | No | No |
| I | No | - | - |

MSI State Transitions:

From Invalid:
- Processor Read → S (BusRd, get data)
- Processor Write → M (BusRdX, get exclusive)

From Shared:
- Processor Read → S (hit)
- Processor Write → M (BusRdX, invalidate others)
- Snoop BusRdX → I (another writing)

From Modified:
- Processor Read → M (hit)
- Processor Write → M (hit)
- Snoop BusRd → S (supply data, downgrade)
- Snoop BusRdX → I (supply data, invalidate)

MSI Limitation:
- Read after write by single processor causes bus transaction
- Exclusive-but-clean state missing

### MESI Protocol (Enhanced)

Four States:
- Modified (M): Exclusive, dirty
- Exclusive (E): Exclusive, clean (only copy, matches memory)
- Shared (S): Shared, clean
- Invalid (I): Not valid

State Characteristics:
| State | Valid | Exclusive | Dirty |
|-------|-------|-----------|-------|
| M | Yes | Yes | Yes |
| E | Yes | Yes | No |
| S | Yes | No | No |
| I | No | - | - |

MESI Advantage:
- Silent transition E → M on write (no bus transaction)
- Reduces traffic for read-then-write patterns

MESI State Transitions:

Read Miss (no sharers):
- I → E (exclusive, clean)

Read Miss (sharers exist):
- I → S (shared)

Write Hit in E:
- E → M (silent, no bus traffic!)

Write Hit in S:
- S → M (BusRdX to invalidate others)

### MOESI Protocol (Extended)

Five States:
- Modified (M): Exclusive, dirty
- Owned (O): Shared, dirty (responsible for data)
- Exclusive (E): Exclusive, clean
- Shared (S): Shared, clean
- Invalid (I): Not valid

State Characteristics:
| State | Valid | Exclusive | Dirty | Owner |
|-------|-------|-----------|-------|-------|
| M | Yes | Yes | Yes | Yes |
| O | Yes | No | Yes | Yes |
| E | Yes | Yes | No | No |
| S | Yes | No | No | No |
| I | No | - | - | No |

MOESI Advantage:
- Owned state allows sharing dirty data without write-back
- M → O transition when another reads (share without memory write)
- Reduces memory traffic

MOESI Scenario:
```
1. CPU0 has X in M (dirty)
2. CPU1 reads X
3. CPU0 transitions M → O, supplies data to CPU1
4. CPU1 gets X in S state
5. Memory NOT updated (O holds dirty copy)
6. Later: O responsible for write-back on eviction
```

### Protocol Comparison

| Protocol | States | Bus Traffic | Complexity |
|----------|--------|-------------|------------|
| MSI | 3 | Highest | Lowest |
| MESI | 4 | Medium | Medium |
| MOESI | 5 | Lowest | Highest |

### State Transition Diagram (MESI)

```
        ┌─────────────────────────────────────┐
        │               Invalid               │
        └──────┬─────────────────┬────────────┘
    PrRd/      │                 │      PrWr/
    BusRd(S)   │                 │      BusRdX
        ▼      │                 ▼
    ┌──────────┴──┐         ┌─────────────┐
    │   Shared    │◄────────│  Modified   │
    └──────┬──────┘  PrRd   └──────┬──────┘
           │          from         │
    PrWr/  │          memory       │PrWr
    BusRdX │                       │(silent)
           ▼                       │
    ┌─────────────────────────────┴┐
    │         Exclusive            │
    └──────────────────────────────┘
```

### Implementation Bits

Each cache line includes state bits:
| Protocol | Bits | States |
|----------|------|--------|
| MSI | 2 | 3 (+1 unused) |
| MESI | 2 | 4 |
| MOESI | 3 | 5 (+3 unused) |

### Real-World Usage

- Intel: MESI variants (MESIF with Forward state)
- AMD: MOESI
- ARM: MESI/MOESI variants (depending on implementation)

### Coherence Actions Summary

| Event | MSI | MESI | MOESI |
|-------|-----|------|-------|
| Read miss, no sharers | S | E | E |
| Read miss, sharers | S | S | S/O |
| Write hit (clean exclusive) | - | M (silent) | M (silent) |
| Write hit (shared) | M (inv) | M (inv) | M (inv) |
| Snoop read on M | S + WB | S + WB | O (no WB) |

## 📌 Key Points (Optional)
- MESI's Exclusive state reduces write traffic for private data.
- MOESI's Owned state reduces memory traffic for shared dirty data.

## 🖼️ Recommended Image
- State transition diagram for MESI protocol with all transitions labeled.

## 🔗 Connections
- [[103-Cache Coherence Problem and Solutions]]
- [[104-Snooping Cache Coherence Protocols]]
- [[091-Cache Write Policies]]
- [[101-Multiprocessor Types SMP AMP and DSM]]