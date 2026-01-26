---
Theme: Real World Analog Effects Parasitics Noise Loading and Bandwidth
Date created: 2026-01-23 21:00
tags: [AnalogLevel, PracticalAnalog, PCB, Noise]
---

## 📚 Idea/Concept

Textbook circuits assume ideal wires and ideal components. Real analog design is dominated by hidden effects.

### Parasitics
- Every trace has resistance R, inductance L, capacitance C
- Components have ESR ESL and internal capacitances
- At high frequency, a wire stops acting like a wire

### Loading
When you connect stages:
- output impedance of stage A and input impedance of stage B form a divider
- this can reduce gain and shift cutoff frequencies

### Noise unavoidable
Common types:
- thermal noise resistors
- flicker noise 1/f in devices
- interference picked up by loops and long traces

### Bandwidth limits
Even if your math says works, real bandwidth may be limited by:
- op-amp GBW and slew rate
- transistor capacitances Miller effect
- PCB parasitics and layout

### EMI EMC mindset
Fast edges and loops radiate.
Good layout reduces:
- loop area
- ground bounce
- coupling between sensitive and noisy sections

## 📌 Key Points (Optional)
- Analog success is often 50 percent analysis + 50 percent layout discipline.
- This note is the bridge into PCB design quality.

## 🖼️ Recommended Image
![[027-Analog57.png]]
![[027-Analog58.png]]


## 🔗 Connections
- [[028-PCB Development with Autodesk EAGLE From Schematic to Gerbers]]
- [[019-Filters and Frequency Response Transfer Function and Bode Intuition]]
- [[026-Analog Building Blocks Op Amps Feedback and Stability Big Picture]]
