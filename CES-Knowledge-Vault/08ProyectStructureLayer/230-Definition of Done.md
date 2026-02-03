---
Theme: Definition of Done
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Scrum, Quality, Agile]
---

## 📚 Idea/Concept

The Definition of Done (DoD) is a shared, transparent agreement that describes the criteria a Product Backlog Item or Increment must meet to be considered complete. It is a quality checklist that ensures all work meets minimum standards before delivery. The DoD creates transparency and common understanding of what "completed work" means, eliminating ambiguities and ensuring consistent quality. It evolves with team maturity and can include technical, functional, documentation, and quality criteria. It is fundamental for keeping technical debt under control.

### Definition of Done Structure

```
┌─────────────────────────────────────────────────────────────────┐
│                    DEFINITION OF DONE                           │
│                  (Example for Web Application)                  │
├─────────────────────────────────────────────────────────────────┤
│  CODE QUALITY                                                   │
│  ☐ Code follows team coding standards                          │
│  ☐ Code reviewed by at least one other developer               │
│  ☐ No compiler warnings or linting errors                      │
│  ☐ Code is self-documenting with clear naming                  │
├─────────────────────────────────────────────────────────────────┤
│  TESTING                                                        │
│  ☐ Unit tests written and passing (>80% coverage)              │
│  ☐ Integration tests passing                                    │
│  ☐ No regression in existing tests                             │
│  ☐ Edge cases considered and tested                            │
├─────────────────────────────────────────────────────────────────┤
│  DOCUMENTATION                                                  │
│  ☐ Technical documentation updated                              │
│  ☐ API documentation current                                    │
│  ☐ README updated if needed                                     │
├─────────────────────────────────────────────────────────────────┤
│  DEPLOYMENT                                                     │
│  ☐ Successfully deployed to staging environment                 │
│  ☐ No breaking changes to existing functionality               │
│  ☐ Feature flags configured (if applicable)                    │
├─────────────────────────────────────────────────────────────────┤
│  ACCEPTANCE                                                     │
│  ☐ Acceptance criteria met                                      │
│  ☐ Product Owner has reviewed                                   │
│  ☐ No open bugs related to the story                           │
└─────────────────────────────────────────────────────────────────┘
```

### DoD vs Acceptance Criteria

| Definition of Done | Acceptance Criteria |
|--------------------|---------------------|
| Applies to ALL items | Specific to ONE story |
| Quality standard | Functional requirement |
| Team-defined | PO-defined |
| Consistent across Sprint | Varies per story |
| Process-focused | Outcome-focused |

### DoD Evolution

```
Team Maturity Level → DoD Sophistication

BEGINNER TEAM:
☐ Code compiles
☐ Basic tests pass
☐ Deployed to test environment

INTERMEDIATE TEAM:
☐ All of above, plus:
☐ Code review completed
☐ 70% test coverage
☐ Documentation updated
☐ No critical bugs

MATURE TEAM:
☐ All of above, plus:
☐ Performance benchmarks met
☐ Security scan passed
☐ Accessibility compliance
☐ Monitoring/alerting configured
☐ Runbook updated
```

### Layers of Done

```
┌─────────────────────────────────────────────────┐
│           ORGANIZATION DoD                      │
│  (Security, Compliance, Architecture)           │
│  ┌─────────────────────────────────────────┐   │
│  │         PRODUCT DoD                     │   │
│  │  (Performance, Documentation, UX)       │   │
│  │  ┌─────────────────────────────────┐   │   │
│  │  │        TEAM DoD                 │   │   │
│  │  │  (Code review, Tests, Deploy)   │   │   │
│  │  │  ┌─────────────────────────┐   │   │   │
│  │  │  │    STORY CRITERIA       │   │   │   │
│  │  │  │  (Feature-specific)     │   │   │   │
│  │  │  └─────────────────────────┘   │   │   │
│  │  └─────────────────────────────────┘   │   │
│  └─────────────────────────────────────────┘   │
└─────────────────────────────────────────────────┘
```

### Anti-patterns

```
❌ "Done-ish" - Almost done, just needs testing
❌ "Done but..." - Done except for code review
❌ Negotiable DoD - Relaxing standards under pressure
❌ Invisible DoD - Team doesn't know criteria
❌ Static DoD - Never evolves with team growth
❌ Ignored DoD - Exists on paper only
```

## 📌 Key Points (Optional)
- The DoD is the team's quality contract with itself
- It's the line in the sand separating professional work from amateur work
- The moment of truth comes when there's pressure to deliver: do you respect the DoD or compromise it?
- Mature teams understand that violating the DoD is borrowing from the future with very high interest
- A robust DoD is the best investment in sustainable velocity

## 🖼️ Recommended Image
- Checklist graphic showing DoD items with checkmarks

## 🔗 Connections
- [[227-Sprint Backlog]]
- [[218-User Story]]
- [[220-Product Backlog]]
- [[207-Scrum Framework]]
- [[226-Sprint Planning]]
- [[233-Velocity]]
- [[216-Non Functional Requirements]]
- [[237-Coding Standards]]