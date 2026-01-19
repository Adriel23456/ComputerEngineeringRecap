---
Fecha de creación: 2025-08-07 19:37
Fecha de Modificación: 2025-08-07 19:37
tags: [estandares-programacion, codigo-limpio, best-practices, implementacion, calidad-codigo]
Tema: Implementación
---

## 📚 Idea/Concepto 
- Los **Estándares de Programación** son un conjunto de reglas, convenciones y mejores prácticas acordadas por un equipo de desarrollo para escribir código consistente, mantenible y de alta calidad. Incluyen convenciones de nomenclatura, estructura de código, formato, documentación, manejo de errores y patrones de diseño. Estos estándares aseguran que el código sea legible y mantenible por cualquier miembro del equipo, reducen la deuda técnica, facilitan el onboarding de nuevos desarrolladores y minimizan bugs. Son fundamentales para la colaboración efectiva y la calidad sostenible del software a largo plazo.

## 📌 Puntos Claves (Opcional)
- **Convenciones de nomenclatura**: camelCase, PascalCase, snake_case según el contexto
- **Estructura de código**: Organización de archivos, carpetas, módulos y componentes
- **Principios SOLID**: Single Responsibility, Open/Closed, Liskov, Interface Segregation, Dependency Inversion
- **Clean Code**: Nombres descriptivos, funciones pequeñas, sin código duplicado (DRY)
- **Documentación**: Comentarios significativos, README, documentación de APIs
- **Manejo de errores**: Try-catch consistente, logging estructurado, códigos de error estándar
- **Code Reviews**: Proceso para asegurar cumplimiento de estándares
- **Linters y formatters**: Herramientas automáticas (ESLint, Prettier, Black, RuboCop)

## 🔗 Connections
- [[Definition of Done (DoD)]] - Incluye cumplimiento de estándares de código
- [[Pruebas Unitarias]] - Parte de los estándares de calidad de código
- [[Sprint Backlog]] - Trabajo debe cumplir estándares definidos
- [[Taskboard]] - Revisión de código como parte del flujo
- [[Microservicios]] - Cada servicio debe seguir estándares consistentes
- [[Azure DevOps]] - Pipelines enforzan estándares automáticamente
- [[Velocity]] - Estándares consistentes mejoran la velocidad a largo plazo
- [[Requerimientos No Funcionales]] - Estándares implementan NFRs de mantenibilidad

## 💡 Personal Insight (Opcional)
- Los estándares de programación son como la higiene del código: invisibles cuando están bien, pero obvios cuando faltan. El secreto está en el balance: suficientes reglas para mantener consistencia, pero no tantas que paralicen el desarrollo. Los mejores estándares son los que el equipo adopta y evoluciona orgánicamente, no los impuestos desde arriba. Mi regla: si tienes que pensar en el formato mientras codificas, necesitas mejor tooling automático. El código debe ser escrito para humanos, no para máquinas. "Any fool can write code that a computer can understand. Good programmers write code that humans can understand." - Martin Fowler.

## 🧾 Recursos (Opcional)
- "Clean Code" por Robert C. Martin
- "The Pragmatic Programmer" por Hunt y Thomas
- Google Style Guides (para varios lenguajes)
- "Refactoring" por Martin Fowler
- Herramientas: SonarQube, ESLint, Prettier, StyleCop, Checkstyle