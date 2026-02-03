---
Theme: Sprint Planning
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Scrum, Event, Agile, Planning]
---

## 📚 Idea/Concept

Sprint Planning is the Scrum ceremony that marks the start of each Sprint, where the Scrum Team collaborates to define the work to be performed during the Sprint. Time-boxed to a maximum of 8 hours for a one-month Sprint (proportionally less for shorter Sprints), this meeting answers two fundamental questions: What can be delivered in the Increment resulting from the upcoming Sprint? and How will the work necessary to deliver the Increment be achieved? The result is a clear Sprint Goal and an initial Sprint Backlog that will guide the team throughout the Sprint.

### Sprint Planning Structure

```
┌─────────────────────────────────────────────────────────────────┐
│                      SPRINT PLANNING                            │
│              (Max 8 hours for 4-week Sprint)                    │
├─────────────────────────────────────────────────────────────────┤
│  PART 1: WHAT (First Half)                                      │
│  ─────────────────────────                                      │
│  Participants: Entire Scrum Team                                │
│                                                                 │
│  Activities:                                                    │
│  • PO presents highest priority items                           │
│  • Team discusses and clarifies requirements                    │
│  • Sprint Goal is crafted                                       │
│  • Team selects items they can complete                         │
│                                                                 │
│  Output: Sprint Goal + Selected PBIs                            │
├─────────────────────────────────────────────────────────────────┤
│  PART 2: HOW (Second Half)                                      │
│  ─────────────────────────                                      │
│  Participants: Development Team (PO available)                  │
│                                                                 │
│  Activities:                                                    │
│  • Decompose PBIs into tasks                                    │
│  • Create implementation plan                                   │
│  • Identify dependencies                                        │
│  • Estimate task effort                                         │
│                                                                 │
│  Output: Sprint Backlog with tasks                              │
└─────────────────────────────────────────────────────────────────┘
```

### Sprint Goal

Characteristics:
- Single coherent objective
- Provides guidance to team during Sprint
- Creates flexibility in exact work done
- Allows for negotiation of scope

Example Sprint Goals:
- "Enable customers to complete checkout process"
- "Stabilize core payment integration"
- "Launch MVP of reporting dashboard"

### Input Requirements

For effective Sprint Planning:
```
✓ Refined Product Backlog (top items ready)
✓ Clear Definition of Done
✓ Team capacity known (vacations, etc.)
✓ Previous Sprint Velocity
✓ Product Owner available
✓ Stakeholder input if needed
```

### Capacity Planning

```
Team Capacity Calculation:

Available Hours = Team Members × Working Days × Hours/Day × Focus Factor

Example:
5 developers × 10 days × 6 productive hours × 0.8 focus = 240 hours

Historical Velocity: 40 Story Points
Selected Work: 38-42 Story Points (within range)
```

### Common Anti-patterns

| Anti-pattern | Problem | Solution |
|--------------|---------|----------|
| Over-commitment | Too much work selected | Use historical velocity |
| No Sprint Goal | Lack of focus | Craft goal before selecting items |
| Estimation in Planning | Too time-consuming | Estimate during refinement |
| PO dictates work | Team doesn't own commitment | Team selects what they can do |
| No decomposition | Hidden complexity | Break down into tasks |

## 📌 Key Points (Optional)
- Sprint Planning is where ambition is balanced with realism
- The temptation is always to commit more than possible, but a mature team knows it's better to under-promise and over-deliver
- Planning quality determines Sprint quality
- A clear and motivating Sprint Goal can transform an ordinary Sprint into an extraordinary one

## 🖼️ Recommended Image
- Flowchart showing Sprint Planning process from backlog to Sprint Backlog

## 🔗 Connections
- [[227-Sprint Backlog]]
- [[220-Product Backlog]]
- [[210-Product Owner]]
- [[233-Velocity]]
- [[224-Story Points]]
- [[218-User Story]]
- [[225-Planning Poker]]
- [[230-Definition of Done]]
- [[207-Scrum Framework]]