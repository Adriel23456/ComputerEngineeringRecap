---
Theme: Pipeline Hazards and Classifications
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Pipeline, Hazards]
---

## 📚 Idea/Concept

Pipeline hazards are situations that prevent the next instruction from executing in its designated clock cycle, causing pipeline stalls (bubbles) and reducing performance.

### Three Categories of Hazards

1. Structural Hazards:
   - Cause: Hardware resource conflict
   - Two instructions need same resource simultaneously
   - Example: Single memory port for both fetch and data access

2. Data Hazards:
   - Cause: Data dependency between instructions
   - Instruction needs result not yet produced
   - Types: RAW, WAR, WAW

3. Control Hazards:
   - Cause: Branch/jump instructions
   - Next instruction unknown until branch resolved
   - Affects instruction fetch decisions

### Structural Hazards

Problem: Resource contention
```
Cycle:     1    2    3    4    5
Instr i:   IF   ID   EX   MEM  WB
Instr i+3:           IF   ←conflict if single memory
```

Solutions:
- Stall: Insert bubbles until resource free
- Hardware duplication: Separate instruction and data memories (Harvard-style)

### Data Hazards Overview

| Type | Name | Dependency |
|------|------|------------|
| RAW | Read After Write | True dependency |
| WAR | Write After Read | Anti-dependency |
| WAW | Write After Write | Output dependency |

### Control Hazards

Problem: Branch target unknown during IF of subsequent instructions

Without mitigation: Stall pipeline until branch resolved (1+ cycles)

Solutions:
- Static prediction: Always predict taken/not-taken
- Dynamic prediction: History-based prediction
- Delayed branch: Execute instructions after branch regardless
- Pipeline flush: Discard wrong-path instructions

### General Mitigation Strategies

| Strategy | Hazard Type | Description |
|----------|-------------|-------------|
| Stalling | All | Insert NOPs/bubbles |
| Forwarding | Data (RAW) | Bypass results directly |
| Reordering | Data | Execute independent instructions first |
| Prediction | Control | Speculate branch outcome |
| Duplication | Structural | Add more hardware resources |

## 📌 Key Points (Optional)
- Hazards are the primary reason pipelines don't achieve ideal speedup.
- Modern processors use sophisticated combinations of all mitigation techniques.

## 🖼️ Recommended Image
- Flowchart showing hazard detection and resolution paths.

## 🔗 Connections
- [[065-Pipeline MIPS Five Stage Design]]
- [[067-Data Hazards RAW WAR WAW]]
- [[068-Control Hazards and Branch Prediction]]
- [[069-Forwarding and Data Bypassing]]