---
Theme: MOSFETs Voltage Controlled Device and Regions of Operation
Date created: 2026-01-23 21:00
tags: [AnalogLevel, ActiveDevices, MOSFET]
---

## 📚 Idea/Concept

A MOSFET is a 3/4-terminal device:
- Gate G, Drain D, Source S, Body Bulk B
Types: NMOS and PMOS.

### Big idea
The gate voltage controls a channel conductivity:
- voltage-controlled behavior unlike BJT current-controlled input

### Regions NMOS perspective
- Cutoff: V_GS < V_TH -> no channel off
- Triode Linear: V_GS > V_TH and V_DS small -> acts like a voltage-controlled resistor
- Saturation: V_GS > V_TH and V_DS large enough -> current mainly set by V_GS

### Why MOSFETs dominate modern electronics
- Very high input resistance gate is insulated
- Great for CMOS logic and many analog blocks
- Scales well in IC fabrication

### Small signal modeling
Like BJTs, you linearize around a bias point:
- g_m and output resistance r_o become key for gain

## 📌 Key Points (Optional)
- In analog MOS design: bias sets g_m and headroom; parasitic caps define speed.
- In power design: switching losses + gate charge matter.

## 🖼️ Recommended Image
![[025-Analog52.png]]

## 🔗 Connections
- [[023-Diodes and the PN Junction The First Nonlinear Device]]
- [[019-Filters and Frequency Response Transfer Function and Bode Intuition]]
- [[027-Real World Analog Effects Parasitics Noise Loading and Bandwidth]]
