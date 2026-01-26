---
Theme: Passive Components R L C Ideal vs Real Models
Date created: 2026-01-23 21:00
tags: [AnalogLevel, PassiveCircuits, Components]
---

## 📚 Idea/Concept

Passive components do not create energy; they **store** or **dissipate** it.

## Resistor R
- v = iR
- Power: p = i^2 R = v^2/R (dissipates as heat)

Real resistor non-idealities:
- tolerance (±1%, ±5%)
- temperature coefficient (resistance changes with temperature)
- parasitic inductance/capacitance at high frequencies

## Capacitor C
- i(t) = C * dv(t)/dt
- v(t) = (1/C) * ∫ i(t) dt
- Energy stored: E = (1/2) C v^2

Real capacitor effects:
- ESR (equivalent series resistance)
- leakage current
- ESL (series inductance)
- capacitance varies with voltage/temperature (especially ceramics)

## Inductor L
- v(t) = L * di(t)/dt
- i(t) = (1/L) * ∫ v(t) dt
- Energy stored: E = (1/2) L i^2

Real inductor effects:
- winding resistance (DCR)
- core saturation (inductance drops at high current)
- parasitic capacitance (self-resonance)

## 📌 Key Points (Optional)
- C and L “fight changes”: capacitor fights voltage changes, inductor fights current changes.
- Real components matter a LOT in PCB + high-frequency behavior.

## 🖼️ Recommended Image
![[017-Analog32.png]]
![[017-Analog33.png]]


## 🔗 Connections
- [[018-AC Steady State Sinusoids Phasors Impedance and Power]]
- [[019-Filters and Frequency Response Transfer Function and Bode Intuition]]
- [[027-Real World Analog Effects Parasitics Noise Loading and Bandwidth]]