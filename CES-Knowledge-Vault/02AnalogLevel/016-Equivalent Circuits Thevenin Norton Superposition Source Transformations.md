---
Theme: Equivalent Circuits Thevenin Norton Superposition Source Transformations
Date created: 2026-01-23 21:00
tags: [AnalogLevel, BasicCircuits, Simplification]
---

## 📚 Idea/Concept

Equivalent circuits let you replace a complicated network with something simpler **as seen from two terminals**.

### Thevenin Equivalent
Any linear two-terminal network can be replaced by:
- a voltage source V_th in series with R_th

Steps:
1. V_th = open-circuit voltage at the terminals
2. R_th = equivalent resistance seen from the terminals with independent sources turned off:
   - voltage sources -> short
   - current sources -> open
   (or use a test source if dependent sources exist)

### Norton Equivalent
Same network can be replaced by:
- a current source I_n in parallel with R_n

Relations:
- R_n = R_th
- I_n = V_th / R_th

### Superposition (linear circuits only)
Total response = sum of responses from each independent source acting alone.
- Turn off others:
  - voltage source -> short
  - current source -> open

### Source transformation
- V source + series R  <->  I source + parallel R
- I = V/R

## 📌 Key Points (Optional)
- These tools are foundational for amplifier input/output modeling.
- Thevenin/Norton is the “interface abstraction” of circuits.

## 🖼️ Recommended Image
![[016-Analog30.png]]
![[016-Analog31.png]]


## 🔗 Connections
- [[015-Ohms Law and KCL KVL The Core Laws of Circuit Analysis]]
- [[023-Diodes and the PN Junction The First Nonlinear Device]]
- [[026-Analog Building Blocks Op Amps Feedback and Stability Big Picture]]