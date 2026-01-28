---
Theme: Basic Block and Control Flow Analysis
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, ILP, Compiler]
---

## 📚 Idea/Concept

A basic block is a maximal sequence of instructions with a single entry point and single exit point, forming the fundamental unit for control flow analysis, optimization, and scheduling.

### Basic Block Definition

Properties:
- Single entry: Only first instruction can be branch target
- Single exit: Only last instruction can be branch
- Sequential: All instructions execute if block entered
- No internal branches or branch targets

### Identifying Basic Blocks

Leaders (first instruction of basic blocks):
1. First instruction of program
2. Target of any branch (conditional or unconditional)
3. Instruction immediately following a branch

Algorithm:
```
1. Find all leaders
2. Each basic block: leader to next leader (exclusive)
```

### Example

```assembly
      MOV  R1, #0        ; 1: Leader (first instruction)
      MOV  R2, #10       ; 2
LOOP: LD   R3, 0(R4)     ; 3: Leader (branch target)
      ADD  R1, R1, R3    ; 4
      ADD  R4, R4, #4    ; 5
      SUB  R2, R2, #1    ; 6
      BNE  R2, LOOP      ; 7: Branch (end of block)
      ST   R1, 0(R5)     ; 8: Leader (follows branch)
      RET                ; 9
```

Basic Blocks:
- Block 1: Instructions 1-2
- Block 2: Instructions 3-7
- Block 3: Instructions 8-9

### Control Flow Graph (CFG)

Nodes: Basic blocks
Edges: Possible control flow between blocks

```
    ┌─────────┐
    │ Block 1 │
    │ MOV R1  │
    │ MOV R2  │
    └────┬────┘
         │
         ▼
    ┌─────────┐
┌──▶│ Block 2 │
│   │ LD R3   │
│   │ ADD R1  │
│   │ ADD R4  │
│   │ SUB R2  │
│   │ BNE     │
│   └────┬────┘
│        │
│   ┌────┴────┐
│   │ Taken   │ Not Taken
└───┘         │
              ▼
         ┌─────────┐
         │ Block 3 │
         │ ST R1   │
         │ RET     │
         └─────────┘
```

### Basic Block Size

Typical statistics:
- Average basic block: 4-7 instructions
- Branch every 4-7 instructions
- Limits available ILP within single block

### Importance for Optimization

Why basic blocks matter:
1. Local optimization: Safe within block (no control flow)
2. Instruction scheduling: Reorder within block freely
3. Register allocation: Track liveness within block
4. Code generation: Generate code per block

### Extended Basic Blocks

Trace: Sequence of basic blocks with single entry
- Includes blocks reachable by fall-through
- Enables optimization across block boundaries

Superblock: Single-entry, multiple-exit trace
- Used in trace scheduling
- Allows aggressive optimization

### Loop Structures in CFG

Loop detection:
- Back edge: Edge to dominating node
- Natural loop: Nodes reachable from back edge target without crossing back edge

```
     ┌─────────┐
     │ Header  │◀───────┐
     └────┬────┘        │
          │             │
     ┌────▼────┐        │
     │  Body   │        │
     └────┬────┘        │
          │             │
     ┌────▼────┐        │
     │  Latch  │────────┘
     └────┬────┘   (back edge)
          │
     ┌────▼────┐
     │  Exit   │
     └─────────┘
```

### Dominator Analysis

Node A dominates node B if every path from entry to B goes through A.

Dominator tree:
- Entry dominates all nodes
- Used for loop detection, SSA construction

### Live Variable Analysis

For each basic block:
- Live-in: Variables live at block entry
- Live-out: Variables live at block exit
- DEF: Variables defined in block
- USE: Variables used before defined in block

Data flow equations:
$$LiveIn(B) = Use(B) \cup (LiveOut(B) - Def(B))$$
$$LiveOut(B) = \bigcup_{S \in successors(B)} LiveIn(S)$$

### Applications

Instruction scheduling:
- Schedule instructions within basic block
- Respect dependencies
- Hide latencies

Register allocation:
- Assign registers to variables
- Spill if insufficient registers
- Use live ranges from blocks

Dead code elimination:
- Remove assignments to dead variables
- Based on live variable analysis

Code motion:
- Move loop-invariant code out of loops
- Requires CFG analysis

### Limitations of Basic Block

Small block size limits:
- Available parallelism
- Optimization opportunities
- Scheduling flexibility

Solutions:
- Loop unrolling (larger blocks)
- Trace scheduling (across blocks)
- Superblock formation
- Global scheduling

## 📌 Key Points (Optional)
- Basic blocks simplify analysis by eliminating internal control flow.
- CFG enables whole-program optimization and analysis.

## 🖼️ Recommended Image
- Control flow graph showing basic blocks connected by edges.

## 🔗 Connections
- [[064-Instruction Level Parallelism ILP Fundamentals]]
- [[080-Code Scheduling for VLIW]]
- [[082-Loop Unrolling and Software ILP Techniques]]
- [[057-WCET Worst Case Execution Time]]
- [[083-Software Pipelining]]