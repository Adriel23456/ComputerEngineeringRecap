---
Theme: Virtual Memory and Paging
Date created: 2026-01-31 12:00
tags: [KernelLevel, Memory, VirtualMemory, Paging]
---

## 📚 Idea/Concept

Virtual memory with paging divides address spaces into fixed-size pages, eliminating external fragmentation and enabling efficient memory management. It's the foundation of modern memory systems.

### From Swapping to Paging

Classic swapping: Move entire processes between RAM and disk
- Problem: Swapping gigabytes is extremely expensive
- Solution: Move only needed **pages**, not entire processes

### Paging Fundamentals

```
Virtual Address Space:           Physical Memory (RAM):
┌─────────────────────┐         ┌─────────────────────┐
│ Page 0              │         │ Frame 0             │
├─────────────────────┤         ├─────────────────────┤
│ Page 1              │    →    │ Frame 1             │
├─────────────────────┤   MMU   ├─────────────────────┤
│ Page 2              │    →    │ Frame 2             │
├─────────────────────┤         ├─────────────────────┤
│ Page 3              │         │ Frame 3             │
├─────────────────────┤         ├─────────────────────┤
│ ...                 │         │ ...                 │
└─────────────────────┘         └─────────────────────┘

CRITICAL: Page size = Frame size (MANDATORY)
Typical sizes: 4KB, 2MB (huge pages), 1GB (giant pages)
```

### Key Paging Properties

1. **Only needed pages in RAM**: Not entire program
2. **No external fragmentation**: All chunks same size
3. **Small internal fragmentation**: At most one page per process (last page)
4. **One page table per process**: Maps virtual pages to physical frames

### Virtual Address Structure

```
Example: 32-bit address, 4KB pages (2^12 = 4096 bytes)

Virtual Address (32 bits):
┌────────────────────────────┬────────────────┐
│      Page Number (20 bits) │  Offset (12 bits)│
└────────────────────────────┴────────────────┘

- Page Number: Which page (indexes into page table)
- Offset: Position within the page (0 to 4095)
- 12 bits for offset because 2^12 = 4096 = page size
```

### Address Translation

```
Virtual Address: 0x00005A3C

Page size: 4KB (0x1000)
Page Number = 0x00005A3C / 0x1000 = 0x5 (page 5)
Offset = 0x00005A3C % 0x1000 = 0xA3C

Page Table lookup: Page 5 → Frame 7

Physical Address = Frame 7 * 4KB + Offset
                 = 0x7000 + 0xA3C
                 = 0x7A3C
```

### Page Table Entry (PTE)

Each entry contains:
```
┌─────────────────────────────────────────────────────────────┐
│                    Page Table Entry                          │
├─────────────────────────────────────────────────────────────┤
│  Present (P)     │ 1 = in RAM, 0 = on disk (page fault)    │
├──────────────────┼──────────────────────────────────────────┤
│  Referenced (R)  │ 1 = page was accessed recently           │
├──────────────────┼──────────────────────────────────────────┤
│  Modified (M)    │ 1 = page was written (dirty)             │
├──────────────────┼──────────────────────────────────────────┤
│  Protection      │ Read/Write/Execute permissions           │
├──────────────────┼──────────────────────────────────────────┤
│  Frame Number    │ Physical frame if present                │
└──────────────────┴──────────────────────────────────────────┘
```

### TLB - Translation Lookaside Buffer

Page table is in RAM → Every memory access needs TWO accesses (table + data)
Solution: Cache recent translations in TLB

```
┌─────────────────────────────────────────────────────────────┐
│                         TLB                                  │
│   (Fast associative cache in MMU)                           │
├───────────────────┬───────────────────┬────────────────────┤
│   Virtual Page    │   Physical Frame  │   Permissions      │
├───────────────────┼───────────────────┼────────────────────┤
│        5          │         7         │      RW            │
├───────────────────┼───────────────────┼────────────────────┤
│       12          │         3         │      RX            │
├───────────────────┼───────────────────┼────────────────────┤
│       23          │        15         │      RW            │
└───────────────────┴───────────────────┴────────────────────┘

TLB Hit: Translate in ~1 cycle
TLB Miss: Access page table in RAM (~100 cycles), update TLB
```

### Page Fault Handling

When accessing page not in RAM (Present bit = 0):

```
1. MMU detects Present = 0, raises page fault
2. CPU traps to OS page fault handler
3. OS finds page on disk (swap space)
4. OS selects victim frame (if memory full)
5. If victim dirty, write to disk
6. Load requested page into frame
7. Update page table (Present = 1, frame number)
8. Restart faulting instruction
```

### Why Paging Eliminates External Fragmentation

```
Without paging:                With paging:
┌──────────────────┐          ┌──────┬──────┬──────┐
│ Process A        │          │ A.p0 │ B.p0 │ A.p1 │
├──────────────────┤          ├──────┼──────┼──────┤
│     Hole         │ ← Can't  │ C.p0 │ A.p2 │ B.p1 │
├──────────────────┤   use!   ├──────┼──────┼──────┤
│ Process B        │          │ B.p2 │ C.p1 │ Free │
├──────────────────┤          └──────┴──────┴──────┘
│     Hole         │ ← Wasted 
└──────────────────┘          Pages can go ANYWHERE!
                              No contiguity required
```

## 📌 Key Points (Optional)
- Paging eliminates external fragmentation completely.
- Page size must equal frame size—this is mandatory.
- TLB caches translations to avoid double memory access penalty.
- R and M bits enable smart page replacement decisions.

## 🖼️ Recommended Image
- Address translation diagram showing virtual address → page table → physical address.

## 🔗 Connections
- [[138-Memory Management Fundamentals]]
- [[141-Page Replacement Algorithms]]
- [[139-Memory Partitioning Schemes]]
- [[095-Memory Management Unit MMU]]
- [[096-Translation Lookaside Buffer TLB]]
