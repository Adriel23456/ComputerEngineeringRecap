---
Fecha de creación: 2025-10-14 01:51
Fecha de Modificación: 2025-10-14 01:51
tags: [throughput, rendimiento, capacidad, ley-de-little, slo, colas, backpressure]  
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
El **Throughput** (rendimiento) es la **tasa efectiva de trabajo completado por unidad de tiempo**, bajo **criterios de validez y latencia acordados**.
Se calcula como:
**Throughput_real = número de operaciones válidas dentro de T / Δt**
donde:
	- **T** es el intervalo de tiempo observado.
	- **Δt** es la duración de la ventana de medición.
El **Throughput** se distingue de la **capacidad (bandwidth)**, que representa la tasa máxima teórica bajo condiciones ideales.
El throughput real está influenciado por la latencia.  
De acuerdo con la **Ley de Little**, se puede aproximar como:
**Throughput_sostenido ≈ Concurrencia / Latencia_efectiva**
En muchos casos, se toma **Latencia_efectiva = P95(L)** (el percentil 95 de la latencia).
Como **Atributo de Calidad** y **driver de SLOs**, el throughput guía objetivos combinados como:  
	- **Throughput ≥ R operaciones por segundo (ops/s) y P95(L) ≤ T.**

## 📌 Puntos Claves (Opcional)
- **Tasa efectiva** (no conteo bruto) condicionada por **latencia objetivo T**
    
- **Diferenciar** throughput vs. **capacidad** (bandwidth)
    
- **Ley de Little** para razonar sobre concurrencia y latencia efectiva
    
- **SLOs combinados**: tasa mínima + percentil de latencia
    
- **Tácticas**: escala horizontal (réplicas/sharding), paralelismo/asincronía, caching, **batching**, **colas** y **backpressure**, optimización de hot paths
    
- **Validación**: **load/stress testing** para comprobar throughput sostenible

## 🔗 Connections
- [[Latencia]]
    
- [[Disponibilidad de la aplicación]]
    
- [[Trade-off en el diseño y arquitectura de software]]
    
- [[Estrategia de modelado de la arquitectura por puntos de vista]]

## 💡 Personal Insight (Opcional)
- Rendimiento útil = **tasa con calidad**: sirve de poco “bombear” más si llega tarde o mal; **primero la latencia efectiva, luego la tasa**.

## 🧾 Recursos (Opcional)
- AWS (2025) – **Throughput vs. Latency**
    
- GeeksforGeeks (2025) – **Bandwidth vs. Throughput**
    
- Bass, Clements & Kazman (2012) – Tácticas de performance/escalabilidad
    
- Wada et al. (2011) – **Consistencia y trade-offs** (perspectiva de consumidores)