---
Theme: Filters and Frequency Response Transfer Function and Bode Intuition
Date created: 2026-01-23 21:00
tags: [AnalogLevel, FrequencyResponse, Filters]
---

## 📚 Idea/Concept

A circuit can be treated as a **system** that maps an input x(t) to an output y(t).

In frequency-domain (or phasor domain):
- H(ω) = Y(ω) / X(ω)

Magnitude/phase form:
- H(ω) = |H(ω)| ∠θ(ω)

### What Bode plots show
- Magnitude in dB: 20 log10(|H|)
- Phase in degrees

Bode plots let you see:
- gain vs frequency
- phase shift vs frequency
- bandwidth and stability “feel”

### Poles, zeros, and intuition
- Poles generally make magnitude roll off and add negative phase.
- Zeros generally add magnitude rise (or reduce roll-off) and add positive phase.

Common building blocks:
- 1st-order low-pass: H(s) = 1 / (1 + s/ω_c)
- 1st-order high-pass: H(s) = (s/ω_c) / (1 + s/ω_c)

Cutoff frequency:
- ω_c = 1/RC for basic RC filters
- f_c = ω_c / (2π)

### Resonance (RLC)
An RLC can exhibit a resonant peak near:
- ω_0 = 1 / sqrt(L*C)

Quality factor (conceptually):
- High Q = sharper resonance (more selective)

## 📌 Key Points (Optional)
- Frequency response is how analog design “feels” in practice.
- This is the bridge between circuits and control/systems.

## 🖼️ Recommended Image
![[019-Analog36.png]]
![[019-Analog37.png]]
![[019-Analog37_1.png]]

## 🔗 Connections
- [[018-AC Steady State Sinusoids Phasors Impedance and Power]]
- [[021-Laplace Transform The s Domain Tool for Systems and Circuits]]
- [[020-Fourier Analysis From Time Signals to Spectra]]
- [[027-Real World Analog Effects Parasitics Noise Loading and Bandwidth]]
