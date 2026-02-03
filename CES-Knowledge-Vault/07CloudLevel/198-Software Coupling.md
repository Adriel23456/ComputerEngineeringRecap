---
Theme: Software Coupling
Date created: 2026-02-02 12:00
tags: [CloudLevel, Coupling, Modularity, Architecture, Maintainability]
---

## 📚 Idea/Concept

Coupling measures the degree of interdependence between software modules. Low coupling, achieved through well-defined interfaces, dependency inversion, and indirect invocation, is a fundamental principle for building maintainable, testable, and evolvable systems. The goal is that changes in one module should not force changes in others.

### Concept Overview

Coupling Definition:
```
┌─────────────────────────────────────────────────────────────────┐
│                    COUPLING SPECTRUM                            │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  HIGH COUPLING                              LOW COUPLING        │
│  (Tight)                                    (Loose)             │
│                                                                  │
│  ┌───────┐      ┌───────┐          ┌───────┐      ┌───────┐   │
│  │Module │══════│Module │          │Module │──────│Module │   │
│  │   A   │══════│   B   │          │   A   │      │   B   │   │
│  └───────┘      └───────┘          └───────┘      └───────┘   │
│      │              │                  │              │        │
│      └──────────────┘                  │    ┌────┐   │        │
│       Direct knowledge                 └────│ IF │───┘        │
│       of internals                          └────┘            │
│                                           Interface            │
│                                                                  │
│  Change in B → Forces change in A    Change in B → A unaffected│
│                                      (if interface unchanged)   │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Coupling Taxonomy

Types from worst to best:
```
┌─────────────────────────────────────────────────────────────────┐
│                 COUPLING TYPES (Worst → Best)                   │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  1. CONTENT COUPLING (Worst)                                    │
│     Module A directly accesses/modifies internals of B         │
│     ┌─────────┐                                                │
│     │ Module A│──── accesses B.internalVar ────►┌─────────┐   │
│     └─────────┘                                  │ Module B│   │
│     Example: Directly accessing private fields   └─────────┘   │
│                                                                  │
│  2. COMMON COUPLING                                             │
│     Modules share global data                                  │
│     ┌─────────┐      ┌────────────┐      ┌─────────┐          │
│     │ Module A│◄────►│Global State│◄────►│ Module B│          │
│     └─────────┘      └────────────┘      └─────────┘          │
│     Example: Global variables, shared mutable state            │
│                                                                  │
│  3. CONTROL COUPLING                                            │
│     Module A passes control flag telling B what to do          │
│     ┌─────────┐      flag=2      ┌─────────┐                  │
│     │ Module A│─────────────────►│ Module B│                  │
│     └─────────┘  "do option 2"   └─────────┘                  │
│     Example: Boolean flags changing behavior                   │
│                                                                  │
│  4. STAMP COUPLING                                              │
│     Modules share composite data structure                     │
│     ┌─────────┐   {entire}   ┌─────────┐                      │
│     │ Module A│──{ object }─►│ Module B│                      │
│     └─────────┘              └─────────┘                      │
│     B only uses part of the object                            │
│                                                                  │
│  5. DATA COUPLING (Good)                                        │
│     Modules share only necessary primitive data                │
│     ┌─────────┐   (id, name)   ┌─────────┐                    │
│     │ Module A│───────────────►│ Module B│                    │
│     └─────────┘                └─────────┘                    │
│     Only what's needed, nothing more                          │
│                                                                  │
│  6. MESSAGE COUPLING (Best)                                     │
│     Modules communicate via messages/events                    │
│     ┌─────────┐    ┌───────┐    ┌─────────┐                   │
│     │ Module A│───►│ Event │───►│ Module B│                   │
│     └─────────┘    └───────┘    └─────────┘                   │
│     No direct dependency, fully decoupled                     │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Mechanisms to Reduce Coupling

```
┌─────────────────────────────────────────────────────────────────┐
│              COUPLING REDUCTION TECHNIQUES                      │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  1. INTERFACES / CONTRACTS                                      │
│                                                                  │
│     Bad (concrete dependency):                                 │
│     class OrderService {                                       │
│       private MySQLDatabase db;  // Coupled to MySQL           │
│     }                                                          │
│                                                                  │
│     Good (interface dependency):                               │
│     class OrderService {                                       │
│       private IDatabase db;  // Any implementation works       │
│     }                                                          │
│                                                                  │
│  2. DEPENDENCY INVERSION PRINCIPLE (DIP)                       │
│                                                                  │
│     "High-level modules should not depend on low-level         │
│      modules. Both should depend on abstractions."             │
│                                                                  │
│     ┌──────────────┐         ┌──────────────┐                  │
│     │ OrderService │         │ OrderService │                  │
│     └──────┬───────┘         └──────┬───────┘                  │
│            │                        │                          │
│            ▼                        ▼                          │
│     ┌──────────────┐         ┌──────────────┐                  │
│     │   MySQL DB   │         │ «interface»  │                  │
│     └──────────────┘         │  IDatabase   │                  │
│                              └──────┬───────┘                  │
│     Before: Direct                  │                          │
│                              ┌──────┴───────┐                  │
│                              ▼              ▼                  │
│                        ┌─────────┐    ┌─────────┐             │
│                        │  MySQL  │    │ Postgres│             │
│                        └─────────┘    └─────────┘             │
│                              After: Inverted                   │
│                                                                  │
│  3. DEPENDENCY INJECTION (IoC)                                  │
│                                                                  │
│     // Dependencies provided externally                        │
│     class OrderService {                                       │
│       constructor(db: IDatabase, logger: ILogger) {           │
│         this.db = db;                                         │
│         this.logger = logger;                                 │
│       }                                                        │
│     }                                                          │
│                                                                  │
│     // Injection at composition root                           │
│     const service = new OrderService(                          │
│       new PostgresDB(),                                        │
│       new CloudLogger()                                        │
│     );                                                         │
│                                                                  │
│  4. EVENT-DRIVEN / PUBLISH-SUBSCRIBE                           │
│                                                                  │
│     ┌─────────┐     ┌───────────┐     ┌─────────┐             │
│     │Publisher│────►│Event Bus  │────►│Subscriber│            │
│     │(doesn't │     │           │     │(doesn't  │            │
│     │ know    │     │           │     │ know     │            │
│     │ who     │     │           │     │ who      │            │
│     │ listens)│     │           │     │ publishes│            │
│     └─────────┘     └───────────┘     └─────────┘             │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Coupling in Distributed Systems

Microservices coupling considerations:
```
┌─────────────────────────────────────────────────────────────────┐
│              DISTRIBUTED COUPLING                               │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  TEMPORAL COUPLING                                              │
│  Services must be available at the same time                   │
│                                                                  │
│  Synchronous (temporally coupled):                             │
│  ┌─────────┐    HTTP    ┌─────────┐                           │
│  │Service A│───────────►│Service B│  B must be up!            │
│  └─────────┘            └─────────┘                           │
│                                                                  │
│  Asynchronous (temporally decoupled):                          │
│  ┌─────────┐    ┌───────┐    ┌─────────┐                      │
│  │Service A│───►│ Queue │    │Service B│  B can be down       │
│  └─────────┘    └───────┘───►└─────────┘  temporarily          │
│                                                                  │
│  SPATIAL COUPLING                                               │
│  Services need to know each other's location                   │
│                                                                  │
│  Coupled: A knows B's IP/hostname                              │
│  Decoupled: A uses service discovery / DNS                     │
│                                                                  │
│  DATA/SCHEMA COUPLING                                          │
│  Services share data structures                                │
│                                                                  │
│  Coupled: Shared database, shared models                       │
│  Decoupled: API contracts, schema versioning                   │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Anti-Corruption Layer

Protecting from external coupling:
```
┌─────────────────────────────────────────────────────────────────┐
│              ANTI-CORRUPTION LAYER (ACL)                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  Problem: External system's model pollutes your domain         │
│                                                                  │
│  Without ACL:                                                   │
│  ┌─────────────────┐         ┌─────────────────┐               │
│  │   Your Domain   │◄────────│ Legacy System   │               │
│  │ (contaminated   │         │ (weird models)  │               │
│  │  with legacy    │         │                 │               │
│  │  concepts)      │         │                 │               │
│  └─────────────────┘         └─────────────────┘               │
│                                                                  │
│  With ACL:                                                      │
│  ┌─────────────────┐  ┌─────┐  ┌─────────────────┐             │
│  │   Your Domain   │◄─│ ACL │◄─│ Legacy System   │             │
│  │ (clean model)   │  │     │  │ (isolated)      │             │
│  └─────────────────┘  │Trans│  └─────────────────┘             │
│                       │late │                                   │
│                       └─────┘                                   │
│                                                                  │
│  ACL responsibilities:                                          │
│  - Translate external models to internal                       │
│  - Shield from external changes                                │
│  - Validate and sanitize data                                  │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Measuring Coupling

Indicators and metrics:
```
┌─────────────────────────────────────────────────────────────────┐
│              COUPLING INDICATORS                                │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  Code-Level Metrics:                                            │
│  - Afferent coupling (Ca): Incoming dependencies              │
│  - Efferent coupling (Ce): Outgoing dependencies              │
│  - Instability = Ce / (Ca + Ce)                               │
│                                                                  │
│  Warning Signs:                                                 │
│  ✗ Change in module A requires changes in B, C, D             │
│  ✗ Cannot test module in isolation                            │
│  ✗ Cannot deploy module independently                         │
│  ✗ Circular dependencies exist                                │
│  ✗ God class/module knows everything                          │
│                                                                  │
│  Healthy Signs:                                                 │
│  ✓ Modules have clear, minimal interfaces                     │
│  ✓ Dependencies flow in one direction                         │
│  ✓ Easy to mock dependencies for testing                      │
│  ✓ Can swap implementations without code changes              │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

## 📌 Key Points (Optional)
- Low coupling means modules interact through well-defined interfaces, not internal knowledge.
- Coupling taxonomy helps diagnose and improve module relationships.
- DIP, IoC, and event-driven patterns are key techniques for reducing coupling.
- In distributed systems, consider temporal, spatial, and data coupling.

## 🖼️ Recommended Image
- Diagram showing high coupling (tangled connections) vs low coupling (clean interfaces).

## 🔗 Connections
- [[197-Software Cohesion]]
- [[202-Event Driven Architecture]]
- [[199-Layered Architecture]]
- [[203-Microservices Architecture]]
- [[196-Trade-offs in Software Architecture]]