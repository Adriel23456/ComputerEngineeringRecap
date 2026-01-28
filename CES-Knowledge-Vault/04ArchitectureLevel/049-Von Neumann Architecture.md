---
Theme: Von Neumann Architecture
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Microarchitecture, Fundamentals]
---

## 📚 Idea/Concept

The Von Neumann architecture, proposed by John von Neumann (1903-1957), is the foundational model for most modern computers, characterized by a unified memory space for both instructions and data.

### Core Principle
Single memory stores both program instructions and data in the same sequential address space.

### Main Components

1. Arithmetic Logic Unit (ALU):
   - Performs mathematical operations (add, subtract, multiply)
   - Performs logical operations (AND, OR, XOR, NOT)

2. Control Unit:
   - Coordinates instruction execution
   - Manages data flow between components
   - Generates control signals

3. Unified Memory:
   - Single storage for instructions and data
   - Sequential addressing

4. Input/Output:
   - Peripherals connected via the same bus

### Execution Cycle
Basic sequence: Fetch → Decode → Execute → Store
- Program Counter (PC) updates sequentially after each instruction

### The Von Neumann Bottleneck
Since instructions and data share the same memory bus:
- CPU cannot fetch instruction and data simultaneously
- Memory bandwidth becomes the limiting factor
- This bottleneck drove the development of cache hierarchies

## 📌 Key Points (Optional)
- Simplicity made it the dominant architecture model.
- The bottleneck limitation inspired Harvard and modified Harvard architectures.

## 🖼️ Recommended Image
- Block diagram showing CPU (ALU + Control Unit), unified memory, and single bus.

## 🔗 Connections
- [[050-Harvard Architecture and Modified Harvard]]
- [[054-ISA Instruction Set Architecture Fundamentals]]
- [[086-Memory Hierarchy Principles and Locality]]
- [[045-ALU Architecture Operations Flags and Control Signals]]