---
Theme: Compiler Fundamentals and Structure
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, Architecture]
---

## 📚 Idea/Concept

A compiler is a program that translates source code written in a high-level programming language into low-level machine code or intermediate code. This translation involves multiple phases organized into three main components: Front End, Middle End, and Back End.

### What is a Compiler?

A compiler performs complete translation of source code before execution:
- Reads the entire source program
- Analyzes its structure and meaning
- Generates equivalent executable code
- Errors are reported before execution begins

### Compiler vs Interpreter

| Aspect | Compiler | Interpreter |
|--------|----------|-------------|
| Translation | Entire program at once | Line by line |
| Execution | After compilation | During translation |
| Speed | Faster execution | Slower execution |
| Memory | Generates executable | No executable stored |
| Debugging | Harder (errors post-compile) | Easier (immediate feedback) |
| Examples | C, C++, Rust | Python, JavaScript |

### The Three Main Phases
```
┌─────────────────────────────────────────────────────────────┐
│                      FRONT END                               │
│  Source Code → Lexical → Syntactic → Semantic → IR          │
│                Analysis   Analysis    Analysis               │
└─────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────┐
│                      MIDDLE END                              │
│  IR → Optimization Passes → Optimized IR                     │
│       (Machine Independent)                                  │
└─────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────┐
│                       BACK END                               │
│  Optimized IR → Instruction → Register → Code Generation     │
│                  Selection    Allocation                     │
└─────────────────────────────────────────────────────────────┘
```

### Front End

Responsible for understanding the source language:
1. **Lexical Analysis**: Breaks code into tokens
2. **Syntactic Analysis**: Builds parse tree from tokens
3. **Semantic Analysis**: Checks meaning and type correctness
4. **IR Generation**: Produces intermediate representation

### Middle End (Optimizer)

Performs machine-independent optimizations:
- Dead code elimination
- Constant folding
- Loop optimization
- Inlining functions
- Common subexpression elimination

### Back End

Generates target machine code:
1. **Instruction Selection**: Maps IR to target instructions
2. **Register Allocation**: Assigns variables to registers
3. **Instruction Scheduling**: Orders instructions optimally
4. **Code Generation**: Produces final executable

### Symbol Table

A central data structure used throughout compilation:
- Stores identifiers (variables, functions, types)
- Tracks scope information
- Records type information
- Used for name resolution and type checking
```
Symbol Table Entry:
┌──────────┬──────────┬───────┬──────────────┐
│   Name   │   Type   │ Scope │   Attributes │
├──────────┼──────────┼───────┼──────────────┤
│  count   │   int    │   1   │  local, var  │
│  sum     │  double  │   1   │  local, var  │
│  main    │ function │   0   │  global, fn  │
└──────────┴──────────┴───────┴──────────────┘
```

## 📌 Key Points (Optional)
- Compilers translate entire programs before execution
- Three phases: Front End (analysis), Middle End (optimization), Back End (synthesis)
- The symbol table maintains information across all compilation phases

## 🖼️ Recommended Image
- Block diagram showing the complete compiler pipeline with data flow

## 🔗 Connections
- [[149-Brief History of Compilers]]
- [[151-Front End Lexical Analysis]]
- [[153-Front End Syntactic Analysis]]
- [[157-Front End Semantic Analysis]]
- [[158-Middle End Optimization]]
- [[159-Back End Code Generation]]