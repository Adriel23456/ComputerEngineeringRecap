---
Theme: Multiprocessor Types SMP AMP and DSM
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, TLP, Multiprocessor]
---

## 📚 Idea/Concept

Multiprocessor systems integrate multiple processing units to achieve higher performance through parallelism. The three main types—SMP, AMP, and DSM—differ in memory organization, symmetry, and programming models.

### SMP (Symmetric Multiprocessing)

Definition:
- Multiple identical processors share a single physical memory
- All processors have equal access to memory and I/O
- Single OS instance manages all processors

Characteristics:
- Uniform Memory Access (UMA): Same latency to all memory
- Shared address space
- Cache coherence required
- Single system image

Architecture:
```
┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐
│ CPU0 │  │ CPU1 │  │ CPU2 │  │ CPU3 │
│  L1  │  │  L1  │  │  L1  │  │  L1  │
└──┬───┘  └──┬───┘  └──┬───┘  └──┬───┘
   │         │         │         │
   └────────┬┴─────────┴┬────────┘
            │  Shared   │
         ┌──┴───────────┴──┐
         │  Shared Memory  │
         └─────────────────┘
```

Advantages:
- Simple programming model
- Load balancing by OS
- All memory equally accessible

Disadvantages:
- Bus/interconnect becomes bottleneck
- Limited scalability (typically <64 processors)
- Cache coherence overhead

Examples:
- Most multi-core desktop/server CPUs
- Intel Xeon multiprocessor systems

### AMP (Asymmetric Multiprocessing)

Definition:
- Processors may differ in capabilities or roles
- Master-slave or specialized processor arrangements
- May run different OS instances or bare-metal

Characteristics:
- Processors have different functions
- May have different ISAs or performance levels
- Heterogeneous computing

Types:
1. Master-Slave: One CPU controls, others execute
2. Heterogeneous: Different processor types (CPU + GPU + DSP)
3. big.LITTLE: Same ISA, different performance/power

Architecture (Heterogeneous):
```
┌────────────┐  ┌────────────┐  ┌──────────┐
│  Big Core  │  │  Big Core  │  │   GPU    │
│  (fast)    │  │  (fast)    │  │          │
└─────┬──────┘  └─────┬──────┘  └────┬─────┘
      │               │              │
┌─────┴──────┐  ┌─────┴──────┐      │
│Little Core │  │Little Core │      │
│(efficient) │  │(efficient) │      │
└─────┬──────┘  └─────┬──────┘      │
      └───────────────┴─────────────┘
                     │
              ┌──────┴──────┐
              │   Memory    │
              └─────────────┘
```

Advantages:
- Optimized for specific workloads
- Power efficiency (use appropriate core)
- Can include specialized accelerators

Disadvantages:
- Complex software management
- Load balancing challenges
- Different programming models per component

Examples:
- ARM big.LITTLE / DynamIQ
- Apple M-series (P-cores + E-cores)
- SoCs with CPU + GPU + NPU

### DSM (Distributed Shared Memory)

Definition:
- Physically distributed memory appears as single shared address space
- Each processor has local memory
- Remote memory access transparent but slower

Characteristics:
- Non-Uniform Memory Access (NUMA)
- Local memory fast, remote memory slow
- Software or hardware address translation
- Scalable to many processors

Architecture:
```
┌─────────────────┐     ┌─────────────────┐
│     Node 0      │     │     Node 1      │
│ ┌─────┐ ┌─────┐ │     │ ┌─────┐ ┌─────┐ │
│ │CPU0 │ │CPU1 │ │     │ │CPU2 │ │CPU3 │ │
│ └──┬──┘ └──┬──┘ │     │ └──┬──┘ └──┬──┘ │
│    └───┬───┘    │     │    └───┬───┘    │
│    ┌───┴───┐    │     │    ┌───┴───┐    │
│    │Memory0│    │     │    │Memory1│    │
│    └───┬───┘    │     │    └───┬───┘    │
└────────┼────────┘     └────────┼────────┘
         │    Interconnect       │
         └───────────┬───────────┘
                     │
                 Network
```

Memory Access Latency (NUMA):
- Local: ~100 ns
- Remote (1 hop): ~150-200 ns
- Remote (2 hops): ~200-300 ns

Advantages:
- Scales to thousands of processors
- Local memory provides fast access
- Cost-effective for large systems

Disadvantages:
- Non-uniform latency complicates programming
- Data placement critical for performance
- Coherence protocols more complex

Examples:
- AMD EPYC (NUMA within socket)
- Intel Xeon Scalable (multi-socket NUMA)
- SGI Origin, HP Superdome

### Comparison Table

| Feature | SMP | AMP | DSM |
|---------|-----|-----|-----|
| Memory | Shared (UMA) | Varied | Distributed (NUMA) |
| Processors | Identical | Different | Identical or varied |
| Scalability | Limited | Moderate | High |
| Programming | Easiest | Complex | Moderate |
| Coherence | Required | Optional | Required |
| Latency | Uniform | Varied | Non-uniform |

### Memory Access Patterns

SMP: Any processor, any address, same time
AMP: Depends on processor type and memory region
DSM: Locality-aware programming critical

## 📌 Key Points (Optional)
- SMP is simplest to program but limited in scale.
- Modern systems often combine approaches (NUMA-aware SMP, heterogeneous AMP).

## 🖼️ Recommended Image
- Three-panel diagram showing SMP, AMP, and DSM memory organizations.

## 🔗 Connections
- [[097-Thread Level Parallelism TLP Fundamentals]]
- [[103-Cache Coherence Problem and Solutions]]
- [[099-Simultaneous Multithreading SMT]]
- [[113-Heterogeneous Computing and big LITTLE]]
- [[102-NUMA Architecture and Memory Placement]]