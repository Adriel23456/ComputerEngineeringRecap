---
Theme: Translation Lookaside Buffer TLB
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, MemoryManagement, VirtualMemory]
---

## 📚 Idea/Concept

The Translation Lookaside Buffer (TLB) is a specialized cache that stores recent virtual-to-physical address translations, eliminating the need to access page tables in memory for most address translations.

### Problem Without TLB

Every memory access requires address translation:
1. Access page table in memory (1+ memory accesses)
2. Access actual data in memory (1 memory access)

Result: 2x or more memory accesses per instruction!

With 4-level page table: 5 memory accesses per data access.

### TLB Solution

Cache frequently used page table entries:
1. Check TLB for translation (very fast)
2. If TLB hit: Use cached translation
3. If TLB miss: Walk page table, cache result

### TLB Organization

Typically fully associative or set-associative:
- Small size: 32-1024 entries
- Fast access: Same cycle as cache (or faster)
- Separate I-TLB and D-TLB common

TLB Entry Contents:
```
┌─────────────────────────────────────────────────────────┐
│ VPN (Tag) │ PFN │ Valid │ ASID │ G │ D │ R │ W │ X │
└─────────────────────────────────────────────────────────┘
```

- VPN: Virtual Page Number (tag for lookup)
- PFN: Physical Frame Number (translation result)
- Valid: Entry valid?
- ASID: Address Space ID (process identifier)
- G: Global (shared across processes)
- D: Dirty (page modified)
- R/W/X: Permission bits

### TLB Reach

TLB Reach = TLB Entries × Page Size

Example:
- 64 entries × 4 KB pages = 256 KB reach
- 64 entries × 2 MB pages = 128 MB reach (large pages)

Increasing reach:
- More TLB entries (expensive)
- Larger pages (internal fragmentation)
- Multiple page size support

### TLB Miss Handling

Hardware-managed TLB (x86, ARM):
1. Hardware page table walker
2. Automatically loads TLB entry
3. Transparent to software
4. Page fault only if page not in memory

Software-managed TLB (MIPS, SPARC):
1. TLB miss causes exception
2. OS handler walks page table
3. OS loads TLB entry
4. More flexible but higher overhead

### TLB and Context Switches

Problem: TLB entries belong to current process

Solutions:

1. Flush TLB on context switch:
   - Simple but expensive
   - All entries invalidated
   - Cold start for new process

2. Address Space Identifiers (ASIDs):
   - Each entry tagged with process ID
   - No flush needed on switch
   - Limited ASID bits (8-16 typically)

3. Global bit:
   - Entries marked global shared by all
   - Kernel mappings, shared libraries

### TLB Hierarchy

Modern processors may have multiple TLB levels:

| Level | Type | Size | Associativity | Latency |
|-------|------|------|---------------|---------|
| L1 I-TLB | Instructions | 32-64 | Fully | 1 cycle |
| L1 D-TLB | Data | 32-64 | Fully | 1 cycle |
| L2 TLB | Unified | 512-4096 | 4-8 way | 6-10 cycles |

### TLB and Cache Interaction

Three approaches:

1. Physically Indexed, Physically Tagged (PIPT):
   - TLB lookup before cache access
   - Simple but adds latency

2. Virtually Indexed, Virtually Tagged (VIVT):
   - No TLB for cache access
   - Aliasing problems, flush on context switch

3. Virtually Indexed, Physically Tagged (VIPT):
   - Index cache with VA while doing TLB lookup
   - Tag comparison uses PA
   - Best of both (if cache ≤ page size × associativity)

### Example: AMD Opteron TLB Organization

```
L1 I-TLB: 32 entries, fully associative, 4KB pages
L1 D-TLB: 32 entries, fully associative, 4KB pages
L2 TLB:   512 entries, 4-way, 4KB pages

Large Page TLBs:
L1 I-TLB: 8 entries for 2MB pages
L1 D-TLB: 8 entries for 2MB pages
```

### TLB Performance Impact

Hit rate typically >99% due to locality

TLB miss cost:
- Hardware walk: 10-100 cycles
- Software walk: 100-1000 cycles
- Page fault (disk): Millions of cycles

Effective access time:
```
EAT = TLB_hit_rate × (TLB_time + Memory_time) +
      TLB_miss_rate × (TLB_time + Page_table_walk + Memory_time)
```

### TLB Shootdown

In multiprocessors, when page mapping changes:
1. Update page table
2. Invalidate TLB entry on all CPUs that might have it
3. Inter-processor interrupt (IPI) to other CPUs
4. Each CPU invalidates local TLB entry
5. Wait for acknowledgment

Expensive operation—minimized by batching.

## 📌 Key Points (Optional)
- TLB transforms virtual memory from impractical to efficient.
- TLB reach and hit rate are critical performance factors.

## 🖼️ Recommended Image
- Diagram showing TLB lookup parallel with cache index, followed by tag comparison.

## 🔗 Connections
- [[094-Virtual Memory and Paging]]
- [[095-Memory Management Unit MMU]]
- [[087-Cache Memory Organization and Parameters]]
- [[090-Average Memory Access Time AMAT]]