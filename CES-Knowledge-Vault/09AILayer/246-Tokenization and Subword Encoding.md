---
Theme: Tokenization and Subword Encoding
Date created: 2026-02-02 12:00
tags: [AILevel, Tokenization, BPE, WordPiece, NLP]
---

## 📚 Idea/Concept

Tokenization is the process of converting raw text into sequences of discrete tokens that can be processed by neural networks. Modern LLMs use subword tokenization algorithms like BPE (Byte Pair Encoding) that balance vocabulary size with sequence length by learning to segment text based on frequency patterns.

### Why Tokenization Matters

Neural networks operate on numbers, not text. Tokenization bridges this gap:

```
"Hello, world!" → [15496, 11, 995, 0] → [embedding vectors]
```

### Tokenization Approaches

| Approach | Granularity | Pros | Cons |
|----------|-------------|------|------|
| Character-level | Single characters | Small vocab, handles OOV | Very long sequences |
| Word-level | Whole words | Semantic units | Large vocab, OOV problems |
| Subword | Variable chunks | Balance of both | Requires learning |

### Byte Pair Encoding (BPE)

The most common subword algorithm, used by GPT models:

**Training Process**:
1. Start with character-level vocabulary
2. Count all adjacent pair frequencies
3. Merge most frequent pair into new token
4. Repeat until desired vocabulary size

**Example**:
```
Corpus: "low lower lowest"

Initial: ['l', 'o', 'w', ' ', 'l', 'o', 'w', 'e', 'r', ...]

Step 1: Most frequent pair ('l', 'o') → merge to 'lo'
Step 2: Most frequent pair ('lo', 'w') → merge to 'low'
Step 3: Most frequent pair ('low', 'e') → merge to 'lowe'
...

Final vocabulary includes: 'low', 'lowe', 'lower', 'lowest', etc.
```

### Byte-Level BPE (GPT)

GPT uses byte-level BPE operating on UTF-8 bytes:

Advantages:
- No unknown tokens ([UNK])
- Handles any Unicode text
- Vocabulary ~50k tokens

Disadvantage:
- Non-ASCII text becomes longer sequences

### WordPiece (BERT)

Similar to BPE but optimizes likelihood instead of frequency:

$$\text{score}(pair) = \frac{\text{freq}(pair)}{\text{freq}(first) \times \text{freq}(second)}$$

Uses ## prefix for word continuations:
```
"tokenization" → ["token", "##ization"]
```

### Tokenization Pipeline

```
Raw Text
    │
    ▼
Normalization (NFKC, lowercasing)
    │
    ▼
Pre-tokenization (split on whitespace/punctuation)
    │
    ▼
Subword Splitting (BPE/WordPiece)
    │
    ▼
Token ID Mapping
    │
    ▼
[15496, 11, 995, 0]
```

### Special Tokens

| Token | Purpose | Example Usage |
|-------|---------|---------------|
| [PAD] | Padding sequences | Batch alignment |
| [UNK] | Unknown token | Out-of-vocabulary |
| [CLS] | Classification | BERT sentence start |
| [SEP] | Separator | BERT sentence boundary |
| [MASK] | Masking | BERT training |
| <\|endoftext\|> | End of text | GPT document boundary |

### Vocabulary Sizes

| Model | Vocabulary Size |
|-------|----------------|
| GPT-2 | 50,257 |
| GPT-3/4 | 100,277 |
| BERT | 30,522 |
| Llama 2 | 32,000 |
| T5 | 32,100 |

### The Fertility Problem

**Fertility** = average tokens per word

Different languages have vastly different fertility:
```
English: "internationalization" → 1-2 tokens
Chinese: "国际化" → 3-6 tokens (due to UTF-8 encoding)
```

Impact: Non-English languages consume more context window for the same semantic content.

### Trade-offs in Vocabulary Size

| Larger Vocabulary | Smaller Vocabulary |
|-------------------|-------------------|
| Shorter sequences | Longer sequences |
| More embedding parameters | Fewer parameters |
| Better coverage | Better generalization |
| Less compositional | More compositional |

### Tokenization Consistency

Critical requirement: The same tokenizer must be used for training and inference.

```python
# Training
tokenizer = GPT2Tokenizer.from_pretrained('gpt2')
model.train(tokenizer.encode(corpus))

# Inference (MUST use same tokenizer)
tokens = tokenizer.encode("Hello world")  # ✓
tokens = other_tokenizer.encode("Hello world")  # ✗ WRONG
```

### Detokenization Challenges

Reversing tokenization requires:
- Preserving whitespace information
- Handling special tokens
- Maintaining case sensitivity

```python
# GPT-style (Ġ prefix for space)
tokens = ["Hello", "Ġworld"]
text = "Hello world"
```

### Practical Example

```python
from transformers import GPT2Tokenizer

tokenizer = GPT2Tokenizer.from_pretrained('gpt2')

text = "Internationalization is important."
tokens = tokenizer.encode(text)
# [15839, 1634, 318, 1593, 13]

decoded = tokenizer.decode(tokens)
# "Internationalization is important."

# View individual tokens
for t in tokens:
    print(f"{t}: '{tokenizer.decode([t])}'")
# 15839: 'International'
# 1634: 'ization'
# 318: ' is'
# 1593: ' important'
# 13: '.'
```

## 📌 Key Points
- Subword tokenization (BPE, WordPiece) balances vocabulary size with sequence length.
- Byte-level BPE eliminates unknown tokens but increases fertility for non-ASCII text.
- Tokenizer consistency between training and inference is critical.
- The fertility problem causes non-English languages to use more context window.

## 🖼️ Recommended Image
- Diagram showing BPE merge process with frequency counts and resulting vocabulary.

## 🔗 Connections
- [[247-Embeddings and Vector Representations]]
- [[248-Context Window Limitations]]
- [[249-Large Language Models LLMs]]
- [[243-Transformer Architecture]]
- [[250-GPT Architecture and Variants]]