---
Theme: Attention Mechanism
Date created: 2026-02-02 12:00
tags: [AILevel, Attention, SelfAttention, Transformer]
---

## 📚 Idea/Concept

The Attention Mechanism is a neural network component that computes weighted sums of values based on the compatibility between queries and keys. It allows models to dynamically focus on relevant parts of the input, enabling effective modeling of dependencies regardless of distance in the sequence.

### Formal Definition

"Attention maps a query and a set of key-value pairs to an output, where the output is computed as a weighted sum of the values, and the weight assigned to each value is computed by a compatibility function of the query with the corresponding key."

### Scaled Dot-Product Attention

The core attention computation:

$$\text{Attention}(Q, K, V) = \text{softmax}\left(\frac{QK^T}{\sqrt{d_k}}\right)V$$

Where:
- Q = Query matrix (what we're looking for)
- K = Key matrix (what each position offers)
- V = Value matrix (actual content to retrieve)
- d_k = dimension of keys (scaling factor)

### Step-by-Step Process

```
1. Compute similarity scores: S = Q · K^T
2. Scale: S_scaled = S / √d_k
3. Apply softmax: A = softmax(S_scaled)
4. Weighted sum: Output = A · V
```

### Visual Analogy: Paint Mixing

Imagine mixing paint colors:
1. **Query**: "I need yellow with a touch of dark orange"
2. **Keys**: Labels on each paint can
3. **Values**: The actual paint in each can
4. **Attention weights**: How much of each can to use
5. **Output**: The blended result

### Why Scale by √d_k?

For large d_k, dot products grow large in magnitude, pushing softmax into regions with extremely small gradients:

Without scaling:
- Large dot products → softmax saturates
- Gradients vanish → training fails

With scaling:
- Variance normalized to ~1
- Softmax operates in effective range

### Worked Example

Input: "The dog ate his dinner" (3 words simplified)

**Embeddings** (dimension 3):
```
dog    = [1.0, 2.0, 3.0]
ate    = [0.5, 1.0, 1.5]
dinner = [2.0, 1.0, 0.5]
```

**Projection Matrices**:
```
W_Q = [[0.6, -0.2], [0.1, 0.5], [-0.3, 0.4]]
W_K = [[0.2, 0.3], [-0.4, 0.7], [0.5, -0.1]]
W_V = [[0.9, 0.1], [0.2, 0.8], [-0.1, 0.6]]
```

**Computing Q, K, V**:
```
Q_dog = dog · W_Q = [-0.10, 2.00]
K_dog = dog · W_K = [0.90, 1.40]
V_dog = dog · W_V = [1.0, 3.5]
```

**Similarity Matrix** (Q · K^T):
```
S[dog,dog] = (-0.10×0.90) + (2.00×1.40) = 2.71
S[dog,ate] = (-0.10×0.45) + (2.00×0.70) = 1.36
S[dog,dinner] = (-0.10×0.25) + (2.00×1.25) = 2.48
```

**After softmax** (attention weights for "dog"):
```
A[dog] = [0.448, 0.172, 0.380]
```

**Output for "dog"**:
```
Output_dog = 0.448×V_dog + 0.172×V_ate + 0.380×V_dinner
           = [1.275, 2.363]
```

### Self-Attention vs Cross-Attention

| Type | Q Source | K,V Source | Use Case |
|------|----------|------------|----------|
| Self-Attention | Same sequence | Same sequence | Encoder, masked decoder |
| Cross-Attention | Decoder | Encoder | Encoder-decoder alignment |

### Masked Self-Attention

In autoregressive generation, future positions must be hidden:

$$\text{Mask}_{ij} = \begin{cases} 0 & \text{if } i \geq j \\ -\infty & \text{if } i < j \end{cases}$$

Applied before softmax:
$$\text{Attention} = \text{softmax}\left(\frac{QK^T}{\sqrt{d_k}} + \text{Mask}\right)V$$

### Parallel Processing Advantage

Unlike RNNs, self-attention processes all positions simultaneously:

```
Step 1: Transform all inputs to Q, K, V (parallel)
Step 2: Compute all pairwise similarities (parallel)  
Step 3: Apply softmax row-wise (parallel)
Step 4: Compute weighted sums (parallel)
```

Total: O(1) sequential operations vs O(n) for RNN

### Attention Patterns Learned

Different attention heads learn different patterns:
- **Syntactic heads**: Subject-verb agreement
- **Semantic heads**: Coreference resolution
- **Positional heads**: Local neighborhood attention
- **Separator heads**: Attend to special tokens

### Complexity Analysis

| Metric | Complexity |
|--------|------------|
| Time | O(n² · d) |
| Memory (storing attention) | O(n² · h · L) |
| Path length | O(1) |

The quadratic complexity is the main limitation for very long sequences.

## 📌 Key Points
- Attention computes dynamic, content-based weighted sums enabling flexible information routing.
- Scaling by √d_k prevents softmax saturation and gradient vanishing.
- Self-attention allows each position to attend to all others in O(1) sequential steps.
- The quadratic complexity O(n²) limits practical context lengths.

## 🖼️ Recommended Image
- Diagram showing Query-Key-Value computation with attention weight visualization as a heatmap.

## 🔗 Connections
- [[243-Transformer Architecture]]
- [[245-Multi-Head Attention]]
- [[248-Context Window Limitations]]
- [[249-Large Language Models LLMs]]