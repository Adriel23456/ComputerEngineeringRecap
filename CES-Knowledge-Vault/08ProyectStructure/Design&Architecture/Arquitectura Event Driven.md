---
Fecha de creación: 2025-11-07 03:48
Fecha de Modificación: 2025-11-07 03:48
tags: [eda, eventos, publish-subscribe, mediator, cqrs, event-sourcing, sagas, idempotencia, consistencia-eventual, nfr]
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
- La **Arquitectura Orientada a Eventos (EDA)** es un estilo donde los componentes **reaccionan a eventos** definidos como **hechos inmutables** ya ocurridos, comunicándose de forma **indirecta y asíncrona**. Se implementa en topologías **Broker** (publish/subscribe, descentralizada) y **Mediator/Orquestador** (coordina flujos). No es solo *broadcast*: los consumidores **suscriben/filtran** con **contratos** (esquemas) y garantías (idempotencia, orden, entrega). Potencia **escalabilidad** y **disponibilidad**, con **trade-offs** de **consistencia eventual**, **latencia** y **complejidad**. Aprovecha **Event Sourcing** y **CQRS** para explotar la inmutabilidad y usa **Sagas** para compensación distribuida; las decisiones se respaldan con **ADRs**, métricas y **PoCs** que validan NFRs.

#### (1) Topologías y contratos
- **Broker**: flujo descentralizado **pub/sub**; productores → *topics/colas*; consumidores se **suscriben/filtran**.  
- **Mediator/Orquestador**: coordina pasos y dependencias de negocio.  
- **Contratos/esquemas** versionados; **information hiding** entre productores y consumidores.

#### (2) Garantías y trade-offs
- **Garantías**: **idempotencia**, orden (por partición/clave), entrega (*at-least/exactly-once* mediante reintentos/offsets).  
- **Trade-offs**: **consistencia eventual** vs. latencia/throughput; **trazabilidad** y depuración más difíciles sin orquestación central.  
- **Mitigaciones**: **Sagas** (compensaciones), **Event Sourcing/CQRS**, telemetría end-to-end, **PoCs/spikes** y **ADRs** con métricas (p99 latencia, pérdidas/duplicados).

### 💡 Notas adicionales
- Diseñar **eventos como hechos del pasado** (inmutables) con claves y tiempo.  
- Gobernar **esquemas** y compatibilidad (evolución *backward/forward*).  
- Observar **PACELC** (consistencia ↔ latencia) en replicación/particionado del *broker*.

## 📌 Puntos Claves (Opcional)
- **Hechos inmutables** + comunicación **asíncrona/indirecta**  
- Topologías: **Broker** (descentralizada) y **Mediator** (coordinación)  
- **Contratos** y **garantías** (idempotencia, orden, entrega)  
- **Event Sourcing/CQRS** y **Sagas** para integridad y proyección  
- **Trade-offs**: consistencia eventual, latencia, trazabilidad; validar con **PoCs** y fijar en **ADRs**

## 🔗 Connections
- [[Broker en arquitecturas Event Driven]]
- [[Trade-off en el diseño y arquitectura de software]]
- [[Cohesión en desarrollo de software]]
- [[Acoplamiento en desarrollo de software]]

## 💡 Personal Insight (Opcional)
- Pensar en “hechos” primero me obliga a modelar el lenguaje del negocio y me regala flexibilidad para crecer sin acoplar de más.

## 🧾 Recursos (Opcional)
- Luckham (2002) – *The Power of Events*  
- Mühl, Fiege & Pietzuch (2006) – *Distributed Event-Based Systems*  
- Richards & Ford (2021) – *Software Architecture: The Hard Parts*  
- Fowler (2023) – *Focusing on Events*  
- Wada et al. (2011) – *Data Consistency Trade-offs in Cloud Storage*  
- Baker et al. (2011) – *Megastore: Scalable, Highly Available Storage*  
- ISO/IEC 25010 (2011) – Modelos de calidad (fiabilidad/disponibilidad)