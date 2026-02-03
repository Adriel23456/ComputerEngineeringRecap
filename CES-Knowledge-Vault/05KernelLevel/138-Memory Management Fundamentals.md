---
Theme: Memory Management Fundamentals
Date created: 2026-01-31 12:00
tags: [KernelLevel, Memory, Management, Abstraction]
---

## 📚 Idea/Concept

Memory management is one of the OS's most critical functions. The ideal memory would be unlimited, cheap, fast as registers, and non-volatile. Reality forces the OS to create abstractions that hide complex memory hierarchies.

### The Memory Manager's Functions

1. Track which regions are in use/free
2. Allocate and deallocate memory regions
3. Protect processes from each other
4. Move data between levels (RAM ↔ disk)
5. Exploit locality (temporal and spatial)

### Without Memory Abstraction

Early systems (no MMU, no virtual memory):
```c
// Program directly addresses physical memory
mov R1, 100    // Read physical address 100
```

Problems:
- No isolation: one process can corrupt another
- Difficult relocation: binaries assume fixed addresses
- No protection: user code can access OS memory

Only acceptable in:
- Simple embedded systems (microwave, thermostat)
- Single-program environments
- Completely trusted code

### The Address Space Abstraction

Key insight: **Address space (what process sees) ≠ Physical memory (actual RAM)**

```
Process View:                  Physical Reality:
┌────────────────────┐        ┌────────────────────┐
│ Address 0x0000     │        │ Process A at 0x5000│
│ My code starts     │   →    │ (translated)       │
│ here               │   MMU  │                    │
├────────────────────┤        ├────────────────────┤
│ Address 0x1000     │        │ Process B at 0x8000│
│ My data            │        │                    │
└────────────────────┘        └────────────────────┘

Each process believes it has memory starting at 0
MMU translates virtual → physical at runtime
```

### Five Requirements of Memory Abstraction

```
┌─────────────────────────────────────────────────────────────┐
│  1. RELOCATION                                              │
│     Process can be placed anywhere in physical memory       │
│     Can be moved during execution (swapping)                │
│     References must remain valid after movement             │
├─────────────────────────────────────────────────────────────┤
│  2. PROTECTION                                              │
│     Process cannot access memory outside its region         │
│     Hardware checks every access against limits             │
│     Violations trigger exceptions                           │
├─────────────────────────────────────────────────────────────┤
│  3. SHARING                                                 │
│     Controlled sharing of memory regions                    │
│     Libraries, IPC buffers, shared data                     │
│     With appropriate permissions                            │
├─────────────────────────────────────────────────────────────┤
│  4. LOGICAL ORGANIZATION                                    │
│     Program sees linear array of bytes                      │
│     OS may organize as segments (code, data, stack, heap)   │
│     Abstraction hides physical organization                 │
├─────────────────────────────────────────────────────────────┤
│  5. PHYSICAL ORGANIZATION                                   │
│     RAM vs disk management                                  │
│     OS decides what stays in RAM                            │
│     Maximizes locality, minimizes I/O                       │
└─────────────────────────────────────────────────────────────┘
```

### Base and Limit Registers

Simple hardware protection mechanism:

```
┌─────────────────────────────────────────────────────────────┐
│                         CPU                                  │
│   ┌──────────────┐    ┌──────────────┐                     │
│   │ Base Register│    │Limit Register│                     │
│   │   = 0x5000   │    │   = 0x3000   │                     │
│   └──────┬───────┘    └──────┬───────┘                     │
│          │                   │                              │
│          └─────────┬─────────┘                              │
│                    ↓                                        │
│   Virtual Addr → [Add Base] → [Check < Limit] → Physical   │
│                                     │                       │
│                              Violation? → Exception         │
└─────────────────────────────────────────────────────────────┘

Process address 0x1000 becomes:
Physical = Base + Virtual = 0x5000 + 0x1000 = 0x6000
Check: 0x1000 < 0x3000 (Limit)? Yes → Access allowed
```

### Key Insight: Disk and Files

Important distinction:
- In disk, data is always managed through **files** (or blocks)
- Not as linear addresses visible to the process
- OS mediates between logical file view and physical storage
- This is why the OS acts as intermediary between program model and storage

### Translation Direction

Critical point: Translation is ALWAYS **Virtual → Physical**
- Never physical → virtual
- MMU performs this translation
- OS sets up the translation tables

## 📌 Key Points (Optional)
- Address space abstraction isolates processes and enables relocation.
- The MMU translates virtual addresses to physical addresses at runtime.
- Five requirements: relocation, protection, sharing, logical org, physical org.

## 🖼️ Recommended Image
- Diagram showing virtual address spaces mapping to physical memory through MMU.

## 🔗 Connections
- [[139-Memory Partitioning Schemes]]
- [[140-Virtual Memory and Paging]]
- [[127-Process Concept and Program vs Process]]