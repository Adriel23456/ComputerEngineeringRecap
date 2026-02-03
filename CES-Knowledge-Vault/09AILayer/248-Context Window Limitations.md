---
Theme: Context Window Limitations
Date created: 2026-02-02 12:00
tags: [AILevel, ContextWindow, Attention, LLMConstraints]
---

## 📚 Idea/Concept

The Context Window is the maximum number of tokens an LLM can process simultaneously, fundamentally limited by the O(n²·d) complexity of self-attention. This creates memory and computational constraints that affect what information the model can access and how effectively it utilizes long-range context.

### The Quadratic Bottleneck

Self-attention computes all pairwise relationships:

$$\text{Memory} = O(n^2 \cdot h \cdot L)$$

Where:
- n = sequence length
- h = number of attention heads
- L = number of layers

**Example**:
- 100k tokens, 96 heads, 96 layers
- Attention scores alone: ~40GB

### Context Window Sizes

| Model | Context Window | Release |
|-------|---------------|---------|
| GPT-2 | 1,024 | 2019 |
| GPT-3 | 2,048 | 2020 |
| GPT-3.5 | 4,096 | 2022 |
| GPT-4 | 8,192 / 32,768 | 2023 |
| Claude 2 | 100,000 | 2023 |
| Llama 2 | 4,096 | 2023 |
| Gemini 1.5 | 1,000,000+ | 2024 |

### Theoretical vs Effective Context

**Theoretical**: Maximum tokens the model can accept

**Effective**: How much the model actually uses

Research shows significant degradation:
- "Lost in the Middle" phenomenon
- Information at start and end is better utilized
- Middle context often ignored

```
Position:  [START] ... [MIDDLE] ... [END]
Retrieval:   High   ...   Low   ...  High
```

### Memory Components

| Component | Memory Usage |
|-----------|--------------|
| Attention scores | O(n² × h × L) |
| KV cache (inference) | O(batch × n × d × L) |
| Activations | O(n × d × L) |
| Model parameters | Fixed |

### KV Cache

During autoregressive generation, keys and values from previous tokens are cached:

```
Token 1: Compute K₁, V₁ → Cache
Token 2: Compute K₂, V₂ → Cache, attend to K₁,K₂
Token 3: Compute K₃, V₃ → Cache, attend to K₁,K₂,K₃
...
```

Memory: proportional to sequence_length × batch_size × hidden_dim

### Positional Encoding Limits

Models trained with positional encodings up to length L struggle beyond L:

**Learned Positions**: Hard cutoff at training length
**Sinusoidal**: Can extrapolate but performance degrades
**RoPE**: Better extrapolation with techniques like YaRN

### Strategies for Long Context

**1. Truncation Methods**:
- Keep first N tokens
- Keep last N tokens  
- Keep first + last (sliding window)

**2. Compression**:
- Summarize older context
- Use memory networks
- Hierarchical attention

**3. Retrieval Augmentation (RAG)**:
```
Query → Retrieve relevant chunks → Concatenate → Generate
```

**4. Architectural Modifications**:
- Sparse attention patterns
- Linear attention approximations
- Sliding window attention

### Efficient Attention Variants

| Method | Complexity | Trade-off |
|--------|------------|-----------|
| Full attention | O(n²) | Exact |
| Sparse attention | O(n√n) | Fixed patterns |
| Linformer | O(n) | Low-rank approx |
| Performer | O(n) | Random features |
| Flash Attention | O(n²) time, O(n) memory | Memory-efficient |

### Flash Attention

Doesn't reduce complexity but optimizes memory access:

```
Standard: Write n² attention matrix to HBM
Flash: Compute attention in tiles, never materialize full matrix
```

Result: Same compute, much less memory, faster in practice.

### Impact on Tasks

| Task | Context Sensitivity |
|------|-------------------|
| QA | High (needs retrieval) |
| Summarization | Medium-High |
| Translation | Medium |
| Code generation | High (long files) |
| Chat | Variable |

### Fertility Impact

Token fertility affects effective context:

```
English: "The quick brown fox" = 4 tokens
Chinese: "敏捷的棕色狐狸" = 8-12 tokens

Same meaning, 2-3x context consumption
```

### Context Window Trade-offs

| Longer Window | Shorter Window |
|---------------|----------------|
| More context | Less context |
| Higher memory | Lower memory |
| Slower inference | Faster inference |
| Better for long docs | Better for chat |
| More expensive | Cheaper |

### Practical Implications

For a 4K context window at ~4 chars/token:
- ~16,000 characters
- ~3,000 words
- ~6 pages of text

This limits:
- Book analysis
- Code repository understanding
- Long conversation history
- Document comparison

## 📌 Key Points
- Context window is limited by O(n²) attention complexity and GPU memory.
- Effective context is smaller than theoretical due to "lost in the middle" phenomenon.
- KV cache enables efficient autoregressive generation but grows with sequence length.
- Solutions include sparse attention, RAG, and efficient implementations like Flash Attention.

## 🖼️ Recommended Image
- Graph showing memory usage scaling quadratically with context length, with annotations for practical limits.

## 🔗 Connections
- [[244-Attention Mechanism]]
- [[243-Transformer Architecture]]
- [[249-Large Language Models LLMs]]
- [[246-Tokenization and Subword Encoding]]