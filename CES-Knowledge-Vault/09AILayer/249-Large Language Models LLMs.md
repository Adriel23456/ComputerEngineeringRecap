---
Theme: Large Language Models LLMs
Date created: 2026-02-02 12:00
tags: [AILevel, LLM, DeepLearning, NLP, GPT]
---

## 📚 Idea/Concept

Large Language Models (LLMs) are deep learning models based on the Transformer architecture containing billions of parameters, trained on massive text corpora to understand and generate human-like text. They represent a paradigm shift from task-specific models to general-purpose language systems capable of emergent abilities not explicitly programmed.

### Defining "Large"

The term "Large" refers to two dimensions:

| Dimension | Scale |
|-----------|-------|
| Parameters | Billions (1B - 1T+) |
| Training Data | Trillions of tokens |

### Taxonomy of AI

```
Artificial Intelligence (AI)
    └── Machine Learning (ML)
            └── Neural Networks
                    └── Deep Learning
                            └── Transformers / LLMs
                                    ├── Encoder-only (BERT)
                                    ├── Decoder-only (GPT)
                                    └── Encoder-Decoder (T5)
```

### Architectural Variants

| Architecture | Representative | Training Objective | Strength |
|--------------|---------------|-------------------|----------|
| Encoder-only | BERT | Masked Language Model | Understanding, Classification |
| Decoder-only | GPT, Llama | Next-token Prediction | Generation |
| Encoder-Decoder | T5, BART | Seq2Seq | Translation, Summarization |

### The Pretraining Objective

Despite the sophistication, LLMs learn from a simple objective:

**Next-Token Prediction** (Autoregressive):
$$P(x_t | x_1, x_2, ..., x_{t-1})$$

Given all previous tokens, predict the next one. This self-supervised objective enables learning from raw text without manual labels.

### Why Next-Token Prediction Works

Learning to predict the next word requires:
- Grammar and syntax
- Semantic relationships
- World knowledge
- Reasoning patterns
- Common sense

All emerge from compression of training data into model weights.

### Scale and Emergent Capabilities

As models scale, new capabilities emerge unpredictably:

| Capability | Description | Emergence Scale |
|------------|-------------|-----------------|
| In-context learning | Learn from examples in prompt | ~1B params |
| Chain-of-thought | Step-by-step reasoning | ~10B params |
| Instruction following | Execute arbitrary instructions | ~100B params |
| Tool use | Interact with external systems | ~100B+ params |

### The Training Pipeline

```
┌─────────────────────────────────────────────────────────────┐
│  Stage 1: PRETRAINING                                       │
│  • Massive unlabeled corpus (trillions of tokens)          │
│  • Next-token prediction objective                          │
│  • Result: Base/Foundation Model                            │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│  Stage 2: SUPERVISED FINE-TUNING (SFT)                     │
│  • Curated instruction-response pairs                       │
│  • Learn to follow instructions                             │
│  • Result: Instruction-tuned Model                          │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│  Stage 3: ALIGNMENT (RLHF/DPO)                             │
│  • Human preference data                                    │
│  • Optimize for helpfulness, harmlessness, honesty         │
│  • Result: Aligned Chat Model                               │
└─────────────────────────────────────────────────────────────┘
```

### Model Scale Comparison

| Model | Parameters | Training Tokens | Context |
|-------|------------|-----------------|---------|
| GPT-2 | 1.5B | 40B | 1K |
| GPT-3 | 175B | 300B | 2K |
| GPT-4 | ~1.7T (est.) | ~13T (est.) | 8K-128K |
| Llama 2 | 7B-70B | 2T | 4K |
| Claude 3 | Unknown | Unknown | 200K |

### Training Costs

Pretraining is extremely expensive:

| Resource | GPT-3 Scale |
|----------|-------------|
| GPU hours | ~3.64M (V100 equivalent) |
| Cost | ~$4.6M (estimated) |
| Time | Weeks-months |
| CO2 | Hundreds of tons |

This cost is why fine-tuning pretrained models is preferred over training from scratch.

### Fundamental Limitations

**Hallucinations**: Generating plausible but false information
- Model has no ground truth access
- Statistical patterns don't guarantee factuality

**Knowledge Cutoff**: Information only up to training date
- Cannot know recent events
- May have outdated information

**Context Limitations**: Fixed window size
- Cannot process arbitrarily long documents
- Information beyond window is inaccessible

**Reasoning Constraints**: Pattern matching, not true reasoning
- Can fail on novel problem structures
- Sensitive to problem presentation

### What LLMs Actually Do

LLMs do NOT:
- Understand in the human sense
- Have beliefs or consciousness
- Access real-time information
- Remember across conversations (without explicit mechanisms)

LLMs DO:
- Learn statistical patterns at massive scale
- Compress knowledge into weights
- Perform sophisticated pattern completion
- Generalize to novel inputs within distribution

### Applications

| Domain | Application |
|--------|-------------|
| Writing | Content generation, editing, summarization |
| Coding | Code generation, debugging, explanation |
| Analysis | Document analysis, sentiment, extraction |
| Conversation | Chatbots, assistants, tutoring |
| Translation | Multi-lingual text conversion |
| Research | Literature review, hypothesis generation |

## 📌 Key Points
- LLMs are Transformer-based models with billions of parameters trained on massive text corpora.
- The core training objective is next-token prediction, which implicitly requires learning language, knowledge, and reasoning.
- Emergent capabilities appear at scale that weren't explicitly programmed.
- The full pipeline includes pretraining, supervised fine-tuning, and alignment via RLHF.

## 🖼️ Recommended Image
- Diagram showing the three-stage training pipeline: Pretraining → SFT → RLHF with data types at each stage.

## 🔗 Connections
- [[243-Transformer Architecture]]
- [[250-GPT Architecture and Variants]]
- [[251-LLM Training Pipeline]]
- [[252-RLHF and Alignment Methods]]
- [[248-Context Window Limitations]]