---
Theme: D Latches and D Flip Flops Memory at the Bit Level
Date created: 2026-01-23 21:00
tags: [LogicLayer, SequentialLogic, Memory]
---

## 📚 Idea/Concept

Memory begins when outputs depend on past inputs.  
The simplest memory element stores 1 bit.

### D latch level sensitive
A D latch is transparent when enabled:
- If enable is active, Q follows D
- If enable is inactive, Q holds its previous value

This is level-sensitive behavior.

### D flip-flop edge triggered
A D flip-flop samples input D on a clock edge:
- on rising edge, Q takes D and holds until next edge (idealized)

### Master slave concept
A classic D flip-flop can be built from two latches:
- master latch samples during one clock phase
- slave latch updates output during opposite phase
This prevents transparency across the whole cycle.

### Registers
A register is many flip-flops in parallel:
- stores multi-bit values (8-bit, 32-bit, etc.)
This is the foundation of:
- CPU registers
- pipeline stages
- counters
- state storage in FSMs

### Reset and control
Many flip-flops include:
- async reset
- sync reset
- enable

Design choices affect safety and timing closure.

## 📌 Key Points (Optional)
- Latches can cause timing complexity if used unintentionally.
- Flip-flops simplify design by enforcing edge-based sampling.

## 🖼️ Recommended Image
![[037-Logic76.png]]

## 🔗 Connections
- [[036-Synchronization Setup Hold and Clocking Basics]]
- [[039-FSM Finite State Machines Moore and Mealy Models]]
- [[038-Registers Counters and Shift Registers Common Sequential Building Blocks]]