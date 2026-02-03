---
Theme: LLVM Compiler Infrastructure
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, LLVM, Tools]
---

## 📚 Idea/Concept

LLVM (Low Level Virtual Machine) is a modular compiler infrastructure that provides reusable components for building compilers and related tools. Its well-defined IR and clean architecture have made it the foundation for many modern compilers including Clang, Rust, and Swift.

### LLVM History

- 2003: Started as research project at UIUC by Chris Lattner
- 2005: Apple begins sponsoring development
- 2012: Clang becomes default compiler for Xcode
- Today: Used by Apple, Google, Sony, NVIDIA, and many others

### LLVM Architecture
```
┌─────────────────────────────────────────────────────────────┐
│                     FRONT ENDS                               │
│  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐              │
│  │Clang│  │Rust │  │Swift│  │Julia│  │Flang│  ...         │
│  │(C++) │  │     │  │     │  │     │  │(Fort)│             │
│  └──┬──┘  └──┬──┘  └──┬──┘  └──┬──┘  └──┬──┘              │
│     └────────┴───────┴────────┴────────┘                   │
│                        ▼                                    │
│              ┌──────────────────┐                           │
│              │    LLVM IR       │                           │
│              └────────┬─────────┘                           │
└───────────────────────┼─────────────────────────────────────┘
                        ▼
┌─────────────────────────────────────────────────────────────┐
│                     LLVM CORE                                │
│         ┌────────────────────────┐                          │
│         │   Analysis Passes      │                          │
│         └────────────────────────┘                          │
│         ┌────────────────────────┐                          │
│         │   Transform Passes     │                          │
│         └────────────────────────┘                          │
│         ┌────────────────────────┐                          │
│         │   Code Generator       │                          │
│         └────────────────────────┘                          │
└───────────────────────┼─────────────────────────────────────┘
                        ▼
┌─────────────────────────────────────────────────────────────┐
│                     BACK ENDS                                │
│  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐              │
│  │x86  │  │ARM  │  │NVPTX│  │WebAs│  │RISCV│  ...         │
│  │     │  │     │  │(CUDA)│  │sembly│  │     │             │
│  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘              │
└─────────────────────────────────────────────────────────────┘
```

### LLVM IR

LLVM's intermediate representation is typed and SSA-based:
```llvm
; Function to add two integers
define i32 @add(i32 %a, i32 %b) {
entry:
    %result = add i32 %a, %b
    ret i32 %result
}

; Factorial function
define i32 @factorial(i32 %n) {
entry:
    %cmp = icmp sle i32 %n, 1
    br i1 %cmp, label %base, label %recursive

base:
    ret i32 1

recursive:
    %n_minus_1 = sub i32 %n, 1
    %fact_n_minus_1 = call i32 @factorial(i32 %n_minus_1)
    %result = mul i32 %n, %fact_n_minus_1
    ret i32 %result
}
```

### LLVM IR Features

**Type System**:
```llvm
i1, i8, i16, i32, i64     ; Integer types
float, double              ; Floating point
<4 x i32>                  ; Vector type
[10 x i32]                 ; Array type
{i32, float}               ; Structure type
i32*                       ; Pointer type
```

**Instructions**:
```llvm
; Arithmetic
%sum = add i32 %a, %b
%prod = mul i32 %a, %b

; Memory
%ptr = alloca i32
store i32 42, i32* %ptr
%val = load i32, i32* %ptr

; Control flow
br i1 %cond, label %true, label %false
ret i32 %result
call i32 @foo(i32 %x)
```

### Clang: LLVM's C/C++ Front End
```bash
# Compile C to LLVM IR
clang -S -emit-llvm hello.c -o hello.ll

# View IR in human-readable form
cat hello.ll

# Compile IR to assembly
llc hello.ll -o hello.s

# Run optimization passes
opt -O2 hello.ll -o hello_opt.ll
```

### LLVM Tools

| Tool | Purpose |
|------|---------|
| `clang` | C/C++/Objective-C compiler |
| `opt` | Optimizer |
| `llc` | LLVM static compiler (IR → assembly) |
| `lli` | LLVM interpreter |
| `llvm-as` | LLVM assembler (text → bitcode) |
| `llvm-dis` | LLVM disassembler (bitcode → text) |
| `llvm-link` | Linker for LLVM bitcode |

### LLVM vs GCC

| Aspect | LLVM | GCC |
|--------|------|-----|
| License | Apache 2.0 | GPL |
| Modularity | Highly modular | Monolithic |
| IR | Well-documented | Internal |
| JIT support | Built-in | Limited |
| Error messages | Excellent | Good |
| Optimization speed | Faster | Slower |
| Mature platforms | Fewer | More |

### LLVM Pass Infrastructure
```cpp
// Simple LLVM pass
struct MyPass : public FunctionPass {
    static char ID;
    MyPass() : FunctionPass(ID) {}
    
    bool runOnFunction(Function &F) override {
        for (BasicBlock &BB : F) {
            for (Instruction &I : BB) {
                // Analyze or transform instruction
            }
        }
        return false; // Return true if IR was modified
    }
};
```

### Common LLVM Optimization Passes
```bash
opt -mem2reg      # Promote memory to registers
opt -simplifycfg  # Simplify control flow graph
opt -instcombine  # Combine redundant instructions
opt -gvn          # Global value numbering
opt -licm         # Loop invariant code motion
opt -loop-unroll  # Unroll loops
```

## 📌 Key Points (Optional)
- LLVM provides a clean, modular compiler infrastructure
- LLVM IR is typed, SSA-based, and well-documented
- Multiple language front ends target LLVM IR
- Clang is the primary C/C++ front end for LLVM

## 🖼️ Recommended Image
- LLVM ecosystem diagram showing language front ends and target back ends

## 🔗 Connections
- [[150-Compiler Fundamentals and Structure]]
- [[160-GCC Compiler Architecture]]
- [[158-Middle End Optimization]]
- [[159-Back End Code Generation]]