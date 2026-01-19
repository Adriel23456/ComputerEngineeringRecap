---
Fecha de creación: 2025-11-30 06:32
Fecha de Modificación: 2025-11-30 06:32
tags: [hybrid-cloud, multi-cloud, on-premise, kubernetes, openshift, portabilidad, disaster-recovery]
Tema: Arquitectura Cloud
---
## 📚 Idea/Concepto 
- Un **cloud híbrido** combina **más de un tipo de entorno** (nube privada/on-premise + nube pública + edge) trabajando como **uno solo** mediante una **plataforma común**. La clave no es solo "conectarlos", sino usar **contenedores/orquestadores** (Kubernetes, OpenShift) con **mismas herramientas y procesos** de despliegue/monitoreo. Permite **portar aplicaciones** entre nubes sin reescribir, combinar **control/seguridad** de lo privado con **flexibilidad/escalabilidad** de lo público, y mejorar **Disaster Recovery (DR)** y continuidad del negocio.
#### (1) Componentes del híbrido
- **Nube privada/on-premise**: control y cumplimiento regulatorio.  
- **Nube pública**: elasticidad y servicios gestionados.  
- **Edge**: procesamiento cercano al usuario/dispositivo.
#### (2) Plataforma unificadora
- **Kubernetes/OpenShift**: orquestación consistente entre entornos.  
- **Mismas herramientas**: CI/CD, monitoreo, logging compartidos.
### 💡 Notas adicionales
- Evita **vendor lock-in** al mantener portabilidad de aplicaciones.  
- Ideal para migración gradual desde on-premise hacia cloud.
## 📌 Puntos Claves (Opcional)
- **Múltiples entornos** (privado + público + edge) trabajando como **uno**  
- **Plataforma común** (Kubernetes, mismas herramientas) ⇒ **portabilidad**  
- Combina **control/seguridad** + **flexibilidad/escalabilidad**  
- Mejora **DR** y continuidad del negocio
## 🔗 Connections
- [[Virtual Private Cloud (VPC)]]
- [[IaaS vs PaaS vs SaaS]]
- [[Almacenamiento geo-redundante (GRS)]]
## 💡 Personal Insight (Opcional)
- El híbrido bien hecho no es "tener dos nubes", es tener una estrategia unificada que usa lo mejor de cada entorno.
## 🧾 Recursos (Opcional)
- Red Hat OpenShift Documentation  
- CNCF Kubernetes Documentation  
- Patrones de arquitectura híbrida por proveedores cloud