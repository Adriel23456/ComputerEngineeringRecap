---
Theme: From Analog Voltages to Digital Bits
Date created: 2026-01-23 21:00
tags: [LogicLayer, DigitalElectronics, Foundations]
---

## 📚 Idea/Concept

Digital circuits are built on **analog physics**. A “bit” is not a magical thing—it's an **analog voltage range** interpreted as either 0 or 1.

### Logic levels as voltage ranges
A digital input does not read an exact voltage; it reads **ranges**:

- **VIL**: maximum voltage still guaranteed to be read as LOW (0)
- **VIH**: minimum voltage guaranteed to be read as HIGH (1)
- Between VIL and VIH is the **undefined region** (could be read as either)

A digital output has guaranteed ranges:
- **VOL**: maximum output voltage when driving LOW
- **VOH**: minimum output voltage when driving HIGH

### Noise margin is your safety buffer
Noise margin measures how much noise you can tolerate without flipping a bit:
- **NML = VIL − VOL**
- **NMH = VOH − VIH**

Bigger margins = more reliable systems.

### Why we can stack gates and build computers
Because outputs are guaranteed to fall into ranges that inputs can correctly interpret (with margin), we can **cascade** logic reliably, even with noise, temperature variation, and manufacturing tolerances.

### What creates digital behavior from analog parts
The core physical trick is **nonlinearity** + **gain**:
- Transistors (BJTs or MOSFETs) can act like **switches**
- With enough gain, the circuit “snaps” toward a stable LOW or HIGH
- This produces **regeneration** and **cleaning** of signals

### Threshold concept
Inside every logic gate is some threshold-like behavior:
- If input crosses a boundary, the output flips state quickly
- This is why digital transitions can be sharp even if input edges are slow-ish (up to a point)

## 📌 Key Points (Optional)
- Digital design is the art of ensuring analog reality stays inside digital promises.
- Noise margins and timing constraints are the “contracts” of digital circuits.

## 🧾 Resources (Optional)
- Look up typical VIH/VIL/VOH/VOL specs for TTL vs CMOS families.
- Study how an inverter is built using CMOS transistors.

## 🔗 Connections
- [[031-Logic Gates Boolean Algebra and Truth Tables]]
- [[034-Noise Margins Logic Thresholds and Signal Integrity]]
- [[030-CMOS Inverter The Fundamental Digital Building Block]]
