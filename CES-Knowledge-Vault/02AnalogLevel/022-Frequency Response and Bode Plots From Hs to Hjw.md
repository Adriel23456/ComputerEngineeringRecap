---
Theme: Frequency Response and Bode Plots From Hs to Hjw
Date created: 2026-01-23 21:00
tags: [AnalogLevel, FrequencyResponse, Bode]
---

## 📚 Idea/Concept

Laplace gives H(s). Frequency response is obtained by evaluating on the imaginary axis:

- H(jω) = H(s) with s = jω

Then:
- magnitude |H(jω)|
- phase ∠H(jω)

### Bode plot building blocks intuition
Typical factors and their effect:
- Gain K: magnitude shifts up/down; phase unchanged
- (jω)^N: slope +20N dB/dec, phase +90N degrees
- 1/(jω)^N: slope -20N dB/dec, phase -90N degrees
- (1 + jω/ωz): adds +20 dB/dec after ωz, adds +90° phase transition
- 1/(1 + jω/ωp): adds -20 dB/dec after ωp, adds -90° phase transition
- 2nd-order terms create resonant peaks depending on damping

### What you read off instantly
- bandwidth where gain drops
- cutoff frequency
- resonance and peaking
- phase margin ideas stability hint

## 📌 Key Points (Optional)
- Bode is design by eyesight after practice.
- In real PCBs, parasitics create unexpected poles zeros -> unexpected Bode shapes.

## 🖼️ Recommended Image
![[022-Analog44.png]]
![[022-Analog45.png]]
![[022-Analog46.png]]
![[022-Analog47.png]]

- Your frequency response sheet with factor table and example H(ω).

## 🔗 Connections
- [[021-Laplace Transform The s Domain Tool for Systems and Circuits]]
- [[019-Filters and Frequency Response Transfer Function and Bode Intuition]]
- [[027-Real World Analog Effects Parasitics Noise Loading and Bandwidth]]
