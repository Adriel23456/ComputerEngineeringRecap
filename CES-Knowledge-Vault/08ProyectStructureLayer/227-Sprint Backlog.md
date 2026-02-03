---
Theme: Sprint Backlog
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Scrum, Artifact, Agile]
---

## 📚 Idea/Concept

The Sprint Backlog is the set of Product Backlog items selected for the Sprint, plus a plan for delivering the product Increment and achieving the Sprint Goal. It is a forecast made by the Development Team about what functionality will be in the next Increment and the work needed to deliver that functionality. The Sprint Backlog is owned exclusively by the Development Team, who modify it throughout the Sprint as new details emerge. It includes at least one process improvement identified in the previous Retrospective.

### Sprint Backlog Structure

```
┌─────────────────────────────────────────────────────────────────┐
│                       SPRINT BACKLOG                            │
│              Sprint Goal: "Enable user checkout flow"           │
├─────────────────────────────────────────────────────────────────┤
│  USER STORY          │ TASKS                    │ STATUS        │
├──────────────────────┼──────────────────────────┼───────────────┤
│  US-101: Add to Cart │ Design cart component    │ Done          │
│  (5 points)          │ Implement cart logic     │ In Progress   │
│                      │ Write unit tests         │ To Do         │
│                      │ Integration testing      │ To Do         │
├──────────────────────┼──────────────────────────┼───────────────┤
│  US-102: View Cart   │ Create cart page UI      │ Done          │
│  (3 points)          │ Connect to cart service  │ Done          │
│                      │ Add quantity controls    │ In Progress   │
├──────────────────────┼──────────────────────────┼───────────────┤
│  US-103: Checkout    │ Design checkout flow     │ To Do         │
│  (8 points)          │ Payment integration      │ To Do         │
│                      │ Order confirmation       │ To Do         │
├──────────────────────┼──────────────────────────┼───────────────┤
│  IMPROVEMENT:        │ Add linting to pipeline  │ In Progress   │
│  (from Retro)        │                          │               │
└─────────────────────────────────────────────────────────────────┘
```

### Key Characteristics

Owned by Development Team:
- Only the team can modify it during Sprint
- Product Owner clarifies but doesn't change it
- Team updates it daily

Emergent Plan:
- Initial plan created in Sprint Planning
- Details emerge throughout Sprint
- Tasks added/removed as understanding grows

Visible and Real-time:
- Reflects current state of Sprint work
- Updated continuously
- Transparent to all stakeholders

### Sprint Backlog vs Product Backlog

| Aspect | Product Backlog | Sprint Backlog |
|--------|-----------------|----------------|
| Owner | Product Owner | Development Team |
| Scope | Entire product | Single Sprint |
| Items | User Stories, Epics | Stories + Tasks |
| Changes | Continuous | During Sprint (team only) |
| Visibility | Long-term vision | Current Sprint work |

### Task Decomposition

```
User Story: "As a customer, I want to add items to cart"

Tasks (each < 1 day):
├── Design cart component mockup (4h)
├── Create Cart React component (6h)
├── Implement addToCart() function (4h)
├── Create cart state management (4h)
├── Write unit tests for cart logic (4h)
├── Integration test with product service (3h)
└── Code review and refinement (2h)

Total: ~27 hours → fits in Sprint capacity
```

### Burndown Chart

```
Story Points Remaining
    │
 40 │●
    │  ●
 30 │    ●
    │      ●──●     (Impediment period)
 20 │          ●
    │            ●
 10 │              ●
    │                ●
  0 │──────────────────●
    └─────────────────────
      1  2  3  4  5  6  7  8  9  10
                Sprint Days
      
─── Ideal Burndown
●── Actual Burndown
```

## 📌 Key Points (Optional)
- The Sprint Backlog is where theory becomes practice
- It's fascinating to see how an initial plan evolves as the team learns
- The common error is treating it as a fixed contract instead of a flexible plan
- Transparency is key: everyone should see real progress, not a polished version

## 🖼️ Recommended Image
- Kanban board showing Sprint Backlog with task cards in different columns

## 🔗 Connections
- [[220-Product Backlog]]
- [[226-Sprint Planning]]
- [[207-Scrum Framework]]
- [[218-User Story]]
- [[229-Taskboard]]
- [[230-Definition of Done]]
- [[233-Velocity]]
- [[224-Story Points]]
- [[228-Daily Scrum]]