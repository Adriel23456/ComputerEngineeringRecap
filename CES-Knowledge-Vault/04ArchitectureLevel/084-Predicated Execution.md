---
Theme: Predicated Execution
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, ILP, ControlFlow]
---

## 📚 Idea/Concept

Predicated execution (also called conditional execution) is a technique that converts control dependencies into data dependencies by executing instructions conditionally based on a predicate register, eliminating branches.

### Basic Concept

Every instruction includes a predicate field:
```
(p) op Rd, Rs1, Rs2
```
- If predicate p is true: instruction executes normally
- If predicate p is false: instruction becomes a NOP (no effect)

### If-Conversion

Converting branches to predicated code:

Original Code with Branch:
```assembly
    BNEZ R4, skip    ; Branch if R4 != 0
    ADD  R1, R2, R3
    LW   R6, 0(R5)
skip:
    ...
```

Predicated Code:
```assembly
    p = (CMP R4, 0)   ; Set predicate
    (p) ADD  R1, R2, R3
    (p) LW   R6, 0(R5)
    ...
```

### Advantages

1. Eliminates Branch Penalty:
   - No misprediction penalty
   - No pipeline flush needed
   - Deterministic execution time

2. Eliminates Control Dependencies:
   - Converts to data dependency on predicate
   - Enables better scheduling
   - Increases basic block size

3. No Branch Prediction Needed:
   - Simplifies hardware
   - Reduces power consumption of predictor

4. Better for VLIW/Static Scheduling:
   - Compiler can schedule predicated code more effectively
   - Fills instruction slots that would be NOPs

### Limitations

1. ISA Complexity:
   - Requires predicate registers
   - Extra encoding bits in instructions
   - More architectural state to save/restore

2. Wasted Work:
   - Both paths execute (one becomes NOPs)
   - Consumes functional unit resources
   - Power wasted on nullified instructions

3. Not Always Beneficial:
   - If one path much longer than other
   - If branch is highly predictable
   - If paths have side effects that can't be nullified

### Effectiveness Criteria

Predication is most effective when:
- Branch misprediction penalty is high
- Both paths are short and similar length
- Branch is hard to predict (50/50 probability)
- Compiler can effectively schedule predicated code

Predication is less effective when:
- One path is much longer
- Branch is highly predictable (>90%)
- Paths have many memory operations with side effects

### Hardware Support

Predicate Registers:
- Dedicated register file for boolean values
- ARM: Condition codes applied to most instructions
- IA-64 (Itanium): 64 predicate registers

Predicate Defining Instructions:
- Compare instructions set predicates
- Can define multiple predicates (true and complement)

### Example: If-Then-Else Conversion

Original:
```c
if (a > b)
    x = a - b;
else
    x = b - a;
```

Predicated:
```assembly
    CMP  p1, p2, a, b  ; p1 = (a>b), p2 = (a<=b)
(p1) SUB  x, a, b
(p2) SUB  x, b, a
```

Both subtractions execute, only one writes result.

### Architectures with Predication

- ARM: Conditional execution field on all instructions
- IA-64 (Itanium): Full predication with 64 predicate registers
- NVIDIA GPUs: Thread predication for SIMT divergence
- Some DSPs: Limited predication support

## 📌 Key Points (Optional)
- Predication trades branch hazards for potential wasted computation.
- Most effective for short, unpredictable branches.

## 🖼️ Recommended Image
- Diagram showing branch vs predicated execution timeline comparison.

## 🔗 Connections
- [[068-Control Hazards and Branch Prediction]]
- [[079-VLIW Architecture and Static Scheduling]]
- [[064-Instruction Level Parallelism ILP Fundamentals]]
- [[085-Speculative Execution and Branch Prediction]]