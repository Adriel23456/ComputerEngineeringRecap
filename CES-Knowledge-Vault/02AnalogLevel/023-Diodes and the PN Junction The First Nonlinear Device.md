---
Theme: Diodes and the PN Junction The First Nonlinear Device
Date created: 2026-01-23 21:00
tags: [AnalogLevel, ActiveDevices, Diode]
---

## 📚 Idea/Concept

A diode is a PN junction that conducts mainly in one direction.

### Core behavior
- Forward bias: diode conducts strongly once forward voltage is sufficient.
- Reverse bias: diode blocks small leakage, until breakdown (device-dependent).

### Why it’s nonlinear
The current-voltage relationship is exponential (conceptually):
- i increases rapidly with forward voltage

### What PN junction creates internally
- A depletion region forms at equilibrium.
- Forward bias reduces the barrier -> carriers cross -> current flows.
- Reverse bias increases the barrier -> current is blocked.

### Common diode circuit uses
- Rectifiers AC to DC
- Clipping and clamping
- Protection reverse polarity, flyback diode
- Signal demodulation

### Small signal idea important for analog
Around a bias point, you can linearize the diode and treat it as an incremental resistance.

## 📌 Key Points (Optional)
- Diodes introduce nonlinearity -> harmonics -> Fourier becomes relevant.
- Junction capacitances exist and matter at high frequency.

## 🖼️ Recommended Image
![[023-Analog48.png]]
![[023-Analog49.png]]

## 🔗 Connections
- [[016-Equivalent Circuits Thevenin Norton Superposition Source Transformations]]
- [[020-Fourier Analysis From Time Signals to Spectra]]
- [[027-Real World Analog Effects Parasitics Noise Loading and Bandwidth]]
