---
Theme: Scoreboard Algorithm
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ILP, OoOE]
---

## 📚 Idea/Concept

The Scoreboard algorithm, introduced in the CDC 6600 (1963), was the first implementation of dynamic scheduling, enabling out-of-order execution through centralized hazard detection and control.

### Key Characteristics

- Issue in order
- Execute out of order
- Complete out of order
- No forwarding (results pass through registers)
- Centralized control in the "scoreboard"
- No register renaming

### Architecture Components

The scoreboard maintains three data structures:

1. Instruction Status:
   - Tracks which stage each instruction is in
   - Stages: Issue, Read Operands, Execute, Write Result

2. Functional Unit Status (per FU):
   - Busy: Is the unit occupied?
   - Op: Operation being performed
   - Fi: Destination register
   - Fj, Fk: Source registers
   - Qj, Qk: FUs producing source operands
   - Rj, Rk: Flags indicating if Fj, Fk are ready

3. Register Result Status:
   - For each register: which FU will write it (or blank)

### The Four Stages

1. Issue:
   - Decode instruction in program order
   - Check structural hazard (FU available?)
   - Check WAW hazard (destination register free?)
   - If clear: issue and update scoreboard
   - If hazard: stall issue

2. Read Operands:
   - Wait until no RAW hazards
   - Operand ready if: no FU will write it OR FU is writing now
   - When ready: read operands from registers

3. Execute:
   - Perform operation
   - Notify scoreboard when complete
   - Duration depends on operation type

4. Write Result:
   - Check for WAR hazards before writing
   - If dependent instruction hasn't read yet: wait
   - When clear: write to register, free FU

### Scoreboard Example

Functional Units:
- ALU: 2 units (1 cycle latency)
- Multiplier: 1 unit (4 cycles latency)
- LD/ST: 2 units (2 cycles latency)

```
INST                    | Issue | RD | EX    | WB
------------------------|-------|-----|-------|----
LDR R0,[R0,#800]       | 1     | 2   | 3-4   | 5
MOV R1,#4              | 2     | 3   | 4     | 5
LDR R4,[R0,R1]         | 3     | 6   | 7-8   | 9
ADD R0,R1,#0x32        | 6     | 7   | 8     | 9
MUL R3,R0,R4           | 7     | 10  | 11-14 | 15
```

### Hazard Detection in Scoreboard

WAW Detection (at Issue):
```
If Register_Result[Fi] ≠ empty: stall
```

RAW Detection (at Read Operands):
```
If Qj ≠ empty OR Qk ≠ empty: wait
```

WAR Detection (at Write Result):
```
For all other FUs f:
  If (Fj[f] = Fi AND Rj[f] = Yes) OR
     (Fk[f] = Fi AND Rk[f] = Yes): wait
```

### Limitations

1. No register renaming → WAW causes stalls
2. WAR hazards delay writes
3. No forwarding → longer waits for data
4. Centralized control → potential bottleneck
5. Limited scalability

### Historical Significance

- First dynamic scheduling implementation
- Proved out-of-order execution viable
- Foundation for later algorithms (Tomasulo)
- CDC 6600 achieved ~10 MFLOPS (remarkable for 1964)

## 📌 Key Points (Optional)
- Scoreboard detects but doesn't eliminate WAR/WAW hazards.
- The centralized approach limits scalability for modern wide-issue processors.

## 🖼️ Recommended Image
- Table showing scoreboard data structures during execution example.

## 🔗 Connections
- [[073-Out of Order Execution Fundamentals]]
- [[076-Tomasulo Algorithm]]
- [[067-Data Hazards RAW WAR WAW]]
- [[064-Instruction Level Parallelism ILP Fundamentals]]