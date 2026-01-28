---
Theme: Forwarding and Data Bypassing
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Pipeline, Hazards]
---

## 📚 Idea/Concept

Forwarding (also called data bypassing) is a hardware technique that reduces pipeline stalls by routing computed results directly to dependent instructions before they are written to the register file.

### The Problem Without Forwarding

```assembly
ADD R1, R2, R3    ; Result available after EX (cycle 3)
SUB R4, R1, R5    ; Needs R1 in EX (cycle 4)
```

Without forwarding: SUB must wait until ADD completes WB (cycle 5)
Penalty: 2 stall cycles

### Forwarding Concept

Instead of waiting for register file write:
- Capture result at pipeline register outputs
- Route directly to ALU inputs of dependent instruction
- Bypass the register file read

### Forwarding Paths in 5-Stage Pipeline

```
EX/MEM → EX:   Forward ALU result to next instruction's ALU input
MEM/WB → EX:   Forward memory load or prior ALU result to ALU input
MEM/WB → MEM:  Forward data for store after load (store forwarding)
```

### Hardware Implementation

Forwarding Unit detects:
1. Source register of instruction in EX matches destination of instruction in EX/MEM
2. Source register of instruction in EX matches destination of instruction in MEM/WB
3. Priority: EX/MEM has precedence over MEM/WB (most recent value)

Multiplexers at ALU inputs:
- Select between register file output and forwarded values
- Controlled by forwarding unit

### Forwarding Conditions

Forward from EX/MEM if:
```
EX/MEM.RegWrite = 1 AND
EX/MEM.Rd ≠ 0 AND
EX/MEM.Rd = ID/EX.Rs1 (or Rs2)
```

Forward from MEM/WB if:
```
MEM/WB.RegWrite = 1 AND
MEM/WB.Rd ≠ 0 AND
NOT (forwarding from EX/MEM) AND
MEM/WB.Rd = ID/EX.Rs1 (or Rs2)
```

### Load-Use Hazard (Forwarding Limitation)

```assembly
LW  R1, 0(R2)     ; Data available after MEM (cycle 4)
ADD R3, R1, R4    ; Needs R1 in EX (cycle 3) - TOO EARLY!
```

Problem: Load data not available until end of MEM stage
Solution: One mandatory stall cycle, then forward from MEM/WB

### Stall + Forward for Load-Use

```
Cycle:    1    2    3    4    5    6    7
LW:       IF   ID   EX   MEM  WB
ADD:           IF   ID   --   EX   MEM  WB
                        stall  ↑
                              forward from MEM/WB
```

### Performance Impact

| Scenario | Stall Cycles |
|----------|-------------|
| No forwarding (RAW) | 2 cycles |
| With forwarding (RAW) | 0 cycles |
| Load-use with forwarding | 1 cycle |

## 📌 Key Points (Optional)
- Forwarding eliminates most RAW stalls but cannot help load-use hazards completely.
- Hardware cost: Additional multiplexers and comparators, but significant performance gain.

## 🖼️ Recommended Image
- Datapath diagram showing forwarding paths from EX/MEM and MEM/WB to ALU inputs.

## 🔗 Connections
- [[067-Data Hazards RAW WAR WAW]]
- [[066-Pipeline Hazards and Classifications]]
- [[065-Pipeline MIPS Five Stage Design]]
- [[070-Hazard Detection Unit]]