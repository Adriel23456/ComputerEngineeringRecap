---
Theme: Code Scheduling for VLIW
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ILP, VLIW]
---

## 📚 Idea/Concept

Code scheduling for VLIW processors is the compiler's task of assigning operations to instruction slots while respecting dependencies, latencies, and resource constraints to maximize parallelism.

### Scheduling Process Overview

1. Decompose code into basic blocks
2. Build dependency graph
3. Calculate critical path
4. Schedule operations to minimize execution time
5. Insert NOPs where necessary

### Dependency Graph

Nodes: Operations
Edges: Dependencies with latency labels

Edge types:
- True dependency (RAW): Solid arrow
- Anti-dependency (WAR): Dashed arrow
- Output dependency (WAW): Dotted arrow

Example graph:
```
    a(MUL) ─────▶ d(MUL)
    7/3         4/3
      │           │
      ▼           ▼
    b(ADD) ────▶ e(SUB) ────▶ f(ST) ────▶ g(SUB)
    5/1         4/1          3/2          1/1
      │
      ▼
    c(ADD)
    4/1

Numbers: Priority/Latency
```

### Critical Path

The critical path is the longest path through the dependency graph (sum of latencies).

Determines minimum execution time with infinite resources.

### Scheduling Algorithm

For each cycle, starting from cycle 1:

1. Build Ready List:
   - Operations whose predecessors are all scheduled
   - And whose latencies have been satisfied

2. Apply Priority:
   - Select operations with highest priority
   - Priority = longest path to end of graph

3. Check Resources:
   - Use Resource Reservation Table (RRT)
   - Verify functional unit available

4. Assign and Update:
   - Place operation in instruction slot
   - Mark resource as used
   - Update ready list for next cycle

### Resource Reservation Table

Tracks FU usage per cycle:
```
Cycle | ALU1 | ALU2 | L/S | MUL
------|------|------|-----|----
  1   |  b   |  -   |  a  |  -
  2   |  c   |  -   |  a  |  -
  3   |  e   |  -   |  -  |  -
  4   |  -   |  -   |  d  |  -
  5   |  -   |  -   |  d  |  -
  6   |  -   |  -   |  f  |  -
  7   |  g   |  -   |  -  |  -
```

### Resulting VLIW Code

```
Cycle | ALU  | L/S  | MUL  | Notes
------|------|------|------|------
  1   |  b   |  a   |  n   | a has 2-cycle latency
  2   |  c   |  n   |  n   | 
  3   |  e   |  n   |  n   | 
  4   |  n   |  d   |  n   | d has 2-cycle latency
  5   |  n   |  n   |  n   | 
  6   |  n   |  f   |  n   | 
  7   |  g   |  n   |  n   | 

n = NOP
```

### Scheduling Challenges

Limited Basic Block Size:
- Average basic block: 4-7 instructions
- Not enough parallelism
- Solutions: Loop unrolling, trace scheduling

Unpredictable Events:
- Cache misses
- Branch mispredictions
- Solutions: Predication, software speculation

Register Pressure:
- Aggressive scheduling needs many registers
- Spilling hurts performance

### Advanced Scheduling Techniques

Trace Scheduling:
- Schedule across basic block boundaries
- Follow most likely execution path
- Insert compensation code for other paths

Software Pipelining:
- Overlap iterations of loops
- Steady-state achieves maximum parallelism

Global Scheduling:
- Move operations across branches
- Speculative and non-speculative motion

## 📌 Key Points (Optional)
- Effective VLIW scheduling requires sophisticated compiler analysis.
- The dependency graph and critical path guide optimal operation placement.

## 🖼️ Recommended Image
- Dependency graph with priority annotations and corresponding scheduled VLIW code.

## 🔗 Connections
- [[079-VLIW Architecture and Static Scheduling]]
- [[082-Loop Unrolling and Software ILP Techniques]]
- [[064-Instruction Level Parallelism ILP Fundamentals]]
- [[081-Basic Block and Control Flow Analysis]]