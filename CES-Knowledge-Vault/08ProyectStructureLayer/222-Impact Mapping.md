---
Theme: Impact Mapping
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Agile, Strategy, Planning]
---

## 📚 Idea/Concept

Impact Mapping is a strategic planning technique that helps teams align their work with business objectives. Created by Gojko Adzic, it creates a visualization that connects business goals to deliverables through a simple hierarchy: Goal → Actors → Impacts → Deliverables. Impact mapping prevents teams from building features that don't contribute to business objectives and provides clear reasoning for why each piece of work matters.

### Impact Map Structure

```
┌─────────────────────────────────────────────────────────────────┐
│                       IMPACT MAP                                │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│                      ┌─────────────┐                           │
│                      │    GOAL     │                           │
│                      │  (WHY?)     │                           │
│                      │ "Increase   │                           │
│                      │ sales 20%"  │                           │
│                      └──────┬──────┘                           │
│               ┌─────────────┼─────────────┐                    │
│               ▼             ▼             ▼                    │
│        ┌──────────┐  ┌──────────┐  ┌──────────┐               │
│        │  ACTOR   │  │  ACTOR   │  │  ACTOR   │               │
│        │  (WHO?)  │  │  (WHO?)  │  │  (WHO?)  │               │
│        │ Customer │  │ Marketing│  │  Sales   │               │
│        └────┬─────┘  └────┬─────┘  └────┬─────┘               │
│             │             │             │                      │
│      ┌──────┼──────┐      │      ┌──────┼──────┐              │
│      ▼      ▼      ▼      ▼      ▼      ▼      ▼              │
│   ┌─────┐┌─────┐┌─────┐┌─────┐┌─────┐┌─────┐┌─────┐          │
│   │IMPACT│IMPACT│IMPACT│IMPACT│IMPACT│IMPACT│IMPACT│          │
│   │(HOW?)│(HOW?)│(HOW?)│(HOW?)│(HOW?)│(HOW?)│(HOW?)│          │
│   │Buy   │Refer │Buy   │Reach │Target│Follow│Close │          │
│   │more  │friend│faster│more  │better│ up   │faster│          │
│   └──┬───┘└──┬──┘└──┬──┘└──┬──┘└──┬──┘└──┬──┘└──┬──┘          │
│      │       │      │      │      │      │      │              │
│      ▼       ▼      ▼      ▼      ▼      ▼      ▼              │
│   ┌─────┐┌─────┐┌─────┐┌─────┐┌─────┐┌─────┐┌─────┐          │
│   │DELIV│DELIV│DELIV│DELIV│DELIV│DELIV│DELIV│                 │
│   │(WHAT)│(WHAT)│(WHAT)│(WHAT)│(WHAT)│(WHAT)│(WHAT)│          │
│   │Recomm│Refer-│1-clk │Email │Analyt│CRM   │Mobile│          │
│   │endatn│ral   │buy   │campgn│ics   │integ │app   │          │
│   └─────┘└─────┘└─────┘└─────┘└─────┘└─────┘└─────┘          │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### The Four Questions

```
┌─────────────────────────────────────────────────────────────────┐
│                  IMPACT MAPPING QUESTIONS                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  1. WHY? (Goal)                                                │
│     └── What business objective are we trying to achieve?      │
│         "Increase quarterly revenue by 20%"                    │
│                                                                 │
│  2. WHO? (Actors)                                              │
│     └── Who can help us achieve this goal?                     │
│     └── Who can prevent us from achieving it?                  │
│         "Customers, Marketing team, Partners"                  │
│                                                                 │
│  3. HOW? (Impacts)                                             │
│     └── How should actor behavior change?                      │
│     └── What should they do differently?                       │
│         "Customers should buy more frequently"                 │
│                                                                 │
│  4. WHAT? (Deliverables)                                       │
│     └── What can we do to support the required impacts?        │
│         "Recommendation engine, loyalty program"               │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Creating an Impact Map

```
STEP 1: Define the Goal
┌────────────────────────────────────────┐
│ SMART Goal:                            │
│ • Specific: What exactly?              │
│ • Measurable: How will we know?        │
│ • Achievable: Is it realistic?         │
│ • Relevant: Does it matter?            │
│ • Time-bound: By when?                 │
│                                        │
│ Example: "Reduce customer support      │
│ calls by 30% within 6 months"          │
└────────────────────────────────────────┘

STEP 2: Identify Actors
┌────────────────────────────────────────┐
│ Primary Actors (direct impact):        │
│ • Customers                            │
│ • Support team                         │
│                                        │
│ Secondary Actors (indirect impact):    │
│ • Product team                         │
│ • Documentation team                   │
└────────────────────────────────────────┘

STEP 3: Define Impacts
┌────────────────────────────────────────┐
│ Customer impacts:                      │
│ • Self-solve problems                  │
│ • Find answers faster                  │
│ • Understand product better            │
│                                        │
│ Support team impacts:                  │
│ • Resolve issues faster                │
│ • Handle more tickets                  │
└────────────────────────────────────────┘

STEP 4: Brainstorm Deliverables
┌────────────────────────────────────────┐
│ For "Self-solve problems":             │
│ • FAQ section                          │
│ • Video tutorials                      │
│ • Chatbot                              │
│ • Knowledge base                       │
│ • In-app help tooltips                 │
└────────────────────────────────────────┘
```

### Impact Map Example

```
Goal: "Increase user retention by 25% in Q3"

├── Actor: New Users
│   ├── Impact: Complete onboarding
│   │   ├── Deliverable: Interactive tutorial
│   │   ├── Deliverable: Progress indicators
│   │   └── Deliverable: Welcome email series
│   └── Impact: Find value quickly
│       ├── Deliverable: Quick-start templates
│       └── Deliverable: Sample data
│
├── Actor: Active Users
│   ├── Impact: Use product daily
│   │   ├── Deliverable: Mobile app
│   │   ├── Deliverable: Email notifications
│   │   └── Deliverable: Browser extension
│   └── Impact: Discover new features
│       ├── Deliverable: Feature highlights
│       └── Deliverable: Tips and tricks
│
└── Actor: At-Risk Users
    └── Impact: Re-engage with product
        ├── Deliverable: Win-back campaign
        ├── Deliverable: Personal outreach
        └── Deliverable: Special offers
```

### Impact Mapping vs Traditional Planning

| Traditional | Impact Mapping |
|-------------|----------------|
| Start with features | Start with goals |
| What to build | Why to build |
| Feature factory | Outcome focused |
| Scope creep risk | Clear boundaries |
| Hard to prioritize | Goal-driven priority |

## 📌 Key Points (Optional)
- Always start with the business goal, not features
- Every deliverable must trace back to a goal through an impact
- If you can't explain how a feature impacts the goal, don't build it
- Use impact maps to facilitate conversations, not just document decisions
- Review and update as you learn what actually works

## 🖼️ Recommended Image
- Mind map style impact map with goal at center

## 🔗 Connections
- [[223-User Story Mapping]]
- [[214-Business Requirements]]
- [[220-Product Backlog]]
- [[210-Product Owner]]
- [[221-MoSCoW Prioritization]]
- [[226-Sprint Planning]]