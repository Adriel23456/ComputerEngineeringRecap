---
Theme: ISA Instruction Set Architecture Fundamentals
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ISA, Fundamentals]
---

## 📚 Idea/Concept

The Instruction Set Architecture (ISA) is the abstract interface between hardware and software, defining what the processor can do and how software communicates with it.

### ISA Components

The ISA specification includes:
- Class of architecture (Register-Memory vs Load-Store)
- Endianness and data alignment requirements
- Addressing modes
- Operand types and sizes
- Operations and control flow
- Instruction encoding format

### Operand Types
Common operand sizes in modern architectures:
- Byte: 8 bits
- Half-Word: 16 bits
- Word: 32 bits (includes single-precision float)
- Double-Word: 64 bits (includes double-precision float)

### Data Encoding
- Integers: Two's complement representation
- Floating Point: IEEE 754 standard
- Characters: ASCII encoding
- Decimal: BCD (Binary-Coded Decimal)

### ISA Classification by Internal Storage

| Class | Description | Advantages | Disadvantages |
|-------|-------------|------------|---------------|
| Stack | Operations on single stack via push/pop | Simple implementation | Stack becomes bottleneck |
| Accumulator | One implicit operand in AC | Small instruction size | High traffic to accumulator |
| Register-Memory | One operand in register, one in memory | Mixed operations | Complex decoding |
| Register-Register | Both operands in GPRs, Load/Store for memory | Simple pipeline, high parallelism | More load/store instructions |

## 📌 Key Points (Optional)
- The ISA is the contract between hardware designers and software developers.
- Modern RISC architectures favor Register-Register (Load/Store) model for pipeline efficiency.

## 🖼️ Recommended Image
- Diagram showing ISA as the interface layer between software and microarchitecture.

## 🔗 Connections
- [[060-CISC vs RISC Design Philosophy]]
- [[061-Addressing Modes and Effective Address Calculation]]
- [[049-Von Neumann Architecture]]
- [[072-RISC-V Architecture and Design Philosophy]]
- [[059-Endianness Little Endian vs Big Endian]]