---
Theme: Hazard Detection Unit
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Pipeline, ControlUnit]
---

## 📚 Idea/Concept

The Hazard Detection Unit is a hardware component that identifies pipeline hazards and generates control signals to stall the pipeline or enable forwarding when necessary.

### Functions

1. Detect data hazards requiring stalls
2. Detect structural hazards
3. Control pipeline register updates
4. Insert bubbles (NOPs) when needed
5. Coordinate with forwarding unit

### Load-Use Hazard Detection

Condition to detect:
```
ID/EX.MemRead = 1 AND
(ID/EX.Rd = IF/ID.Rs1 OR ID/EX.Rd = IF/ID.Rs2)
```

When detected:
- Stall IF and ID stages (prevent update)
- Insert bubble in EX stage
- Allow load to complete MEM stage

### Stall Mechanism

Pipeline Control Signals:
- PCWrite: Enable/disable PC update
- IF/IDWrite: Enable/disable IF/ID register update
- Control MUX: Select real controls or zeros (bubble)

When stalling:
```
PCWrite = 0         ; Freeze PC
IF/IDWrite = 0      ; Freeze IF/ID register
Insert NOP in ID/EX ; Bubble propagates through pipeline
```

### Interaction with Forwarding Unit

```
┌─────────────────┐     ┌─────────────────┐
│  Hazard         │     │  Forwarding     │
│  Detection      │────▶│  Unit           │
│  Unit           │     │                 │
└────────┬────────┘     └────────┬────────┘
         │                       │
         ▼                       ▼
    Stall signals          Forward select
    (PCWrite,              (ALU MUX
    IF/IDWrite)            controls)
```

### Branch Hazard Detection

For control hazards:
- Detect branch instruction in ID
- Check if branch condition depends on prior instruction
- Stall if comparison operands not yet available

### Structural Hazard Detection

Memory access conflict:
- IF needs instruction memory
- MEM needs data memory
- If single-port memory: detect and stall

### Complete Hazard Detection Logic

```
LoadUseHazard = ID/EX.MemRead AND
                ((ID/EX.Rd = IF/ID.Rs1) OR (ID/EX.Rd = IF/ID.Rs2))

BranchHazard = (IF/ID.Branch OR IF/ID.Jump) AND
               ((ID/EX.Rd = IF/ID.Rs1) OR (ID/EX.Rd = IF/ID.Rs2) OR
                (EX/MEM.Rd = IF/ID.Rs1) OR (EX/MEM.Rd = IF/ID.Rs2))

Stall = LoadUseHazard OR BranchHazard OR StructuralHazard
```

## 📌 Key Points (Optional)
- The hazard detection unit works in the ID stage to detect hazards early.
- Proper hazard detection is critical for correct pipeline execution.

## 🖼️ Recommended Image
- Block diagram showing hazard detection unit connections to pipeline registers and control signals.

## 🔗 Connections
- [[069-Forwarding and Data Bypassing]]
- [[066-Pipeline Hazards and Classifications]]
- [[048-Control Unit Architecture]]
- [[065-Pipeline MIPS Five Stage Design]]