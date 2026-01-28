---
Theme: Speculative Execution and Branch Prediction
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, ILP, BranchPrediction]
---

## 📚 Idea/Concept

Speculative execution is a technique where the processor executes instructions before knowing if they should be executed, based on predictions about branches and other uncertain events, with mechanisms to recover if speculation is wrong.

### Motivation

ILP is limited by:
- Data dependencies (addressed by OoO, forwarding)
- Control dependencies (branches)

Branches occur every 4-7 instructions on average.
Without speculation, pipeline stalls waiting for branch resolution.

### Types of Speculation

Branch Prediction Speculation:
- Predict branch direction (taken/not-taken)
- Predict target address
- Execute predicted path speculatively

Value Speculation:
- Predict data values (addresses, operands)
- Continue computation with predicted values
- Verify and recover if wrong

Memory Dependence Speculation:
- Speculate that load doesn't conflict with pending stores
- Execute load early
- Verify memory ordering

### Hardware Support for Speculation

Reorder Buffer (ROB):
- Holds speculative results until verification
- Maintains program order for commits
- Enables rollback on misspeculation

Recovery Mechanism:
1. Detect misspeculation (branch resolved differently)
2. Flush all instructions younger than mispredicted branch
3. Restore architectural state from ROB
4. Redirect fetch to correct path

Checkpoint Recovery:
- Save architectural state at speculation points
- Restore checkpoint on misspeculation

### Software Speculation (Static)

Compiler-directed speculation:
1. Compiler identifies instructions to speculate
2. Moves instructions before uncertain branch
3. Inserts verification code
4. Provides recovery code if wrong

Requirements:
- Hardware support for speculative loads
- Exception deferral mechanisms
- Compiler analysis capabilities

### Speculation Process

1. Branch encountered in pipeline
2. Predictor provides prediction
3. Fetch continues on predicted path
4. Instructions execute speculatively (results in ROB)
5. Branch resolves:
   - Correct: Speculative results commit
   - Wrong: Flush and recover

### Exception Handling with Speculation

Challenge: Speculative instruction may cause exception

Solutions:
- Defer exceptions until instruction commits
- Store exception info in ROB
- Raise exception only for committed instructions
- Speculative instructions never raise exceptions

### SPECTRE Vulnerability

Modern security concern with speculation:

Attack Mechanism:
1. Attacker trains branch predictor maliciously
2. Processor speculatively executes wrong path
3. Speculative execution accesses protected memory
4. Even though results discarded, cache state changes
5. Attacker infers secret data through timing side-channel

Impact:
- Affected nearly all modern processors
- Required microcode updates and OS patches
- Motivates redesign of speculation mechanisms

Mitigations:
- Speculative load restrictions
- Cache isolation
- Hardware barriers
- Compiler mitigations

### Performance Impact

Branch misprediction penalty:
- Proportional to pipeline depth
- Modern processors: 10-20 cycles
- Makes prediction accuracy critical

Typical prediction accuracy:
- Static prediction: 60-70%
- 2-bit counter: 85-90%
- Tournament predictor: 95-98%
- Neural predictor: 97-99%

### Speculation Depth

How far to speculate:
- More speculation → more ILP extracted
- More speculation → larger recovery cost
- Balance controlled by:
  - ROB size
  - Branch confidence
  - Resource availability

## 📌 Key Points (Optional)
- Speculation enables ILP extraction across branches but requires efficient recovery.
- Security implications (Spectre) have forced reconsideration of aggressive speculation.

## 🖼️ Recommended Image
- Pipeline diagram showing speculative execution, branch resolution, and recovery paths.

## 🔗 Connections
- [[068-Control Hazards and Branch Prediction]]
- [[077-Reorder Buffer and Precise Exceptions]]
- [[073-Out of Order Execution Fundamentals]]
- [[084-Predicated Execution]]
- [[064-Instruction Level Parallelism ILP Fundamentals]]