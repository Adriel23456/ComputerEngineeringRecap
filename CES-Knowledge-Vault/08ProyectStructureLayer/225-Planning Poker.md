---
Theme: Planning Poker
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Agile, Estimation, Scrum]
---

## 📚 Idea/Concept

Planning Poker is a gamified, consensus-based agile estimation technique used to estimate the relative effort of User Stories or tasks. Each team member has cards with Fibonacci sequence values (or similar), and everyone reveals their estimation simultaneously to avoid anchoring bias. Significant differences are discussed until consensus is reached. This technique combines individual expert opinions, analogies, and decomposition to produce more accurate estimates while fostering participation and shared understanding of the work.

### Planning Poker Process

```
┌─────────────────────────────────────────────────────────────────┐
│                    PLANNING POKER FLOW                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  1. PO PRESENTS STORY                                          │
│     "As a user, I want to reset my password..."                │
│                                                                 │
│  2. TEAM ASKS QUESTIONS                                        │
│     • What email service?                                       │
│     • Security requirements?                                    │
│     • Expiration time for link?                                │
│                                                                 │
│  3. INDIVIDUAL ESTIMATION (private)                            │
│     Each member selects a card                                  │
│                                                                 │
│  4. SIMULTANEOUS REVEAL                                        │
│     ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐                            │
│     │ 5 │ │ 8 │ │ 5 │ │ 13│ │ 5 │                            │
│     └───┘ └───┘ └───┘ └───┘ └───┘                            │
│     Ana   Bob   Carol  Dan   Eva                               │
│                                                                 │
│  5. DISCUSS OUTLIERS                                           │
│     Dan (13): "The email service integration is complex"       │
│     Ana (5): "We did similar work last Sprint"                 │
│                                                                 │
│  6. RE-ESTIMATE (if needed)                                    │
│     ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐                            │
│     │ 8 │ │ 8 │ │ 8 │ │ 8 │ │ 5 │                            │
│     └───┘ └───┘ └───┘ └───┘ └───┘                            │
│                                                                 │
│  7. CONSENSUS REACHED: 8 points                                │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Card Deck Values

```
Standard Fibonacci:
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ ½ │ 1 │ 2 │ 3 │ 5 │ 8 │ 13│ 20│ 40│100│
└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘

Special Cards:
┌───┬───┬───┐
│ ? │ ∞ │ ☕ │
└───┴───┴───┘
  │   │   └── Break/trivial
  │   └────── Too large, needs splitting
  └────────── Need more information
```

### Why Simultaneous Reveal?

```
ANCHORING BIAS PREVENTION:

Without simultaneous reveal:
  Senior Dev: "I think it's a 3"
  Junior Devs: "Yeah... 3 sounds right" (influenced)

With simultaneous reveal:
  Everyone shows: 3, 8, 5, 3, 13
  Discussion reveals: Junior knew about hidden complexity!
```

### Facilitating Discussion

When estimates differ significantly:

High Estimator Explains:
- What complexity do you see?
- What risks are you considering?
- What unknowns concern you?

Low Estimator Explains:
- What similar work have we done?
- What makes this straightforward?
- What assumptions are you making?

### Time-boxing

```
Per Story: 2-5 minutes typical

If exceeding 5 minutes:
├── Story may need splitting
├── Requirements unclear → ask PO
├── Technical spike needed
└── Use "?" card and move on
```

### Best Practices

| Practice | Reason |
|----------|--------|
| Everyone participates | Diverse perspectives |
| PO doesn't estimate | Avoids influence |
| Discuss extremes first | Reveals hidden info |
| Use reference stories | Consistent scale |
| Time-box discussions | Maintain momentum |
| Don't average | Seek understanding |

## 📌 Key Points (Optional)
- Planning Poker is more than an estimation technique; it's a team-building and knowledge-transfer tool
- The discussions that arise from disagreement are where real learning happens
- The team not only estimates but aligns their technical and business understanding
- The gamification keeps energy high and prevents estimation from becoming tedious

## 🖼️ Recommended Image
- Photo of team holding Planning Poker cards with different values

## 🔗 Connections
- [[224-Story Points]]
- [[233-Velocity]]
- [[218-User Story]]
- [[226-Sprint Planning]]
- [[220-Product Backlog]]
- [[227-Sprint Backlog]]
- [[207-Scrum Framework]]