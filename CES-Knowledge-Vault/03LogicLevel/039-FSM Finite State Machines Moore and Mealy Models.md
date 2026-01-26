---
Theme: FSM Finite State Machines Moore and Mealy Models
Date created: 2026-01-23 21:00
tags: [LogicLayer, FSM, SequentialLogic]
---

## 📚 Idea/Concept

An FSM is a formal model for sequential logic:
- system has a finite set of states
- transitions depend on inputs
- outputs depend on state and possibly inputs

### Core parts of an FSM
1. State register (flip-flops)
2. Next-state combinational logic
3. Output logic

### Moore machine
- outputs depend only on current state
Pros:
- outputs are stable between transitions
- easier to avoid glitches
Cons:
- may need more states

### Mealy machine
- outputs depend on state and inputs
Pros:
- can respond faster to inputs
- fewer states sometimes
Cons:
- outputs can glitch if inputs change asynchronously

### Typical FSM design workflow
1. Define states in plain language
2. Draw state diagram
3. Make state transition table
4. Encode states in bits
5. Derive next-state equations
6. Implement with flip-flops + combinational logic
7. Verify with simulation and timing checks

### Why FSMs matter in CSE
FSMs are the core of:
- CPU control units
- communication protocols
- UI controllers
- embedded device behavior
- pipeline controllers and hazard logic

## 📌 Key Points (Optional)
- Moore is safer for clean outputs.
- Mealy is faster but needs careful synchronization.

## 🖼️ Recommended Image
![[039-Logic79.png]]

## 🔗 Connections
- [[036-Synchronization Setup Hold and Clocking Basics]]
- [[037-D Latches and D Flip Flops Memory at the Bit Level]]
- [[033-Combinational Logic Design From Truth Table to Gate Level]]
- [[041-ALU Architecture Operations Flags and Control Signals]]
