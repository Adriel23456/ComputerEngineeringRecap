---
Theme: Performance Metrics CPI IPC and Execution Time
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Performance, Metrics]
---

## 📚 Idea/Concept

Performance evaluation requires precise metrics to compare architectures and identify bottlenecks. CPI, IPC, and execution time form the core measurement framework.

### Fundamental Performance Equation

CPU Time = Instruction Count × CPI × Clock Cycle Time

Or equivalently:

CPU Time = (Instruction Count × CPI) / Clock Frequency

### CPI (Cycles Per Instruction)

Average number of clock cycles required to execute one instruction:

CPI = Total Clock Cycles / Instruction Count

Factors affecting CPI:
- Pipeline stalls (hazards)
- Cache misses
- Branch mispredictions
- Instruction mix (different instructions have different latencies)

### IPC (Instructions Per Cycle)

Number of instructions completed per clock cycle:

IPC = Instruction Count / Total Clock Cycles = 1 / CPI

- Scalar processors aim for IPC ≈ 1
- Superscalar processors target IPC > 1
- Real-world IPC often < 1 due to stalls

### Execution Time Components

For memory-intensive workloads:

CPI_effective = CPI_base + (Memory_Access_Rate × Miss_Rate × Miss_Penalty)

Where:
- CPI_base: cycles without memory stalls
- Memory_Access_Rate: fraction of instructions accessing memory
- Miss_Rate: cache miss probability
- Miss_Penalty: cycles to service a miss

### Performance Comparison

Speedup = Performance_new / Performance_old = Time_old / Time_new

### Example Calculation

Given:
- Clock frequency: 3 GHz
- CPI_base: 0.5
- 0.2% of instructions require off-chip access
- Off-chip access time: 200 ns

Remote access cost in cycles:
200 ns / (1/3 GHz) = 200 ns / 0.333 ns = 600 cycles

CPI_effective = 0.5 + (0.002 × 600) = 0.5 + 1.2 = 1.7

Slowdown factor = 1.7 / 0.5 = 3.4×

## 📌 Key Points (Optional)
- Clock frequency alone is misleading; CPI and instruction count matter equally.
- Memory latency often dominates modern performance bottlenecks.

## 🖼️ Recommended Image
- Flow diagram showing how each factor contributes to total execution time.

## 🔗 Connections
- [[055-Amdahls Law and Speedup Limits]]
- [[087-Cache Memory Organization and Parameters]]
- [[065-Pipeline MIPS Five Stage Design]]
- [[058-Benchmarking Latency and Throughput Metrics]]