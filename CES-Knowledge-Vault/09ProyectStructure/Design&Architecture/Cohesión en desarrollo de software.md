---
Fecha de creación: 2025-11-07 03:48
Fecha de Modificación: 2025-11-07 03:48
tags: [cohesion, modularización, soc, mantenibilidad, testabilidad]
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
- La **cohesión** mide qué tan fuertemente relacionados están los elementos de un módulo alrededor de un **rol bien definido**; buscamos que las partes que **cambian juntas** estén **juntas** (*SoC*/**Modularización**). La **alta cohesión** favorece **bajo acoplamiento**, reduce **carga cognitiva** y mejora **mantenibilidad/testabilidad**. Se evalúa con la **taxonomía** (p. ej., funcional, secuencial, comunicacional, temporal, lógica).

#### (1) Criterios y señales
- **Nombre del módulo** describe claramente su propósito (rol).  
- Los elementos **cambian juntos** ante el mismo trigger.  
- Pocas razones de cambio; foco funcional claro.

#### (2) Evaluación y métricas
- Revisiones de diseño y **ADRs**; conteo de razones de cambio.  
- Mapear a **taxonomía de cohesión** para detectar mejoras.

### 💡 Notas adicionales
- Alta cohesión reduce la **superficie de impacto** y acelera el onboarding.  
- Úsese junto con **bajo acoplamiento** para lograr **módulos evolutivos**.

## 📌 Puntos Claves (Opcional)
- **Rol bien definido** + “**cambian juntos**”  
- Menos razones de cambio ⇒ **menos complejidad**  
- Taxonomía para diagnóstico: **funcional** ideal

## 🔗 Connections
- [[Acoplamiento en desarrollo de software]]
- [[Trade-off en el diseño y arquitectura de software]]
- [[Estrategia de modelado de la arquitectura por puntos de vista]]

## 💡 Personal Insight (Opcional)
- Si no puedo ponerle un **buen nombre** a un módulo, probablemente su cohesión es baja.

## 🧾 Recursos (Opcional)
- Parnas (1972) – *On the criteria to be used in decomposing systems into modules*  
- McConnell (2004) – *Code Complete*  
- Sommerville (2016) – *Software Engineering*  
- ISO/IEC/IEEE 42010 (2011) – *Architecture description*