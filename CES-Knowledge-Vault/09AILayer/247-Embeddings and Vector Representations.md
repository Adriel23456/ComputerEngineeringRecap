---
Theme: Embeddings and Vector Representations
Date created: 2026-02-02 12:00
tags: [AILevel, Embeddings, WordEmbeddings, VectorRepresentations]
---

## 📚 Idea/Concept

Embeddings are learnable projections that map discrete tokens to dense continuous vectors. The embedding matrix E ∈ ℝ^(|V|×d) transforms vocabulary V to dimension d via lookup, enabling neural networks to process symbolic data and capture semantic relationships geometrically.

### Core Concept

```
Token ID → Embedding Matrix → Dense Vector

   42    →    E[42]      → [0.23, -0.15, 0.87, ..., 0.12]
              (lookup)         (d-dimensional vector)
```

### Embedding Matrix

$$E \in \mathbb{R}^{|V| \times d}$$

Where:
- |V| = vocabulary size (e.g., 50,000)
- d = embedding dimension (e.g., 768, 1024, 4096)

Lookup operation:
$$e_i = E[\text{token\_id}]$$

### Embedding Composition in Transformers

The final input embedding combines multiple components:

$$\text{Input} = \text{TokenEmb}(x) + \text{PosEmb}(pos) + \text{TypeEmb}(type)$$

| Component | Purpose | Example |
|-----------|---------|---------|
| Token Embedding | Semantic meaning | "dog" → vector |
| Positional Embedding | Sequence order | position 5 → vector |
| Type/Segment Embedding | Sentence identity | Sentence A vs B |

### Semantic Properties

Well-trained embeddings exhibit geometric structure:

**Vector Arithmetic**:
$$\vec{king} - \vec{man} + \vec{woman} \approx \vec{queen}$$

**Similarity Measurement**:
$$\text{similarity}(A, B) = \frac{A \cdot B}{||A|| \cdot ||B||}$$

**Clustering**: Similar concepts cluster together in embedding space.

### Visualization Example

```
                    Royalty
                       │
          king ●───────┼───────● queen
                       │
                       │
           man ●───────┼───────● woman
                       │
                    Gender
```

### Weight Tying

Modern LLMs often share the embedding matrix between:
- Input embeddings (token → vector)
- Output projection (vector → logits)

Benefits:
- Reduces parameters significantly
- Improves generalization
- Maintains semantic consistency

$$\text{logits} = h \cdot E^T$$

### Static vs Contextualized Embeddings

| Aspect | Static (Word2Vec) | Contextualized (BERT/GPT) |
|--------|-------------------|---------------------------|
| Context | None | Full sentence |
| Polysemy | Single vector | Multiple representations |
| Computation | Lookup only | Full forward pass |
| Example | "bank" = one vector | "river bank" ≠ "money bank" |

### Embedding Dimensions in Practice

| Model | Embedding Dimension |
|-------|---------------------|
| Word2Vec | 100-300 |
| BERT-base | 768 |
| BERT-large | 1024 |
| GPT-2 | 768-1600 |
| GPT-3 | 12,288 |
| Llama 2 70B | 8,192 |

### Initialization

Embeddings are typically initialized from:
$$E_{ij} \sim \mathcal{N}(0, 0.02)$$

Then optimized via gradients during training like any other parameter.

### Positional Embeddings

Two main approaches:

**Sinusoidal (Fixed)**:
$$PE_{(pos, 2i)} = \sin(pos / 10000^{2i/d})$$
$$PE_{(pos, 2i+1)} = \cos(pos / 10000^{2i/d})$$

**Learned**: Separate embedding matrix for positions
$$\text{PosEmb} = P[\text{position}], \quad P \in \mathbb{R}^{L_{max} \times d}$$

**Rotary (RoPE)**: Modern approach rotating embeddings by position

### Embedding Space Properties

| Property | Description |
|----------|-------------|
| Linearity | Analogies work via vector arithmetic |
| Clustering | Semantic categories form clusters |
| Smoothness | Similar meanings have similar vectors |
| Anisotropy | Vectors tend to occupy narrow cone |

### Computing Similarity

```python
import torch.nn.functional as F

# Cosine similarity
sim = F.cosine_similarity(emb_a, emb_b, dim=-1)

# Euclidean distance
dist = torch.norm(emb_a - emb_b, dim=-1)

# Dot product (unnormalized)
score = (emb_a * emb_b).sum(dim=-1)
```

### Subword Embeddings

With BPE/WordPiece, rare words are composed from subword embeddings:

```
"internationalization" → "international" + "ization"
                              ↓               ↓
                         [e₁, e₂, ...]    [f₁, f₂, ...]
```

The model learns to combine these through attention and other layers.

## 📌 Key Points
- Embeddings convert discrete tokens to continuous vectors via learned lookup tables.
- Semantic relationships are encoded geometrically, enabling vector arithmetic.
- Contextualized embeddings from Transformers capture word meaning based on context.
- Weight tying between input embeddings and output projection reduces parameters and improves consistency.

## 🖼️ Recommended Image
- 2D/3D visualization of embedding space showing semantic clusters and vector relationships.

## 🔗 Connections
- [[246-Tokenization and Subword Encoding]]
- [[243-Transformer Architecture]]
- [[244-Attention Mechanism]]
- [[249-Large Language Models LLMs]]