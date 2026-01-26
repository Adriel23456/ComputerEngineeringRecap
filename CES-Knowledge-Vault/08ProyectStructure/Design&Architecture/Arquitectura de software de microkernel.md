---
Fecha de creación: 2025-11-07 03:48
Fecha de Modificación: 2025-11-07 03:48
tags: [microkernel, plugins, registry, contratos, ipc, cohesion, acoplamiento, mantenibilidad, testabilidad]
Tema: Arquitectura de Software
---

## 📚 Idea/Concepto 
- En una arquitectura de **microkernel** (monolítica), un **core mínimo** (arranque, **IPC**, **seguridad** y *cross-cutting concerns*) **reduce la complejidad ciclomatica** y delega la funcionalidad variable a **plug-ins** de **alta cohesión** y **bajo acoplamiento** con el núcleo. Los plug-ins se **descubren** en un **Registry** y se integran mediante **Contratos** (APIs/protocolos) con interacciones IPC bien definidas. Este aislamiento permite **instalar/actualizar/reiniciar** módulos sin impactar el core, mejorando **mantenibilidad**, **testabilidad** y **extensibilidad**; difiere de **capas** al priorizar un núcleo estable y extensiones *plug-in*.

#### (1) Basada en tiempo
- *N/A directa; medir tiempos de carga/registro de plug-ins y MTTR de reinicio modular.*

#### (2) Basada en eventos
- *N/A directa; evaluar tasa de éxitos en activación/llamadas IPC de plug-ins.*

### 💡 Notas adicionales
- Diseñar **contratos estables** y un **Registry** con versionado/compatibilidad.  
- Mantener **bajo acoplamiento** núcleo↔plug-ins y **alta cohesión** interna en cada plug-in.

## 📌 Puntos Claves (Opcional)
- **Core mínimo** + **plug-ins**; **Registry** + **Contratos**  
- **IPC** seguro y estable; reinicios aislados  
- **Alta cohesión** en plug-ins; **bajo acoplamiento** con el core  
- Enfoque distinto a **capas**: núcleo estable + extensiones

## 🔗 Connections
- [[Cohesión en desarrollo de software]]
- [[Acoplamiento en desarrollo de software]]
- [[Arquitectura en capas]]

## 💡 Personal Insight (Opcional)
- Un core pequeño me deja mover el resto como piezas de LEGO sin tocar el corazón del sistema.

## 🧾 Recursos (Opcional)
- Bass, Clements & Kazman (2012) – *Software Architecture in Practice*  
- Buschmann et al. (1996) – *Pattern-Oriented Software Architecture*  
- Brown (2018) – *Software Architecture for Developers*