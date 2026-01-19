---
Fecha de creación: 2025-08-29 18:50
Fecha de Modificación: 2025-08-29 18:50
tags: [embeddings, word-embeddings, vector-representations, nlp, transformers]
Tema: Inteligencia Artificial
---

## 📚 Idea/Concepto 
- Los **embeddings** son proyecciones aprendibles de tokens discretos a vectores continuos densos. La matriz de embeddings E ∈ R^(|V|×d) mapea vocabulario V a dimensión d vía lookup: ei = E[token_id]. En transformers, el embedding final combina múltiples componentes: token embeddings (semántica), positional embeddings (orden vía sinusoides o learned), y opcionalmente segment/type embeddings (BERT). Matemáticamente: input = TokenEmb(x) + PosEmb(pos) + TypeEmb(type). Weight tying (compartir E entre input/output) reduce parámetros y mejora generalización. Los embeddings aprenden estructura del lenguaje: relaciones geométricas codifican analogías semánticas/sintácticas (medibles vía cosine similarity), clustering captura categorías conceptuales, y las dimensiones individuales pueden correlacionar con propiedades lingüísticas específicas. Subword tokenization (BPE/WordPiece) permite embeddings compositivos: palabras raras se construyen desde subunidades frecuentes. Durante training, embeddings se inicializan ~N(0, 0.02) y se optimizan vía gradientes como cualquier parámetro. Los embeddings contextualizados de capas superiores (vs estáticos de entrada) capturan significado dependiente del contexto, resolviendo polisemia.

## 📌 Puntos Claves
- **Lookup table**: Matriz E donde cada fila es un vector denso para cada token
- **Alta dimensionalidad**: Típicamente 128-4096 dimensiones (GPT-3: 12,288)
- **Componentes combinados**: Token + positional + type embeddings
- **Propiedades algebraicas**: vec(king) - vec(man) + vec(woman) ≈ vec(queen)
- **Weight tying**: Compartir embeddings entrada/salida reduce parámetros
- **Contextualización**: Self-attention transforma embeddings estáticos en contextuales

## 🔗 Connections
- [[Tokenización]] - Define los IDs que indexan la matriz de embeddings
- [[Mecanismo de atención]] - Transforma embeddings en Q, K, V para procesamiento
- [[Redes Neuronales]] - Primera capa que convierte símbolos discretos en vectores continuos
- [[Large Language Models (LLMs)]] - Componente fundamental de la arquitectura transformer
- [[Ventana de Contexto]] - Positional embeddings limitan generalización a secuencias largas

## 💡 Personal Insight
- Los embeddings son quizás el concepto más elegante en NLP moderno: convierten el problema simbólico discreto del lenguaje en uno geométrico continuo. La magia está en que durante el entrenamiento, palabras con roles similares naturalmente se agrupan en el espacio vectorial, creando una "geografía del significado" navegable matemáticamente.

## 🧾 Recursos
- IBM Technology - "What are Word Embeddings?"
- Madriz H., D. - "CE 1116 Diseño y Calidad de Productos Tecnológicos"
- 3Blue1Brown - "Transformers, the tech behind LLMs | Deep Learning Chapter 5"