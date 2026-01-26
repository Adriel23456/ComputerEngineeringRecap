---
Theme: AC Steady State Sinusoids Phasors Impedance and Power
Date created: 2026-01-23 21:00
tags: [AnalogLevel, ACAnalysis, Phasors]
---

## 📚 Idea/Concept

When signals are sinusoidal and the circuit is linear, you can replace calculus with algebra using **phasors**.

### Sinusoids
- v(t) = V_m cos(ωt + θ_v)
- i(t) = I_m cos(ωt + θ_i)

### Euler’s identity (the bridge)
- e^(jφ) = cos(φ) + j sin(φ)

### Phasor representation
A sinusoid can be represented as a complex constant:
- V = V_m ∠θ_v
- I = I_m ∠θ_i

(You can also use RMS phasors; just be consistent.)

### Impedance (AC “resistance”)
- Z = V/I  (ohms)

Component impedances:
- Resistor: Z_R = R
- Inductor: Z_L = jωL
- Capacitor: Z_C = 1/(jωC)

Admittance:
- Y = 1/Z = G + jB  (siemens)

### RMS values (very common in power)
- V_rms = V_m / sqrt(2)
- I_rms = I_m / sqrt(2)

### Power in AC (big idea)
- Complex power: S = V_rms * I_rms*  (conjugate)
- Real power: P (watts)  -> actually consumed
- Reactive power: Q (VAR) -> energy sloshing in L/C
- Apparent power: |S| (VA)

Power factor:
- PF = cos(θ_v - θ_i)

## 📌 Key Points (Optional)
- In steady-state sinusoidal analysis: derivatives become multiplication by jω.
- This is why Z_L and Z_C show up so clean.

## 🖼️ Recommended Image
![[018-Analog34.png]]
![[018-Analog35.png]]

## 🔗 Connections
- [[017-Passive Components R L C Ideal vs Real Models]]
- [[019-Filters and Frequency Response Transfer Function and Bode Intuition]]
- [[020-Fourier Analysis From Time Signals to Spectra]]
