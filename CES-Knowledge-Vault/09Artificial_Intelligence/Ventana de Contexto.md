---
Fecha de creación: 2025-08-29 18:30
Fecha de Modificación: 2025-08-29 18:30
tags: [context-window, transformers, memory-limitations, attention-complexity, llm-constraints]
Tema: Inteligencia Artificial
---

## 📚 Idea/Concepto 
- La **ventana de contexto** es el límite de tokens que un modelo puede procesar simultáneamente, determinado por la complejidad O(n²·d) de self-attention donde n=longitud, d=dimensión. Requiere almacenar matrices de atención n×n×num_heads×num_layers, escalando cuadráticamente en memoria (ej: 100k tokens ≈ 40GB solo para scores). Los modelos se entrenan con positional encodings hasta longitud máxima L; extrapolar más allá degrada performance ("length generalization problem"). El effective context length es menor que el teórico: estudios muestran degradación en retrieval accuracy con distancia ("lost in the middle"). Estrategias de manejo incluyen: truncamiento (sliding window, mantener inicio/fin), compresión (resumir chunks antiguos), o memoria externa (retrieval-augmented generation). Optimizaciones como Flash Attention reducen memoria vía tiling pero mantienen O(n²); alternativas como Linformer/Performer aproximan con O(n). El KV-cache en inferencia almacena keys/values pasados, requiriendo memoria proporcional a batch_size×seq_len×hidden_dim. Ventanas actuales: 4k-8k (modelos estándar), 32k-128k (optimizados), 1M+ (modelos especializados como Gemini 1.5).

## 📌 Puntos Claves
- **Complejidad O(n²)**: Memoria escala cuadráticamente con longitud de secuencia
- **Límite hardware**: GPUs tienen memoria finita para almacenar attention scores
- **Effective vs theoretical**: Performance degrada antes del límite teórico
- **"Lost in the middle"**: Información en medio de contextos largos se pierde
- **KV-cache**: Optimización para inferencia que consume memoria adicional
- **Trade-offs**: Ventana grande = más contexto pero más costo computacional

## 🔗 Connections
- [[Mecanismo de atención]] - La complejidad cuadrática de attention define los límites
- [[Large Language Models (LLMs)]] - Característica limitante fundamental de LLMs
- [[Tokenización]] - Fertility afecta contexto efectivo (más tokens por palabra = menos contexto)
- [[Embeddings]] - Positional encodings limitan extrapolación a secuencias largas
- [[Redes Neuronales]] - Restricción arquitectural de transformers vs RNNs

## 💡 Personal Insight
- La ventana de contexto es quizás la limitación más frustrante de los LLMs actuales. No es solo un problema de memoria, sino de cómo el modelo realmente usa ese contexto. Incluso con ventanas de 100k tokens, los modelos tienden a "olvidar" información del medio. RAG (retrieval-augmented generation) es una solución pragmática hasta que tengamos attention sub-cuadrática efectiva.

## 🧾 Recursos
- IBM Technology - "Transforming Language with Generative Pre-trained Transformers (GPT)"
- Madriz H., D. - "CE 1116 Diseño y Calidad de Productos Tecnológicos"