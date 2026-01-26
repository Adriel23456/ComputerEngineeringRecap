---
Fecha de creación: 2025-11-30 06:32
Fecha de Modificación: 2025-11-30 06:32
tags: [vpc, red-privada, subnets, nsg, cloud-networking, seguridad, aislamiento]
Tema: Arquitectura Cloud
---
## 📚 Idea/Concepto 
- Una **VPC** es tu **red privada dentro de la nube**, aislada del resto de clientes. Vive en una **región específica** y se **divide en subredes** (públicas/privadas) para organizar servidores e IPs. Controlas el **enrutamiento** con tablas de rutas, decides **qué entra y sale** con **firewalls/Network Security Groups (NSG)**, y la puedes **conectar con tu red on-premise** o con otras VPC usando **VPN Gateway o peering**. Es la base para desplegar infraestructura con **aislamiento** y **control de red**.
#### (1) Componentes principales
- **Subnets**: segmentación pública/privada de direcciones IP.  
- **Tablas de rutas**: control de flujo de tráfico entre subnets.  
- **NSG/Firewalls**: reglas de entrada/salida para seguridad.
#### (2) Conectividad
- **VPN Gateway**: conexión segura con infraestructura on-premise.  
- **Peering**: interconexión con otras VPC.
### 💡 Notas adicionales
- La VPC es el primer paso para implementar arquitecturas **multi-tier** en la nube.  
- Diseñar bien las subnets desde el inicio evita rediseños costosos.
## 📌 Puntos Claves (Opcional)
- **Red privada aislada** en una región específica del proveedor cloud  
- **Subnets** para organizar (públicas/privadas) + **tablas de rutas**  
- **NSG/Firewalls** controlan entrada/salida  
- **VPN Gateway/Peering** para conectividad híbrida/multi-VPC
## 🔗 Connections
- [[Cloud Híbrido]]
- [[IaaS vs PaaS vs SaaS]]
- [[Public IP en el cloud]]
- [[Almacenamiento redundante en zona (ZRS)]]
## 💡 Personal Insight (Opcional)
- Una VPC bien diseñada es como tener el control de tu propio data center, pero sin el overhead físico.
## 🧾 Recursos (Opcional)
- Documentación oficial de AWS VPC, Azure VNet, Google Cloud VPC