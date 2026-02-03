---
Theme: GPT Architecture and Variants
Date created: 2026-02-02 12:00
tags: [AILevel, GPT, DecoderOnly, Autoregressive, LLM]
---

## 📚 Idea/Concept

GPT (Generative Pre-trained Transformer) is a decoder-only Transformer architecture that generates text autoregressively by predicting one token at a time. It forms the basis for most modern LLMs including ChatGPT, and demonstrates that a single architecture can solve diverse tasks through prompting alone.

### Architecture Overview

GPT uses only the decoder portion of the original Transformer:

```
┌─────────────────────────┐
│    Output Probabilities │
│         (Softmax)       │
└───────────┬─────────────┘
            │
┌───────────┴─────────────┐
│     Linear Projection   │
│      (to vocab size)    │
└───────────┬─────────────┘
            │
┌───────────┴─────────────┐
│                         │
│   ┌─────────────────┐   │
│   │   Add & Norm    │   │
│   ├─────────────────┤   │
│   │  Feed Forward   │   │  × N layers
│   ├─────────────────┤   │
│   │   Add & Norm    │   │
│   ├─────────────────┤   │
│   │ Masked Self-Attn│   │
│   └─────────────────┘   │
│                         │
└───────────┬─────────────┘
            │
┌───────────┴─────────────┐
│  Token + Position Emb   │
└───────────┬─────────────┘
            │
        [Input Tokens]
```

### Why Decoder-Only?

The key insight: A decoder with causal masking can be trained on any text without needing input-output pairs.

```
Input:  "The cat sat on the"
Target: "cat sat on the mat"
        (shifted by one position)
```

Every position predicts the next token using only previous context.

### Causal (Masked) Self-Attention

Each token can only attend to previous tokens:

```
         The  cat  sat  on  mat
The      [✓]  [✗]  [✗]  [✗]  [✗]
cat      [✓]  [✓]  [✗]  [✗]  [✗]
sat      [✓]  [✓]  [✓]  [✗]  [✗]
on       [✓]  [✓]  [✓]  [✓]  [✗]
mat      [✓]  [✓]  [✓]  [✓]  [✓]
```

This is implemented by adding -∞ to attention scores for future positions before softmax.

### GPT Evolution

| Model | Year | Parameters | Key Innovation |
|-------|------|------------|----------------|
| GPT-1 | 2018 | 117M | Pretrain + finetune paradigm |
| GPT-2 | 2019 | 1.5B | Zero-shot task transfer |
| GPT-3 | 2020 | 175B | In-context learning, few-shot |
| InstructGPT | 2022 | 175B | RLHF alignment |
| GPT-4 | 2023 | ~1.7T | Multimodal, extended context |

### GPT-3 Architecture Details

| Component | Specification |
|-----------|---------------|
| Layers | 96 |
| Hidden dimension | 12,288 |
| Attention heads | 96 |
| Head dimension | 128 |
| FFN dimension | 49,152 |
| Vocabulary | 50,257 |
| Context length | 2,048 |
| Parameters | 175B |

### Training Data (GPT-3)

| Source | Tokens | Weight |
|--------|--------|--------|
| Common Crawl (filtered) | 410B | 60% |
| WebText2 | 19B | 22% |
| Books1 | 12B | 8% |
| Books2 | 55B | 8% |
| Wikipedia | 3B | 3% |
| **Sampled Total** | ~300B | - |

### Modern Improvements (Llama-style)

Current GPT-like models incorporate:

| Improvement | Original | Modern |
|-------------|----------|--------|
| Normalization | Post-LayerNorm | Pre-RMSNorm |
| Activation | GELU | SwiGLU |
| Position encoding | Learned/Sinusoidal | RoPE |
| Attention | Standard | Grouped-Query (GQA) |

### Autoregressive Generation

```python
def generate(model, prompt, max_tokens):
    tokens = tokenize(prompt)
    
    for _ in range(max_tokens):
        # Forward pass
        logits = model(tokens)
        next_token_logits = logits[-1]  # Last position
        
        # Sample next token
        probs = softmax(next_token_logits / temperature)
        next_token = sample(probs)
        
        tokens.append(next_token)
        
        if next_token == EOS:
            break
    
    return detokenize(tokens)
```

### Sampling Strategies

| Strategy | Description | Use Case |
|----------|-------------|----------|
| Greedy | Always pick highest probability | Deterministic output |
| Temperature | Scale logits before softmax | Control randomness |
| Top-k | Sample from top k tokens | Limit vocabulary |
| Top-p (Nucleus) | Sample from smallest set summing to p | Dynamic vocabulary |

**Temperature Effect**:
$$P(x_i) = \frac{e^{z_i/T}}{\sum_j e^{z_j/T}}$$

- T < 1: More deterministic (peaked distribution)
- T = 1: Original distribution
- T > 1: More random (flattened distribution)

### Zero-Shot vs Few-Shot

**Zero-shot**: Task described in prompt only
```
Translate English to French:
sea otter =>
```

**Few-shot**: Examples provided in prompt
```
Translate English to French:
sea otter => loutre de mer
cheese => fromage
plush giraffe =>
```

GPT-3 demonstrated that few-shot learning emerges from scale without any gradient updates.

### GPT vs BERT Comparison

| Aspect | GPT (Decoder) | BERT (Encoder) |
|--------|---------------|----------------|
| Attention | Causal (left-to-right) | Bidirectional |
| Training | Next-token prediction | Masked token prediction |
| Generation | Natural | Requires modification |
| Understanding | Good | Excellent |
| Primary use | Generation, chat | Classification, NLU |

### The "G" in GPT: Generative

The model generates by repeatedly:
1. Processing all tokens so far
2. Predicting distribution over next token
3. Sampling from distribution
4. Appending to sequence
5. Repeating until done

This autoregressive process enables open-ended generation.

## 📌 Key Points
- GPT is a decoder-only Transformer using causal masking for autoregressive generation.
- Training objective is next-token prediction on massive text corpora.
- Few-shot learning emerges at scale: the model can learn tasks from examples in the prompt.
- Modern GPT-like models (Llama, etc.) incorporate architectural improvements like RoPE, SwiGLU, and RMSNorm.

## 🖼️ Recommended Image
- Side-by-side comparison of encoder-only (BERT) vs decoder-only (GPT) attention patterns.

## 🔗 Connections
- [[243-Transformer Architecture]]
- [[249-Large Language Models LLMs]]
- [[244-Attention Mechanism]]
- [[251-LLM Training Pipeline]]
- [[252-RLHF and Alignment Methods]]