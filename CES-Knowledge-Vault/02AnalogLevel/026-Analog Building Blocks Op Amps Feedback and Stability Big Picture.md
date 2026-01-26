---
Theme: Analog Building Blocks Op Amps Feedback and Stability Big Picture
Date created: 2026-01-23 21:00
tags: [AnalogLevel, ActiveCircuits, OpAmp, Feedback]
---

## 📚 Idea/Concept

Analog design becomes lego once you recognize core building blocks:
- op-amps
- transistor amplifiers
- filters
- oscillators
- regulators

### Ideal op amp assumptions first pass
- infinite gain
- infinite input resistance no input current
- zero output resistance
- infinite bandwidth

With negative feedback, you get predictable behavior:
- closed-loop gain set by resistors
- linear operation if within limits

### Feedback the deep idea
Feedback trades raw gain for:
- stability
- linearity
- predictable gain
- bandwidth extension

### Real op amp limitations what matters in real life
- finite gain and bandwidth gain-bandwidth product
- input offset voltage
- input bias currents
- slew rate max dv/dt
- output swing and output current limit
- noise

### Stability and frequency response
Feedback systems can oscillate if phase shift accumulates too much.
This is why Bode plots and phase margins are not optional.

## 📌 Key Points (Optional)
- Many analog circuits are really feedback systems.
- Understanding stability early makes you dangerous in a good way.

## 🖼️ Recommended Image
![[026-Analog53.png]]
![[026-Analog54.png]]
![[026-Analog55.png]]
![[026-Analog56.png]]

## 🔗 Connections
- [[019-Filters and Frequency Response Transfer Function and Bode Intuition]]
- [[022-Frequency Response and Bode Plots From Hs to Hjw]]
- [[024-BJTs Current Controlled Amplification and Pi Small Signal Model]]
- [[025-MOSFETs Voltage Controlled Device and Regions of Operation]]
