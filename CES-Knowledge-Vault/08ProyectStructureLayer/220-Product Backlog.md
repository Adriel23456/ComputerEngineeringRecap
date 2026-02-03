---
Theme: Product Backlog
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Scrum, Artifact, Agile, Requirements]
---

## 📚 Idea/Concept

The Product Backlog is an ordered, prioritized list of everything needed in the product. It is the single source of requirements for any changes to be made to the product and constantly evolves as the product and environment change. Each Product Backlog Item (PBI) includes a description, order, estimate, and value. The Product Owner is responsible for the Product Backlog, including its content, availability, and ordering according to business value.

### Product Backlog Structure

```
┌─────────────────────────────────────────────────────────────────┐
│                      PRODUCT BACKLOG                            │
│                    (Ordered by Priority)                        │
├─────────────────────────────────────────────────────────────────┤
│  HIGH PRIORITY (Ready for Sprint)                               │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │ Story 1 │ 5 pts │ Clear criteria │ Estimated │ Ready    │   │
│  │ Story 2 │ 3 pts │ Clear criteria │ Estimated │ Ready    │   │
│  │ Story 3 │ 8 pts │ Clear criteria │ Estimated │ Ready    │   │
│  └─────────────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────────────┤
│  MEDIUM PRIORITY (Being Refined)                                │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │ Story 4 │ ~5 pts │ Needs discussion │ Rough estimate  │    │
│  │ Story 5 │ ~8 pts │ Needs discussion │ Rough estimate  │    │
│  └─────────────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────────────┤
│  LOW PRIORITY (Future Consideration)                            │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │ Epic A  │ Large  │ Conceptual      │ Not estimated    │    │
│  │ Epic B  │ Large  │ Conceptual      │ Not estimated    │    │
│  └─────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
```

### Backlog Refinement

Progressive Elaboration:
- Items at top: Detailed, estimated, ready
- Items in middle: Partially refined
- Items at bottom: Conceptual, epics

Refinement Activities:
- Adding detail to items
- Estimating items
- Splitting large items
- Reordering based on new information
- Removing obsolete items

### Definition of Ready (DoR)

Criteria for items to enter a Sprint:
- Clear acceptance criteria
- Estimated by team
- Dependencies identified
- Small enough for Sprint
- Understood by team
- Value articulated

### Backlog Item Types

| Type | Description | Size |
|------|-------------|------|
| Epic | Large feature area | Multiple Sprints |
| Feature | Specific capability | 1-3 Sprints |
| User Story | User-focused functionality | Within Sprint |
| Technical Story | Technical improvement | Within Sprint |
| Bug | Defect to fix | Within Sprint |
| Spike | Research/investigation | Time-boxed |

### Prioritization Techniques

MoSCoW:
- Must have
- Should have
- Could have
- Won't have (this time)

Value vs Effort Matrix:
```
High Value │ Quick Wins  │ Major Projects
           │   DO FIRST  │   PLAN CAREFULLY
───────────┼─────────────┼─────────────────
Low Value  │ Fill-ins    │ Avoid/Eliminate
           │   IF TIME   │   DON'T DO
           └─────────────┴─────────────────
             Low Effort    High Effort
```

## 📌 Key Points (Optional)
- A well-managed Product Backlog is like a treasure map that evolves
- The key is not having all details from the start, but refining progressively
- The discipline to say "no" and remove obsolete items is as important as adding new ones
- Never treat it as a static document or infinite wish list

## 🖼️ Recommended Image
- Iceberg diagram showing refined items at top, rough items below waterline

## 🔗 Connections
- [[210-Product Owner]]
- [[218-User Story]]
- [[227-Sprint Backlog]]
- [[226-Sprint Planning]]
- [[224-Story Points]]
- [[215-Functional Requirements]]
- [[216-Non Functional Requirements]]
- [[230-Definition of Done]]
- [[221-MoSCoW Prioritization]]