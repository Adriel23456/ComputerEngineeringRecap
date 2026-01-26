---
Fecha de creación: 2025-11-30 06:32
Fecha de Modificación: 2025-11-30 06:32
tags: [escalamiento, horizontal-scaling, vertical-scaling, alta-disponibilidad, load-balancer, stateless, elasticidad]
Tema: Arquitectura Cloud
---
## 📚 Idea/Concepto 
- **Escalamiento vertical** es **hacer más potente una sola máquina** (aumentar CPU/RAM/disco); es **sencillo** pero tiene **límite físico**, puede ser **costoso** y mantiene **punto único de falla**. **Escalamiento horizontal** es **agregar más instancias** para repartir carga usando **Load Balancer**; requiere aplicaciones **stateless** (estado en bases de datos/cachés compartidos), logra **alta disponibilidad** y **escala flexible** según demanda. En la práctica, el **horizontal es el preferido** para aplicaciones **nativas de nube** que soportan picos de tráfico, usando vertical solo hasta donde sea razonable.
#### (1) Vertical (Scale Up)
- **Aumentar recursos** de una instancia (CPU, RAM, disco).  
- **Pros**: simple, sin cambios arquitectónicos profundos.  
- **Contras**: límite físico, costoso, punto único de falla.
#### (2) Horizontal (Scale Out)
- **Agregar más instancias** + **Load Balancer**.  
- **Requisito**: aplicaciones **stateless** (estado externalizado).  
- **Pros**: alta disponibilidad, elasticidad, preferido para cloud-native.
### 💡 Notas adicionales
- El diseño **stateless** es fundamental para aprovechar el horizontal.  
- Combinar ambos puede ser estratégico: vertical hasta cierto punto, luego horizontal.
## 📌 Puntos Claves (Opcional)
- **Vertical** = más potencia en 1 máquina (límites, punto único de falla)  
- **Horizontal** = más instancias + Load Balancer (requiere **stateless**)  
- Horizontal ⇒ **alta disponibilidad** + **elasticidad**  
- En cloud moderno: **horizontal como estrategia principal**
## 🔗 Connections
- [[Virtual Private Cloud (VPC)]]
- [[IaaS vs PaaS vs SaaS]]
## 💡 Personal Insight (Opcional)
- Cuando diseño para horizontal desde el inicio, el sistema respira mejor ante picos de carga inesperados.
## 🧾 Recursos (Opcional)
- Patrones de escalabilidad de AWS, Azure, Google Cloud  
- Martin Fowler – Patterns of Scalability  
- The Twelve-Factor App – Stateless processes