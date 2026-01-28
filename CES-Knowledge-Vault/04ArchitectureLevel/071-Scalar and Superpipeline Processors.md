---
Theme: Scalar and Superpipeline Processors
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Microarchitecture, Pipeline]
---

## 📚 Idea/Concept

Scalar processors issue one instruction per cycle, while superpipeline processors increase throughput by subdividing pipeline stages to enable higher clock frequencies.

### Scalar Processor

Characteristics:
- Fetches 1 instruction per cycle
- Single functional unit (or one active at a time)
- CPI target ≈ 1 (ideal)
- Simple control logic
- Low power consumption

Operation modes:
- Without multithreading: Single thread execution
- With interleaved MT: Switch threads each cycle
- With blocked MT: Switch on long-latency events

### Superpipeline Concept

Idea: Divide each pipeline stage into sub-stages

Traditional 5-stage: Each stage = 1 clock cycle
Superpipeline: Each stage = 1/K clock cycle (K sub-stages)

Effect:
- Higher clock frequency (stages complete faster)
- More instructions in flight
- Same throughput per cycle, but more cycles per second

### Superpipeline Example

Standard Pipeline (100 MHz):
```
Stage duration: 10 ns
IF(10ns) → ID(10ns) → EX(10ns) → MEM(10ns) → WB(10ns)
```

Superpipeline 2× (200 MHz):
```
Stage duration: 5 ns
IF₁→IF₂ → ID₁→ID₂ → EX₁→EX₂ → MEM₁→MEM₂ → WB₁→WB₂
```

### Superpipeline Benefits

- Higher instruction throughput via frequency
- Launch new instruction every half cycle
- IPC remains ~1, but more cycles per second

### Superpipeline Challenges

More hazard opportunities:
- Branch misprediction penalty increases
- More pipeline stages to flush
- Data hazards span more stages

Pipeline register overhead:
- More registers between stages
- Each register adds delay

Diminishing returns:
- Stage division has physical limits
- Wire delays become significant

### Comparison: Parallelism Types

| Type | Mechanism | Throughput Gain |
|------|-----------|-----------------|
| Temporal (Pipeline) | Overlap stages | ≈ N stages |
| Frequency (Superpipeline) | Faster clock | ≈ K× frequency |
| Spatial (Superscalar) | Multiple pipelines | ≈ M issue width |

### Combined Approaches

Modern processors combine all three:
- Deep pipeline (15-20 stages) for high frequency
- Multiple pipelines (4-8 wide) for superscalar
- Result: High frequency × wide issue

### Scalar with Multithreading

```
Scalar + Interleaved MT:
Cycle:    1   2   3   4   5   6   7   8
Thread:   A   B   A   B   A   B   A   B
          ↑───┘   ↑───┘
      Each thread executes every other cycle

Scalar + Blocked MT:
Cycle:    1   2   3   4   5   6   7   8
Thread:   A   A   A   A   B   B   B   B
                      ↑
                Switch on stall/quantum
```

### Pipeline Depth Trade-offs

| Depth | Frequency | Branch Penalty | Power |
|-------|-----------|----------------|-------|
| Shallow (5-7) | Lower | Low | Lower |
| Medium (10-15) | Medium | Medium | Medium |
| Deep (20+) | Higher | High | Higher |

Intel NetBurst (Pentium 4): 31 stages, very high frequency but high penalties
Modern designs: 12-19 stages, balanced approach

## 📌 Key Points (Optional)
- Superpipelining trades hazard penalty for frequency gain.
- Modern processors balance pipeline depth against branch penalty costs.

## 🖼️ Recommended Image
- Side-by-side comparison of scalar, superpipeline, and superscalar execution patterns.

## 🔗 Connections
- [[065-Pipeline MIPS Five Stage Design]]
- [[078-Superscalar Processor Architecture]]
- [[066-Pipeline Hazards and Classifications]]
- [[068-Control Hazards and Branch Prediction]]