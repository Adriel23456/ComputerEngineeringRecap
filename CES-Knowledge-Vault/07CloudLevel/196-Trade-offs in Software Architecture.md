---
Theme: Trade-offs in Software Architecture
Date created: 2026-02-02 12:00
tags: [CloudLevel, TradeOffs, ArchitecturalDecisions, ADR, QualityAttributes]
---

## 📚 Idea/Concept

A trade-off is an intentional architectural decision, driven by business requirements and design context, where one or more quality attributes are prioritized at the expense of others. Trade-offs must be explicit, documented in ADRs with rationale, alternatives considered, and reviewed over time as context changes.

### Concept Overview

Trade-off Nature:
```
┌─────────────────────────────────────────────────────────────────┐
│                  ARCHITECTURAL TRADE-OFFS                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  "Every architectural decision is a trade-off"                 │
│                                                                  │
│  You CANNOT maximize everything simultaneously:                │
│                                                                  │
│           Performance                                           │
│               ▲                                                 │
│              ╱ ╲                                                │
│             ╱   ╲                                               │
│            ╱     ╲                                              │
│           ╱   ●   ╲  ← Your system lives somewhere            │
│          ╱         ╲    in this trade-off space                │
│         ▼───────────▼                                          │
│     Security    Simplicity                                     │
│                                                                  │
│  Key principle: Make trade-offs EXPLICIT and DOCUMENTED        │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Common Trade-off Pairs

Fundamental tensions in system design:
```
┌─────────────────────────────────────────────────────────────────┐
│                 COMMON TRADE-OFF PAIRS                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  CONSISTENCY ◄─────────────────────────► AVAILABILITY          │
│  (CAP theorem)                                                  │
│  Strong consistency requires coordination → higher latency     │
│  High availability requires replicas → eventual consistency    │
│                                                                  │
│  LATENCY ◄───────────────────────────────► THROUGHPUT          │
│  Batching improves throughput but adds latency                 │
│  Immediate processing = low latency, lower efficiency          │
│                                                                  │
│  FLEXIBILITY ◄───────────────────────────► SIMPLICITY          │
│  More configuration options = more complexity                  │
│  Opinionated choices = simpler but less flexible               │
│                                                                  │
│  SECURITY ◄──────────────────────────────► USABILITY           │
│  More authentication = more friction                           │
│  Frictionless access = higher risk                             │
│                                                                  │
│  COST ◄──────────────────────────────────► PERFORMANCE         │
│  Better performance requires more/better resources             │
│  Cost optimization often sacrifices peak performance           │
│                                                                  │
│  BUILD ◄─────────────────────────────────► BUY                 │
│  Custom = full control, high development cost                  │
│  Buy/SaaS = faster, but vendor lock-in                        │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### CAP and PACELC Theorems

Distributed systems trade-offs:
```
┌─────────────────────────────────────────────────────────────────┐
│                    CAP THEOREM                                  │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  In presence of network Partition, choose:                     │
│  - Consistency: All nodes see same data                        │
│  - Availability: System responds to every request              │
│                                                                  │
│           Consistency                                           │
│               ▲                                                 │
│              / \                                                │
│             /   \                                               │
│            / CP  \  AP                                         │
│           /───────\                                            │
│          /         \                                           │
│   Partition ◄──────► Availability                              │
│   Tolerance                                                    │
│                                                                  │
│  CP Systems: MongoDB, HBase, Redis (cluster)                  │
│  AP Systems: Cassandra, DynamoDB, CouchDB                     │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                    PACELC THEOREM                               │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  Extends CAP: Even without Partition, trade-off exists:        │
│                                                                  │
│  if (Partition) {                                               │
│      choose(Availability, Consistency);  // CAP                │
│  } else {                                                       │
│      choose(Latency, Consistency);        // Normal operation  │
│  }                                                              │
│                                                                  │
│  Examples:                                                      │
│  - DynamoDB: PA/EL (prefer availability and latency)          │
│  - Spanner: PC/EC (prefer consistency always)                 │
│  - Cassandra: PA/EL (tunable per query)                       │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Architecture Decision Records (ADR)

Documenting trade-offs:
```
┌─────────────────────────────────────────────────────────────────┐
│                    ADR TEMPLATE                                 │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  # ADR-001: Use PostgreSQL for primary data store              │
│                                                                  │
│  ## Status                                                      │
│  Accepted (2026-02-01)                                         │
│                                                                  │
│  ## Context                                                     │
│  We need a primary database for our e-commerce platform.       │
│  Expected load: 10K transactions/day, growing 20% annually.    │
│  Strong consistency required for inventory and orders.         │
│                                                                  │
│  ## Decision                                                    │
│  Use PostgreSQL (managed: AWS RDS or Azure PostgreSQL)         │
│                                                                  │
│  ## Alternatives Considered                                     │
│  | Option     | Pros              | Cons                     | │
│  |------------|-------------------|--------------------------|  │
│  | PostgreSQL | ACID, mature, SQL | Scaling complexity       | │
│  | MongoDB    | Flexible schema   | Eventual consistency     | │
│  | DynamoDB   | Auto-scaling      | Vendor lock-in, cost    | │
│                                                                  │
│  ## Consequences                                                │
│  Positive:                                                      │
│  - Strong consistency for transactions                         │
│  - Rich SQL ecosystem, team familiarity                       │
│                                                                  │
│  Negative:                                                      │
│  - Horizontal scaling requires effort (read replicas, sharding)│
│  - Schema migrations need careful planning                     │
│                                                                  │
│  ## Trade-offs Accepted                                         │
│  - Consistency over horizontal scalability                     │
│  - Relational model over schema flexibility                    │
│                                                                  │
│  ## Review Trigger                                              │
│  - If transaction volume exceeds 100K/day                      │
│  - If schema changes become frequent bottleneck                │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Quality Attribute Trade-off Analysis

Evaluating trade-offs systematically:
```
┌─────────────────────────────────────────────────────────────────┐
│              QUALITY ATTRIBUTE RADAR                            │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│                    Performance                                  │
│                        ▲                                        │
│                        │                                        │
│                    8   │                                        │
│                   ╱────┼────╲                                   │
│   Maintainability╱     │     ╲Security                         │
│                 7      │      6                                 │
│                  ╲     │     ╱                                  │
│                   ╲────┼────╱                                   │
│              ─────────5┼─────────                              │
│                   ╱────┼────╲                                   │
│                  ╱     │     ╲                                  │
│      Scalability 9     │      4 Cost                           │
│                  ╲     │     ╱                                  │
│                   ╲────┼────╱                                   │
│                        │                                        │
│                        ▼                                        │
│                   Simplicity                                    │
│                                                                  │
│  This system prioritizes: Scalability, Performance             │
│  This system deprioritizes: Cost, Security                     │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Technical Debt as Trade-off

Conscious debt decisions:
```
┌─────────────────────────────────────────────────────────────────┐
│              TECHNICAL DEBT QUADRANT                            │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│             Deliberate                    Inadvertent           │
│         ┌─────────────────────────┬─────────────────────────┐  │
│         │                         │                         │  │
│ Prudent │ "We know this is tech   │ "Now we know how we     │  │
│         │  debt, shipping now,    │  should have done it"   │  │
│         │  will fix in sprint 5"  │                         │  │
│         │                         │                         │  │
│         │ ✓ Good trade-off        │ Learning outcome        │  │
│         ├─────────────────────────┼─────────────────────────┤  │
│         │                         │                         │  │
│ Reckless│ "We don't have time     │ "What's layered         │  │
│         │  for design"            │  architecture?"         │  │
│         │                         │                         │  │
│         │ ✗ Bad trade-off         │ ✗ Training issue        │  │
│         └─────────────────────────┴─────────────────────────┘  │
│                                                                  │
│  Technical Debt Formula:                                        │
│  Cost = Principal (fix effort) + Interest (ongoing pain)       │
│                                                                  │
│  Document deliberate debt as trade-offs with payoff plan       │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Making Trade-off Decisions

Decision framework:
```
┌─────────────────────────────────────────────────────────────────┐
│              TRADE-OFF DECISION PROCESS                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  1. IDENTIFY DRIVERS                                            │
│     - What business requirements constrain us?                 │
│     - What quality attributes matter most?                     │
│     - What are the hard constraints (compliance, budget)?      │
│                                                                  │
│  2. ENUMERATE OPTIONS                                           │
│     - List viable alternatives                                 │
│     - Include "do nothing" as baseline                         │
│                                                                  │
│  3. ANALYZE TRADE-OFFS                                          │
│     - Score each option against quality attributes             │
│     - Identify what each option sacrifices                     │
│     - Estimate costs (implementation + ongoing)                │
│                                                                  │
│  4. DECIDE AND DOCUMENT                                         │
│     - Choose based on prioritized attributes                   │
│     - Write ADR with full context                              │
│     - Get stakeholder buy-in                                   │
│                                                                  │
│  5. ESTABLISH REVIEW TRIGGERS                                   │
│     - What conditions would invalidate this decision?          │
│     - When should we revisit?                                  │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

## 📌 Key Points (Optional)
- Every architectural decision involves trade-offs; the goal is making them explicit.
- CAP/PACELC frame distributed system trade-offs between consistency, availability, and latency.
- ADRs document decisions with context, alternatives, and consequences.
- Technical debt is a valid trade-off when deliberate and prudent.

## 🖼️ Recommended Image
- Trade-off triangle or radar chart showing quality attributes with different system configurations.

## 🔗 Connections
- [[193-Application Availability]]
- [[194-Latency as Quality Attribute]]
- [[195-Throughput as Quality Attribute]]
- [[198-Software Coupling]]
- [[197-Software Cohesion]]
- [[202-Event Driven Architecture]]