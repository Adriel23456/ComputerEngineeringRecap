---
Theme: MoSCoW Prioritization
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Agile, Prioritization, Requirements]
---

## 📚 Idea/Concept

MoSCoW is a prioritization technique used to reach a common understanding with stakeholders on the importance of each requirement. The acronym stands for Must have, Should have, Could have, and Won't have (this time). It helps teams focus on delivering the most critical functionality first while managing stakeholder expectations about what will and won't be included in a release.

### MoSCoW Categories

```
┌─────────────────────────────────────────────────────────────────┐
│                    MoSCoW PRIORITIZATION                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  M - MUST HAVE (Critical)                                      │
│  ══════════════════════                                        │
│  • Non-negotiable requirements                                  │
│  • Product fails without these                                  │
│  • No workaround exists                                         │
│  • Legal/compliance requirements                                │
│  • Core functionality                                           │
│                                                                 │
│  Example: "Users must be able to log in"                       │
│                                                                 │
├─────────────────────────────────────────────────────────────────┤
│  S - SHOULD HAVE (Important)                                   │
│  ═══════════════════════════                                   │
│  • Important but not vital                                      │
│  • Painful to leave out                                         │
│  • Workaround may exist                                         │
│  • Can be postponed if necessary                               │
│                                                                 │
│  Example: "Users should be able to reset password via email"   │
│                                                                 │
├─────────────────────────────────────────────────────────────────┤
│  C - COULD HAVE (Desirable)                                    │
│  ═══════════════════════════                                   │
│  • Nice to have                                                 │
│  • Small improvement                                            │
│  • First to be removed if time runs short                      │
│  • Low impact if left out                                       │
│                                                                 │
│  Example: "Users could see login history"                      │
│                                                                 │
├─────────────────────────────────────────────────────────────────┤
│  W - WON'T HAVE (This Time)                                    │
│  ════════════════════════════                                  │
│  • Explicitly excluded from this release                        │
│  • Agreed to be out of scope                                    │
│  • May be considered for future                                 │
│  • Manages expectations                                         │
│                                                                 │
│  Example: "Won't have biometric login (future release)"        │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### MoSCoW Distribution Rule

```
RECOMMENDED EFFORT DISTRIBUTION:

┌─────────────────────────────────────────────────────────────────┐
│                                                                 │
│   Must Have:    ████████████████████████████████░░░░  ~60%     │
│                                                                 │
│   Should Have:  ████████████░░░░░░░░░░░░░░░░░░░░░░░  ~20%     │
│                                                                 │
│   Could Have:   ████████░░░░░░░░░░░░░░░░░░░░░░░░░░░  ~20%     │
│                                                                 │
│   Won't Have:   (Excluded from this release)         0%        │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘

Why this distribution?
• Ensures core functionality is delivered (Must)
• Leaves buffer for important features (Should)
• Provides flexibility for scope management (Could)
• If things go well, Could items get done
• If issues arise, Could items are dropped first
```

### Decision Framework

```
PRIORITIZATION QUESTIONS:

For each requirement, ask:

┌─────────────────────────────────────────────────────────────────┐
│                                                                 │
│  "What happens if we don't deliver this?"                      │
│                                                                 │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │ Product/project fails completely → MUST HAVE            │   │
│  ├─────────────────────────────────────────────────────────┤   │
│  │ Significant pain, but workaround exists → SHOULD HAVE   │   │
│  ├─────────────────────────────────────────────────────────┤   │
│  │ Minor inconvenience → COULD HAVE                        │   │
│  ├─────────────────────────────────────────────────────────┤   │
│  │ No impact / out of scope → WON'T HAVE                   │   │
│  └─────────────────────────────────────────────────────────┘   │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### MoSCoW Example: E-Commerce MVP

```
┌─────────────────────────────────────────────────────────────────┐
│  E-COMMERCE MVP - MoSCoW ANALYSIS                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  MUST HAVE (60% effort)                                        │
│  ├── User registration and login                               │
│  ├── Product catalog display                                   │
│  ├── Shopping cart functionality                               │
│  ├── Checkout and payment processing                           │
│  ├── Order confirmation                                        │
│  └── Basic security (HTTPS, password hashing)                  │
│                                                                 │
│  SHOULD HAVE (20% effort)                                      │
│  ├── Product search                                            │
│  ├── Order history                                             │
│  ├── Email notifications                                       │
│  └── Password reset                                            │
│                                                                 │
│  COULD HAVE (20% effort)                                       │
│  ├── Product reviews                                           │
│  ├── Wishlist                                                  │
│  ├── Social media login                                        │
│  └── Product recommendations                                   │
│                                                                 │
│  WON'T HAVE (this release)                                     │
│  ├── Mobile app                                                │
│  ├── Multiple currencies                                       │
│  ├── Live chat support                                         │
│  └── Loyalty program                                           │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Common Mistakes

```
❌ ANTI-PATTERNS:

• Everything is "Must Have"
  → Defeats the purpose of prioritization
  
• No "Won't Have" items
  → Sets unrealistic expectations
  
• Prioritizing without stakeholders
  → Leads to disagreements later
  
• Not revisiting priorities
  → Priorities change as you learn
  
• Confusing "important" with "urgent"
  → Must = critical, not just wanted
```

### MoSCoW vs Other Techniques

| Technique | Best For | Complexity |
|-----------|----------|------------|
| MoSCoW | Release planning | Simple |
| Value/Effort | Backlog ordering | Medium |
| WSJF | SAFe/scaled agile | Complex |
| Kano Model | Feature analysis | Medium |
| RICE | Product roadmaps | Medium |

## 📌 Key Points (Optional)
- MoSCoW is about managing expectations, not just making lists
- The "Won't Have" category is crucial for setting clear boundaries
- Revisit priorities regularly as circumstances change
- Get stakeholder buy-in on the categorization
- Use the 60/20/20 rule to maintain delivery buffer

## 🖼️ Recommended Image
- Four-quadrant diagram with color-coded MoSCoW categories

## 🔗 Connections
- [[220-Product Backlog]]
- [[218-User Story]]
- [[226-Sprint Planning]]
- [[223-User Story Mapping]]
- [[210-Product Owner]]