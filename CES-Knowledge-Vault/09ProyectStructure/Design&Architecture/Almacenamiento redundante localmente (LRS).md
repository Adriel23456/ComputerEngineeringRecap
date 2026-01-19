---
Fecha de creación: 2025-11-30 06:32
Fecha de Modificación: 2025-11-30 06:32
tags: [lrs, redundancia, almacenamiento, durabilidad, storage, resiliencia, datacenter]
Tema: Arquitectura Cloud
---
## 📚 Idea/Concepto 
- El **Locally Redundant Storage (LRS)** guarda tus datos en **varias copias dentro de un mismo centro de datos**, distribuidas en **distintos discos, servidores e incluso racks**. Protege frente a **fallas de hardware locales** (disco, servidor), pero **no replica a otras Zonas de Disponibilidad ni regiones**—todo ocurre en una **única localización física**. Es el nivel más básico y económico de redundancia; si el centro de datos completo falla, los datos no están protegidos (para eso se necesita ZRS o GRS).
#### (1) Protección que ofrece
- **Múltiples copias** en discos/servidores/racks distintos del mismo datacenter.  
- **Resiliencia** frente a fallas de hardware individuales.
#### (2) Limitaciones
- **No protege** contra fallas del datacenter completo.  
- **Una sola localización física** dentro de la región.
### 💡 Notas adicionales
- LRS es adecuado para datos no críticos o cuando se tienen backups externos.  
- Para mayor resiliencia, considerar ZRS o GRS según criticidad.
## 📌 Puntos Claves (Opcional)
- **Varias copias** en un **mismo datacenter** (discos/servidores/racks)  
- Protege contra **fallas de hardware locales**  
- **No replica** a otras AZs ni regiones  
- Nivel más **básico y económico** de redundancia
## 🔗 Connections
- [[Almacenamiento redundante en zona (ZRS)]]
- [[Almacenamiento geo-redundante (GRS)]]
## 💡 Personal Insight (Opcional)
- LRS es mi punto de partida para datos no críticos; si es crítico, siempre subo a ZRS como mínimo.
## 🧾 Recursos (Opcional)
- Azure Storage redundancy – LRS documentation  
- AWS S3 storage classes comparison  
- Google Cloud Storage redundancy options