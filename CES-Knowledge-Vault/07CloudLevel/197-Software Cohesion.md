---
Theme: Software Cohesion
Date created: 2026-02-02 12:00
tags: [CloudLevel, Cohesion, Modularity, Architecture, Maintainability]
---

## 📚 Idea/Concept

Cohesion measures how strongly related and focused the responsibilities within a module are. High cohesion means a module has a single, well-defined purpose where all elements work together toward that purpose. Combined with low coupling, high cohesion is the foundation of good modular design, reducing cognitive load and improving maintainability.

### Concept Overview

Cohesion Principle:
```
┌─────────────────────────────────────────────────────────────────┐
│                    COHESION PRINCIPLE                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  "Things that change together should live together"            │
│                                                                  │
│  LOW COHESION                        HIGH COHESION              │
│  ┌─────────────────────┐            ┌─────────────────────┐    │
│  │     UtilsModule     │            │   UserService       │    │
│  │ ┌─────────────────┐ │            │ ┌─────────────────┐ │    │
│  │ │ formatDate()    │ │            │ │ createUser()    │ │    │
│  │ │ sendEmail()     │ │            │ │ updateUser()    │ │    │
│  │ │ calculateTax()  │ │            │ │ deleteUser()    │ │    │
│  │ │ validateUser()  │ │            │ │ validateUser()  │ │    │
│  │ │ compressImage() │ │            │ │ getUserById()   │ │    │
│  │ └─────────────────┘ │            │ └─────────────────┘ │    │
│  └─────────────────────┘            └─────────────────────┘    │
│                                                                  │
│  Unrelated functions                 All about User domain     │
│  "Junk drawer"                       Single responsibility     │
│  Hard to name                        Easy to name              │
│  Changes for many reasons            Changes for one reason    │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Cohesion Taxonomy

Types from worst to best:
```
┌─────────────────────────────────────────────────────────────────┐
│                 COHESION TYPES (Worst → Best)                   │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  1. COINCIDENTAL COHESION (Worst)                              │
│     Elements grouped arbitrarily                               │
│     Example: UtilityFunctions, MiscHelpers                     │
│     "Things are together because... they had to go somewhere"  │
│                                                                  │
│  2. LOGICAL COHESION                                            │
│     Elements do similar things but are unrelated               │
│     Example: InputHandler (handles mouse, keyboard, file input)│
│     "Grouped by category, not by purpose"                      │
│                                                                  │
│  3. TEMPORAL COHESION                                           │
│     Elements executed at the same time                         │
│     Example: StartupModule (init DB, load config, start server)│
│     "Grouped by when, not by what"                             │
│                                                                  │
│  4. PROCEDURAL COHESION                                         │
│     Elements part of same procedure/sequence                   │
│     Example: ReportGenerator (query, format, print, email)     │
│     "Steps in a process, but different concerns"               │
│                                                                  │
│  5. COMMUNICATIONAL COHESION                                    │
│     Elements operate on same data                              │
│     Example: CustomerRecord (validate, save, format customer)  │
│     "Same data, but different operations"                      │
│                                                                  │
│  6. SEQUENTIAL COHESION                                         │
│     Output of one element is input to next                     │
│     Example: DataPipeline (extract → transform → load)         │
│     "Connected by data flow"                                   │
│                                                                  │
│  7. FUNCTIONAL COHESION (Best)                                  │
│     All elements contribute to single, well-defined task       │
│     Example: PasswordHasher (hash, verify, generateSalt)       │
│     "Everything serves one clear purpose"                      │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Single Responsibility Principle

Cohesion formalized:
```
┌─────────────────────────────────────────────────────────────────┐
│              SINGLE RESPONSIBILITY PRINCIPLE                    │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  "A module should have one, and only one, reason to change"    │
│                                     - Robert C. Martin          │
│                                                                  │
│  Bad Example (multiple reasons to change):                     │
│  ┌────────────────────────────────────────────┐                │
│  │             Employee                        │                │
│  │  ┌────────────────────────────────────┐   │                │
│  │  │ calculatePay()      // Accounting  │   │                │
│  │  │ reportHours()       // HR          │   │                │
│  │  │ save()              // IT/DBA      │   │                │
│  │  └────────────────────────────────────┘   │                │
│  └────────────────────────────────────────────┘                │
│  Changes when: pay rules change, reporting changes, DB changes │
│                                                                  │
│  Good Example (single reason to change):                       │
│  ┌──────────────────┐  ┌──────────────────┐  ┌──────────────┐ │
│  │ PayCalculator    │  │ HourReporter     │  │EmployeeRepo  │ │
│  │ ┌──────────────┐ │  │ ┌──────────────┐ │  │┌────────────┐│ │
│  │ │calculatePay()│ │  │ │reportHours() │ │  ││save()      ││ │
│  │ └──────────────┘ │  │ └──────────────┘ │  ││find()      ││ │
│  └──────────────────┘  └──────────────────┘  │└────────────┘│ │
│                                               └──────────────┘ │
│  Each module has ONE reason to change                         │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Cohesion at Different Levels

Applying cohesion principle:
```
┌─────────────────────────────────────────────────────────────────┐
│              COHESION AT DIFFERENT LEVELS                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  METHOD/FUNCTION LEVEL:                                         │
│  ┌────────────────────────────────────────────────────────┐    │
│  │ // Low cohesion: does too many things                  │    │
│  │ function processOrder(order) {                         │    │
│  │   validateOrder(order);                                │    │
│  │   calculateTax(order);                                 │    │
│  │   chargePayment(order);                                │    │
│  │   sendConfirmationEmail(order);                        │    │
│  │   updateInventory(order);                              │    │
│  │   notifyWarehouse(order);                              │    │
│  │ }                                                      │    │
│  │                                                        │    │
│  │ // High cohesion: each function does one thing         │    │
│  │ function validateOrder(order) { ... }                  │    │
│  │ function processPayment(order) { ... }                 │    │
│  │ function fulfillOrder(order) { ... }                   │    │
│  └────────────────────────────────────────────────────────┘    │
│                                                                  │
│  CLASS/MODULE LEVEL:                                            │
│  One clear responsibility per class                            │
│                                                                  │
│  SERVICE/MICROSERVICE LEVEL:                                    │
│  Each service owns a bounded context                           │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐           │
│  │ User    │  │ Order   │  │ Payment │  │Inventory│           │
│  │ Service │  │ Service │  │ Service │  │ Service │           │
│  └─────────┘  └─────────┘  └─────────┘  └─────────┘           │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Identifying Low Cohesion

Warning signs:
```
┌─────────────────────────────────────────────────────────────────┐
│              LOW COHESION WARNING SIGNS                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  NAMING DIFFICULTY                                              │
│  ✗ "Manager", "Handler", "Processor", "Utils", "Helper"        │
│  ✗ Names with "And" (UserAndOrderService)                      │
│  ✗ Generic names that don't describe purpose                   │
│                                                                  │
│  CODE SMELLS                                                    │
│  ✗ Large classes/modules (>500 lines as rule of thumb)        │
│  ✗ Many unrelated methods                                      │
│  ✗ Methods that don't use most instance variables             │
│  ✗ Shotgun surgery (one change affects many places)           │
│                                                                  │
│  TESTING DIFFICULTY                                             │
│  ✗ Tests require extensive setup                               │
│  ✗ Hard to test in isolation                                   │
│  ✗ Tests break for unrelated changes                          │
│                                                                  │
│  CHANGE PATTERNS                                                │
│  ✗ Module changes for multiple different reasons               │
│  ✗ Multiple teams need to modify same module                   │
│  ✗ Frequent merge conflicts in same files                     │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Cohesion and Coupling Relationship

The fundamental balance:
```
┌─────────────────────────────────────────────────────────────────┐
│              COHESION-COUPLING BALANCE                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  Goal: HIGH COHESION + LOW COUPLING                            │
│                                                                  │
│                    ▲ Cohesion                                   │
│                    │                                            │
│            ┌───────┼───────┐                                   │
│            │       │       │                                   │
│     Good   │   ●   │   ○   │  Monolith                        │
│     Design │ Target│       │  (high cohesion                  │
│            │       │       │   but also high coupling)         │
│            ├───────┼───────┤                                   │
│            │       │       │                                   │
│    Chaos   │   ○   │   ○   │  Distributed                     │
│   (worst)  │       │       │  Monolith (worst)                │
│            │       │       │                                   │
│            └───────┴───────┴────────► Coupling                 │
│                   Low      High                                 │
│                                                                  │
│  Trade-off: Increasing cohesion may require more modules,      │
│             which could increase inter-module coupling.         │
│             Find the right granularity.                        │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Practical Guidelines

Achieving high cohesion:
```
┌─────────────────────────────────────────────────────────────────┐
│              COHESION GUIDELINES                                │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  1. NAME TEST                                                   │
│     Can you give it a clear, specific name without "And"?      │
│     Good: OrderValidator, PriceCalculator, EmailSender         │
│     Bad: OrderProcessor, DataManager, ServiceHelper            │
│                                                                  │
│  2. DESCRIPTION TEST                                            │
│     Can you describe it in one sentence without "and"/"or"?    │
│     Good: "Validates order data against business rules"        │
│     Bad: "Handles orders and sends notifications"              │
│                                                                  │
│  3. REASON TO CHANGE TEST                                       │
│     Does it have only one reason to change?                    │
│     List all possible change triggers                          │
│     If > 1, consider splitting                                 │
│                                                                  │
│  4. FEATURE ENVY TEST                                           │
│     Do methods mostly use data from other classes?             │
│     If yes, maybe method belongs in that other class           │
│                                                                  │
│  5. TEAM OWNERSHIP TEST                                         │
│     Could one team own this completely?                        │
│     Multiple teams = probably multiple responsibilities        │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

## 📌 Key Points (Optional)
- High cohesion means all elements in a module contribute to a single, clear purpose.
- The naming test is a quick heuristic: if you can't name it clearly, it lacks cohesion.
- Single Responsibility Principle formalizes cohesion as "one reason to change."
- High cohesion + low coupling is the target for maintainable modular design.

## 🖼️ Recommended Image
- Comparison diagram showing scattered responsibilities (low cohesion) vs focused module (high cohesion).

## 🔗 Connections
- [[198-Software Coupling]]
- [[199-Layered Architecture]]
- [[203-Microservices Architecture]]
- [[196-Trade-offs in Software Architecture]]
- [[176-Algorithm Complexity Analysis]]