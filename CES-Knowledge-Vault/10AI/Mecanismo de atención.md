---
Fecha de creación: 2025-08-29 18:10
Fecha de Modificación: 2025-08-29 18:10
tags: [attention-mechanism, transformers, self-attention, multi-head, deep-learning]
Tema: Inteligencia Artificial
---

## 📚 Idea/Concepto 
- El **mecanismo de atención** en Transformers mapea queries a una suma ponderada de values mediante compatibilidad con keys. Matemáticamente: Attention(Q,K,V) = softmax(QK^T/√dk)V, donde dk=dmodel/h para multi-head. Cada cabeza opera en subespacios diferentes: headi = Attention(QWQi, KWKi, VWVi), luego se concatenan y proyectan: MultiHead(Q,K,V) = Concat(head1...headh)WO. Self-attention (Q=K=V=X) permite modelar dependencias intra-secuencia; masked self-attention previene atención a posiciones futuras mediante máscara -∞; cross-attention usa Q del decoder con K,V del encoder para alineación. Computacionalmente es O(n²d) en memoria pero O(1) en profundidad vs O(n) de RNNs. Los patrones de atención emergen durante entrenamiento: cabezas superficiales capturan relaciones locales/sintácticas, profundas capturan semántica/dependencias largas. Positional encodings se suman a embeddings porque la atención es permutation-invariant. Limitaciones incluyen complejidad cuadrática en longitud de secuencia y dificultad para generalizar a secuencias más largas que las vistas en entrenamiento.

## 📌 Puntos Claves
- **Query, Key, Value**: Proyecciones aprendidas que determinan relevancia entre tokens
- **Scaled dot-product**: QK^T/√dk previene saturación del softmax
- **Multi-head attention**: Múltiples cabezas capturan diferentes tipos de relaciones
- **Self vs Cross-attention**: Intra-secuencia vs inter-secuencia (encoder-decoder)
- **Complejidad O(n²)**: Cuadrática en longitud, limita secuencias largas
- **Permutation-invariant**: Requiere positional encodings para orden

## 🔗 Connections
- [[Large Language Models (LLMs)]] - Componente fundamental de arquitecturas transformer
- [[Redes Neuronales]] - Mecanismo que reemplaza recurrencia en RNNs
- [[Ventana de Contexto]] - Complejidad cuadrática determina límites prácticos
- [[Embeddings]] - Entrada que se transforma en Q, K, V mediante proyecciones
- [[Tokenización]] - Define las unidades sobre las que opera la atención

## 💡 Personal Insight
- La atención es "todo lo que necesitas" porque permite que cada token vea toda la secuencia simultáneamente, eliminando el cuello de botella secuencial de RNNs. La genialidad está en que es diferenciable y paralelizable. Sin embargo, la complejidad cuadrática sigue siendo el talón de Aquiles para secuencias muy largas.

## 🧾 Recursos
- Vaswani et al. (2017) - "Attention Is All You Need"
- 3Blue1Brown - "Attention in transformers, step-by-step | Deep Learning Chapter 6"
- 3Blue1Brown - "Transformers, the tech behind LLMs | Deep Learning Chapter 5"