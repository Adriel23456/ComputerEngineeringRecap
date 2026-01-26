---
Theme: Noise Margins Logic Thresholds and Signal Integrity
Date created: 2026-01-23 21:00
tags: [LogicLayer, SignalIntegrity, Noise]
---

## 📚 Idea/Concept

Digital systems fail when analog reality violates digital assumptions.

### Noise sources
- switching noise ground bounce
- coupled noise capacitive or inductive coupling
- power supply ripple
- electromagnetic interference EMI
- crosstalk between traces

### Noise margin revisited
Using logic thresholds:
- NML = VIL − VOL
- NMH = VOH − VIH

If noise pushes a “guaranteed” signal into the undefined region, the receiver may interpret incorrectly.

### Why undefined region is dangerous
Inside the undefined region:
- small changes can flip the gate output
- internal transistors may partially conduct
- power increases and timing becomes unpredictable

### Common protection techniques
- good decoupling capacitors near ICs
- solid ground plane and short return paths
- series resistors for edge control when needed
- Schmitt trigger inputs for noisy slow edges
- proper termination for fast long traces

### Schmitt trigger idea
A Schmitt trigger adds hysteresis:
- different thresholds for rising and falling edges
This prevents bouncing around the threshold.

## 📌 Key Points (Optional)
- Signal integrity is a system-level problem: device + PCB + power + environment.
- Clean power and good grounding are digital reliability multipliers.

## 🖼️ Recommended Image
![[034-Logic73.png]]

## 🔗 Connections
- [[029-From Analog Voltages to Digital Bits]]
- [[036-Synchronization Setup Hold and Clocking Basics]]
- [[035-Propagation Delay and Glitches Hazards in Combinational Logic]]
