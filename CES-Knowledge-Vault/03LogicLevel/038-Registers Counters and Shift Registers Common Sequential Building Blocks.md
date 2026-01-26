---
Theme: Registers Counters and Shift Registers Common Sequential Building Blocks
Date created: 2026-01-23 21:00
tags: [LogicLayer, SequentialLogic, BuildingBlocks]
---

## 📚 Idea/Concept

Once you have flip-flops, you can build many useful sequential blocks.

### Register
- Stores N bits
- Usually has clock, reset, and maybe enable

### Counter
A counter increments through states each clock:
- up counter
- down counter
- modulo-N counter
Used for:
- timers
- frequency division
- address generation

### Shift register
Moves bits each clock:
- serial in serial out
- serial in parallel out
- parallel in serial out
Used for:
- data serialization
- simple buffers
- delay lines
- IO expansion

### Key design concept
All these are made from:
- combinational logic + flip-flops
The combinational logic computes the next state.

## 📌 Key Points (Optional)
- These blocks appear everywhere in CPUs and embedded systems.
- A surprising amount of “hardware behavior” is counters + FSM.

## 🖼️ Recommended Image
![[038-Logic77.png]]
![[038-Logic78.png]]

## 🔗 Connections
- [[037-D Latches and D Flip Flops Memory at the Bit Level]]
- [[039-FSM Finite State Machines Moore and Mealy Models]]
- [[040-Arithmetic Circuits Adders Subtractors and Overflow]]
