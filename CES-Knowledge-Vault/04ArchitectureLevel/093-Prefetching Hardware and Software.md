---
Theme: Prefetching Hardware and Software
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Cache, Optimization]
---

## 📚 Idea/Concept

Prefetching is a technique that fetches data into cache before it is needed by the processor, hiding memory latency by overlapping data transfer with computation.

### Basic Concept

Without prefetching:
```
Compute → Request A → Wait → Use A → Compute → Request B → Wait → Use B
          ←──────latency──────→          ←──────latency──────→
```

With prefetching:
```
Compute → Prefetch A → Use A → Prefetch B → Use B
          └─────────────┘       └─────────────┘
          Overlap latency       Overlap latency
```

### Prefetch Effectiveness Requirements

Useful prefetch:
- Timely: Arrives before needed
- Accurate: Data actually needed
- Not harmful: Doesn't evict useful data

### Hardware Prefetching

Automatic, no software changes required.

#### Sequential Prefetching

On miss, prefetch next N blocks:
```
Miss on block B → Prefetch B+1, B+2, ..., B+N
```

Simple and effective for sequential access.

Degree: Number of blocks prefetched (1, 2, 4, ...)

#### Stride Prefetching

Detect and predict strided access patterns:
```
Access: A, A+8, A+16, A+24...
Detected stride: 8
Prefetch: A+32, A+40, ...
```

Reference Prediction Table (RPT):
- Tracks recent addresses per PC
- Computes stride
- Generates prefetches

#### Stream Buffers

Dedicated buffers for prefetched data:
```
┌───────────────────────────────────────┐
│              L1 Cache                 │
└───────────────┬───────────────────────┘
                │
        ┌───────┴───────┐
        │               │
┌───────▼──────┐  ┌─────▼────────┐
│ Stream Buffer│  │ Stream Buffer│
│    (4-8 entries) │ (4-8 entries) │
└──────────────┘  └──────────────┘
```

On L1 miss:
1. Check stream buffers
2. If hit: Promote to L1, advance stream
3. If miss: Start new stream

Multiple streams for multiple access patterns.

#### Correlation-Based Prefetching

Track sequences of misses:
- A followed by B followed by C
- On A miss, prefetch B and C

Markov prefetcher:
- Build probability graph of miss sequences
- Prefetch likely successors

### Software Prefetching

Compiler or programmer inserts prefetch instructions.

#### Explicit Prefetch Instructions

```c
// Without prefetch
for (i = 0; i < n; i++) {
    sum += A[i];
}

// With software prefetch
for (i = 0; i < n; i++) {
    __builtin_prefetch(&A[i + DISTANCE]);
    sum += A[i];
}
```

DISTANCE chosen to match memory latency.

#### Prefetch Distance Calculation

$$\text{Distance} = \frac{\text{Memory Latency}}{\text{Loop Iteration Time}}$$

Example:
- Memory latency: 100 cycles
- Loop body: 10 cycles
- Distance: 100/10 = 10 iterations ahead

### Types of Prefetch

Binding prefetch:
- Loads directly into register
- Must arrive before use
- Fails if data not ready

Non-binding prefetch:
- Loads into cache
- Regular load later
- More flexible timing

Hint vs demand:
- Hint: May be ignored under pressure
- Demand: Treated like regular miss

### Prefetch Placement

| Location | Pros | Cons |
|----------|------|------|
| Into L1 | Fastest access | May pollute L1 |
| Into L2 | Less pollution | Higher latency |
| Into buffer | No pollution | Extra structure |

### Prefetch Timeliness

```
Too early: Prefetched data evicted before use
Too late: Data not ready when needed

      ←── Too Early ──┼── Timely ──┼── Too Late ──→
                      │            │
    Prefetched data   │  Perfect   │  Still
    may be evicted    │  overlap   │  waiting
```

### Prefetch Accuracy

$$\text{Accuracy} = \frac{\text{Useful Prefetches}}{\text{Total Prefetches}}$$

Consequences of inaccuracy:
- Wasted bandwidth
- Cache pollution
- Possible negative performance

### Prefetch Coverage

$$\text{Coverage} = \frac{\text{Prefetched Misses}}{\text{Total Original Misses}}$$

High coverage = prefetcher catches most misses.

### Throttling

Adaptive prefetch aggressiveness:
- Monitor accuracy and coverage
- Reduce prefetching if polluting cache
- Increase if bandwidth available

Signals for throttling:
- Cache miss rate
- Prefetch accuracy
- Memory bandwidth utilization

### Modern Hardware Prefetchers

Intel:
- Instruction pointer-based stride
- DCU prefetcher (L1)
- IP prefetcher (L2)
- Spatial prefetcher
- Adjacent cache line prefetcher

ARM:
- Stride detection
- Multiple stream tracking
- Software prefetch instructions (PRFM)

### Prefetching for Different Access Patterns

| Pattern | Technique | Example |
|---------|-----------|---------|
| Sequential | Next-line | Array scan |
| Strided | Stride detection | Matrix column |
| Linked list | Software prefetch | Pointer chasing |
| Irregular | Correlation, SW hints | Graph traversal |

### Software Prefetch Best Practices

1. Prefetch far enough ahead (cover latency)
2. Avoid prefetch in tight loops (instruction overhead)
3. Use non-temporal hints for streaming data
4. Profile to verify benefit

```c
// Unrolled loop with prefetch
for (i = 0; i < n; i += 4) {
    __builtin_prefetch(&A[i + 32]);
    sum += A[i] + A[i+1] + A[i+2] + A[i+3];
}
```

## 📌 Key Points (Optional)
- Effective prefetching requires balancing timeliness, accuracy, and coverage.
- Hardware prefetching handles common patterns; software prefetching enables control.

## 🖼️ Recommended Image
- Diagram showing prefetch timing relative to memory access and computation.

## 🔗 Connections
- [[087-Cache Memory Organization and Parameters]]
- [[090-Average Memory Access Time AMAT]]
- [[089-Cache Miss Types Three Cs]]
- [[086-Memory Hierarchy Principles and Locality]]
- [[092-Victim Cache and Miss Reduction]]