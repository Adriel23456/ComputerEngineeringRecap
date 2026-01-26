---
Fecha de creación: 2025-11-07 03:48
Fecha de Modificación: 2025-11-07 03:48
tags: [acoplamiento, modularización, interfaces, dip, ioc, pub-sub, mantenibilidad]
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
- El **acoplamiento** es la **interdependencia** entre módulos; buscamos **bajo acoplamiento** con **alta cohesión** como base de la **Modularización**. Es bajo cuando los módulos se relacionan por **interfaces/contratos** con **separación interfaz–implementación** (*information hiding*) e **invocación indirecta** (IoC/DIP, inyección, eventos/**pub–sub**), evitando la **propagación** de cambios. Se **clasifica** para diagnosticar riesgos (contenido/control/común/sello/datos/**mensaje**→preferible) y se **gestionan** las **dependencias externas** (versionado, compatibilidad). Reducirlo mejora **mantenibilidad**, **pruebas** y **evolución**.

#### (1) Mecanismos para reducirlo
- **Interfaces/contratos** estables; **DIP/IoC** e inyección.  
- **Mensajería/eventos** para desacoplar tiempo/espacio; **ACLs** / límites de contexto.

#### (2) Diagnóstico y gestión
- Usar la **taxonomía** de acoplamiento; evitar **contenido/control**.  
- **Catálogo de dependencias**, versionado semántico, políticas de compatibilidad.

### 💡 Notas adicionales
- Bajo acoplamiento facilita **paralelizar equipos** y minimizar regresiones.  
- Documentar en **ADRs** las dependencias críticas y alternativas evaluadas.

## 📌 Puntos Claves (Opcional)
- **Bajo acoplamiento** + **alta cohesión** ⇒ **módulos evolutivos**  
- **Interfaces/contratos** + **invocación indirecta** (IoC/DIP, eventos)  
- **Taxonomía** para analizar y mitigar riesgos

## 🔗 Connections
- [[Cohesión en desarrollo de software]]
- [[Arquitectura en capas]]
- [[Arquitectura Event Driven]]

## 💡 Personal Insight (Opcional)
- Cuando el cambio en A **no** obliga a tocar B, sé que voy en el camino correcto.

## 🧾 Recursos (Opcional)
- Parnas (1972) – *On the criteria to be used in decomposing systems into modules*  
- Bass, Clements & Kazman (2012) – *Software Architecture in Practice*  
- Fowler (2003) – *Patterns of Enterprise Application Architecture*  
- McConnell (2004) – *Code Complete*