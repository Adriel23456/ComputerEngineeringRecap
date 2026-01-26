---
Fecha de creación: 2025-10-14 01:51
Fecha de Modificación: 2025-10-14 01:51
tags: [latencia, rendimiento, p95, p99, qos, slo, colas]  
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
- La **Latencia** es un **Atributo de Calidad** (y una restricción de **QoS**) que cuantifica el tiempo entre la **solicitud del cliente** y la **respuesta útil del sistema**.
- Se calcula como:
**Latencia = tiempo_respuesta - tiempo_solicitud ≈ L_red + L_cola + L_proceso**
donde:
- **L_red** representa la latencia de red (transmisión o ida y vuelta - RTT).
- **L_cola** corresponde al tiempo de espera o contención.
- **L_proceso** refleja el tiempo de ejecución (síncrona o asíncrona).
La latencia se **gestiona por percentiles**, no por promedios.  
Por ejemplo: **SLO: P95(L) ≤ T**, donde el percentil 95 de la latencia debe estar por debajo del umbral T definido según los **drivers de negocio** y el **Costo de la Calidad**.
Como **driver**, la latencia impone **trade-offs** (por ejemplo, entre **consistencia** y **costo**) y guía tácticas de optimización por componente.

## 📌 Puntos Claves (Opcional)
- **Descomposición**: LredL_{red}Lred​, LcolaL_{cola}Lcola​, LprocesoL_{proceso}Lproceso​
    
- **Medición por percentiles** (P95/P99) + **SLI/SLO** explícitos
    
- **Trade-offs**: latencia vs. consistencia/costo
    
- **Tácticas**:
    
    - LredL_{red}Lred​: co-ubicación, CDN, conexiones persistentes, compresión, payloads ligeros
        
    - LcolaL_{cola}Lcola​: control de admisión, backpressure, bulkheads, priorización y dimensionamiento
        
    - LprocesoL_{proceso}Lproceso​: caching/memoización, precálculo, asincronía/paralelismo, optimización de consultas/índices
        
- **Validación**: pruebas de carga/estrés enfocadas en P95/P99 + evaluación arquitectónica (ATAM)

## 🔗 Connections
- [[Throughput]]
    
- [[Disponibilidad de la aplicación]]
    
- [[Trade-off en el diseño y arquitectura de software]]
    
- [[Estrategia de modelado de la arquitectura por puntos de vista]]

## 💡 Personal Insight (Opcional)
- La media **miente**: el usuario sufre el **P95/P99**. Diseñar para percentiles altos protege la experiencia real.

## 🧾 Recursos (Opcional)
- Brutlag (2009) – **Speed Matters** (impacto de ms en negocio)
    
- IBM Think (2025) – **What is latency?**
    
- Bass, Clements & Kazman (2012) – Tácticas de performance
    
- Clements, Kazman & Klein (2001) – **ATAM**