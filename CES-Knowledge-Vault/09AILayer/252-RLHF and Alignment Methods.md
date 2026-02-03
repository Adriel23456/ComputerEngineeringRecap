---
Theme: RLHF and Alignment Methods
Date created: 2026-02-02 12:00
tags: [AILevel, RLHF, Alignment, InstructGPT, HumanFeedback]
---

## 📚 Idea/Concept

Reinforcement Learning from Human Feedback (RLHF) is the training methodology that aligns LLMs with human intentions by optimizing for human preferences rather than just next-token prediction. Introduced in the InstructGPT paper, it transforms base models into helpful, harmless, and honest assistants.

### The Alignment Problem

Base LLMs trained on next-token prediction have a fundamental misalignment:

**Training objective**: Predict likely next token from internet text
**Desired behavior**: Follow user instructions helpfully and safely

These objectives don't match—a model trained to mimic the internet may produce toxic, false, or unhelpful content.

### The HHH Framework

RLHF aims to make models:

| Property | Description |
|----------|-------------|
| **Helpful** | Follow instructions, provide useful information |
| **Harmless** | Avoid generating harmful, offensive, or dangerous content |
| **Honest** | Don't fabricate information, acknowledge uncertainty |

### The Three Steps of RLHF

```
┌─────────────────────────────────────────────────────────────┐
│  STEP 1: Supervised Fine-Tuning (SFT)                      │
│  • Collect demonstration data from human labelers           │
│  • Train model to imitate desired behavior                  │
│  • Creates initial instruction-following capability         │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│  STEP 2: Reward Model Training                             │
│  • Collect comparison data (response A vs B)               │
│  • Human labelers indicate which response is better        │
│  • Train reward model: R(prompt, response) → scalar        │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────────┐
│  STEP 3: RL Policy Optimization (PPO)                      │
│  • Generate responses from current policy                   │
│  • Score with reward model                                  │
│  • Update policy to maximize reward with KL constraint     │
└─────────────────────────────────────────────────────────────┘
```

### Step 1: SFT Data Collection

Human labelers write ideal responses:

```
Prompt: "Explain photosynthesis to a 5-year-old"

Human-written demonstration:
"Plants are like little chefs! They use sunlight as their 
stove, water as one ingredient, and air as another. They 
mix these together to make their own food, which helps 
them grow big and strong. And guess what? While they're 
cooking, they make fresh air for us to breathe!"
```

### Step 2: Reward Model

**Data Collection**:
```
Prompt: "What is the capital of France?"

Response A: "The capital of France is Paris, a city known 
            for the Eiffel Tower and rich cultural heritage."

Response B: "Paris is the capital. It's in Europe somewhere."

Human ranking: A > B
```

**Reward Model Architecture**:
- Same architecture as LLM (often smaller, e.g., 6B)
- Replace language modeling head with scalar output
- Trained with pairwise ranking loss

**Loss Function**:
$$\mathcal{L}_{RM} = -\log(\sigma(r_w - r_l))$$

Where:
- $r_w$ = reward for preferred response
- $r_l$ = reward for rejected response
- $\sigma$ = sigmoid function

### Step 3: PPO Optimization

**Objective**:
$$\max_\pi \mathbb{E}_{x \sim D, y \sim \pi}[R(x, y)] - \beta \cdot KL[\pi || \pi_{SFT}]$$

Components:
- Maximize expected reward from reward model
- KL penalty prevents diverging too far from SFT model
- β controls the trade-off

**Why KL Penalty?**
- Prevents reward hacking (exploiting reward model flaws)
- Maintains language quality from pretraining
- Ensures stable optimization

### InstructGPT Results

| Model | Size | Human Preference vs GPT-3 175B |
|-------|------|------------------------------|
| InstructGPT 1.3B | 1.3B | 85% preferred |
| InstructGPT 6B | 6B | 87% preferred |
| InstructGPT 175B | 175B | - |

Key finding: A 1.3B aligned model is preferred over 175B unaligned model.

### Alignment Tax

RLHF can slightly degrade performance on some benchmarks:

| Benchmark | GPT-3 | InstructGPT | Change |
|-----------|-------|-------------|--------|
| HellaSwag | 78.9 | 77.2 | -1.7 |
| SQuAD | 52.6 | 48.4 | -4.2 |

This "alignment tax" is generally acceptable given the improvements in safety and helpfulness.

### Separate Reward Models (Llama 2 Approach)

Llama 2 uses two reward models:

| Model | Optimizes For |
|-------|---------------|
| Helpfulness RM | Useful, informative responses |
| Safety RM | Avoiding harmful content |

This addresses the tension: being maximally helpful might mean providing dangerous information.

### Iterative RLHF

Llama 2 performed 5 iterations:
```
RLHF-V1 → RLHF-V2 → RLHF-V3 → RLHF-V4 → RLHF-V5
```

Each iteration:
1. Collect new preference data with current model
2. Retrain reward model
3. Run PPO optimization
4. Evaluate and repeat

### Alternative: Direct Preference Optimization (DPO)

DPO simplifies RLHF by eliminating the reward model:

$$\mathcal{L}_{DPO} = -\log \sigma\left(\beta \log \frac{\pi_\theta(y_w|x)}{\pi_{ref}(y_w|x)} - \beta \log \frac{\pi_\theta(y_l|x)}{\pi_{ref}(y_l|x)}\right)$$

**Advantages**:
- No separate reward model needed
- More stable training
- Simpler implementation

**Disadvantages**:
- Less flexible than reward-based approach
- May be less effective for complex preferences

### Human Annotator Selection

InstructGPT process:
1. Initial screening for agreement with researchers
2. Sensitive content identification ability
3. Ranking consistency
4. Demonstration quality
5. ~40 contractors selected from Upwork/ScaleAI

Inter-annotator agreement: ~72.6%

### Limitations of RLHF

| Limitation | Description |
|------------|-------------|
| Reward hacking | Model exploits reward model flaws |
| Annotator bias | Preferences reflect annotator demographics |
| Scalability | Human feedback is expensive |
| Specification | Hard to fully specify "good" behavior |
| Gaming | Model learns to appear helpful without being helpful |

### Computational Cost

For InstructGPT 175B:
```
SFT:     4.9 petaflops/s-days
PPO:     60 petaflops/s-days
Total:   ~65 petaflops/s-days

Compare to pretraining: 3,640 petaflops/s-days
```

Alignment cost is ~2% of pretraining cost but provides substantial benefits.

## 📌 Key Points
- RLHF aligns LLMs with human preferences through a three-step process: SFT, reward modeling, and policy optimization.
- A small aligned model can be preferred over a much larger unaligned model.
- The KL penalty prevents the model from diverging too far from good language modeling.
- Separate reward models for helpfulness and safety can address competing objectives.

## 🖼️ Recommended Image
- Diagram showing the RLHF pipeline with data flow between SFT model, reward model, and PPO optimization.

## 🔗 Connections
- [[249-Large Language Models LLMs]]
- [[251-LLM Training Pipeline]]
- [[250-GPT Architecture and Variants]]
- [[242-Neural Networks Fundamentals]]