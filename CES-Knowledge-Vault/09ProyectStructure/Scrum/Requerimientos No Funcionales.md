---
Fecha de creación: 2025-08-07 19:37
Fecha de Modificación: 2025-08-07 19:37
tags: [requerimientos-no-funcionales, requerimientos, calidad, arquitectura]
Tema: Requerimientos
---

## 📚 Idea/Concepto 
- Los **Requerimientos No Funcionales** (NFR) especifican criterios de calidad y restricciones sobre cómo el sistema debe operar, en lugar de qué funciones debe realizar. Definen atributos del sistema como rendimiento, seguridad, usabilidad, confiabilidad, disponibilidad, mantenibilidad y escalabilidad. Estos requerimientos son críticos para la satisfacción del usuario y el éxito del sistema, aunque a menudo son menos visibles que los funcionales. Impactan significativamente la arquitectura del sistema y las decisiones técnicas. También conocidos como "atributos de calidad" o "requerimientos de calidad".

## 📌 Puntos Claves (Opcional)
- **Categorías principales**: Performance, Seguridad, Usabilidad, Confiabilidad, Escalabilidad, Mantenibilidad
- **Medibles y específicos**: "El sistema debe responder en menos de 2 segundos para el 95% de las solicitudes"
- **Impacto arquitectónico**: Determinan decisiones fundamentales de diseño y tecnología
- **Trade-offs**: Mejorar un NFR puede afectar negativamente otro (ej: seguridad vs usabilidad)
- **Validación continua**: Deben monitorearse constantemente en producción
- **Costo significativo**: Pueden representar 60-80% del esfuerzo total del desarrollo
- **SLA/SLO**: Base para acuerdos de nivel de servicio

## 🔗 Connections
- [[Requerimientos Funcionales]] - NFRs califican cómo se ejecutan las funciones
- [[Requerimientos de Sistema]] - NFRs son parte integral de la especificación del sistema
- [[Definition of Done (DoD)]] - Incluye criterios no funcionales como performance y seguridad
- [[Product Backlog]] - NFRs como historias técnicas o criterios de aceptación
- [[Pruebas Funcionales]] - Se complementan con pruebas no funcionales
- [[User Story]] - NFRs pueden expresarse como historias técnicas o constraints
- [[Sprint Backlog]] - Trabajo técnico para cumplir NFRs
- [[Stakeholder]] - Diferentes stakeholders priorizan diferentes NFRs

## 💡 Personal Insight (Opcional)
- Los NFRs son los héroes silenciosos del desarrollo. Un sistema puede hacer todo lo funcionalmente correcto, pero si es lento, inseguro o difícil de usar, fracasará. El error más costoso es tratarlos como "nice to have" o dejarlos para el final. Los NFRs deben definirse temprano porque retrofitting es extremadamente caro. Mi regla: por cada User Story, pregunta "¿Qué tan rápido? ¿Qué tan seguro? ¿Qué tan fácil?" Los NFRs mal definidos son la principal causa de deuda técnica.

## 🧾 Recursos (Opcional)
- ISO/IEC 25010 - Modelo de calidad del software
- "Software Architecture in Practice" por Bass, Clements y Kazman
- FURPS+ (Functionality, Usability, Reliability, Performance, Supportability)
- Herramientas: JMeter (performance), OWASP ZAP (seguridad), Lighthouse (usabilidad)
