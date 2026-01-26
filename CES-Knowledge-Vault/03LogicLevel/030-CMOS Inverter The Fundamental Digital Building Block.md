---
Theme: CMOS Inverter The Fundamental Digital Building Block
Date created: 2026-01-23 21:00
tags: [LogicLayer, CMOS, Foundations]
---

## 📚 Idea/Concept

If you understand the **CMOS inverter**, you understand the heart of modern digital circuits.

### Structure
A CMOS inverter uses:
- **PMOS** connected to VDD (pull-up)
- **NMOS** connected to GND (pull-down)
- Both gates tied to the input
- Output is the shared node between them

### Behavior
- Input LOW → PMOS ON, NMOS OFF → output HIGH
- Input HIGH → PMOS OFF, NMOS ON → output LOW

So the inverter “flips” the bit.

### Why CMOS is amazing
- Very low static power: ideally, in steady state, one transistor is OFF so almost no DC current flows.
- Strong logic levels: output is pulled strongly to rails (near VDD or near GND)
- High input impedance: easy to drive many inputs

### Transfer curve and switching threshold
The inverter has a voltage transfer characteristic:
- At low Vin → Vout ≈ VDD
- At high Vin → Vout ≈ 0
- In the middle it switches rapidly

The steep middle region gives:
- high gain
- noise rejection
- clean digital transitions

### Basis for building all gates
By combining pull-up and pull-down networks, you can build NAND, NOR, etc.
Complex logic is built from networks of transistors.

## 📌 Key Points (Optional)
- CMOS inverter is the “Hello World” of VLSI.
- Noise margins depend heavily on the transfer curve shape.

## 🖼️ Recommended Image
![[030-Logic62.png]]
![[030-Logic63.png]]

## 🔗 Connections
- [[029-From Analog Voltages to Digital Bits]]
- [[031-Logic Gates Boolean Algebra and Truth Tables]]
- [[034-Noise Margins Logic Thresholds and Signal Integrity]]
