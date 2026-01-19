---
Fecha de creación: 2025-08-29 17:50
Fecha de Modificación: 2025-08-29 17:50
tags: [llm, transformers, deep-learning, nlp, gpt, bert]
Tema: Inteligencia Artificial
---

## 📚 Idea/Concepto 
- Los **Large Language Models** son modelos de deep learning basados en transformers que contienen miles de millones de parámetros (1B-1T+). Existen variantes arquitectónicas: decoder-only (GPT, Llama), encoder-only (BERT), y encoder-decoder (T5, BART). El preentrenamiento usa objetivos como next-token prediction (autoregresivo) o masked language modeling (bidireccional) sobre corpus de billones de tokens. El proceso de alineación incluye supervised fine-tuning con demostraciones, RLHF con modelos de recompensa, y técnicas como Constitutional AI. Arquitectónicamente, combinan multi-head self-attention (permite atender múltiples relaciones), feedforward networks (procesan representaciones), layer normalization, y residual connections. La generación usa sampling strategies (temperatura, top-k, nucleus sampling) para balancear creatividad y coherencia. Las capacidades emergentes (in-context learning, chain-of-thought, tool use) surgen con escala, aunque persisten limitaciones como alucinaciones, sesgos del dataset, y falta de conocimiento actualizado post-cutoff.

## 📌 Puntos Claves
- **Escala masiva**: Miles de millones de parámetros (GPT-3: 175B, Llama 2: 70B)
- **Arquitecturas**: Decoder-only (GPT), Encoder-only (BERT), Encoder-decoder (T5)
- **Preentrenamiento**: Next-token prediction sobre billones de tokens de texto
- **Alineación**: Supervised fine-tuning + RLHF para seguir instrucciones
- **Capacidades emergentes**: Few-shot learning, chain-of-thought reasoning
- **Limitaciones**: Alucinaciones, sesgos, conocimiento con fecha de corte

## 🔗 Connections
- [[Redes Neuronales]] - Base arquitectural fundamental de los LLMs
- [[Mecanismo de atención]] - Componente central que permite modelar dependencias largas
- [[Ventana de Contexto]] - Límite de tokens procesables simultáneamente
- [[Embeddings]] - Primera capa que transforma tokens en representaciones vectoriales
- [[Tokenización]] - Proceso inicial para convertir texto en formato procesable

## 💡 Personal Insight
- Los LLMs no "comprenden" el lenguaje como los humanos, sino que han aprendido patrones estadísticos extremadamente complejos. Su poder real viene de la escala: con suficientes parámetros y datos, emergen capacidades que no fueron explícitamente programadas. El reto actual no es hacerlos más grandes, sino más alineados y eficientes.

## 🧾 Recursos
- Touvron et al. (2023) - "Llama 2: Open Foundation and Fine-Tuned Chat Models"
- Ouyang et al. - "Training language models to follow instructions with human feedback"
- Vaswani et al. (2017) - "Attention Is All You Need"
- Karpathy, A. - "[1hr Talk] Intro to Large Language Models"