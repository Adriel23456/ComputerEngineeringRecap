---
Theme: Deadlock Prevention Strategies
Date created: 2026-01-31 12:00
tags: [KernelLevel, Deadlock, Prevention]
---

## 📚 Idea/Concept

Deadlock prevention structurally ensures that at least one Coffman condition can never hold. Unlike avoidance (which checks at runtime), prevention designs the system to make deadlock impossible.

### Strategy Overview

Break any ONE condition → Deadlock impossible

```
┌─────────────────────────────────────────────────────────────┐
│          Breaking Coffman Conditions                        │
├─────────────────────────────────────────────────────────────┤
│  Condition          │  Prevention Strategy                  │
├─────────────────────┼───────────────────────────────────────┤
│  Mutual Exclusion   │  Make resources shareable             │
│  Hold and Wait      │  Request all at once OR release first │
│  No Preemption      │  Allow forced resource takeover       │
│  Circular Wait      │  Impose ordering on resource requests │
└─────────────────────┴───────────────────────────────────────┘
```

### 1. Breaking Mutual Exclusion

Strategy: Make resources shareable through intermediaries.

Example - Print Spooling:
```
Without spooling:               With spooling:
┌─────────┐                    ┌─────────┐
│  Word   │──┐                 │  Word   │──┐
└─────────┘  │                 └─────────┘  │
             │  Direct         ┌─────────┐  │    ┌─────────┐
┌─────────┐  ├─→ Printer       │  Excel  │──┼───→│ Spooler │──→ Printer
│  Excel  │──┤  (Conflict!)    └─────────┘  │    └─────────┘
└─────────┘  │                 ┌─────────┐  │     (Queue)
             │                 │PowerPnt │──┘
┌─────────┐  │                 └─────────┘
│PowerPnt │──┘                 
└─────────┘                    Only spooler touches printer!
                               No mutual exclusion conflict
```

Principle: Use queues and intermediaries to reduce direct resource contention.

### 2. Breaking Hold and Wait

Strategy A: Request ALL resources at start
```
Process must declare everything needed upfront
Granted only if ALL available
Releases ALL when done

Problems:
- Resources held but unused (waste)
- May not know needs in advance
- Can cause long blocking
```

Strategy B: Release before requesting more
```
To request new resource:
1. Release everything currently held
2. Request all needed resources (old + new)
3. Wait until all granted

Problems:
- Complex to implement
- State must be saveable/restorable
- May lose work in progress
```

### 3. Breaking No Preemption

Strategy: Allow system to take resources away

```
If process P requests resource R held by Q:
  Option 1: Take R from Q, give to P
  Option 2: Roll back Q to checkpoint, release R

When works:
- CPU: Easy (save registers, preempt)
- Memory: Possible (swap to disk)

When doesn't work:
- Printer mid-page
- Database transaction mid-commit
- Any resource with partial state
```

Virtualization helps: Save complete device state, restore later.

### 4. Breaking Circular Wait

Strategy: Impose total ordering on resources

```
Assign each resource type a unique number:
R1 = 1 (Keyboard)
R2 = 2 (Disk)
R3 = 3 (Printer)
R4 = 4 (Scanner)

Rule: Process can only request Ri if:
      i > j for all Rj currently held

Example:
- Holding R2 (Disk) → Can request R3 or R4
- Holding R2 (Disk) → CANNOT request R1
```

Why this works:
```
For circular wait P1→R1→P2→R2→...→Pn→Rn→P1:
- P1 holds R1, wants R2 → num(R2) > num(R1)
- P2 holds R2, wants R3 → num(R3) > num(R2)
- ...
- Pn holds Rn, wants R1 → num(R1) > num(Rn)

But this means: num(R1) > num(Rn) > ... > num(R2) > num(R1)
Contradiction! Circle impossible.
```

Limitations:
- Finding good ordering is hard
- May force awkward programming patterns
- Reduces concurrency

### Practical Combinations

Modern OS use combinations:
- Timeouts (break hold-and-wait)
- Priority preemption (break no-preemption)
- Resource ordering conventions (break circular wait)
- Service restart on detection (recovery)

### Summary Table

| Strategy | Breaks | Effectiveness | Cost |
|----------|--------|---------------|------|
| Spooling/Queues | Mutual Exclusion | Good for I/O | Design overhead |
| All-at-once | Hold & Wait | Complete prevention | Poor utilization |
| Preemption | No Preemption | Limited applicability | May lose work |
| Ordering | Circular Wait | Complete prevention | Reduces concurrency |

## 📌 Key Points (Optional)
- Prevention eliminates deadlock by design, not runtime checking.
- Each strategy has tradeoffs in performance and flexibility.
- Ordering resources is often the most practical prevention method.

## 🖼️ Recommended Image
- Diagram showing how resource ordering prevents circular dependency.

## 🔗 Connections
- [[135-Deadlock Fundamentals and Coffman Conditions]]
- [[136-Deadlock Avoidance and Bankers Algorithm]]
- [[134-Synchronization Solutions and Semaphores]]