---
Theme: Story Points
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Agile, Estimation, Scrum]
---

## 📚 Idea/Concept

Story Points are an abstract unit of measurement used in Agile to estimate the relative effort needed to implement a User Story or any other work. They represent a combination of complexity, effort, risk, and uncertainty—not direct time. They use relative scales (typically Fibonacci: 1, 2, 3, 5, 8, 13, 21) where each number represents approximately double the effort of the previous one. Story Points allow the team to focus on the relative size of work without the pressure of committing to specific timeframes.

### Story Point Scale

```
┌─────────────────────────────────────────────────────────────────┐
│                    FIBONACCI SCALE                              │
├─────────────────────────────────────────────────────────────────┤
│  1   │ Trivial - Very small, well understood                   │
│  2   │ Small - Straightforward, minimal complexity              │
│  3   │ Medium-Small - Some complexity, clear approach           │
│  5   │ Medium - Moderate complexity, some unknowns              │
│  8   │ Large - Significant work, multiple components            │
│  13  │ Very Large - Complex, consider splitting                 │
│  21  │ Epic-sized - Too large for single Sprint, must split     │
│  40+ │ Way too large - Break down immediately                   │
├─────────────────────────────────────────────────────────────────┤
│  ?   │ Need more information to estimate                        │
│  ∞   │ Cannot be estimated, needs decomposition                 │
│  ☕  │ Break time / trivial task                                │
└─────────────────────────────────────────────────────────────────┘
```

### What Story Points Measure

```
Story Points = f(Complexity, Effort, Uncertainty, Risk)

NOT:
- Hours or days
- Lines of code
- Number of tasks
- Individual performance

YES:
- Relative size comparison
- Team consensus
- Combined factors
```

### Estimation Factors

| Factor | Description | Example |
|--------|-------------|---------|
| Complexity | Technical difficulty | New algorithm vs CRUD |
| Effort | Amount of work | 1 screen vs 10 screens |
| Uncertainty | Unknowns in requirements | Clear spec vs vague idea |
| Risk | Things that could go wrong | Proven tech vs new library |

### Reference Story Technique

```
Establish a baseline story that the team understands well:

REFERENCE: "Login form with email/password" = 3 points

New Story: "User registration with email verification"
- More complex than login? Yes
- About twice the work? Yes
- Estimate: 5 points

New Story: "OAuth integration with Google"
- External dependency (risk)
- New technology (uncertainty)
- Estimate: 8 points
```

### Story Points vs Hours

| Story Points | Time Estimates |
|--------------|----------------|
| Relative | Absolute |
| Team-specific | Universal illusion |
| Embrace uncertainty | False precision |
| Focus on size | Focus on deadline |
| Consistent velocity | Variable productivity |

### Common Mistakes

```
❌ Converting points to hours
❌ Comparing team velocities
❌ Using points for individual performance
❌ Re-estimating completed stories
❌ Inflating estimates under pressure
❌ Estimating alone (not as team)
```

## 📌 Key Points (Optional)
- Story Points are brilliant because they acknowledge the fallacy of precise software estimates
- By using relative numbers and non-linear scales, we accept the inherent uncertainty of development
- The biggest mistake is treating them as disguised hours
- A team that understands this can plan effectively without the stress of impossible deadlines

## 🖼️ Recommended Image
- Visual comparison chart showing relative sizes of stories with Fibonacci numbers

## 🔗 Connections
- [[225-Planning Poker]]
- [[233-Velocity]]
- [[218-User Story]]
- [[226-Sprint Planning]]
- [[220-Product Backlog]]
- [[227-Sprint Backlog]]
- [[207-Scrum Framework]]