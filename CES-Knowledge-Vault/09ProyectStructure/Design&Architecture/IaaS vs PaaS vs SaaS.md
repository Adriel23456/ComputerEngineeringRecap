---
Fecha de creación: 2025-11-30 06:32
Fecha de Modificación: 2025-11-30 06:32
tags: [iaas, paas, saas, cloud-services, vendor-lock-in, responsabilidad-compartida, service-models]
Tema: Arquitectura Cloud
---
## 📚 Idea/Concepto 
- **IaaS** te da **infraestructura básica** (VMs, redes, almacenamiento); tú gestionas **SO, middleware, runtime, apps y datos**—mucho **control** pero **alto overhead operativo**. **PaaS** entrega **plataforma lista** para desarrollar; el proveedor gestiona **SO, middleware y runtime**; tú te enfocas en **código, configuración y datos**—acelera desarrollo pero riesgo de **vendor lock-in**. **SaaS** te da la **aplicación completa lista** (acceso por Internet); pagas por **suscripción/uso**—mínimo control arquitectónico, máximo **vendor lock-in**, pero **cero administración** de infraestructura.
#### (1) IaaS – Infrastructure as a Service
- **Proveedor**: VMs, red, almacenamiento.  
- **Tú gestionas**: SO, middleware, runtime, apps, datos.  
- **Características**: máximo control, alto overhead, pago por uso.
#### (2) PaaS – Platform as a Service
- **Proveedor**: infraestructura + SO + middleware + runtime.  
- **Tú gestionas**: código, configuración, datos.  
- **Características**: acelera desarrollo, riesgo de lock-in moderado.
#### (3) SaaS – Software as a Service
- **Proveedor**: aplicación completa lista para usar.  
- **Tú gestionas**: solo configuración/uso.  
- **Características**: cero administración, alto lock-in, pago por suscripción.
### 💡 Notas adicionales
- Elegir el modelo correcto depende del **trade-off** entre control y overhead operativo.  
- Muchas organizaciones usan **combinación** de los tres según la aplicación.
## 📌 Puntos Claves (Opcional)
- **IaaS** = infraestructura (máximo control, alto overhead)  
- **PaaS** = plataforma (acelera desarrollo, lock-in moderado)  
- **SaaS** = aplicación completa (cero admin, alto lock-in)  
- Trade-off: **control vs overhead operativo vs lock-in**
## 🔗 Connections
- [[Virtual Private Cloud (VPC)]]
- [[Cloud Híbrido]]
- [[Escalamiento horizontal y vertical en cloud]]
## 💡 Personal Insight (Opcional)
- IaaS para cuando necesito control fino, PaaS para acelerar entrega, SaaS para funciones no diferenciadoras del negocio.
## 🧾 Recursos (Opcional)
- NIST Cloud Computing Reference Architecture  
- Documentación de modelos de servicio de AWS, Azure, Google Cloud  
- Gartner – Cloud Service Models Comparison