---
Theme: Hierarchical Reasoning Model HRM
Date created: 2026-02-02 12:00
tags: [AILevel, HRM, Reasoning, BrainInspired, RecurrentNetworks]
---

## 📚 Idea/Concept

The Hierarchical Reasoning Model (HRM) is a novel recurrent architecture inspired by the brain's hierarchical and multi-timescale processing. It achieves significant computational depth while maintaining training stability, solving complex reasoning tasks that defeat Chain-of-Thought methods—using only ~27M parameters and ~1000 training examples, without pretraining.

### The Problem with Current Approaches

**Standard Transformers**:
- Fixed computational depth (number of layers)
- Placed in complexity classes AC⁰ or TC⁰
- Cannot solve problems requiring polynomial time
- Not Turing-complete in end-to-end manner

**Chain-of-Thought (CoT)**:
- Externalizes reasoning into token sequences
- Relies on brittle, human-defined decompositions
- Single misstep can derail entire reasoning
- Requires massive training data
- High latency from generating many tokens

### Brain Inspiration

HRM is inspired by three principles of neural computation:

| Principle | Brain Mechanism | HRM Implementation |
|-----------|-----------------|-------------------|
| Hierarchical Processing | Cortical hierarchy | High-level + Low-level modules |
| Temporal Separation | Theta (4-8Hz) vs Gamma (30-100Hz) | Different update frequencies |
| Recurrent Connectivity | Feedback loops | Iterative state refinement |

### Architecture Overview

```
                    ┌─────────────────┐
                    │   Output Head   │
                    │    fO(zH)       │
                    └────────┬────────┘
                             │
    ┌────────────────────────┼────────────────────────┐
    │                        │                        │
    │  HIGH-LEVEL MODULE     │     Updates every      │
    │       (H)              │      T steps          │
    │   Slow, Abstract       │                        │
    │   Planning             │                        │
    │                        │                        │
    │   zH = fH(zH, zL)      │                        │
    │                        │                        │
    └────────────────────────┼────────────────────────┘
                             │
                             │ (provides context)
                             ▼
    ┌────────────────────────────────────────────────┐
    │                                                │
    │  LOW-LEVEL MODULE      Updates every          │
    │       (L)              timestep               │
    │   Fast, Detailed                              │
    │   Computations                                │
    │                                                │
    │   zL = fL(zL, zH, x̃)                          │
    │                                                │
    └────────────────────────┬───────────────────────┘
                             │
                    ┌────────┴────────┐
                    │ Input Network   │
                    │   x̃ = fI(x)    │
                    └────────┬────────┘
                             │
                         [Input x]
```

### Mathematical Formulation

**Input Processing**:
$$\tilde{x} = f_I(x; \theta_I)$$

**Low-Level Update** (every timestep i):
$$z^i_L = f_L(z^{i-1}_L, z^{i-1}_H, \tilde{x}; \theta_L)$$

**High-Level Update** (every T timesteps):
$$z^i_H = \begin{cases} f_H(z^{i-1}_H, z^{i-1}_L; \theta_H) & \text{if } i \equiv 0 \pmod{T} \\ z^{i-1}_H & \text{otherwise} \end{cases}$$

**Output**:
$$\hat{y} = f_O(z^{NT}_H; \theta_O)$$

### Hierarchical Convergence

The key innovation solving the "early convergence" problem:

**Problem with Standard RNNs**:
- Hidden state quickly converges to fixed point
- Update magnitudes shrink
- Subsequent computation becomes inert
- Effective depth is limited

**HRM Solution**:
1. L-module converges to local equilibrium within each cycle
2. H-module updates after T steps, changing context
3. L-module is effectively "reset" to begin new computation
4. Result: NT effective computational steps vs T for standard RNN

```
Standard RNN:     [Active] → [Converging] → [Inert] → [Inert] → ...
HRM L-module:     [Active] → [Converge] | [Reset/Active] → [Converge] | ...
                            ↑ H updates ↑            ↑ H updates ↑
```

### One-Step Gradient Approximation

**Problem with BPTT**:
- Requires O(T) memory for T timesteps
- Biologically implausible
- Computationally expensive

**HRM Solution**: Based on Deep Equilibrium Models (DEQ) theory:

At fixed point, use Implicit Function Theorem:
$$\frac{\partial z^*_H}{\partial \theta} = (I - J_F)^{-1} \frac{\partial F}{\partial \theta}$$

**1-Step Approximation**: $(I - J_F)^{-1} \approx I$

Gradient path becomes:
```
Output head → final H state → final L state → input embedding
```

Memory: O(1) instead of O(T)

### Implementation (PyTorch Pseudocode)

```python
def hrm(z, x, N=2, T=2):
    x = input_embedding(x)
    zH, zL = z
    
    with torch.no_grad():
        for _i in range(N * T - 1):
            zL = L_net(zL, zH, x)
            if (_i + 1) % T == 0:
                zH = H_net(zH, zL)
    
    # 1-step gradient only on final step
    zL = L_net(zL, zH, x)
    zH = H_net(zH, zL)
    
    return (zH, zL), output_head(zH)
```

### Deep Supervision

Inspired by neural oscillations regulating learning:

```python
for x, y_true in train_dataloader:
    z = z_init
    for step in range(N_supervision):
        z, y_hat = hrm(z, x)
        loss = cross_entropy(y_hat, y_true)
        z = z.detach()  # Key: detach between segments
        loss.backward()
        optimizer.step()
```

Multiple forward passes, each providing feedback.

### Adaptive Computation Time (ACT)

HRM can dynamically allocate compute based on task difficulty:

**Q-Learning for Halting**:
- Q-head predicts value of "halt" vs "continue"
- Halt when Q_halt > Q_continue (after minimum steps)
- Reward = 1 if correct prediction, 0 otherwise

This enables "thinking fast and slow"—easy problems use fewer steps.

### Benchmark Results

| Benchmark | HRM | o3-mini-high | Claude 3.7 8K | DeepSeek R1 |
|-----------|-----|--------------|---------------|-------------|
| ARC-AGI-1 | **40.3%** | 34.5% | 21.2% | 21.0% |
| ARC-AGI-2 | **5.0%** | 3.0% | 1.3% | 0.9% |
| Sudoku-Extreme | **55.0%** | 0.0% | 0.0% | 0.0% |
| Maze-Hard 30×30 | **74.5%** | 0.0% | 0.0% | 0.0% |

Key insight: CoT models completely fail (0%) on tasks requiring extensive search and backtracking.

### Why HRM Succeeds Where CoT Fails

**Sudoku-Extreme**: Requires tree search with backtracking
- CoT must verbalize every step
- Single wrong step propagates errors
- HRM performs search in latent space

**Maze-Hard**: Optimal path finding in 30×30 grid
- Requires exploring many possibilities
- CoT becomes extremely long
- HRM explores efficiently in hidden state

### Brain Correspondence

Analyzing trained HRM reveals brain-like properties:

**Dimensionality Hierarchy** (Participation Ratio):
- L-module: PR = 30.22 (lower dimensional)
- H-module: PR = 89.95 (higher dimensional)

This matches biological findings:
- Higher cortical areas have higher-dimensional representations
- Ratio (H/L ≈ 2.98) matches mouse cortex measurements (≈ 2.25)

### Computational Depth Scaling

```
Parameters (millions)
         27M    54M   109M   218M   436M   872M
         │      │      │      │      │      │
Width    ●──────●──────●──────●──────●──────●  (8 layers, ~20% accuracy)
Depth    ●──────●──────●──────●──────●──────●  (512 hidden, scales better)

Depth (Transformer layers computed)
         8     16     32     64    128    256    512
         │      │      │      │      │      │      │
Trans.   ●      ●      ●      ●      ●      ●      ●  (saturates ~60%)
Rec.Trans●      ●      ●      ●      ●      ●      ●  (saturates ~65%)  
HRM      ●      ●      ●      ●      ●      ●      ●  (reaches ~98%)
```

HRM uniquely benefits from increased computational depth.

### Model Specifications

| Component | Implementation |
|-----------|---------------|
| Total parameters | ~27M |
| Module architecture | Encoder-only Transformer blocks |
| Normalization | RMSNorm (Post-Norm) |
| Activation | SwiGLU |
| Position encoding | Rotary (RoPE) |
| Optimizer | Adam-atan2 |
| Training samples | ~1000 |

### Inference-Time Scaling

Models trained with specific Mmax generalize to higher limits:

```
Trained Mmax = 8, Inference Mmax = 16 → Continued improvement
```

Unlike CoT, additional compute directly improves reasoning without generating more tokens.

### Significance for AGI

HRM demonstrates:
1. Reasoning without language externalization is possible
2. Brain-inspired architectures can outperform scale
3. Small models with right inductive biases beat large models
4. Path toward Turing-complete neural computation

## 📌 Key Points
- HRM uses hierarchical, multi-timescale recurrence inspired by the brain to achieve effective computational depth.
- Hierarchical convergence prevents the early saturation problem of standard RNNs.
- One-step gradient approximation enables O(1) memory training without BPTT.
- With only 27M parameters and 1000 examples, HRM solves tasks where state-of-the-art CoT models completely fail.

## 🖼️ Recommended Image
- Architecture diagram showing H-module and L-module interaction with different update frequencies, plus comparison chart against CoT models.

## 🔗 Connections
- [[242-Neural Networks Fundamentals]]
- [[243-Transformer Architecture]]
- [[249-Large Language Models LLMs]]
- [[244-Attention Mechanism]]
- [[248-Context Window Limitations]]