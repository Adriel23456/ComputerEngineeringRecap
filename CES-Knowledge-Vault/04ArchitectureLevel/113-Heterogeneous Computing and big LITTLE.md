---
Theme: Heterogeneous Computing and big LITTLE
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, AMP, Heterogeneous]
---

## 📚 Idea/Concept

Heterogeneous computing combines different types of processors (CPUs, GPUs, DSPs, accelerators) in a single system to optimize performance and energy efficiency by matching workloads to the most suitable processing unit.

### Motivation

No single processor type is optimal for all workloads:
- CPUs: Good for serial, control-heavy code
- GPUs: Good for data-parallel computation
- DSPs: Good for signal processing
- Custom accelerators: Optimal for specific tasks

Heterogeneous approach:
- Use the right processor for each task
- Balance performance and power efficiency
- Maximize silicon utilization

### ARM big.LITTLE Architecture

Concept:
- Combine high-performance "big" cores with energy-efficient "LITTLE" cores
- Same ISA (instruction set), different microarchitecture
- Software can run on either type transparently

Configuration Example (Exynos):
```
┌─────────────────────────────────────────┐
│              big.LITTLE SoC             │
│  ┌───────────────┐  ┌───────────────┐  │
│  │   big Cluster │  │ LITTLE Cluster│  │
│  │  ┌───┐ ┌───┐  │  │  ┌───┐ ┌───┐  │  │
│  │  │A76│ │A76│  │  │  │A55│ │A55│  │  │
│  │  └───┘ └───┘  │  │  └───┘ └───┘  │  │
│  │  ┌───┐ ┌───┐  │  │  ┌───┐ ┌───┐  │  │
│  │  │A76│ │A76│  │  │  │A55│ │A55│  │  │
│  │  └───┘ └───┘  │  │  └───┘ └───┘  │  │
│  └───────┬───────┘  └───────┬───────┘  │
│          │      L3 Cache    │          │
│          └────────┬─────────┘          │
│                   │                    │
│              Memory Controller         │
└─────────────────────────────────────────┘
```

### big vs LITTLE Cores

| Characteristic | big Cores | LITTLE Cores |
|---------------|-----------|--------------|
| Performance | High | Moderate |
| Power | High | Low |
| Pipeline | Deep, OoO | Shallow, in-order |
| Cache | Large | Small |
| Use case | Burst workloads | Background tasks |
| Clock | ~2-3 GHz | ~1-2 GHz |

### Usage Models

Cluster Migration:
- All tasks on one cluster at a time
- Switch clusters based on overall load
- Simple but coarse-grained

CPU Migration:
- Individual threads migrate between paired cores
- More responsive than cluster migration
- Requires core pairing

Global Task Scheduling (GTS):
- All cores available simultaneously
- Scheduler assigns tasks to appropriate cores
- Most flexible, used in modern implementations

### ARM DynamIQ

Evolution of big.LITTLE:
- More flexible core combinations
- Shared L3 cache across big and LITTLE
- Per-core voltage/frequency scaling
- Up to 8 cores in any mix

DynamIQ Shared Unit (DSU):
- Manages L3 cache
- Snoop filtering
- Power management
- Connects to memory system

### Asymmetric Speedup with Amdahl's Law

For heterogeneous with n small cores, r equivalent big cores:

$$Speedup = \frac{1}{(1-f) + \frac{f}{r + n - r}} = \frac{1}{(1-f) + \frac{f}{n}}$$

Optimal configuration depends on parallelizable fraction f.

### Intel Hybrid Architecture

Similar concept to big.LITTLE:
- Performance cores (P-cores): High IPC, hyperthreading
- Efficiency cores (E-cores): Lower power, no hyperthreading
- Intel Thread Director: Hardware-assisted scheduling

Example (12th Gen Core):
- 8 P-cores (Golden Cove)
- 8 E-cores (Gracemont)
- Thread Director monitors instruction mix

### Apple Silicon

Extreme heterogeneity:
- Performance cores (Firestorm/Avalanche)
- Efficiency cores (Icestorm/Blizzard)
- GPU cores (integrated)
- Neural Engine (ML accelerator)
- Media engines (video encode/decode)
- Unified memory architecture

### System-on-Chip Integration

Modern SoCs integrate many components:
```
┌─────────────────────────────────────────────────┐
│                    SoC                          │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌───────┐ │
│  │  CPU    │ │   GPU   │ │   NPU   │ │  DSP  │ │
│  │Cluster  │ │         │ │ (Neural)│ │       │ │
│  └────┬────┘ └────┬────┘ └────┬────┘ └───┬───┘ │
│       └──────────┬┴───────────┴──────────┘     │
│             ┌────┴────┐                        │
│             │Interconnect                      │
│             └────┬────┘                        │
│       ┌──────────┴──────────┐                 │
│  ┌────┴────┐  ┌────────────┐│┌────────────┐   │
│  │  DRAM   │  │   Image    │││   Video    │   │
│  │Controller│  │ Processor  │││  Encoder   │   │
│  └─────────┘  └────────────┘│└────────────┘   │
└─────────────────────────────────────────────────┘
```

### Workload Mapping

| Workload | Best Processor | Reason |
|----------|---------------|--------|
| UI rendering | LITTLE/E-core | Low latency, low power |
| Web browsing | Mix | Burst compute + idle |
| Video playback | DSP/Media engine | Dedicated hardware |
| ML inference | NPU/GPU | Parallel computation |
| Gaming | big/P-core + GPU | High performance |
| Background sync | LITTLE/E-core | Power efficiency |

### Scheduling Challenges

Hardware signals:
- Performance counters
- Power measurements
- Temperature sensors
- Instruction mix analysis

Scheduling decisions:
- Which core for each thread?
- When to migrate?
- How to balance load?
- Energy vs performance trade-off

## 📌 Key Points (Optional)
- Heterogeneous computing matches workloads to optimal processors.
- big.LITTLE and similar architectures dominate mobile and increasingly desktop/laptop.

## 🖼️ Recommended Image
- SoC block diagram showing different processing elements and interconnect.

## 🔗 Connections
- [[101-Multiprocessor Types SMP AMP and DSM]]
- [[055-Amdahls Law and Speedup Limits]]
- [[110-GPU Architecture and SIMT Model]]
- [[097-Thread Level Parallelism TLP Fundamentals]]