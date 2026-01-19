---
Fecha de creación: 2025-10-14 01:51
Fecha de Modificación: 2025-10-14 01:51
tags: [disponibilidad, fiabilidad, mtbf, mttr, slo, sli, sre, caos]  
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
- La **Disponibilidad** es la **capacidad del sistema de estar operativo y responder correctamente dentro de umbrales acordados**, definida como **Atributo de Calidad** y **gobernada por SLO/SLI**.
Se mide en dos planos complementarios:
#### (1) Basada en tiempo
La disponibilidad basada en tiempo se calcula con la fórmula:
**Disponibilidad_tiempo = MTBF / (MTBF + MTTR)**
donde:
- **MTBF** = Mean Time Between Failures (tiempo promedio entre fallas)
    
- **MTTR** = Mean Time To Repair (tiempo promedio de reparación)
#### (2) Basada en eventos
La disponibilidad basada en eventos se calcula con la fórmula:
**Disponibilidad_eventos = (respuestas válidas dentro de T) / (solicitudes válidas)**
donde:
- “Respuestas válidas dentro de T” son aquellas que cumplen con la **latencia objetivo T**
    
- Se consideran **fallas lógicas** y respuestas que **exceden la latencia objetivo**
### 💡 Notas adicionales
- La Disponibilidad guía **trade-offs** como disponibilidad vs. latencia o consistencia.
- Se habilita mediante tácticas que:
    - **Aumentan MTBF**, eliminando puntos únicos de falla (SPOF), aplicando redundancia, partición/aislamiento y _circuit breakers_.
    - **Reducen MTTR**, mediante observabilidad, despliegues blue/green o canary, _failover_ automatizado y _runbooks_.

## 📌 Puntos Claves (Opcional)
- **Dos lecturas**: uptime (MTBF/MTTR) y **tasa de éxito** con umbral de **latencia**
    
- **SLO/SLI** claros y presupuestos de error
    
- **Trade-offs** con latencia y consistencia (percibida como indisponibilidad)
    
- **Tácticas**: redundancia, partición, circuit breakers, observabilidad, despliegues seguros
    
- **“Nueves” justificados** por **Costo de la Calidad** / evaluación (ATAM)
    
- **Validación continua**: **Failover Testing** y **Chaos Engineering**; **alineación organizacional** (Conway, SRE)

## 🔗 Connections
- [[Latencia]]
    
- [[Trade-off en el diseño y arquitectura de software]]
    
- [[Throughput]]
    
- [[Estrategia de modelado de la arquitectura por puntos de vista]]

## 💡 Personal Insight (Opcional)
- La disponibilidad útil no es solo “estar arriba”: es **servir bien y a tiempo** bajo falla, picos y cambios; lo demás es ilusión de uptime.

## 🧾 Recursos (Opcional)
- ISO/IEC 25010 – Modelo de calidad (fiabilidad/disponibilidad)
    
- DevOps Handbook (2021) – **SRE/operabilidad**
    
- Conway (1968) – **Estructura organizacional ↔ arquitectura**
    
- Bass, Clements & Kazman (2012) – Tácticas de disponibilidad
    
- Clements, Kazman & Klein (2001) – **ATAM**