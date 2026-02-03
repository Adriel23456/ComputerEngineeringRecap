---
Theme: Multi-Head Attention
Date created: 2026-02-02 12:00
tags: [AILevel, MultiHeadAttention, Transformer, Attention]
---

## 📚 Idea/Concept

Multi-Head Attention extends the basic attention mechanism by running multiple attention operations in parallel, each with different learned projections. This allows the model to jointly attend to information from different representation subspaces at different positions.

### Motivation

A single attention head can only focus on one type of relationship at a time. Multi-head attention solves this by:
- Learning multiple attention patterns simultaneously
- Capturing different types of dependencies (syntactic, semantic, positional)
- Increasing model expressiveness without excessive parameter increase

### Mathematical Formulation

$$\text{MultiHead}(Q, K, V) = \text{Concat}(\text{head}_1, ..., \text{head}_h)W^O$$

Where each head is:
$$\text{head}_i = \text{Attention}(QW_i^Q, KW_i^K, VW_i^V)$$

### Projection Dimensions

For model dimension d_model and h heads:
- Each head dimension: d_k = d_v = d_model / h
- Total parameters similar to single full-dimensional attention

Example (base Transformer):
- d_model = 512
- h = 8 heads
- d_k = d_v = 512/8 = 64 per head

### Architecture Diagram

```
        Input (d_model)
              │
    ┌─────────┼─────────┐
    │         │         │
   W_Q       W_K       W_V
    │         │         │
    ▼         ▼         ▼
┌───────────────────────────┐
│     Split into h heads    │
└───────────────────────────┘
    │    │    │    │    │
   head₁ head₂ ... head_h
    │    │    │    │    │
    └────┴────┴────┴────┘
              │
         Concat
              │
             W^O
              │
        Output (d_model)
```

### What Different Heads Learn

Research has shown heads specialize in different patterns:

| Head Type | What It Learns | Example |
|-----------|---------------|---------|
| Positional | Adjacent tokens | "the" → "dog" |
| Syntactic | Grammar structure | Subject → Verb |
| Semantic | Meaning relations | Pronoun → Antecedent |
| Rare pattern | Specific constructs | Quotes, brackets |

### Computational Flow

```python
def multi_head_attention(Q, K, V, W_Q, W_K, W_V, W_O, h):
    heads = []
    for i in range(h):
        Q_i = Q @ W_Q[i]  # Project to subspace
        K_i = K @ W_K[i]
        V_i = V @ W_V[i]
        head_i = scaled_dot_product_attention(Q_i, K_i, V_i)
        heads.append(head_i)
    
    concat = concatenate(heads, axis=-1)
    output = concat @ W_O
    return output
```

### Parameter Count

For a single multi-head attention layer:
- Query projection: d_model × d_model
- Key projection: d_model × d_model  
- Value projection: d_model × d_model
- Output projection: d_model × d_model

Total: 4 × d_model²

### Attention Visualizations

Each head produces an attention matrix that can be visualized:

```
Head 1 (Positional):     Head 2 (Syntactic):
  The dog ate           The dog ate
The [■ □ □]            The [□ ■ □]
dog [□ ■ □]            dog [■ □ □]
ate [□ □ ■]            ate [□ ■ □]

■ = high attention, □ = low attention
```

### Benefits Over Single-Head

| Aspect | Single Head | Multi-Head |
|--------|-------------|------------|
| Patterns learned | 1 | h different patterns |
| Subspace modeling | Full d_model | h subspaces of d_k |
| Gradient flow | Single path | Multiple paths |
| Redundancy | None | Built-in through heads |

### Attention Head Pruning

Research shows that not all heads are equally important:
- Many heads can be pruned with minimal performance loss
- Some heads are critical for specific tasks
- Head importance varies by layer depth

### Implementation Efficiency

Modern implementations use batched matrix operations:

```python
# Efficient: Single batched operation
Q_all = Q @ W_Q.view(d_model, h, d_k)  # (seq, h, d_k)
K_all = K @ W_K.view(d_model, h, d_k)
V_all = V @ W_V.view(d_model, h, d_v)

# Compute all heads in parallel
attn_scores = torch.einsum('ihd,jhd->hij', Q_all, K_all)
```

## 📌 Key Points
- Multi-head attention runs h parallel attention operations in different subspaces.
- Each head can specialize in different types of relationships (positional, syntactic, semantic).
- The output projection W^O combines information from all heads back to d_model dimension.
- Computational cost is similar to single-head attention of the same total dimension.

## 🖼️ Recommended Image
- Diagram showing parallel attention heads with different projection matrices, concatenation, and output projection.

## 🔗 Connections
- [[244-Attention Mechanism]]
- [[243-Transformer Architecture]]
- [[249-Large Language Models LLMs]]
- [[250-GPT Architecture and Variants]]
- [[248-Context Window Limitations]]