---
Theme: Logic Gates Boolean Algebra and Truth Tables
Date created: 2026-01-23 21:00
tags: [LogicLayer, BooleanAlgebra, LogicGates]
---

## 📚 Idea/Concept

Logic gates compute Boolean functions: inputs and outputs are binary.

### Core gates and meanings
- NOT: invert
- AND: true only if both true
- OR: true if any true
- NAND: NOT(AND)
- NOR: NOT(OR)
- XOR: true if inputs differ
- XNOR: true if inputs equal

### Truth tables define behavior
A truth table lists all input combinations and output results.
For two inputs A, B there are 4 combinations:
00, 01, 10, 11

### Boolean algebra operators
Common mapping:
- Addition “+” means OR
- Multiplication “·” means AND
- Overbar means NOT

Examples:
- AND: F = A·B
- OR: F = A + B
- NOT: F = A̅

### Canonical forms
Two standard ways to express any Boolean function:

#### Sum of Products SOP
OR of minterms (minterm = AND term that is true for exactly one input row)
- F = m1 + m3 + ...

Sigma notation:
- F(A,B) = Σ(m1, m3)
or sometimes index form:
- F(A,B) = Σ(1, 3)

Interpretation:
- List the rows where output is 1.

#### Product of Sums POS
AND of maxterms (maxterm = OR term that is false for exactly one row)
- F = Π(M0, M2, ...)

Interpretation:
- List the rows where output is 0.

### Why this matters in CSE
- Boolean algebra is the math behind:
  - digital circuits
  - instruction decoding
  - CPU control logic
  - state machines
  - ALU operations

## 📌 Key Points (Optional)
- Truth tables are the ground truth; algebra is the simplification tool.
- SOP is often used for implementation with AND/OR/NOT, or NAND-only logic.

## 🖼️ Recommended Image
![[031-Logic64.png]]
![[031-Logic67.png]]
![[031-Logic65.png]]
![[031-Logic66.png]]

## 🔗 Connections
- [[032-Boolean Simplification Identities De Morgan and K Map Strategy]]
- [[033-Combinational Logic Design From Truth Table to Gate Level]]
- [[040-Arithmetic Circuits Adders Subtractors and Overflow]]
