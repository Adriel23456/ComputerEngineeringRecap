---
Fecha de creación: 2025-11-30 06:32
Fecha de Modificación: 2025-11-30 06:32
tags: [public-ip, nat, networking, seguridad, nsg, ip-estatica, ip-dinamica, accesibilidad]
Tema: Arquitectura Cloud
---
## 📚 Idea/Concepto 
- Una **Public IP** es una **dirección IP pública y globalmente única** asignada a un recurso en cloud (VM, Load Balancer, servicio) para hacerlo **accesible desde Internet**. Normalmente usa **Network Address Translation (NAT)** para traducir la IP pública a la **IP privada** del recurso dentro de su VPC/VNet. Puede ser **Dinámica** (cambia si se libera/reasigna) o **Estática** (fija, útil para DNS/allowlists). Tener Public IP **no significa exposición total**; la seguridad se controla con **Network Security Groups (NSG)** o firewalls que definen **qué puertos y orígenes** tienen acceso.
#### (1) Tipos de asignación
- **IP Pública Dinámica**: cambia si el recurso se libera/reasigna.  
- **IP Pública Estática**: se mantiene fija (necesaria para DNS, allowlists).
#### (2) Control de seguridad
- **NAT**: traduce IP pública → IP privada interna.  
- **NSG/Firewalls**: reglas de puertos y rangos de IP permitidos.
### 💡 Notas adicionales
- La Public IP es la "cara visible" del recurso, pero el acceso real lo controlan las reglas de seguridad.  
- IP estática es crítica para servicios que requieren dirección conocida.
## 📌 Puntos Claves (Opcional)
- **Dirección IP globalmente única** para acceso desde Internet  
- **NAT** traduce pública → privada dentro de VPC  
- **Dinámica** (cambia) vs **Estática** (fija)  
- **NSG/Firewalls** controlan exposición real (puertos/orígenes)
## 🔗 Connections
- [[Virtual Private Cloud (VPC)]]
- [[IaaS vs PaaS vs SaaS]]
## 💡 Personal Insight (Opcional)
- Una IP pública sin NSG bien configurado es como una puerta abierta; la IP solo es el número de la casa.
## 🧾 Recursos (Opcional)
- Documentación de Public IP en AWS (Elastic IP), Azure (Public IP), Google Cloud (External IP)  
- Best practices de seguridad de red en cloud