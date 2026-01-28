---
Theme: Tomasulo Algorithm
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ILP, OoOE]
---

## 📚 Idea/Concept

Tomasulo's algorithm, developed for the IBM 360/91 (1967), advances beyond the Scoreboard by introducing register renaming through reservation stations and a Common Data Bus (CDB) for result broadcasting.

### Key Innovations

1. Distributed control (vs. centralized scoreboard)
2. Register renaming via reservation stations
3. Common Data Bus for result forwarding
4. Eliminates WAR and WAW hazards

### Architecture Components

Reservation Stations (RS):
- Buffer instructions waiting for operands
- Each functional unit has associated RS
- Hold operand values or tags

Common Data Bus (CDB):
- Broadcasts results to all waiting RS and register file
- Contains: Tag (RS identifier) + Value

Register Status Table:
- For each register: RS that will produce its value (or empty)

### Reservation Station Fields

| Field | Description |
|-------|-------------|
| Tag | Unique identifier for this RS |
| Op | Operation to perform |
| Vj, Vk | Values of source operands (if available) |
| Qj, Qk | Tags of RS producing Vj, Vk (if not available) |
| Busy | Is this RS occupied? |

### The Three Stages

1. Issue:
   - Get instruction from queue (in order)
   - If RS available: allocate and fill
   - Read available operands from registers
   - For unavailable operands: store producing RS tag
   - Update register status table
   - If no RS available: structural hazard, stall

2. Execute:
   - Monitor CDB for missing operands
   - When all operands ready (Qj = Qk = 0): execute
   - For Load/Store: first compute effective address
   - Loads wait for memory unit; stores wait for value

3. Write Result:
   - Broadcast result on CDB with RS tag
   - All RS and registers watching for this tag capture value
   - Free the reservation station

### Register Renaming in Tomasulo

Implicit renaming through RS tags:

```assembly
ADD F1, F0, F8   ; Issued to RS1
...
SUB F1, F8, F14  ; Issued to RS5, F1 now points to RS5
```

If ADD finishes after SUB:
- ADD writes to CDB with tag RS1
- Register F1 expects RS5, ignores RS1's result
- WAW hazard eliminated!

### WAR Hazard Elimination

```assembly
LD  F1, ...      ; RS1, register file says F1 from RS1
ADD ..., F1, ... ; Captures value or tag RS1
SUB F1, ...      ; RS2, register file now says F1 from RS2
```

The ADD has already captured F1's source (RS1 tag or value).
SUB writing to F1 (through RS2) doesn't affect ADD.
WAR hazard eliminated!

### Tomasulo Example

```
INST                    | Issue | EX    | WB
------------------------|-------|-------|----
LDR R0, [R0, #800]     | 1     | 2-3   | 4
MOV R1, #4             | 2     | 3     | 4
LDR R4, [R0, R1]       | 3     | 5-6   | 7
ADD R0, R1, #0x32      | 4     | 5     | 6
MUL R3, R0, R4         | 5     | 8-11  | 12
```

Note: Fewer stalls than Scoreboard due to register renaming.

### Comparison: Tomasulo vs Scoreboard

| Feature | Scoreboard | Tomasulo |
|---------|------------|----------|
| Control | Centralized | Distributed |
| Register Renaming | No | Yes (implicit) |
| WAR/WAW Hazards | Stall | Eliminated |
| Forwarding | No | Yes (via CDB) |
| Scalability | Limited | Better |
| Hardware Complexity | Lower | Higher |

### Modern Usage

Tomasulo's concepts used in:
- Alpha 21264
- MIPS R10000
- Intel Pentium Pro/II/III/4
- AMD K6/K7/K8
- PowerPC 604

## 📌 Key Points (Optional)
- Tomasulo enables more ILP by eliminating false dependencies.
- The CDB is a broadcast mechanism—all units monitor it simultaneously.

## 🖼️ Recommended Image
- Block diagram showing reservation stations, CDB, and register file interconnection.

## 🔗 Connections
- [[075-Scoreboard Algorithm]]
- [[073-Out of Order Execution Fundamentals]]
- [[074-Register Renaming and False Dependencies]]
- [[077-Reorder Buffer and Precise Exceptions]]
- [[078-Superscalar Processor Architecture]]