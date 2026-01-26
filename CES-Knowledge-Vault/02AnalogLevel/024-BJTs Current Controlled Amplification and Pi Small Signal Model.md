---
Theme: BJTs Current Controlled Amplification and Pi Small Signal Model
Date created: 2026-01-23 21:00
tags: [AnalogLevel, ActiveDevices, BJT, Amplifiers]
---

## 📚 Idea/Concept

A BJT Bipolar Junction Transistor is a three-terminal device:
- Emitter E, Base B, Collector C
Types: NPN and PNP.

### Big idea
A small base-emitter action controls a larger collector current:
- small input controls large output -> amplification

### Regions of operation
- Cutoff off
- Active amplification region
- Saturation fully on switching

### Small signal model Pi model
For amplifier design, you replace the transistor with a linearized model around a bias point:
- r_pi between base and emitter
- g_m*v_pi controlled current source from collector to emitter

This lets you calculate:
- gain
- input resistance
- output resistance
- bandwidth with capacitances

### Why biasing matters
Amplification only works predictably if the transistor sits in the correct region active.

## 📌 Key Points (Optional)
- Analog is basically choose bias point + analyze small variations.
- The small signal model is the circuit engineering version of local linear approximation.

## 🖼️ Recommended Image
![[024-Analog50.png]]
![[024-Analog51.png]]

## 🔗 Connections
- [[015-Ohms Law and KCL KVL The Core Laws of Circuit Analysis]]
- [[019-Filters and Frequency Response Transfer Function and Bode Intuition]]
- [[026-Analog Building Blocks Op Amps Feedback and Stability Big Picture]]
- [[027-Real World Analog Effects Parasitics Noise Loading and Bandwidth]]
