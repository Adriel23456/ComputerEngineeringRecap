---
Fecha de creación: 2025-11-07 03:48
Fecha de Modificación: 2025-11-07 03:48
tags: [arquitectura-en-capas, modularización, cohesión, acoplamiento, iso42010, nfr, mantenibilidad, testabilidad]
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
- La **arquitectura en capas** es una **partición lógica** que agrupa responsabilidades (presentación, dominio, datos) y regula su interacción con **políticas de dependencia**: flujo **top–down** y llamadas solo a la **capa adyacente** vía **contratos** claros. Favorece **bajo acoplamiento**, **mantenibilidad** y **testabilidad**; puede desplegarse en uno o varios *tiers* físicos. Se deben explicitar **reglas de interacción** y **trade-offs** (latencia, hops) para evitar erosión; **Dependency Inversion** y **PoCs** validan NFRs.

#### (1) Estructura y reglas
- **Capas lógicas**: UI/Presentación ↔ Dominio/Aplicación ↔ Datos/Infraestructura.  
- **Políticas de dependencia**: llamadas solo a la **capa inmediata inferior** (por adyacencia).  
- **Contratos/Interfaces** como frontera: **separación interfaz–implementación** (information hiding).  
- **Tiers físicos** opcionales: despliegue 1-N sin alterar el modelo lógico.

#### (2) Validación y trade-offs
- **Trade-offs**: más saltos (hops) pueden añadir **latencia**; reglas relajadas incrementan riesgo de **erosión del patrón**.  
- **Mitigaciones**: **DIP/IoC**, anti-corrupción entre capas, revisiones/linters de dependencias.  
- **Validación**: **PoCs/spikes** para NFRs (p. ej., tiempo de respuesta, capacidad de cambio); documentar decisiones con **ADRs**.

### 💡 Notas adicionales
- Distinguir **layers (lógico)** de **tiers (físico)**.  
- El **dominio** suele “cortar verticalmente” todas las capas; cuidar **alta cohesión** en cada capa y **bajo acoplamiento** entre ellas.  
- Medir con métricas simples (p. ej., **violaciones de adyacencia**, dependencias ascendentes, complejidad de cambios).

## 📌 Puntos Claves (Opcional)
- **Separación lógica** con flujo **top–down** por adyacencia  
- **Contratos** en fronteras + **DIP/IoC**  
- **Bajo acoplamiento** + **alta cohesión** ⇒ **Modularización**  
- **Trade-offs**: latencia vs. claridad; reglas estrictas evitan erosión  
- **Validación** de NFRs con **PoCs** y decisiones en **ADRs**

## 🔗 Connections
- [[Cohesión en desarrollo de software]]
- [[Acoplamiento en desarrollo de software]]
- [[Trade-off en el diseño y arquitectura de software]]
- [[Estrategia de modelado de la arquitectura por puntos de vista]]

## 💡 Personal Insight (Opcional)
- Las capas me sirven como “líneas de control”: menos atajos hoy = menos deuda mañana.

## 🧾 Recursos (Opcional)
- Bass, Clements & Kazman (2012) – *Software Architecture in Practice*  
- Rozanski & Woods (2011) – *Software Systems Architecture*  
- Kruchten (1995) – “The 4+1 View Model of Architecture”  
- ISO/IEC/IEEE 42010 (2011) – *Architecture description*  
- Fowler (2003) – *Patterns of Enterprise Application Architecture*  
- Brown (2018) – *Software Architecture for Developers*  
- Clements, Kazman & Klein (2001) – *ATAM*