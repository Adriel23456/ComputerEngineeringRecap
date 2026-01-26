---
Theme: Boolean Simplification Identities De Morgan and K Map Strategy
Date created: 2026-01-23 21:00
tags: [LogicLayer, BooleanAlgebra, Simplification]
---

## 📚 Idea/Concept

Real hardware cost depends on the number of gates, levels, and wiring.  
So we simplify Boolean expressions.

### Core Boolean identities
- Identity:
  - A + 0 = A
  - A·1 = A
- Null:
  - A + 1 = 1
  - A·0 = 0
- Idempotent:
  - A + A = A
  - A·A = A
- Complement:
  - A + A̅ = 1
  - A·A̅ = 0
- Commutative:
  - A + B = B + A
  - A·B = B·A
- Associative:
  - (A + B) + C = A + (B + C)
  - (A·B)·C = A·(B·C)
- Distributive:
  - A·(B + C) = A·B + A·C
  - A + (B·C) = (A + B)(A + C)

### De Morgan’s laws (absolutely essential)
- (A·B)̅ = A̅ + B̅
- (A + B)̅ = A̅·B̅

De Morgan lets you convert between:
- AND/OR logic
- NAND/NOR implementations
This matters because NAND and NOR are “universal”.

### Karnaugh Map K Map
A K Map is a structured truth-table visualization that groups adjacent 1s:
- Adjacent means only one variable changes.
- Group sizes must be powers of two: 1,2,4,8...

Goal:
- Find the largest groups of 1s
- Each group becomes a simplified product term
- Fewer gates, fewer levels, faster circuits

## 📌 Key Points (Optional)
- Simplification reduces gate count and reduces propagation delay.
- NAND-only logic is common in real hardware libraries.

## 🖼️ Recommended Image
![[032-Logic68.png]]
![[032-Logic69.png]]

## 🔗 Connections
- [[031-Logic Gates Boolean Algebra and Truth Tables]]
- [[033-Combinational Logic Design From Truth Table to Gate Level]]
- [[035-Propagation Delay and Glitches Hazards in Combinational Logic]]
