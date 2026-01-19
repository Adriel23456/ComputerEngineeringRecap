---
Fecha de creación: 2025-11-30 06:32
Fecha de Modificación: 2025-11-30 06:32
tags: [grs, geo-redundancia, disaster-recovery, paired-regions, failover, continuidad, rto, rpo, resiliencia]
Tema: Arquitectura Cloud
---
## 📚 Idea/Concepto 
- El **Geo-redundant Storage (GRS)** replica tus datos en **al menos dos regiones geográficas distintas**, físicamente separadas (cientos de km). Los datos se guardan en una **región primaria** y se replican continuamente a una **región secundaria emparejada** (*paired region*), diseñada para no verse afectada por el mismo desastre. Protege incluso si una **región completa falla** (desastre natural, corte masivo), permitiendo **failover** a la región secundaria como parte de una estrategia de **Disaster Recovery (DR)**. El failover suele ser **manual** (decisión operativa), aunque algunos servicios ofrecen automatización, con implicaciones en RTO/RPO.
#### (1) Protección que ofrece
- **Réplicas entre regiones** (primaria → secundaria emparejada).  
- **Máxima resiliencia**: tolera fallas regionales completas.  
- Estrategia central de **DR y continuidad del negocio**.
#### (2) Operación de failover
- **Normalmente manual**: equipo decide cuándo conmutar.  
- Algunos servicios ofrecen **failover gestionado/automatizado**.  
- Define **RTO/RPO** (tiempo de recuperación/pérdida de datos).
### 💡 Notas adicionales
- GRS es el nivel más alto de resiliencia, pero con mayor costo y complejidad.  
- Paired regions están diseñadas para actualizaciones coordinadas y mínima latencia.
## 📌 Puntos Claves (Opcional)
- **Réplicas entre regiones** (primaria + secundaria emparejada)  
- Protege contra **fallas regionales completas**  
- **Failover manual** (típicamente) para DR  
- Máxima resiliencia con implicaciones en **costo/complejidad**
## 🔗 Connections
- [[Almacenamiento redundante en zona (ZRS)]]
- [[Almacenamiento redundante localmente (LRS)]]
- [[Cloud Híbrido]]
## 💡 Personal Insight (Opcional)
- GRS es para cuando el "qué pasaría si..." incluye desastres a escala regional; no es barato, pero es tranquilidad.
## 🧾 Recursos (Opcional)
- Azure paired regions and geo-redundancy  
- AWS Cross-Region Replication  
- Google Cloud disaster recovery planning  
- RTO/RPO planning guidelines