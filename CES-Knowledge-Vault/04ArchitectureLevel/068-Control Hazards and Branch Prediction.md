---
Theme: Control Hazards and Branch Prediction
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, Pipeline, BranchPrediction]
---

## 📚 Idea/Concept

Control hazards occur when the pipeline must make instruction fetch decisions before a branch outcome is known, potentially fetching and partially executing wrong-path instructions.

### The Problem

Branch instructions create uncertainty:
- Target address unknown until decode/execute
- Branch condition unknown until execute
- Pipeline has already fetched subsequent instructions

Without mitigation: Stall pipeline for 1+ cycles per branch

### Branch Penalty

Penalty depends on pipeline depth and when branch resolves:
- Resolve in ID: 1 cycle penalty
- Resolve in EX: 2 cycle penalty
- Deeper pipelines: Higher penalties

### Static Prediction Strategies

Always Not-Taken:
- Assume branch falls through
- Continue fetching sequential instructions
- Flush if branch taken
- Works well for loops (backwards) but not for conditionals

Always Taken:
- Assume branch will be taken
- Requires early target calculation
- Good for backward branches (loops)

Compiler Hints:
- `__builtin_expect()` in GCC
- Hint bits in instruction encoding
- Profile-guided optimization

### Dynamic Prediction

1-Bit Predictor:
- Remember last outcome
- Predict same as last time
- Problem: Alternating patterns cause 100% misprediction

2-Bit Saturating Counter:
- States: Strongly Not-Taken, Weakly Not-Taken, Weakly Taken, Strongly Taken
- Requires two consecutive mispredictions to change prediction
- Tolerates occasional anomalies

Correlating Predictors:
- Consider history of recent branches
- Global history register (GHR)
- Better for correlated branch patterns

Tournament Predictors:
- Multiple prediction mechanisms
- Selector chooses best predictor per branch
- Used in modern high-performance processors

### Branch Target Buffer (BTB)

Cache storing:
- Branch instruction addresses
- Predicted target addresses
- Prediction history

Allows target fetch in same cycle as prediction

### Pipeline Flush

When prediction wrong:
- Discard all speculative instructions
- Restore architectural state
- Restart from correct path
- Penalty proportional to pipeline depth

### Modern Techniques

- Branch Target Prediction
- Return Address Stack (for function returns)
- Indirect Branch Prediction
- Neural Branch Predictors

## 📌 Key Points (Optional)
- Modern predictors achieve >95% accuracy on typical workloads.
- Misprediction penalty increases with pipeline depth, motivating sophisticated predictors.

## 🖼️ Recommended Image
- State diagram of 2-bit saturating counter predictor.

## 🔗 Connections
- [[066-Pipeline Hazards and Classifications]]
- [[085-Speculative Execution and Branch Prediction]]
- [[065-Pipeline MIPS Five Stage Design]]
- [[073-Out of Order Execution Fundamentals]]