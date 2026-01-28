---
Theme: Data Hazards RAW WAR WAW
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Pipeline, Hazards]
---

## 📚 Idea/Concept

Data hazards occur when instructions have dependencies on data produced or consumed by other instructions in the pipeline, potentially causing incorrect execution if not properly handled.

### RAW (Read After Write) - True Dependency

Definition: Instruction j tries to read an operand before instruction i writes it, getting stale value.

Example:
```assembly
1. ADD R3, R2, R1    ; Writes to R3
2. SUB R1, R3, 1     ; Reads R3 (needs result from instruction 1)
```

Characteristics:
- Most common data hazard
- Cannot be eliminated by renaming
- Represents actual data flow

Solutions:
- Stalling: Wait until data available
- Forwarding: Bypass result from EX/MEM or MEM/WB to next instruction's input

### WAR (Write After Read) - Anti-dependency

Definition: Instruction j writes to a destination before instruction i reads it, causing i to read wrong value.

Example:
```assembly
1. ADD R4, R2, R1    ; Reads R1
2. SUB R1, R3, 1     ; Writes to R1 (must wait for instruction 1 to read)
```

Characteristics:
- Only occurs in out-of-order execution
- False dependency (no actual data flow)
- Can be eliminated by register renaming

### WAW (Write After Write) - Output Dependency

Definition: Instruction j writes to an operand before instruction i writes to it, resulting in wrong final value.

Example:
```assembly
1. ADD R1, R2, R3    ; Writes to R1
2. SUB R1, R3, 1     ; Also writes to R1 (must complete after instruction 1)
```

Characteristics:
- Only occurs in out-of-order or superscalar execution
- False dependency
- Can be eliminated by register renaming

### Dependency Summary Table

| Hazard | Dependency Type | Real/False | Renaming Solves? |
|--------|----------------|------------|------------------|
| RAW | True | Real | No |
| WAR | Anti | False | Yes |
| WAW | Output | False | Yes |

### Forwarding Paths for RAW
```
EX/MEM → EX:  Forward result after ALU computation
MEM/WB → EX:  Forward result after memory access
```

Load-Use Hazard (special RAW case):
- Load result not available until end of MEM stage
- Requires one stall cycle even with forwarding

### Register Renaming Example

Original code with WAR and WAW:
```assembly
R1 = M[1024]
R1 = R1 + 2
M[1032] = R1
R1 = M[2048]    ; WAW with line 1, WAR with line 2
R1 = R1 + 4
M[2056] = R1
```

After renaming:
```assembly
R1 = M[1024]
R1 = R1 + 2
M[1032] = R1
R2 = M[2048]    ; Independent, can execute in parallel
R2 = R2 + 4
M[2056] = R2
```

## 📌 Key Points (Optional)
- RAW hazards require forwarding or stalling; they cannot be renamed away.
- Modern out-of-order processors handle WAR/WAW through register renaming automatically.

## 🖼️ Recommended Image
- Pipeline diagram showing forwarding paths between stages.

## 🔗 Connections
- [[066-Pipeline Hazards and Classifications]]
- [[069-Forwarding and Data Bypassing]]
- [[074-Register Renaming and False Dependencies]]
- [[073-Out of Order Execution Fundamentals]]