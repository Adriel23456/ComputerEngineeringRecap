---
Theme: GCC Compiler Architecture
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, GCC, Tools]
---

## 📚 Idea/Concept

GCC (GNU Compiler Collection) is a comprehensive compiler infrastructure supporting multiple languages and target architectures. Its modular design with language-specific front ends, common middle end, and target-specific back ends makes it highly portable and extensible.

### GCC History and Overview

- 1987: Richard Stallman releases GCC 1.0
- Originally "GNU C Compiler"
- Now supports C, C++, Fortran, Ada, Go, D
- Targets 100+ processor architectures

### GCC Architecture
```
┌─────────────────────────────────────────────────────────────┐
│                     FRONT ENDS                               │
│  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐              │
│  │  C  │  │ C++ │  │ Fort│  │ Ada │  │ Go  │  ...         │
│  └──┬──┘  └──┬──┘  └──┬──┘  └──┬──┘  └──┬──┘              │
│     └────────┼───────┼────────┼────────┘                   │
│              ▼       ▼        ▼                             │
│         ┌────────────────────────┐                          │
│         │    GENERIC Trees       │                          │
│         └───────────┬────────────┘                          │
└─────────────────────┼───────────────────────────────────────┘
                      ▼
┌─────────────────────────────────────────────────────────────┐
│                     MIDDLE END                               │
│         ┌────────────────────────┐                          │
│         │    GIMPLE (High IR)    │                          │
│         └───────────┬────────────┘                          │
│                     ▼                                        │
│         ┌────────────────────────┐                          │
│         │    Tree SSA Passes     │                          │
│         └───────────┬────────────┘                          │
│                     ▼                                        │
│         ┌────────────────────────┐                          │
│         │    RTL (Low IR)        │                          │
│         └───────────┬────────────┘                          │
└─────────────────────┼───────────────────────────────────────┘
                      ▼
┌─────────────────────────────────────────────────────────────┐
│                     BACK ENDS                                │
│  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐              │
│  │x86  │  │ARM  │  │MIPS │  │RISCV│  │PPC  │  ...         │
│  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘              │
└─────────────────────────────────────────────────────────────┘
```

### GIMPLE (High-Level IR)

GCC's high-level IR for machine-independent optimizations:
```c
// Source code
int foo(int a, int b) {
    return (a + b) * (a - b);
}

// GIMPLE representation
foo (int a, int b) {
    int D.1234;
    int D.1235;
    int D.1236;
    
    D.1234 = a + b;
    D.1235 = a - b;
    D.1236 = D.1234 * D.1235;
    return D.1236;
}
```

### RTL (Register Transfer Language)

Low-level IR close to machine instructions:
```
(insn (set (reg:SI 0 ax)
       (plus:SI (reg:SI 0 ax)
                (reg:SI 1 dx))))
```

RTL Components:
- `insn`: Instruction
- `set`: Assignment
- `reg`: Register reference
- `plus`, `minus`, etc.: Operations
- `SI`, `DI`, etc.: Modes (Single Integer, Double Integer)

### Compilation Phases
```bash
gcc -o hello hello.c

# Explicit phases:
gcc -E hello.c > hello.i    # Preprocessing
gcc -S hello.i              # Compilation → hello.s
gcc -c hello.s              # Assembly → hello.o
gcc hello.o -o hello        # Linking → hello
```

### Optimization Passes

**GIMPLE Passes** (tree-ssa-*):
- Constant propagation
- Dead code elimination
- Loop optimizations
- Vectorization

**RTL Passes**:
- Instruction combination
- Register allocation
- Peephole optimization
- Instruction scheduling

### GCC Command Options

**Optimization Levels**:
```bash
-O0    # No optimization (default, for debugging)
-O1    # Moderate optimization
-O2    # Full optimization (recommended)
-O3    # Aggressive optimization
-Os    # Optimize for size
-Ofast # Disregard strict standards compliance
```

**Debugging and Profiling**:
```bash
-g          # Debug information
-pg         # Profiling for gprof
-fprofile-generate  # Generate profiling data
-fprofile-use       # Use profiling data
```

**Warnings**:
```bash
-Wall       # Common warnings
-Wextra     # Additional warnings
-Werror     # Treat warnings as errors
-pedantic   # Strict ISO compliance
```

### Viewing Intermediate Representations
```bash
# Dump GIMPLE
gcc -fdump-tree-all hello.c

# Dump RTL
gcc -fdump-rtl-all hello.c

# View assembly
gcc -S -fverbose-asm hello.c
```

### GCC Extensions

**Inline Assembly**:
```c
int add(int a, int b) {
    int result;
    __asm__ ("addl %1, %0"
             : "=r" (result)
             : "r" (a), "0" (b));
    return result;
}
```

**Built-in Functions**:
```c
__builtin_expect(x, 0)   // Branch prediction hint
__builtin_prefetch(addr) // Prefetch hint
__builtin_clz(x)         // Count leading zeros
```

## 📌 Key Points (Optional)
- GCC uses a three-phase architecture: front end, middle end, back end
- GIMPLE is the high-level IR; RTL is the low-level IR
- Multiple language front ends share common optimization infrastructure
- Optimization levels control the trade-off between compile time and code quality

## 🖼️ Recommended Image
- Detailed block diagram of GCC's internal structure showing all passes

## 🔗 Connections
- [[150-Compiler Fundamentals and Structure]]
- [[158-Middle End Optimization]]
- [[159-Back End Code Generation]]
- [[161-LLVM Compiler Infrastructure]]