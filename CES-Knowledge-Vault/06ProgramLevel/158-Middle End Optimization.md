---
Theme: Middle End Optimization
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, Optimization, MiddleEnd]
---

## 📚 Idea/Concept

The Middle End of a compiler performs machine-independent optimizations on the Intermediate Representation (IR). These optimizations improve code efficiency without knowledge of the target architecture, focusing on reducing operations, eliminating redundancy, and improving control flow.

### Intermediate Representation (IR)

**Three-Address Code (TAC)**:
Most common IR format where each instruction has at most three operands.
```
Source:     a = b + c * d
TAC:        t1 = c * d
            t2 = b + t1
            a = t2
```

**IR Instruction Types**:
```
x = y op z     // Binary operation
x = op y       // Unary operation
x = y          // Copy
goto L         // Unconditional jump
if x goto L    // Conditional jump
param x        // Function parameter
call p, n      // Function call with n arguments
return x       // Return value
x = y[i]       // Indexed access
x[i] = y       // Indexed assignment
x = &y         // Address of
x = *y         // Dereference
*x = y         // Indirect assignment
```

### Static Single Assignment (SSA)

Each variable is assigned exactly once, with versions:
```
Original:           SSA Form:
x = 1               x₁ = 1
x = x + 1           x₂ = x₁ + 1
x = x * 2           x₃ = x₂ * 2
```

**Phi Functions** for control flow merge:
```c
// Original
if (cond) {
    x = 1;
} else {
    x = 2;
}
y = x;

// SSA
if (cond) {
    x₁ = 1;
} else {
    x₂ = 2;
}
x₃ = φ(x₁, x₂);  // Select based on which branch taken
y₁ = x₃;
```

### Common Optimizations

**Constant Folding**:
Evaluate constant expressions at compile time.
```
t1 = 3 * 4        →   t1 = 12
t2 = t1 + 5       →   t2 = 17
```

**Constant Propagation**:
Replace variable uses with known constants.
```
x = 5
y = x + 3         →   y = 5 + 3   →   y = 8
```

**Dead Code Elimination**:
Remove code that doesn't affect output.
```
x = 1           // Dead if x never used
x = 2
use(x)
```

**Common Subexpression Elimination (CSE)**:
Avoid recomputing identical expressions.
```
t1 = a + b        t1 = a + b
t2 = a + b   →    t2 = t1
t3 = t1 * t2      t3 = t1 * t1
```

**Copy Propagation**:
Replace copies with original values.
```
x = y
z = x + 1    →    z = y + 1
```

### Loop Optimizations

**Loop Invariant Code Motion**:
Move constant computations outside the loop.
```c
// Before                 // After
for (i = 0; i < n; i++)   t = a + b;
    x[i] = (a + b) * i;   for (i = 0; i < n; i++)
                              x[i] = t * i;
```

**Strength Reduction**:
Replace expensive operations with cheaper ones.
```c
// Before                 // After
for (i = 0; i < n; i++)   t = 0;
    x[i] = i * 4;         for (i = 0; i < n; i++) {
                              x[i] = t;
                              t = t + 4;
                          }
```

**Loop Unrolling**:
Reduce loop overhead by duplicating body.
```c
// Before                 // After (unroll 4x)
for (i = 0; i < 100; i++) for (i = 0; i < 100; i += 4) {
    a[i] = b[i];              a[i] = b[i];
                              a[i+1] = b[i+1];
                              a[i+2] = b[i+2];
                              a[i+3] = b[i+3];
                          }
```

### Data Flow Analysis

**Reaching Definitions**:
Which definitions of a variable can reach a point?

**Live Variable Analysis**:
Which variables may be used before next assignment?

**Available Expressions**:
Which expressions have been computed and not invalidated?

### GCC Optimization Pipeline
```
High-Level IR (GIMPLE)
        ↓
    SSA Conversion
        ↓
    Machine-Independent Optimizations
        ↓
    Low-Level IR (RTL)
        ↓
    Machine-Dependent Optimizations
        ↓
    Assembly Output
```

**GCC Optimization Levels**:
- `-O0`: No optimization (debugging)
- `-O1`: Basic optimizations
- `-O2`: Most optimizations (recommended)
- `-O3`: Aggressive (may increase code size)
- `-Os`: Optimize for size

## 📌 Key Points (Optional)
- Middle End performs machine-independent optimizations
- SSA form simplifies many optimization algorithms
- Loop optimizations often yield the biggest performance gains
- Data flow analysis provides information for safe transformations

## 🖼️ Recommended Image
- Flowchart showing optimization passes and their dependencies

## 🔗 Connections
- [[150-Compiler Fundamentals and Structure]]
- [[157-Front End Semantic Analysis]]
- [[159-Back End Code Generation]]
- [[160-GCC Compiler Architecture]]
- [[161-LLVM Compiler Infrastructure]]