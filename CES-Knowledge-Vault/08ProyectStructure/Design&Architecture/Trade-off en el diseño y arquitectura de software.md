---
Fecha de creación: 2025-10-14 01:51
Fecha de Modificación: 2025-10-14 01:51
tags: [trade-offs, decisiones-arquitectonicas, adr, atam, calidad, deuda-tecnica]  
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
- Un **trade-off** es una **decisión intencional guiada por drivers de negocio y el contexto de diseño** (restricciones técnicas/organizacionales) donde **priorizo** uno o más **atributos de calidad** (rendimiento, seguridad, disponibilidad, costo, etc.) **a costa de otros**. Se **formaliza en un ADR**: problema y criterios, **priorización explícita de QAs** (árbol de utilidad/radar), **comparación de alternativas con un método de evaluación** (p. ej., **ATAM**), estimación de **costo de cambio**, **riesgos**, **deuda técnica** (e “interés” futuro) y **mitigaciones**, con **métricas y disparadores de revisión** para re-evaluarlo en el tiempo.

## 📌 Puntos Claves (Opcional)
- **Impulsado por negocio** y **contexto** (no por gusto tecnológico)
    
- **ADRs** con alternativas elegidas y rechazadas + rationale
    
- **Priorizar QAs** con método formal (árbol de utilidad/radar)
    
- **ATAM** (o equivalente) para comparar alternativas con evidencia
    
- **Deuda técnica** cuantificada y su costo futuro (“interés”)
    
- **Señales de revisión** y métricas para re-evaluación continua

## 🔗 Connections
- [[Estrategia de modelado de la arquitectura por puntos de vista]]
    
- [[Disponibilidad de la aplicación]]
    
- [[Latencia]]
    
- [[Throughput]]

## 💡 Personal Insight (Opcional)
- En arquitectura, **todo es un trade-off**: la diferencia entre improvisar y diseñar es **dejar registro** de por qué ganamos algo sacrificando otra cosa y **cómo lo mitigamos**.

## 🧾 Recursos (Opcional)
- Clements, Kazman & Klein (2001) – **Evaluating Software Architectures (ATAM)**
    
- Richards et al. (2021) – **Software Architecture: The Hard Parts**
    
- Erder et al. (2021) – **Continuous Architecture in Practice**
    
- Bass, Clements & Kazman (2012) – **Software Architecture in Practice**
    
- Stonebraker (2010) – **CAP y consistencia eventual** (discusión práctica)