---
Theme: Transformer Architecture
Date created: 2026-02-02 12:00
tags: [AILevel, Transformer, Architecture, AttentionIsAllYouNeed]
---

## 📚 Idea/Concept

The Transformer is a neural architecture introduced in the 2017 paper "Attention Is All You Need" that revolutionized sequence processing by eliminating recurrence entirely and relying solely on attention mechanisms. It enables massive parallelization and superior modeling of long-range dependencies.

### Historical Context

Prior to Transformers, sequence models relied on:
- **RNNs**: Sequential processing, O(n) depth, vanishing gradients
- **LSTMs/GRUs**: Better long-term memory, still sequential
- **CNNs + Attention**: Partial parallelization

The Transformer proposal: Replace recurrence entirely with self-attention.

### Overall Architecture

```
                    ENCODER                         DECODER
                 ┌──────────┐                    ┌──────────┐
                 │  Output  │                    │  Output  │
                 │Probabilities                  │Probabilities
                 └────┬─────┘                    └────┬─────┘
                      │                               │
                 ┌────┴─────┐                    ┌────┴─────┐
              ×N │ Add&Norm │                 ×N │ Add&Norm │
                 ├──────────┤                    ├──────────┤
                 │Feed Forw.│                    │Feed Forw.│
                 ├──────────┤                    ├──────────┤
                 │ Add&Norm │                    │ Add&Norm │
                 ├──────────┤                    ├──────────┤
                 │Multi-Head│                    │Cross-Attn│
                 │Self-Attn │                    ├──────────┤
                 └────┬─────┘                    │ Add&Norm │
                      │                          ├──────────┤
                 ┌────┴─────┐                    │ Masked   │
                 │Positional│                    │Self-Attn │
                 │Encoding  │                    └────┬─────┘
                 └────┬─────┘                         │
                      │                          ┌────┴─────┐
                 ┌────┴─────┐                    │Positional│
                 │Input Emb.│                    │Encoding  │
                 └──────────┘                    └──────────┘
```

### Encoder Structure

Each of N=6 identical layers contains:

1. **Multi-Head Self-Attention**
   - Each position attends to all positions in input
   - Captures contextual relationships

2. **Position-wise Feed-Forward Network**
   - Two linear transformations with ReLU
   - FFN(x) = max(0, xW₁ + b₁)W₂ + b₂

3. **Residual Connections + Layer Normalization**
   - Output = LayerNorm(x + Sublayer(x))
   - Enables training of deep networks

### Decoder Structure

Each of N=6 identical layers contains:

1. **Masked Multi-Head Self-Attention**
   - Prevents attending to future positions
   - Maintains autoregressive property

2. **Encoder-Decoder Attention (Cross-Attention)**
   - Queries from decoder, Keys/Values from encoder
   - Aligns output with input sequence

3. **Position-wise Feed-Forward Network**

### Base Model Dimensions

| Parameter | Value |
|-----------|-------|
| d_model (embedding dim) | 512 |
| d_ff (feed-forward dim) | 2048 |
| h (attention heads) | 8 |
| d_k = d_v (head dim) | 64 |
| N (layers) | 6 |

### Computational Advantages

| Aspect | Self-Attention | Recurrent | Convolutional |
|--------|---------------|-----------|---------------|
| Complexity per layer | O(n²·d) | O(n·d²) | O(k·n·d²) |
| Sequential operations | O(1) | O(n) | O(1) |
| Max path length | O(1) | O(n) | O(log_k(n)) |

Key insight: Self-attention has constant path length between any two positions, enabling better gradient flow for long-range dependencies.

### Positional Encoding

Since attention is permutation-invariant, positional information must be injected:

$$PE_{(pos, 2i)} = \sin(pos / 10000^{2i/d_{model}})$$
$$PE_{(pos, 2i+1)} = \cos(pos / 10000^{2i/d_{model}})$$

Properties:
- Each dimension corresponds to a sinusoid
- Wavelengths form geometric progression
- Allows model to learn relative positions
- Can extrapolate to longer sequences

### Training Details (Original Paper)

- **Data**: WMT 2014 En→De (~4.5M pairs), En→Fr (~36M pairs)
- **Hardware**: 8 NVIDIA P100 GPUs
- **Optimizer**: Adam (β₁=0.9, β₂=0.98)
- **Learning Rate**: Warmup + decay schedule
- **Regularization**: Dropout (0.1-0.3), Label smoothing (ε=0.1)

### Architectural Variants

| Variant | Architecture | Training Objective | Use Case |
|---------|-------------|-------------------|----------|
| BERT | Encoder-only | Masked LM | Classification, NLU |
| GPT | Decoder-only | Next-token prediction | Generation |
| T5/BART | Encoder-Decoder | Seq2Seq | Translation, Summarization |

### Results from Original Paper

- **En→De Translation**: 28.4 BLEU (new SOTA, +2.0 over previous best)
- **En→Fr Translation**: 41.8 BLEU (surpassed all single models)
- **Training Cost**: Fraction of previous approaches

## 📌 Key Points
- Transformers replace recurrence with self-attention, enabling full parallelization.
- The encoder-decoder structure with residual connections and layer normalization enables deep, stable training.
- Positional encodings inject sequence order information into the permutation-invariant attention.
- Constant path length between positions enables superior long-range dependency modeling.

## 🖼️ Recommended Image
- Full Transformer architecture diagram showing encoder-decoder stack with attention layers, feed-forward networks, and residual connections.

## 🔗 Connections
- [[244-Attention Mechanism]]
- [[245-Multi-Head Attention]]
- [[249-Large Language Models LLMs]]
- [[250-GPT Architecture and Variants]]