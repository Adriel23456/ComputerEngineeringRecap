---
Fecha de creación: 2025-08-29 19:10
Fecha de Modificación: 2025-08-29 19:10
tags: [tokenization, bpe, wordpiece, nlp-preprocessing, subword-segmentation]
Tema: Inteligencia Artificial
---

## 📚 Idea/Concepto 
- La **tokenización** transforma texto en secuencias de enteros vía segmentación learned o rule-based. Algoritmos principales: BPE aprende fusiones greedy de pares frecuentes; WordPiece optimiza log-likelihood unigram; Unigram Language Model poda vocabulario vía probabilidades marginales. GPT usa byte-level BPE sobre bytes UTF-8 (vocab ~50k), eliminando tokens UNK pero aumentando longitud para texto no-ASCII. El pipeline: texto → normalización (NFKC, casing) → pre-tokenización (split whitespace/punctuation) → subword splitting → mapeo a IDs. Tokens especiales estructuran input/output: padding [PAD], unknown [UNK], mask [MASK], separadores ([CLS], [SEP], <|endoftext|>), y control tokens para fine-tuning. Trade-offs críticos: vocabulario grande (mejor coverage, más parámetros en embeddings) vs pequeño (secuencias más largas, mejor generalización); granularidad character-level (flexible, secuencias largas) vs word-level (semánticamente rico, ineficiente para OOV). Fertility (tokens/palabra) afecta directamente uso del contexto: idiomas no-latinos pueden tener 3-10x fertility vs inglés, reduciendo contexto efectivo. Tokenizer training aprende merges/vocabulario en corpus; inference debe usar modelo idéntico o rompe embeddings. Reversibilidad requiere preservar espacios/casing en vocabulario.

## 📌 Puntos Claves
- **BPE (Byte Pair Encoding)**: Fusiona iterativamente pares de caracteres frecuentes
- **Vocabulario típico**: 30k-50k tokens (GPT-3: 50,257, Llama 2: 32,000)
- **Tokens especiales**: [CLS], [SEP], [PAD], [MASK], <|endoftext|>
- **Fertility problem**: Idiomas no-latinos requieren más tokens, reduciendo contexto efectivo
- **Determinismo**: Tokenizer debe ser idéntico en training e inference
- **Trade-offs**: Balance entre tamaño vocabulario y longitud de secuencias

## 🔗 Connections
- [[Embeddings]] - Los token IDs producidos indexan la matriz de embeddings
- [[Ventana de Contexto]] - Fertility afecta directamente el contexto efectivo disponible
- [[Large Language Models (LLMs)]] - Primer paso del pipeline de procesamiento en LLMs
- [[Redes Neuronales]] - Convierte texto en formato numérico procesable
- [[Mecanismo de atención]] - Define las unidades atómicas sobre las que opera attention

## 💡 Personal Insight
- La tokenización es el héroe no reconocido del NLP moderno. Un mal tokenizer puede arruinar incluso el mejor modelo. El problema de fertility es especialmente frustrante: modelos entrenados principalmente en inglés penalizan sistemáticamente otros idiomas al darles menos contexto efectivo. Es un sesgo arquitectural profundo que necesitamos resolver.

## 🧾 Recursos
- Karpathy, A. - "[1hr Talk] Intro to Large Language Models"
- Madriz H., D. - "CE 1116 Diseño y Calidad de Productos Tecnológicos"