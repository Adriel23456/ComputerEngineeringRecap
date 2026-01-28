---
Theme: RISC-V Architecture and Design Philosophy
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, ISA, RISCV]
---

## 📚 Idea/Concept

RISC-V is a free, open-source Instruction Set Architecture (ISA) designed for simplicity, modularity, and extensibility, becoming a major alternative to proprietary ISAs like x86 and ARM.

### Origins and History

- 2010: Initiated as research project at UC Berkeley
- Motivation: Avoid complexity, licensing issues of commercial ISAs
- Design philosophy: Define only the ISA specification, not implementation
- Two specification levels: User and Privileged
- User profile frozen at version 2.2 (May 2014)
- Privileged specification at version 1.11
- Governed by RISC-V Foundation (100+ member companies)

### Key Characteristics

Simplicity:
- Smaller, cleaner specification than commercial ISAs
- Easier to learn, implement, and verify

Modularity:
- Minimal base ISA (RV32I, RV64I, RV128I)
- Optional standard extensions
- Custom extensions supported

Microarchitecture-agnostic:
- Same ISA for tiny embedded cores to high-performance superscalars
- Implementation freedom

### Naming Convention

Format: RV<'XLEN><'Extensions>

- RV: RISC-V identifier
- XLEN: Register width (32, 64, or 128)
- Extensions: Feature letters

Base ISAs:
- RV32I: 32-bit integer base
- RV64I: 64-bit integer base
- RV128I: 128-bit integer base

### Standard Extensions

| Letter | Extension | Description |
|--------|-----------|-------------|
| M | Multiply | Integer multiplication/division |
| A | Atomic | Atomic memory operations |
| F | Float | Single-precision floating-point |
| D | Double | Double-precision floating-point |
| C | Compressed | 16-bit compressed instructions |
| G | General | Shorthand for IMAFD |

Examples:
- RV32I: Basic 32-bit implementation
- RV64IMAC: 64-bit with multiply, atomics, compressed
- RV64GC: 64-bit general-purpose with compressed

### Register File

Integer Registers (32):
- x0: Hardwired to zero
- x1 (ra): Return address
- x2 (sp): Stack pointer
- x3-x31: General purpose

Floating-Point Registers (optional, 32):
- f0-f31: FP operands

### Instruction Formats

Fixed 32-bit instructions (base), naturally aligned:
- R-type: Register operations
- I-type: Immediate operations
- S-type: Store operations
- U-type: Upper immediate
- B-type: Branch operations
- J-type: Jump operations

### Addressing Modes

- Base + displacement: `lw rd, offset(rs1)`
- PC-relative: `beq rs1, rs2, offset`
- Register indirect: `jalr rd, rs1, 0`

### Ecosystem

Major adopters: Google, NVIDIA, Western Digital, Samsung, Qualcomm, SiFive, Andes

Applications: Embedded systems, IoT, AI accelerators, data centers, education

## 📌 Key Points (Optional)
- RISC-V's openness enables innovation without licensing barriers.
- Modularity allows tailored implementations from microcontrollers to servers.

## 🖼️ Recommended Image
- Diagram showing RISC-V modular extension architecture.

## 🔗 Connections
- [[054-ISA Instruction Set Architecture Fundamentals]]
- [[060-CISC vs RISC Design Philosophy]]
- [[061-Addressing Modes and Effective Address Calculation]]
- [[065-Pipeline MIPS Five Stage Design]]