---
Theme: Velocity
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Scrum, Metrics, Agile]
---

## 📚 Idea/Concept

Velocity is a key metric in Scrum that measures the amount of work a team can complete in a Sprint, typically expressed in Story Points. It is calculated as the average of points completed in the last 3-5 Sprints and serves as a forecasting tool for planning future Sprints and releases. Velocity is specific to each team and should not be used to compare different teams, as estimations and contexts vary. It is a planning tool, not a performance metric.

### Velocity Calculation

```
┌─────────────────────────────────────────────────────────────────┐
│                    VELOCITY TRACKING                            │
├─────────────────────────────────────────────────────────────────┤
│  Sprint 1: 34 points completed                                  │
│  Sprint 2: 28 points completed (team member sick)               │
│  Sprint 3: 36 points completed                                  │
│  Sprint 4: 32 points completed                                  │
│  Sprint 5: 35 points completed                                  │
├─────────────────────────────────────────────────────────────────┤
│  Average Velocity = (34+28+36+32+35) / 5 = 33 points/Sprint    │
│                                                                 │
│  Range: 28-36 points (accounts for variability)                │
│  Use for planning: 30-35 points per Sprint                     │
└─────────────────────────────────────────────────────────────────┘
```

### Velocity Chart

```
Points
  40 │          ┌───┐
     │  ┌───┐   │   │       ┌───┐
  35 │  │   │   │   │ ┌───┐ │   │
     │  │   │   │   │ │   │ │   │
  30 │  │   │ ┌─┴───┤ │   │ │   │
     │  │   │ │     │ │   │ │   │
  25 │  │   │ │     │ │   │ │   │
     │  │   │ │     │ │   │ │   │
  20 │──┴───┴─┴─────┴─┴───┴─┴───┴──
        S1   S2   S3   S4   S5
        
     ─── Average Velocity Line (33)
```

### Using Velocity for Forecasting

```
Release Planning Example:

Total Backlog: 200 Story Points
Average Velocity: 33 points/Sprint

Optimistic (high velocity): 200 / 36 = 5.5 Sprints
Expected (average): 200 / 33 = 6 Sprints
Pessimistic (low velocity): 200 / 28 = 7.1 Sprints

Forecast: Release in 6-7 Sprints (12-14 weeks)
```

### Factors Affecting Velocity

| Factor | Impact |
|--------|--------|
| Team changes | New members decrease temporarily |
| Vacations/holidays | Reduced capacity |
| Technical debt | Slows development |
| New technology | Learning curve |
| Clear requirements | Increases velocity |
| Stable team | Higher, consistent velocity |

### What Velocity Is NOT

```
❌ A performance KPI
❌ A comparison tool between teams
❌ A commitment contract
❌ A measure of individual productivity
❌ Something to "improve" artificially

✓ A planning tool
✓ A forecasting mechanism
✓ Team-specific measurement
✓ Historical data for estimation
✓ Input for capacity planning
```

### Velocity Stabilization

```
New Team Velocity Pattern:

Sprint:    1    2    3    4    5    6    7    8
Points:   15   22   18   28   32   30   33   31
           └─────────────┘    └──────────────┘
           Forming/Storming   Norming/Performing
           (Unstable)         (Stable ~32)

Reliable velocity typically emerges after 3-5 Sprints
```

## 📌 Key Points (Optional)
- Velocity is one of the most misunderstood metrics in Agile
- It's not a KPI or a tool to pressure the team
- It's simply a compass to navigate uncertainty
- Managers who try to artificially "improve" velocity only cause teams to inflate estimates
- A team with stable and predictable velocity is more valuable than one with high but erratic velocity

## 🖼️ Recommended Image
- Velocity chart showing Sprint-over-Sprint completion with trend line

## 🔗 Connections
- [[224-Story Points]]
- [[226-Sprint Planning]]
- [[227-Sprint Backlog]]
- [[220-Product Backlog]]
- [[207-Scrum Framework]]
- [[225-Planning Poker]]