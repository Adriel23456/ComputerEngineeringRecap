---
Theme: Memory Management Unit MMU
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, MemoryManagement, Hardware]
---

## 📚 Idea/Concept

The Memory Management Unit (MMU) is hardware that performs virtual-to-physical address translation, memory protection, and access control for every memory reference made by the processor.

### Core Functions

1. Address Translation:
   - Convert virtual addresses to physical addresses
   - Use TLB and page tables
   - Support multiple page sizes

2. Memory Protection:
   - Enforce read/write/execute permissions
   - Prevent unauthorized access
   - Isolate process address spaces

3. Cache Control:
   - Determine cacheability of memory regions
   - Control cache policies per page

### MMU Location in System

```
┌─────────────────────────────────────────┐
│                  CPU                     │
│  ┌──────────┐    ┌─────────────────┐    │
│  │  Core    │───▶│      MMU        │    │
│  │(Virtual) │    │ ┌─────┐ ┌─────┐ │    │
│  └──────────┘    │ │ TLB │ │PTW  │ │    │
│                  │ └─────┘ └─────┘ │    │
│                  └────────┬────────┘    │
│                           │(Physical)   │
│                  ┌────────┴────────┐    │
│                  │     L1 Cache    │    │
│                  └─────────────────┘    │
└─────────────────────────────────────────┘
                    │
              Physical Bus
                    │
            ┌───────┴───────┐
            │  Main Memory  │
            └───────────────┘
```

### Translation Process

1. CPU issues virtual address
2. MMU checks TLB:
   - Hit: Return physical address immediately
   - Miss: Invoke page table walker
3. Page Table Walker (PTW):
   - Hardware state machine
   - Walks multi-level page table in memory
   - Returns PTE to TLB
4. Check permissions and valid bit:
   - Valid and permitted: Complete translation
   - Invalid: Raise page fault
   - Protection violation: Raise exception

### Page Table Walker (PTW)

Hardware component that traverses page table hierarchy:

For x86-64 (4-level):
```
CR3 (Page Table Base)
    └─▶ PML4 Entry [bits 47:39]
         └─▶ PDPT Entry [bits 38:30]
              └─▶ PD Entry [bits 29:21]
                   └─▶ PT Entry [bits 20:12]
                        └─▶ Physical Frame + Offset [bits 11:0]
```

PTW optimizations:
- Page table entry caches (intermediate levels)
- Parallel lookups for multiple levels
- Speculative walks

### Protection Mechanisms

Permission Bits (per page):
- Read (R): Data can be read
- Write (W): Data can be modified
- Execute (X): Code can be executed
- User/Supervisor (U/S): Accessible from user mode?

Protection Checks:
```
if (access_type NOT IN page_permissions):
    raise Protection_Fault
if (user_mode AND NOT page.user_accessible):
    raise Protection_Fault
```

### Special MMU Features

No-Execute (NX/XD):
- Prevents code execution from data pages
- Defends against buffer overflow attacks

Supervisor Mode Execute Prevention (SMEP):
- Kernel can't execute user-space code
- Prevents privilege escalation attacks

Supervisor Mode Access Prevention (SMAP):
- Kernel can't read/write user-space data (unless explicit)
- Prevents data leakage

### MMU Registers

Key control registers (x86-64):
- CR0: Paging enable bit
- CR3: Page table base address (+ PCID)
- CR4: Control features (PAE, PSE, SMEP, SMAP)

ARM equivalents:
- TTBR0/TTBR1: Translation Table Base Registers
- SCTLR: System Control Register
- TCR: Translation Control Register

### Cacheability Attributes

MMU controls memory type per page:
- Write-Back (WB): Normal cacheable
- Write-Through (WT): Cache with immediate write
- Write-Combining (WC): Buffer writes, no cache
- Uncacheable (UC): Direct to memory (I/O devices)

### IOMMU (I/O Memory Management Unit)

Separate MMU for DMA devices:
- Translates device addresses to physical
- Protects system from malicious devices
- Enables virtualization of I/O

Intel: VT-d (Virtualization Technology for Directed I/O)
AMD: AMD-Vi

### MMU and Virtualization

Nested/Extended Page Tables:
- Guest virtual → Guest physical (guest MMU)
- Guest physical → Host physical (hypervisor EPT/NPT)

Hardware support:
- Intel EPT (Extended Page Tables)
- AMD NPT (Nested Page Tables)

### MMU Miss Handling Timeline

```
Cycle 0:    VA issued by CPU
Cycle 1:    TLB lookup (parallel with cache index)
Cycle 2:    TLB hit → PA available, cache tag compare
            TLB miss → PTW starts
Cycle 3-10: PTW walks page table levels
Cycle 11:   PTE returned, TLB updated
Cycle 12:   Retry access with translation
```

## 📌 Key Points (Optional)
- The MMU is essential for memory protection and virtual memory operation.
- Hardware page table walkers minimize TLB miss latency.

## 🖼️ Recommended Image
- Block diagram showing MMU components: TLB, PTW, and protection logic.

## 🔗 Connections
- [[094-Virtual Memory and Paging]]
- [[096-Translation Lookaside Buffer TLB]]
- [[086-Memory Hierarchy Principles and Locality]]
- [[087-Cache Memory Organization and Parameters]]