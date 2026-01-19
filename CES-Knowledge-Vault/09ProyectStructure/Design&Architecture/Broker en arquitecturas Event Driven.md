---
Fecha de creación: 2025-11-07 03:48
Fecha de Modificación: 2025-11-07 03:48
tags: [eda, broker, publish-subscribe, topics, colas, pacelc, idempotencia, sagas, adr, poc]
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
- Un **broker** habilita **invocación indirecta** y **publish–subscribe** en una topología de **flujo descentralizado**: productores publican en **topics/colas** y consumidores se **suscriben/filtran** sin acoplamiento directo. El broker **enruta** y aplica **garantías** (orden, reintentos, entrega *at-least/exactly-once*), pero **no orquesta** flujos complejos (rol de un **Mediator**). **Trade-offs**: **trazabilidad** y **consistencia** más difíciles; la **replicación/partición** del broker expone **PACELC** (consistencia vs. latencia). Para integridad de negocio se usan **Sagas/compensaciones**; las garantías se prueban con **spikes/PoCs** y se fijan en **ADRs**.

#### (1) Basada en tiempo
- *N/A para broker; mantener SLAs/latencias objetivo del pipeline y del delivery (p50/p95/p99).*

#### (2) Basada en eventos
- Éxito de entrega = **mensajes procesados sin duplicación** / **mensajes publicados**, sujeto a garantías y reintentos.

### 💡 Notas adicionales
- Definir **contratos/esquemas** versionados; documentar **semántica de entrega** y **clave de ordenación**.
- Observabilidad: **trazas** end-to-end, **correlation IDs**, **DLQs** y **retención** controlada.

## 📌 Puntos Claves (Opcional)
- **Pub/Sub** con **flujo descentralizado**; el broker **no** orquesta  
- **Garantías**: orden, entrega, reintentos; **DLQ**  
- **PACELC** por replicación/partición  
- **Sagas** para integridad; **ADRs/PoCs** para validar

## 🔗 Connections
- [[Arquitectura Event Driven]]
- [[Trade-off en el diseño y arquitectura de software]]
- [[Acoplamiento en desarrollo de software]]

## 💡 Personal Insight (Opcional)
- El broker me da velocidad y desacoplamiento; pago con observabilidad y control de consistencia.

## 🧾 Recursos (Opcional)
- Richards & Ford (2021) – *Software Architecture: The Hard Parts*  
- Luckham (2002) – *The Power of Events*  
- Mühl, Fiege & Pietzuch (2006) – *Distributed Event-Based Systems*  
- Wada et al. (2011) – *Data Consistency Trade-offs in Cloud Storage*