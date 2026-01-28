---
Theme: RAM and ROM Memory Technologies
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Memory, Hardware]
---

## 📚 Idea/Concept

RAM (Random Access Memory) and ROM (Read-Only Memory) are fundamental memory technologies that differ in volatility, write capability, and use cases, forming the foundation of computer memory systems.

### Memory Classification

```
                    Memory
                      │
        ┌─────────────┴─────────────┐
        │                           │
    Volatile                   Non-Volatile
        │                           │
   ┌────┴────┐              ┌───────┴───────┐
   │         │              │               │
  SRAM     DRAM           ROM            Flash
                           │
              ┌────────────┼────────────┐
              │            │            │
           Mask ROM     PROM        EPROM/EEPROM
```

### SRAM (Static RAM)

Structure: 6-transistor cell
```
       Vdd
        │
    ┌───┴───┐
    │       │
   ─┼─     ─┼─
    │   ×   │
   ─┼─     ─┼─
    │       │
    └───┬───┘
        │
       GND

    Cross-coupled inverters + access transistors
```

Characteristics:
- Fast (1-10 ns access time)
- Low density (6T per bit)
- High power (constant current in stable state)
- No refresh needed
- Expensive

Uses:
- CPU caches (L1, L2, L3)
- Register files
- High-speed buffers

### DRAM (Dynamic RAM)

Structure: 1-transistor, 1-capacitor cell
```
    Word Line
        │
       ─┼─ Access Transistor
        │
        ├───┤├── Capacitor
        │
    Bit Line
```

Characteristics:
- Slower than SRAM (50-100 ns)
- High density (1T1C per bit)
- Requires periodic refresh (capacitor leaks)
- Lower power per bit
- Less expensive

Types:
- SDRAM: Synchronous DRAM
- DDR: Double Data Rate (DDR4, DDR5)
- LPDDR: Low Power DDR (mobile)
- GDDR: Graphics DDR (GPUs)
- HBM: High Bandwidth Memory (3D stacked)

### DRAM Operation

Read:
1. Precharge bit line to Vdd/2
2. Assert word line (enable access transistor)
3. Charge sharing between capacitor and bit line
4. Sense amplifier detects small voltage change
5. Data is destructive read (capacitor discharged)
6. Write back to restore data

Refresh:
- Must refresh every cell every 64 ms (typical)
- Controller cycles through rows
- Consumes bandwidth and power

### DDR Memory Evolution

| Generation | Data Rate | Voltage | Year |
|------------|-----------|---------|------|
| DDR | 200-400 MT/s | 2.5V | 2000 |
| DDR2 | 400-800 MT/s | 1.8V | 2003 |
| DDR3 | 800-1600 MT/s | 1.5V | 2007 |
| DDR4 | 1600-3200 MT/s | 1.2V | 2014 |
| DDR5 | 3200-6400 MT/s | 1.1V | 2021 |

### ROM Types

Mask ROM:
- Data written during manufacturing
- Cannot be changed
- Lowest cost at high volume
- Used for firmware in mass production

PROM (Programmable ROM):
- One-time programmable
- Uses fusible links
- Cannot be erased

EPROM (Erasable PROM):
- UV light erases entire chip
- Can be reprogrammed
- Has quartz window

EEPROM (Electrically Erasable PROM):
- Byte-erasable electrically
- Limited write cycles (~100K)
- Used for BIOS, configuration

### Flash Memory

Evolution of EEPROM with block erase:

NOR Flash:
- Random access read (like RAM)
- Byte-programmable
- Block erase
- Used for code storage (firmware)

NAND Flash:
- Sequential access
- Page-programmable
- Block erase
- Higher density
- Used for mass storage (SSD, USB drives)

### Flash Memory Structure

Floating gate transistor:
```
        Control Gate
            │
    ┌───────┴───────┐
    │ Floating Gate │ ← Trapped electrons
    │   (isolated)  │
    └───────────────┘
    ─────────────────  Oxide
          │
       Channel
    ──┬───────┬──
      │       │
   Source   Drain
```

States:
- Charged floating gate → Logic 0 (high Vt)
- Empty floating gate → Logic 1 (low Vt)

### SLC, MLC, TLC, QLC

Bits per cell:
| Type | Bits/Cell | Levels | Endurance | Speed |
|------|-----------|--------|-----------|-------|
| SLC | 1 | 2 | 100K cycles | Fastest |
| MLC | 2 | 4 | 10K cycles | Fast |
| TLC | 3 | 8 | 3K cycles | Moderate |
| QLC | 4 | 16 | 1K cycles | Slower |

### Memory Comparison

| Property | SRAM | DRAM | Flash |
|----------|------|------|-------|
| Volatile | Yes | Yes | No |
| Speed | Fastest | Fast | Slower |
| Density | Lowest | High | Highest |
| Cost/bit | Highest | Medium | Lowest |
| Power | High | Medium | Low (standby) |
| Endurance | Unlimited | Unlimited | Limited |

### Memory Hierarchy Placement

```
        Speed       Capacity      Cost
          │            │           │
  ┌───────┼────────────┼───────────┼───────┐
  │       ▼            ▼           ▼       │
  │    Registers   (bytes)     Highest    │
  │       │                               │
  │    SRAM Cache  (KB-MB)      High      │
  │       │                               │
  │    DRAM Main   (GB)        Medium     │
  │       │                               │
  │    Flash/SSD   (TB)         Low       │
  │       │                               │
  │    HDD         (TB)        Lowest     │
  └───────────────────────────────────────┘
```

### Emerging Memory Technologies

MRAM (Magnetoresistive):
- Non-volatile, fast, unlimited endurance
- Used in some embedded applications

PCM (Phase Change Memory):
- Non-volatile, faster than Flash
- Higher endurance than Flash
- Intel Optane (discontinued)

ReRAM (Resistive RAM):
- High density potential
- Fast switching
- Research/early production

### DRAM Controller

Manages DRAM complexity:
- Row buffer management
- Refresh scheduling
- Bank interleaving
- Timing constraints (tRCD, tCAS, tRP)

## 📌 Key Points (Optional)
- SRAM for speed (caches), DRAM for capacity (main memory), Flash for persistence.
- Each technology has fundamental trade-offs in speed, density, and cost.

## 🖼️ Recommended Image
- Comparison diagram of SRAM 6T cell vs DRAM 1T1C cell structure.

## 🔗 Connections
- [[086-Memory Hierarchy Principles and Locality]]
- [[087-Cache Memory Organization and Parameters]]
- [[094-Virtual Memory and Paging]]
- [[043-Performance Metrics CPI IPC and Execution Time]]