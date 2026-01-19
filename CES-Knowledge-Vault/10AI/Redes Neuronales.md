---
Fecha de creación: 2025-08-29 17:30
Fecha de Modificación: 2025-08-29 17:30
tags: [redes-neuronales, deep-learning, ai, machine-learning, neural-networks]
Tema: Inteligencia Artificial
---

## 📚 Idea/Concepto 
- Las **Redes Neuronales** son una familia de modelos matemáticos y sistemas computacionales inspirados en el cerebro que aprenden a mapear entradas a salidas. Están armadas en capas con 'neuronas' conectadas por pesos y sesgos, los cuales son los parámetros que el entrenamiento ajusta. La clave para capturar relaciones complejas son las funciones de activación, sin ellas, todo sería lineal. Aprenden con el ciclo forward - pérdida - gradientes - actualización (descenso de gradiente + retropropagación que reparte el error hacia atrás). Ya entrenadas y en producción, solo se ejecuta el forward: no se tocan los parámetros. Si es clasificación, los logits pasan por softmax para leer probabilidades y elegir la clase más probable. Más profundidad y conexiones suman capacidad, pero lo que importa es cómo quedan ajustados los parámetros, qué activaciones usan y cómo se entrenan, apuntando a generalizar bien en datos no vistos (evitando sobreajuste y subajuste). Y además, hay arquitecturas según el tipo de dato: CNN para imágenes, RNN/LSTM/GRU para secuencias y Transformers para texto y tareas de lenguaje.

## 📌 Puntos Claves
- **Arquitectura básica**: Capas de neuronas conectadas por pesos y sesgos ajustables
- **Funciones de activación**: ReLU, Sigmoid, tanh - introducen no-linealidad esencial
- **Proceso de aprendizaje**: Forward pass → cálculo de pérdida → backpropagation → actualización de pesos
- **Modo inferencia**: Solo forward pass, parámetros congelados
- **Arquitecturas especializadas**: CNN (imágenes), RNN/LSTM (secuencias), Transformers (texto)
- **Generalización**: Balance entre underfitting y overfitting mediante regularización

## 🔗 Connections
- [[Large Language Models (LLMs)]] - Los LLMs son redes neuronales masivas basadas en transformers
- [[Mecanismo de atención]] - Componente clave en arquitecturas transformer modernas
- [[Embeddings]] - Capa inicial que convierte tokens en vectores para procesamiento neuronal
- [[Tokenización]] - Preprocesamiento necesario antes de alimentar texto a la red
- [[Ventana de Contexto]] - Limitación arquitectural en redes transformer

## 💡 Personal Insight
- Las redes neuronales no "entienden" realmente, sino que aprenden patrones estadísticos complejos. El verdadero poder viene de la composición de transformaciones simples en capas profundas. Lo más importante no es la cantidad de neuronas, sino cómo se entrenan y regularizan para generalizar bien.

## 🧾 Recursos
- 3Blue1Brown - "But what is a neural network? | Deep learning chapter 1"
- Nielsen, M. - "Neural networks and deep learning"
- Madriz H., D. - "CE 1116 Diseño y Calidad de Productos Tecnológicos"