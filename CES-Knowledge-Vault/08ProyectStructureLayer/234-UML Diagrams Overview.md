---
Theme: UML Diagrams Overview
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, UML, Documentation, Modeling]
---

## 📚 Idea/Concept

UML (Unified Modeling Language) is a standardized visual modeling language used to specify, visualize, construct, and document software system artifacts. It provides a common vocabulary for software design and is divided into two main categories: Structural Diagrams (showing static system structure) and Behavioral Diagrams (showing dynamic system behavior). UML is essential for communicating complex designs among stakeholders and development teams.

### UML Diagram Categories

```
┌─────────────────────────────────────────────────────────────────┐
│                      UML DIAGRAMS                               │
├─────────────────────────────────────────────────────────────────┤
│  STRUCTURAL DIAGRAMS           │  BEHAVIORAL DIAGRAMS           │
│  (Static Structure)            │  (Dynamic Behavior)            │
├────────────────────────────────┼────────────────────────────────┤
│  • Class Diagram ★             │  • Use Case Diagram ★          │
│  • Object Diagram              │  • Sequence Diagram ★          │
│  • Component Diagram           │  • Activity Diagram            │
│  • Package Diagram             │  • State Machine Diagram       │
│  • Deployment Diagram          │  • Communication Diagram       │
│  • Composite Structure         │  • Interaction Overview        │
│  • Profile Diagram             │  • Timing Diagram              │
│                                │                                │
│  ★ Most commonly used          │  ★ Most commonly used          │
└────────────────────────────────┴────────────────────────────────┘
```

### Most Important Diagrams for Projects

```
PROJECT PHASE → RECOMMENDED DIAGRAMS

Requirements:
├── Use Case Diagram (user interactions)
└── Activity Diagram (business processes)

Design:
├── Class Diagram (OOP structure)
├── Sequence Diagram (object interactions)
└── Component Diagram (system architecture)

Deployment:
└── Deployment Diagram (infrastructure)
```

### 1. Use Case Diagram

```
Purpose: Show system functionality from user perspective

┌─────────────────────────────────────────┐
│           E-Commerce System             │
│                                         │
│    ┌─────────┐                         │
│    │ Browse  │                         │
│    │ Products│◄────────┐               │
│    └─────────┘         │               │
│         │              │               │
│         │         ┌────┴────┐          │
│    ┌────▼────┐    │ Customer │          │
│    │ Add to  │◄───│  (Actor) │          │
│    │  Cart   │    └────┬────┘          │
│    └────┬────┘         │               │
│         │              │               │
│    ┌────▼────┐         │               │
│    │Checkout │◄────────┘               │
│    └─────────┘                         │
└─────────────────────────────────────────┘

Elements:
• Actor (stick figure): External user/system
• Use Case (oval): System function
• Association (line): Actor uses function
• System boundary (rectangle): System scope
```

### 2. Sequence Diagram

```
Purpose: Show object interactions over time

    Customer      WebUI       OrderService    Database
        │           │              │             │
        │──Browse───▶              │             │
        │           │──getProducts──▶            │
        │           │              │───query────▶│
        │           │              │◄──results───│
        │           │◄─productList─│             │
        │◄─display──│              │             │
        │           │              │             │
        │──addCart──▶              │             │
        │           │──createOrder──▶            │
        │           │              │───insert───▶│
        │           │              │◄───ok──────│
        │           │◄──orderId────│             │
        │◄─confirm──│              │             │
        ▼           ▼              ▼             ▼

Elements:
• Lifeline (vertical dashed): Object existence
• Message (arrow): Method call
• Activation box: Processing time
• Return (dashed arrow): Response
```

### 3. Activity Diagram

```
Purpose: Show workflow or process flow

        ●──────────────────────────────────
        │                                 │
        ▼                                 │
   ┌─────────┐                           │
   │ Receive │                           │
   │  Order  │                           │
   └────┬────┘                           │
        │                                 │
        ◇ (Decision)                     │
       ╱ ╲                               │
      ╱   ╲                              │
   Yes     No                            │
    │       │                            │
    ▼       ▼                            │
┌───────┐ ┌────────┐                    │
│Process│ │ Reject │                    │
│Payment│ │ Order  │                    │
└───┬───┘ └────────┘                    │
    │                                    │
    ▼                                    │
┌───────┐                               │
│ Ship  │                               │
│ Order │                               │
└───┬───┘                               │
    │                                    │
    ▼                                    │
    ◎ (End)                              │
─────────────────────────────────────────

Elements:
• Initial node (filled circle): Start
• Final node (circle with dot): End
• Action (rounded rectangle): Activity
• Decision (diamond): Branch point
• Fork/Join (bar): Parallel activities
```

### 4. Component Diagram

```
Purpose: Show system components and dependencies

┌────────────────────────────────────────────────┐
│              <<subsystem>>                     │
│              E-Commerce App                    │
│                                                │
│  ┌──────────────┐      ┌──────────────┐       │
│  │  <<component>>│      │ <<component>>│       │
│  │   Frontend   │──────▶│   Backend   │       │
│  │              │      │              │       │
│  └──────────────┘      └──────┬───────┘       │
│                               │               │
│                               ▼               │
│                        ┌──────────────┐       │
│                        │ <<component>>│       │
│                        │   Database   │       │
│                        └──────────────┘       │
└────────────────────────────────────────────────┘

Elements:
• Component (rectangle with icon): Module
• Interface (lollipop): Provided service
• Dependency (dashed arrow): Uses
• Port (square): Connection point
```

### 5. Deployment Diagram

```
Purpose: Show physical deployment of artifacts

┌─────────────────┐      ┌─────────────────┐
│  <<device>>     │      │   <<device>>    │
│  Client PC      │      │   Web Server    │
│  ┌───────────┐  │      │  ┌───────────┐  │
│  │ <<artifact>>│ │ HTTP │  │<<artifact>>│ │
│  │  Browser   │─┼──────┼─▶│   nginx    │  │
│  └───────────┘  │      │  └─────┬─────┘  │
└─────────────────┘      │        │        │
                         │        ▼        │
                         │  ┌───────────┐  │
                         │  │<<artifact>>│ │
                         │  │  node.js  │  │
                         │  └─────┬─────┘  │
                         └────────┼────────┘
                                  │
                         ┌────────▼────────┐
                         │   <<device>>    │
                         │   DB Server     │
                         │  ┌───────────┐  │
                         │  │<<artifact>>│ │
                         │  │  PostgreSQL│ │
                         │  └───────────┘  │
                         └─────────────────┘

Elements:
• Node (3D box): Hardware/environment
• Artifact (rectangle): Deployed software
• Communication path (line): Network connection
```

## 📌 Key Points (Optional)
- You don't need all diagrams for every project
- Choose diagrams based on what needs to be communicated
- Use Case and Sequence diagrams are most commonly needed
- Class diagrams are essential for OOP design documentation
- Deployment diagrams help DevOps understand infrastructure

## 🖼️ Recommended Image
- Quick reference card showing all UML diagram types with examples

## 🔗 Connections
- [[235-UML Class Diagram]]
- [[219-Software Requirements Specification]]
- [[236-C4 Model Architecture]]