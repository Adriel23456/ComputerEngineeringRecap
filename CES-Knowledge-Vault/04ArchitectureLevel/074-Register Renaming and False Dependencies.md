---
Theme: Register Renaming and False Dependencies
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ILP, OoOE]
---

## 📚 Idea/Concept

Register renaming is a technique that eliminates false dependencies (WAR and WAW) by mapping architectural registers to a larger set of physical registers, enabling more instruction-level parallelism.

### The Problem: False Dependencies

```assembly
1. R1 = M[1024]      ; Load into R1
2. R1 = R1 + 2       ; RAW on R1 (true)
3. M[1032] = R1      ; RAW on R1 (true)
4. R1 = M[2048]      ; WAW with line 1! WAR with line 3!
5. R1 = R1 + 4       ; RAW on R1
6. M[2056] = R1      ; RAW on R1
```

Instructions 4-6 are independent of 1-3 but cannot execute in parallel due to R1 reuse.

### After Renaming

```assembly
1. P1 = M[1024]      ; R1 → P1
2. P2 = P1 + 2       ; R1 → P2
3. M[1032] = P2
4. P3 = M[2048]      ; R1 → P3 (different physical register!)
5. P4 = P3 + 4       ; R1 → P4
6. M[2056] = P4
```

Now instructions 1-3 and 4-6 can execute in parallel!

### Implementation Components

Register Alias Table (RAT):
- Maps architectural registers to physical registers
- Updated on every instruction that writes a register
- Consulted for every register read

Free List:
- Pool of available physical registers
- Allocates register on instruction issue
- Returns register when no longer needed

### Renaming Process

For instruction `R3 = R1 + R2`:

1. Lookup R1 in RAT → Get physical register P_a
2. Lookup R2 in RAT → Get physical register P_b
3. Allocate new physical register P_c from free list
4. Update RAT: R3 → P_c
5. Issue: P_c = P_a + P_b

### Physical Register Management

When can physical register be freed?
- When a newer mapping exists AND
- All instructions using old mapping have completed AND
- Instruction creating old mapping has committed

### Renaming Table Example

| Architectural | Physical | Valid |
|--------------|----------|-------|
| R0 | P0 | 1 |
| R1 | P5 | 1 |
| R2 | P12 | 1 |
| R3 | P7 | 1 |
| ... | ... | ... |

After `R1 = R2 + R3`:
| Architectural | Physical | Valid |
|--------------|----------|-------|
| R1 | P18 | 1 |  ← New mapping

### Types of Renaming

Explicit Renaming:
- Separate physical register file
- RAT maintained explicitly
- Used in most modern designs

Implicit Renaming (Tomasulo-style):
- Reservation stations hold values
- Tags identify producers
- Values broadcast on common data bus

### Benefits

1. Eliminates WAR hazards: Reads use old physical register
2. Eliminates WAW hazards: Each write gets new physical register
3. Enables more OoO execution: Independent instructions can proceed
4. Simplifies hazard detection: Only RAW remains

### Costs

- Large physical register file (typically 2-3× architectural)
- RAT hardware and lookup logic
- Free list management
- Complexity in exception handling

## 📌 Key Points (Optional)
- Register renaming is essential for aggressive out-of-order execution.
- The number of physical registers limits the instruction window size.

## 🖼️ Recommended Image
- Diagram showing RAT, physical register file, and free list interactions.

## 🔗 Connections
- [[073-Out of Order Execution Fundamentals]]
- [[067-Data Hazards RAW WAR WAW]]
- [[076-Tomasulo Algorithm]]
- [[077-Reorder Buffer and Precise Exceptions]]