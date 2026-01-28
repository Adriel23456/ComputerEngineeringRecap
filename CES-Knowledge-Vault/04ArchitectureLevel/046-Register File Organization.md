---
Theme: Register File Organization
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Microarchitecture, Datapath]
---

## 📚 Idea/Concept

The register file is a small, fast memory structure within the processor that provides rapid access to frequently used operands, organized with multiple read and write ports to support instruction execution.

### Register File Basics

Purpose:
- Store instruction operands
- Fastest storage in memory hierarchy
- Directly accessed by instructions

Typical configuration:
- 32 registers (MIPS, ARM, RISC-V)
- 32 or 64 bits per register
- 2 read ports, 1 write port

### Register File Structure

```
                  Read addresses     Write address
                    │     │              │
                    ▼     ▼              ▼
              ┌─────┴─────┴──────────────┴─────┐
              │         5-to-32 Decoders       │
              └─────────────┬──────────────────┘
                            │
              ┌─────────────┼─────────────────┐
              │             ▼                 │
              │    ┌───────────────────┐     │
              │    │    R0 (zero)      │     │
              │    ├───────────────────┤     │
              │    │    R1             │     │
              │    ├───────────────────┤     │
              │    │    R2             │     │
              │    ├───────────────────┤     │
              │    │       ...         │     │
              │    ├───────────────────┤     │
              │    │    R31            │     │
              │    └───────────────────┘     │
              │             │                 │
              │    ┌────────┴────────┐       │
              │    │  Read MUXes     │       │
              │    └─────┬──────┬────┘       │
              │          │      │            │
              └──────────┼──────┼────────────┘
                         ▼      ▼
                    Read Data 1  Read Data 2

                    Write Data ─────────▶ (to selected register)
                    Write Enable ───────▶
```

### Read Port Implementation

32-to-1 Multiplexer per read port:
```
     R0 ───▶┌──────────┐
     R1 ───▶│          │
     R2 ───▶│  32:1    │───▶ Read Data
     ...    │   MUX    │
     R31 ──▶│          │
            └─────┬────┘
                  ▲
             Read Address
              (5 bits)
```

### Write Port Implementation

Decoder + Write Enable:
```
     Write Address    Write Enable
          │                │
          ▼                │
     ┌─────────┐          │
     │ 5-to-32 │          │
     │ Decoder │          │
     └────┬────┘          │
          │               │
    ┌─────┼─────┐         │
    │     ▼     │         │
    │  ┌─────┐  │         │
    │  │ AND │◀─┼─────────┘
    │  └──┬──┘  │
    │     │     │
    │     ▼     │
    │  Register │ ◀─── Write Data
    │   Enable  │
    └───────────┘
```

### Special Registers

MIPS Register Conventions:
| Register | Name | Use |
|----------|------|-----|
| $0 | $zero | Constant 0 (hardwired) |
| $1 | $at | Assembler temporary |
| $2-$3 | $v0-$v1 | Return values |
| $4-$7 | $a0-$a3 | Arguments |
| $8-$15 | $t0-$t7 | Temporaries |
| $16-$23 | $s0-$s7 | Saved |
| $24-$25 | $t8-$t9 | More temporaries |
| $26-$27 | $k0-$k1 | Kernel |
| $28 | $gp | Global pointer |
| $29 | $sp | Stack pointer |
| $30 | $fp | Frame pointer |
| $31 | $ra | Return address |

### Hardwired Zero Register

Implementation:
```
Read from R0:
  Always returns 0x00000000

Write to R0:
  Ignored (no effect)
```

Benefits:
- Provides constant zero without immediate
- Simplifies comparisons
- Enables register moves (ADD Rd, Rs, $zero)

### Multi-Port Register File

Superscalar requirements:
- Multiple simultaneous reads (4-8)
- Multiple simultaneous writes (2-4)

Port scaling issues:
- Area: O(ports²)
- Power: O(ports²)
- Access time: Increases with ports

Solutions:
- Banking (divide into sub-files)
- Replication (duplicate for reads)
- Multi-cycle access

### Register File Timing

Read timing:
```
     Address valid ────┬────────────────────────
                       │                        
     Read Data valid ──┼────────┬───────────────
                       │        │               
                    Decode   MUX delay          
                    delay                       
```

Write timing:
```
     Clock ─────────────────────┬───────────
                                │           
     Address, Data valid ───────┤           
                            Setup time      
```

### Register Renaming Considerations

For OoO processors:
- Architectural registers: 32 (visible to programmer)
- Physical registers: 96-256 (internal)

Register Alias Table (RAT):
```
     Architectural Register → Physical Register
     
     R1 → P15
     R2 → P23
     R3 → P7
     ...
```

### Floating-Point Register File

Separate from integer registers:
- MIPS: 32 FP registers (F0-F31)
- May be 32-bit (single) or 64-bit (double)
- Paired for double-precision in some ISAs

### Register Windows (SPARC)

Overlapping register sets:
```
     Current window: r0-r31
     
     ┌──────────────────────────────────┐
     │ Global │  Out  │ Local │   In   │
     │ r0-r7  │r8-r15 │r16-r23│r24-r31 │
     └────────┴───────┴───────┴────────┘
                  │              │
                  └──────┬───────┘
                         │
            ┌────────────▼────────────┐
            │ Next window's In = Out  │
            └─────────────────────────┘
```

Benefits:
- Fast procedure calls (no save/restore)
- Overlapping for parameter passing

### SIMD/Vector Register Files

Wider registers:
- SSE: 128-bit XMM registers (16 registers)
- AVX: 256-bit YMM registers (16 registers)
- AVX-512: 512-bit ZMM registers (32 registers)

### Register File Power

Major power consumer:
- Multiple ports = multiple accesses
- Large number of bit cells

Optimizations:
- Clock gating unused ports
- Banking to reduce active cells
- Multi-Vt cells for leakage

### Implementation Technologies

SRAM-based:
- Standard approach
- 6T cells (or more for multi-port)

Latch-based:
- Faster access
- Higher area
- Used in high-speed designs

## 📌 Key Points (Optional)
- Register file design balances port count, speed, and power.
- Special registers (zero, link) simplify common operations.

## 🖼️ Recommended Image
- Schematic of multi-port register file with decoders and MUXes.

## 🔗 Connections
- [[047-Datapath Components and Organization]]
- [[045-ALU Architecture Operations Flags and Control Signals]]
- [[074-Register Renaming and False Dependencies]]
- [[073-Out of Order Execution Fundamentals]]
- [[054-ISA Instruction Set Architecture Fundamentals]]