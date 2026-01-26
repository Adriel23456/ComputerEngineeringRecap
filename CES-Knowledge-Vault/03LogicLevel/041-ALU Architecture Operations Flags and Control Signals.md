---
Theme: ALU Architecture Operations Flags and Control Signals
Date created: 2026-01-23 21:00
tags: [LogicLayer, ALU, CPUFoundations]
---

## 📚 Idea/Concept

An ALU Arithmetic Logic Unit is the core combinational block that performs:
- arithmetic operations (add, subtract, increment)
- logical operations (AND, OR, XOR, NOT)
- comparisons (less than, equal)
- shifts (in many architectures)

### Inputs and outputs
Typical ALU interface:
- A bus (N bits)
- B bus (N bits)
- control signals (select operation)
- result bus (N bits)
- status flags

### Control signals
ALUControl encodes which operation to perform.
Examples:
- ADD
- SUB
- AND
- OR
- XOR
- SLT set less than

Hardware idea:
- compute multiple candidate results in parallel
- select one via a multiplexer controlled by ALUControl

### Flags status outputs
Common flags:
- Z zero flag result == 0
- N negative flag MSB == 1 for signed
- C carry flag carry out
- V overflow flag signed overflow

These flags feed:
- branch decisions
- condition codes
- CPU control logic

### ALU slice concept
A scalable design uses 1-bit ALU slices chained for N bits:
- each slice handles bit i
- carries chain between slices
This is how many classic datapaths are structured.

## 📌 Key Points (Optional)
- ALU is the datapath muscle; FSM/controller is the brain.
- Flags are essential for conditional branching and comparisons.

## 🖼️ Recommended Image
![[041-Logic81.png]]

## 🔗 Connections
- [[040-Arithmetic Circuits Adders Subtractors and Overflow]]
- [[039-FSM Finite State Machines Moore and Mealy Models]]
- [[033-Combinational Logic Design From Truth Table to Gate Level]]
- [[038-Registers Counters and Shift Registers Common Sequential Building Blocks]]
