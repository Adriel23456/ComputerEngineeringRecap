---
Theme: Circuit Symbols Schematics and What a Circuit Even Is
Date created: 2026-01-23 21:00
tags: [AnalogLevel, BasicCircuits, Foundations]
---

## 📚 Idea/Concept

A circuit is a **closed network** of electrical elements where **voltage** (energy per charge) drives **current** (charge flow). Before math, you need the “language” of circuits: **schematics** and **symbols**.

A schematic is NOT the physical layout. It’s a **logical graph**:
- nodes = connection points (same voltage)
- branches = components between nodes
- loops = closed paths used in KVL

### Common symbols (must be instant-recognition)
- Resistor (R), Capacitor (C), Inductor (L)
- Voltage source (DC/AC), Current source (DC/AC)
- Ground (reference node)
- Switch, diode, LED
- BJT (NPN/PNP), MOSFET (NMOS/PMOS)
- Op-amp, transformer, etc.

### Polarity and sign conventions (super important)
- Passive sign convention: if current enters the terminal marked “+”, then power p = v*i is **positive** (element absorbs power).
- Conventional current flows from higher potential to lower potential (even though electrons move opposite).

## 📌 Key Points (Optional)
- A **node** is a set of points connected by ideal wire -> same voltage.
- “Ground” is a **reference**, not magical zero everywhere.
- A schematic is basically a **graph problem** (this is very CSE-coded).

## 🖼️ Recommended Image
![[014-Analog27.png]]

## 🔗 Connections
- [[Ohm’s Law + KCL/KVL The Core Laws of Circuit Analysis]]
- [[017-Passive Components R L C Ideal vs Real Models]]
- [[023-Diodes and the PN Junction The First Nonlinear Device]]
- [[024-BJTs Current Controlled Amplification and Pi Small Signal Model]]
- [[025-MOSFETs Voltage Controlled Device and Regions of Operation]]