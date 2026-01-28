---
Theme: Reorder Buffer and Precise Exceptions
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ILP, OoOE]
---

## 📚 Idea/Concept

The Reorder Buffer (ROB) extends Tomasulo's algorithm to support precise exceptions and speculative execution by ensuring that results commit to architectural state in program order.

### The Problem

In pure Tomasulo:
- Results written to registers out of order
- If exception occurs, state is inconsistent
- Cannot determine exact instruction that faulted
- Speculation cannot be rolled back cleanly

### Precise Exceptions

Definition: After handling an exception, architectural state reflects:
- All instructions before faulting instruction completed
- No instructions after faulting instruction have effects
- Faulting instruction's effects depend on exception type

Required for:
- Debugging
- Virtual memory (page faults)
- Arithmetic exceptions
- Context switching
- Speculative execution recovery

### Reorder Buffer Structure

Circular buffer with entries containing:

| Field | Description |
|-------|-------------|
| Instruction Type | Branch, Store, Register write |
| Destination | Register number or memory address |
| Value | Result (when computed) |
| Ready | Has instruction completed execution? |
| Exception | Exception information (if any) |

### Modified Pipeline with ROB

```
┌──────┐   ┌──────┐   ┌──────┐   ┌─────────┐   ┌────────┐
│Issue │──▶│ Exec │──▶│ Write│──▶│ ROB     │──▶│ Commit │
│      │   │      │   │Result│   │ (wait)  │   │(in-order)│
└──────┘   └──────┘   └──────┘   └─────────┘   └────────┘
```

### Four Stages with ROB

1. Issue:
   - Allocate RS and ROB entry
   - ROB entry number becomes result tag
   - If no RS or ROB available: stall

2. Execute:
   - Same as Tomasulo
   - Operations proceed when operands ready

3. Write Result:
   - Broadcast result on CDB with ROB entry number
   - Write result to ROB entry, set Ready bit
   - DO NOT write to architectural register yet

4. Commit (new stage):
   - When instruction at ROB head is Ready:
     - If no exception: write to register/memory, deallocate ROB
     - If exception: flush pipeline, handle exception
   - Only ROB head can commit (in-order)

### Speculative Execution Support

Branch prediction with ROB:
1. Predict branch, fetch predicted path
2. Execute speculatively, results go to ROB
3. When branch resolves:
   - Correct: branch commits normally
   - Wrong: flush all ROB entries after branch, restart

### ROB for Register Renaming

ROB entries serve as physical registers:
- Register status table points to ROB entry (not RS)
- Value read from ROB if instruction not committed
- Value read from register file if committed

### Example ROB State

| Entry | Type | Dest | Value | Ready |
|-------|------|------|-------|-------|
| 0 | Reg | R1 | 42 | Yes | ← Head (can commit)
| 1 | Reg | R3 | 17 | Yes |
| 2 | Branch | - | - | No |
| 3 | Reg | R5 | - | No |
| 4 | Store | M[100] | 8 | Yes |
| 5 | Reg | R2 | - | No | ← Tail

### Memory Operations with ROB

Stores:
- Compute address and value
- Hold in ROB until commit
- Only write to memory at commit time
- Allows store speculation and rollback

Loads:
- May need to check pending stores in ROB
- Store-to-load forwarding from ROB

## 📌 Key Points (Optional)
- ROB enables speculation by making all changes reversible until commit.
- In-order commit guarantees precise exceptions and correct memory ordering.

## 🖼️ Recommended Image
- Diagram showing ROB as circular buffer with head/tail pointers and commit process.

## 🔗 Connections
- [[076-Tomasulo Algorithm]]
- [[073-Out of Order Execution Fundamentals]]
- [[085-Speculative Execution and Branch Prediction]]
- [[074-Register Renaming and False Dependencies]]