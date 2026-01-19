---
Fecha de creación: 2025-11-30 06:32
Fecha de Modificación: 2025-11-30 06:32
tags: [zrs, redundancia, availability-zones, almacenamiento, resiliencia, durabilidad, alta-disponibilidad]
Tema: Arquitectura Cloud
---
## 📚 Idea/Concepto 
- El **Zone-Redundant Storage (ZRS)** replica tus datos **automáticamente en múltiples Zonas de Disponibilidad (AZs)** dentro de la **misma región**. Cada AZ es un **centro de datos físicamente separado** (edificio distinto, energía/red/refrigeración independientes). Los datos se guardan en **al menos tres zonas**; si una zona completa falla, las otras siguen con copias válidas. Ofrece **más resiliencia que LRS** (que solo protege dentro de un datacenter), pero sigue limitado a **una sola región**—para desastres regionales se necesita GRS.
#### (1) Protección que ofrece
- **Réplicas en múltiples AZs** (mínimo 3) dentro de la región.  
- **Resiliencia** ante falla de un datacenter completo (AZ).
#### (2) Limitaciones
- **Limitado a una región**: no protege contra caída regional total.  
- Mayor costo y latencia que LRS, pero menor que GRS.
### 💡 Notas adicionales
- ZRS es ideal para aplicaciones de alta disponibilidad dentro de una región.  
- Combinar ZRS con arquitectura multi-AZ maximiza resiliencia.
## 📌 Puntos Claves (Opcional)
- **Réplicas automáticas** en múltiples **AZs** (mínimo 3) de la región  
- Cada AZ = **datacenter físicamente separado**  
- **Más resiliencia que LRS**, tolera caída de AZ completa  
- Limitado a **una región** (para regional → GRS)
## 🔗 Connections
- [[Almacenamiento redundante localmente (LRS)]]
- [[Almacenamiento geo-redundante (GRS)]]
- [[Virtual Private Cloud (VPC)]]
## 💡 Personal Insight (Opcional)
- ZRS es mi estándar para producción; protege contra lo más común sin la complejidad de GRS.
## 🧾 Recursos (Opcional)
- Azure Storage redundancy – ZRS documentation  
- AWS S3 availability and durability  
- Google Cloud multi-regional vs regional storage