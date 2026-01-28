---
Theme: Endianness Little Endian vs Big Endian
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, DataRepresentation, MemoryAccess]
---

## 📚 Idea/Concept

Endianness defines the byte ordering when storing multi-byte values in memory, affecting how data is interpreted across different systems.

### Little-Endian

The least significant byte (LSB) is stored at the lowest memory address.

Example: Value 0x12345678 stored at address 0x100:
- Address 0x100: 0x78 (LSB)
- Address 0x101: 0x56
- Address 0x102: 0x34
- Address 0x103: 0x12 (MSB)

Used in: x86/x64, ARM (programmable mode, default little)

### Big-Endian

The most significant byte (MSB) is stored at the lowest memory address.

Example: Value 0x12345678 stored at address 0x100:
- Address 0x100: 0x12 (MSB)
- Address 0x101: 0x34
- Address 0x102: 0x56
- Address 0x103: 0x78 (LSB)

Used in: Historically PowerPC, SPARC, network protocols (TCP/IP)

### Practical Implications

Network byte order:
- TCP/IP uses big-endian (network byte order)
- Little-endian systems must convert when sending/receiving network data
- Functions: htons(), htonl(), ntohs(), ntohl()

Cross-platform data:
- Binary file formats must specify endianness
- Data exchange between different architectures requires conversion

### Bi-Endian Architectures
Some processors can operate in either mode:
- ARM: Configurable endianness
- PowerPC: Switchable modes
- MIPS: Configurable at reset

## 📌 Key Points (Optional)
- Endianness matters when reading binary data, network protocols, or cross-platform code.
- Most modern desktop/laptop processors are little-endian (x86 dominance).

## 🖼️ Recommended Image
- Memory diagram showing same value stored in both endianness formats side by side.

## 🔗 Connections
- [[054-ISA Instruction Set Architecture Fundamentals]]
- [[062-Memory Alignment and Data Structure Optimization]]
- [[072-RISC-V Architecture and Design Philosophy]]