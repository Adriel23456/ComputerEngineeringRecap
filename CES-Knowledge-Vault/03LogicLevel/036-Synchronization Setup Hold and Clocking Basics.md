---
Theme: Synchronization Setup Hold and Clocking Basics
Date created: 2026-01-23 21:00
tags: [LogicLayer, SequentialLogic, Timing]
---

## 📚 Idea/Concept

Synchronous digital systems use a clock so that:
- combinational logic computes between clock edges
- flip-flops capture results at defined times

This reduces chaos and makes timing analyzable.

### Key timing parameters
For a flip-flop:

- **t_setup**: input must be stable BEFORE the clock edge
- **t_hold**: input must remain stable AFTER the clock edge
- **t_cq** (clock-to-Q): time from clock edge to output change
  - sometimes split into:
    - t_ccq contamination (min)
    - t_pcq propagation (max)

For combinational logic:
- t_pd logic max delay
- t_cd logic min delay

### Setup constraint
To meet setup:
- t_pcq + t_pd_logic + t_setup <= T_clk
Where T_clk is the clock period.

### Hold constraint
To meet hold:
- t_ccq + t_cd_logic >= t_hold

### Why violations are catastrophic
If setup or hold is violated, the flip-flop may enter metastability:
- output may take unpredictable time to settle
- could settle to 0 or 1 randomly
- may propagate bad values downstream

### Practical solutions
- choose slower clock or optimize critical path
- add pipeline stages
- add delay buffers to fix hold
- use synchronizers for async signals

## 📌 Key Points (Optional)
- Setup is about the maximum path delay.
- Hold is about the minimum path delay.
- Timing is the real reason why architecture matters.

## 🖼️ Recommended Image
![[036-Logic75.png]]

## 🔗 Connections
- [[037-D Latches and D Flip Flops Memory at the Bit Level]]
- [[035-Propagation Delay and Glitches Hazards in Combinational Logic]]
- [[039-FSM Finite State Machines Moore and Mealy Models]]
