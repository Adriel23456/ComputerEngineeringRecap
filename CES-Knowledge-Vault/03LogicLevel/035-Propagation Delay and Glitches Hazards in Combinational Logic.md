---
Theme: Propagation Delay and Glitches Hazards in Combinational Logic
Date created: 2026-01-23 21:00
tags: [LogicLayer, Timing, Hazards]
---

## 📚 Idea/Concept

Real gates do not switch instantly. They have delays.
That creates temporary wrong outputs called **glitches**.

### Propagation delay
- **t_pd** is how long it takes for output to settle after an input change.
Often specified as:
- t_PLH low to high
- t_PHL high to low

### Contamination delay
- **t_cd** is the minimum time after an input change when the output can start changing.
So:
- output is guaranteed stable for at least t_cd
- output is guaranteed settled by t_pd

### Why glitches happen
Different paths through logic have different delays.
If two signals that should change “simultaneously” reach a gate at different times, intermediate logic may briefly compute the wrong value.

### Hazards
- Static hazard: output should remain 1 (or 0) but briefly flips
- Dynamic hazard: output should change once but toggles multiple times

### How engineers reduce hazards
- Add redundant terms in Boolean expression (K-map hazard elimination)
- Balance path delays
- Register outputs (synchronize to a clock)
- Use proper design style for synchronous systems

## 📌 Key Points (Optional)
- Glitches are not “bugs”—they are physics plus unequal delays.
- Sequential logic often hides glitches by sampling only at clock edges.

## 🖼️ Recommended Image
![[035-Logic74.png]]

## 🔗 Connections
- [[036-Synchronization Setup Hold and Clocking Basics]]
- [[033-Combinational Logic Design From Truth Table to Gate Level]]
- [[039-FSM Finite State Machines Moore and Mealy Models]]
