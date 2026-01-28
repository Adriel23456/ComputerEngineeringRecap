---
Theme: Warehouse Scale Computers and HPC
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Systems, HPC]
---

## 📚 Idea/Concept

Warehouse-Scale Computers (WSCs) and High-Performance Computing (HPC) systems represent the extreme end of computer architecture, deploying thousands to millions of processors to tackle the world's most demanding computational problems.

### Warehouse-Scale Computers

Definition:
- Data center as a single computer
- Tens of thousands of servers
- Designed for internet services
- Optimized for throughput and cost

Characteristics:
- Commodity hardware at scale
- Software-defined infrastructure
- Redundancy handles failures
- Optimize for total cost of ownership (TCO)

Examples:
- Google data centers
- Amazon AWS
- Microsoft Azure
- Facebook infrastructure

### WSC Architecture Hierarchy

```
┌─────────────────────────────────────────────────────────────┐
│                    Data Center                               │
│  ┌─────────────────────────────────────────────────────┐   │
│  │                    Cluster                           │   │
│  │  ┌─────────────────────────────────────────────┐    │   │
│  │  │                   Rack                       │    │   │
│  │  │  ┌───────┐ ┌───────┐ ┌───────┐ ┌───────┐   │    │   │
│  │  │  │Server │ │Server │ │Server │ │Server │   │    │   │
│  │  │  │┌─────┐│ │┌─────┐│ │┌─────┐│ │┌─────┐│   │    │   │
│  │  │  ││ CPU ││ ││ CPU ││ ││ CPU ││ ││ CPU ││   │    │   │
│  │  │  │└─────┘│ │└─────┘│ │└─────┘│ │└─────┘│   │    │   │
│  │  │  └───────┘ └───────┘ └───────┘ └───────┘   │    │   │
│  │  │                    │                        │    │   │
│  │  │              Top-of-Rack Switch             │    │   │
│  │  └─────────────────────────────────────────────┘    │   │
│  │                         │                           │   │
│  │                 Aggregation Switch                  │   │
│  └─────────────────────────────────────────────────────┘   │
│                            │                               │
│                      Core Network                          │
└─────────────────────────────────────────────────────────────┘
```

### HPC Systems

Definition:
- Supercomputers optimized for floating-point performance
- Solve scientific and engineering problems
- Measured in FLOPS (Floating Point Operations Per Second)

Applications:
- Climate modeling
- Molecular dynamics
- Nuclear simulations
- Astrophysics
- Drug discovery
- AI training

### HPC Architecture Types

Cluster:
- Commodity nodes connected by network
- Message passing (MPI)
- Cost-effective

MPP (Massively Parallel Processor):
- Custom interconnect
- Proprietary design
- Highest performance

Vector supercomputers:
- Traditional HPC architecture
- Long vector registers
- Still used for specific workloads

### Performance Metrics

Peak Performance:
$$P_{peak} = Cores \times Frequency \times FLOPS/cycle$$

Sustained Performance:
- Actual performance on real applications
- Often 5-20% of peak
- Limited by memory bandwidth, communication

LINPACK Benchmark:
- Dense linear algebra (Ax = b)
- Used for TOP500 ranking
- Not representative of all workloads

### Top500 List

Rankings of world's fastest supercomputers:
- Updated twice yearly (June, November)
- Ranked by LINPACK performance
- Measured in PFLOPS (10¹⁵ FLOPS)

Recent leaders (as of knowledge cutoff):
| System | Performance | Location |
|--------|-------------|----------|
| Frontier | 1.2 EFLOPS | Oak Ridge, USA |
| Fugaku | 442 PFLOPS | RIKEN, Japan |
| LUMI | 309 PFLOPS | Finland |
| Leonardo | 250 PFLOPS | Italy |

### Green500 List

Focuses on energy efficiency:
- Performance per watt (GFLOPS/W)
- Important for operational costs
- Environmental considerations

Efficiency improvements:
- 2008: ~0.5 GFLOPS/W
- 2023: ~50+ GFLOPS/W

### Power and Cooling

Major concerns:
- Electricity costs dominate TCO
- Cooling infrastructure expensive
- PUE (Power Usage Effectiveness) metric

$$PUE = \frac{Total\ Facility\ Power}{IT\ Equipment\ Power}$$

- PUE = 1.0: Perfect (all power to computing)
- Typical WSC: 1.1-1.2
- Older data centers: 1.5-2.0

Cooling techniques:
- Air cooling (traditional)
- Liquid cooling (direct to chip)
- Immersion cooling
- Free cooling (outside air)

### Interconnect Technologies

| Technology | Bandwidth | Latency | Use Case |
|------------|-----------|---------|----------|
| Ethernet | 100-400 Gbps | ~1-10 μs | WSC |
| InfiniBand | 200-400 Gbps | ~1 μs | HPC |
| Slingshot | 200 Gbps | Sub-μs | HPC (HPE) |
| Custom (Google) | Varies | Optimized | WSC |

Network topology:
- Fat tree
- Dragonfly
- Torus
- Hypercube

### Reliability at Scale

With 100,000 servers:
- Component failures are constant
- Mean Time Between Failures (system) ≈ hours

Solutions:
- Software handles failures
- Checkpointing
- Redundant data (replication)
- Graceful degradation

### Programming Models

WSC:
- MapReduce / Spark
- Microservices
- Container orchestration (Kubernetes)
- Request-level parallelism

HPC:
- MPI (Message Passing Interface)
- OpenMP (shared memory)
- CUDA / OpenCL (GPUs)
- Task-based models

### Accelerators in HPC

Modern HPC increasingly uses:
- GPUs (NVIDIA A100, H100)
- Custom accelerators
- FPGAs

Example: Frontier
- AMD EPYC CPUs
- AMD Instinct MI250X GPUs
- 4 GPUs per node
- >9,400 nodes

### Future Trends

Exascale computing:
- 10¹⁸ FLOPS achieved (Frontier, 2022)
- Power challenges (~20-30 MW)
- New architectures needed

Emerging technologies:
- Neuromorphic computing
- Quantum computing (hybrid)
- Photonic interconnects
- 3D stacked memory

## 📌 Key Points (Optional)
- Scale brings unique challenges in reliability, power, and programming.
- Efficiency (FLOPS/W) increasingly important alongside raw performance.

## 🖼️ Recommended Image
- Diagram of data center hierarchy from chip to building.

## 🔗 Connections
- [[101-Multiprocessor Types SMP AMP and DSM]]
- [[110-GPU Architecture and SIMT Model]]
- [[109-Arithmetic Intensity and Roofline Model]]
- [[058-Benchmarking Latency and Throughput Metrics]]
- [[086-Memory Hierarchy Principles and Locality]]