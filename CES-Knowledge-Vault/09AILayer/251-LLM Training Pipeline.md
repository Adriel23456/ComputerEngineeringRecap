---
Theme: LLM Training Pipeline
Date created: 2026-02-02 12:00
tags: [AILevel, Training, Pretraining, FineTuning, SFT]
---

## 📚 Idea/Concept

The LLM Training Pipeline is the multi-stage process that transforms a randomly initialized neural network into a capable, aligned assistant. It consists of pretraining on massive text corpora, supervised fine-tuning on curated examples, and alignment through human feedback—each stage building upon the previous.

### The Three Stages

```
Stage 1: PRETRAINING        Stage 2: SFT           Stage 3: ALIGNMENT
┌────────────────────┐   ┌────────────────────┐   ┌────────────────────┐
│  ~10TB raw text    │   │  ~100K instruction │   │  Human preferences │
│  Next-token pred.  │──▶│  pairs             │──▶│  RLHF / DPO        │
│  ~$2-5M compute    │   │  Supervised loss   │   │  Reward modeling   │
│  Base Model        │   │  Instruction Model │   │  Chat Model        │
└────────────────────┘   └────────────────────┘   └────────────────────┘
     6000 GPUs              Fine-tuning              Final alignment
     ~12 days               hours-days               hours-days
```

### Stage 1: Pretraining

**Objective**: Learn general language understanding and knowledge

**Data**:
- Trillions of tokens from diverse sources
- Web crawl, books, code, scientific papers
- Filtered for quality and deduplicated

**Process**:
```
Input:  "The capital of France is"
Target: "Paris"
Loss:   CrossEntropy(predicted, "Paris")
```

**Scale (GPT-3 example)**:
- 300B tokens processed
- 6,000 GPUs for 12 days
- ~$4.6M in compute

**Result**: Base/Foundation model that can:
- Complete text naturally
- Has encoded world knowledge
- Understands grammar, style, facts
- BUT: Not optimized for following instructions

### Stage 2: Supervised Fine-Tuning (SFT)

**Objective**: Teach the model to follow instructions

**Data Format**:
```
<USER>
Write a short poem about the ocean.

<ASSISTANT>
Waves crash upon the sandy shore,
A rhythm ancient, evermore.
The sea whispers secrets deep,
Where countless creatures gently sleep.
```

**Key Insight from Llama 2**: Quality > Quantity

| Approach | Data Size | Result |
|----------|-----------|--------|
| Large-scale third-party | Millions | Lower quality |
| Curated high-quality | ~27,000 | Better alignment |

**Training**:
- Standard supervised learning
- Cross-entropy loss on assistant responses
- Often mask user turns from loss computation

**Result**: Model that:
- Follows instructions
- Responds in appropriate format
- But may still produce harmful/unhelpful content

### Stage 3: Alignment (RLHF)

**Objective**: Align model with human preferences for helpfulness, harmlessness, honesty

**Sub-stages**:

**3a. Reward Model Training**:
```
Prompt: "Explain quantum computing"
Response A: [detailed, accurate explanation]
Response B: [vague, incorrect explanation]
Human label: A > B

Train reward model: R(prompt, response) → scalar
```

**3b. Policy Optimization (PPO)**:
```
For each prompt:
    Generate response from policy (LLM)
    Score with reward model
    Update policy to increase expected reward
    Add KL penalty to stay close to SFT model
```

**Loss Function**:
$$\mathcal{L} = -\mathbb{E}[R(x, y)] + \beta \cdot KL(\pi || \pi_{SFT})$$

### Data Requirements by Stage

| Stage | Data Type | Typical Size | Cost |
|-------|-----------|--------------|------|
| Pretraining | Raw text | Trillions of tokens | $Millions |
| SFT | Instruction pairs | 10K-100K examples | $10K-100K |
| RLHF | Comparisons | 100K-1M comparisons | $100K-1M |

### The Importance of Each Stage

**Without Pretraining**:
- No language understanding
- No world knowledge
- Cannot generalize

**Without SFT**:
- Completes text but doesn't follow instructions
- May continue prompts unexpectedly
- Format inconsistent

**Without RLHF**:
- Follows instructions but may be harmful
- Less helpful, less nuanced
- More likely to hallucinate confidently

### Training Infrastructure

**Hardware**:
- GPU clusters (thousands of A100/H100)
- High-bandwidth interconnect (NVLink, InfiniBand)
- Distributed training frameworks (DeepSpeed, FSDP)

**Techniques**:
- Mixed precision (FP16/BF16)
- Gradient checkpointing
- Pipeline and tensor parallelism
- ZeRO optimization

### Llama 2 Training Specifics

| Aspect | Details |
|--------|---------|
| Pretraining tokens | 2 trillion |
| Context length | 4,096 (up from 2,048) |
| Hardware | Meta RSC + production clusters |
| GPU hours | 3.3M A100 hours |
| CO2 emissions | 539 tCO2eq (offset) |
| SFT data | 27,540 high-quality examples |
| RLHF iterations | 5 versions (V1-V5) |

### Alternative Alignment: DPO

Direct Preference Optimization skips reward modeling:

$$\mathcal{L}_{DPO} = -\log \sigma\left(\beta \log \frac{\pi(y_w|x)}{\pi_{ref}(y_w|x)} - \beta \log \frac{\pi(y_l|x)}{\pi_{ref}(y_l|x)}\right)$$

Advantages:
- Simpler (no separate reward model)
- More stable training
- Competitive results

### Evaluation During Training

| Stage | Metrics |
|-------|---------|
| Pretraining | Perplexity, validation loss |
| SFT | Task-specific benchmarks |
| RLHF | Human evaluation, safety benchmarks |

### Cost Breakdown

For a GPT-3 scale model:
```
Pretraining:  ~$4.6M  (98% of compute)
SFT:          ~$50K   (1% of compute)
RLHF:         ~$50K   (1% of compute)
                      ─────────────
                      ~$4.7M total
```

The pretraining is the expensive part; fine-tuning is relatively cheap.

## 📌 Key Points
- LLM training follows three stages: pretraining (knowledge), SFT (instruction-following), RLHF (alignment).
- Pretraining is the most expensive stage, requiring massive compute and data.
- SFT quality matters more than quantity—thousands of good examples beat millions of mediocre ones.
- RLHF aligns the model with human preferences, making it helpful and safe.

## 🖼️ Recommended Image
- Flowchart showing the three stages with data types, costs, and outputs at each stage.

## 🔗 Connections
- [[249-Large Language Models LLMs]]
- [[250-GPT Architecture and Variants]]
- [[252-RLHF and Alignment Methods]]
- [[242-Neural Networks Fundamentals]]
- [[243-Transformer Architecture]]