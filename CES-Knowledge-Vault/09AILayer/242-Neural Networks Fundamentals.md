---
Theme: Neural Networks Fundamentals
Date created: 2026-02-02 12:00
tags: [AILevel, NeuralNetworks, DeepLearning, Fundamentals]
---

## 📚 Idea/Concept

Neural Networks are mathematical models and computational systems inspired by the biological brain that learn to map inputs to outputs through adjustable parameters. They form the foundation of modern deep learning and are the building blocks for Large Language Models.

### Core Architecture

A neural network consists of layers of interconnected neurons:

```
Input Layer → Hidden Layers → Output Layer
    [x₁]         [h₁]           [y₁]
    [x₂]    →    [h₂]      →    [y₂]
    [x₃]         [h₃]           [y₃]
    ...          ...            ...
```

### Mathematical Foundation

Each neuron computes:
$$z = w_1 \cdot x_1 + w_2 \cdot x_2 + ... + w_n \cdot x_n + b$$
$$a = f(z)$$

Where:
- **w** = weights (learned parameters)
- **x** = inputs
- **b** = bias term
- **f** = activation function
- **a** = activation output

### Key Components

| Component | Description | Role |
|-----------|-------------|------|
| Weights (w) | Learned parameters | Determine influence of each input |
| Biases (b) | Offset terms | Shift activation threshold |
| Activation Functions | Non-linear transforms | Enable complex pattern learning |
| Layers | Stacked neuron groups | Build hierarchical representations |

### Activation Functions

Without activation functions, the network would only compute linear transformations:

**ReLU (Rectified Linear Unit)**:
$$f(x) = \max(0, x)$$

**Sigmoid**:
$$f(x) = \frac{1}{1 + e^{-x}}$$

**Tanh**:
$$f(x) = \frac{e^x - e^{-x}}{e^x + e^{-x}}$$

### Parameters vs Hyperparameters

**Parameters** (learned during training):
- Weights connecting neurons
- Bias terms
- Embedding matrices

**Hyperparameters** (set manually):
- Number of layers
- Hidden dimension size
- Learning rate
- Batch size
- Number of epochs

### Training Process

The learning cycle:
```
1. Forward Pass: Input → Prediction (ŷ)
2. Loss Calculation: L = Loss(ŷ, y)
3. Backward Pass: Compute gradients ∂L/∂θ
4. Parameter Update: θ ← θ - α·∇L
5. Repeat for all samples
```

### Network as Parameterized Function

Mathematically, a neural network is:
$$\hat{Y} = f_\theta(x)$$

Where:
- x = input vector
- ŷ = output prediction
- f_θ = function implemented by network
- θ = all learnable parameters

### Output Transformation

For classification tasks:
1. **Logits**: Raw network outputs (e.g., [-2.89, 0.0, -3.40])
2. **Softmax**: Converts to probabilities summing to 1
3. **Argmax**: Selects most probable class

$$\text{softmax}(z_i) = \frac{e^{z_i}}{\sum_j e^{z_j}}$$

### Architecture Types

| Architecture | Data Type | Key Feature |
|--------------|-----------|-------------|
| CNN | Images/Spatial | Convolutional filters, hierarchical patterns |
| RNN/LSTM/GRU | Sequences | Temporal dependencies, hidden state |
| Transformer | Text/General | Self-attention, parallelization |

### Operating Modes

**Training Phase**:
- Weights and biases adjusted iteratively
- Minimize error between prediction and ground truth
- Uses backpropagation for gradient computation

**Inference Phase**:
- Parameters frozen
- Only forward pass executed
- Model deployed for predictions

## 📌 Key Points
- Neural networks learn by adjusting parameters to minimize a loss function.
- Activation functions introduce non-linearity, enabling complex pattern recognition.
- The network is a parameterized function that maps inputs to outputs.
- Training uses gradient descent with backpropagation; inference only runs forward pass.

## 🖼️ Recommended Image
- Diagram showing multi-layer network with input, hidden, and output layers, highlighting weights, biases, and activation functions.

## 🔗 Connections
- [[243-Transformer Architecture]]
- [[244-Attention Mechanism]]
- [[249-Large Language Models LLMs]]
- [[247-Embeddings and Vector Representations]]
- [[112-CUDA Programming Model]]