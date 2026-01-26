---
Theme: Arithmetic Circuits Adders Subtractors and Overflow
Date created: 2026-01-23 21:00
tags: [LogicLayer, ArithmeticLogic, CombinationalLogic]
---

## 📚 Idea/Concept

Arithmetic circuits are combinational blocks that perform numeric operations on bits.

### Half adder
Inputs: A, B
Outputs:
- Sum: S = A ⊕ B
- Carry: C = A · B

### Full adder
Inputs: A, B, C_in
Outputs:
- S = A ⊕ B ⊕ C_in
- C_out = A·B + A·C_in + B·C_in

Full adders chain together to create an N-bit adder:
- ripple-carry adder: simplest, but carry ripples slowly

### Subtraction with two’s complement
To compute A − B:
- A − B = A + (two’s complement of B)
Two’s complement of B:
- invert bits and add 1

So subtractor can be built from an adder with control:
- if subtract = 1, invert B and set C_in = 1

### Overflow
Depends on signed vs unsigned.

Unsigned overflow:
- carry out of MSB indicates overflow

Signed overflow (two’s complement):
- overflow occurs when adding two numbers with same sign gives result with different sign

## 📌 Key Points (Optional)
- Adders are the core of ALUs and address calculations.
- Speed matters, so advanced adders exist: carry-lookahead, carry-select, etc.

## 🖼️ Recommended Image
![[040-Logic80.png]]

## 🔗 Connections
- [[033-Combinational Logic Design From Truth Table to Gate Level]]
- [[041-ALU Architecture Operations Flags and Control Signals]]
- [[036-Synchronization Setup Hold and Clocking Basics]]