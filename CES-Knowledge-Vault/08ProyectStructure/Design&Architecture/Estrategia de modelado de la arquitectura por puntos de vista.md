---
Fecha de creación: 2025-10-14 01:00  
Fecha de Modificación: 2025-10-14 01:00  
tags: [arquitectura, puntos-de-vista, vistas, iso-42010, adr, calidad]  
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
- La **estrategia de modelado por puntos de vista** consiste en planear y comunicar la arquitectura como un **conjunto coherente de vistas**, cada una generada por un **punto de vista formal** (propósito, alcance, notación y reglas) que responde a **preocupaciones medibles** (atributos de calidad como rendimiento, seguridad, escalabilidad) de cada _stakeholder_. Se sigue la cadena **stakeholders → concerns → viewpoints → views**, se despliega la solución en **múltiples niveles de abstracción** (dominio/lógico, componentes/procesos, despliegue) con **lenguaje visual común**, se **valida con escenarios** críticos y se **documentan ADRs con sus trade-offs** para mantener trazabilidad y facilitar decisiones futuras.

## 📌 Puntos Claves (Opcional)
- Cadena: **stakeholders → concerns → viewpoints → views**
    
- **Atributos de calidad** (QAs) explícitos y medibles como eje de las vistas
    
- **Múltiples niveles** (p. ej., C4 o lógico/proceso/físico) para distintas audiencias
    
- **ADRs**: decisiones significativas + **trade-offs** y costo de cambio
    
- **Escenarios de validación** (+1 del 4+1) para comprobar coherencia entre vistas
    
- **Lenguaje visual ubicuo** para comunicación efectiva

## 🔗 Connections
- [[Trade-off en el diseño y arquitectura de software]]
    
- [[Disponibilidad de la aplicación]]
    
- [[Latencia]]
    
- [[Throughput]]

## 💡 Personal Insight (Opcional)
- Las vistas son el **mapa** y los ADRs el **libro contable** de la arquitectura: juntos evitan el “diagrama gigante” y sostienen decisiones con trazabilidad y contexto.

## 🧾 Recursos (Opcional)
- ISO/IEC/IEEE 42010 (2011/2022) – Descripción de arquitectura y viewpoints
    
- Kruchten (1995) – **Modelo 4+1** de vistas
    
- Rozanski & Woods (2011) – **Viewpoints & Perspectives**
    
- Clements et al. (2010) – **Documenting Software Architectures**
    
- Jansen & Bosch (2005) – **Architecture as Decisions (ADRs)** 