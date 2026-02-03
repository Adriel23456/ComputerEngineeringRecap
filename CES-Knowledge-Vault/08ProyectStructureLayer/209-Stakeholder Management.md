---
Theme: Stakeholder Management
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Management, Communication, Agile]
---

## 📚 Idea/Concept

A Stakeholder is any person, group, or organization that has interest in, is affected by, or can influence the development, outcome, or success of a project. Stakeholders can be internal (development team, managers, employees) or external (customers, end users, vendors, regulators). Their proper identification and management is fundamental for project success, as their needs, expectations, and level of influence can determine critical decisions and product direction.

### Stakeholder Identification

```
┌─────────────────────────────────────────────────────────────────┐
│                  STAKEHOLDER CATEGORIES                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  INTERNAL STAKEHOLDERS                                          │
│  ├── Executive Sponsor (funding, strategic direction)          │
│  ├── Project Manager (delivery, coordination)                  │
│  ├── Product Owner (product decisions, backlog)                │
│  ├── Development Team (implementation)                         │
│  ├── QA Team (quality assurance)                               │
│  ├── Operations (deployment, maintenance)                      │
│  └── Support Team (customer issues)                            │
│                                                                 │
│  EXTERNAL STAKEHOLDERS                                          │
│  ├── End Users (primary users of system)                       │
│  ├── Customers (buyers, may differ from users)                 │
│  ├── Vendors (third-party services)                            │
│  ├── Regulators (compliance requirements)                      │
│  ├── Partners (business collaborators)                         │
│  └── Competitors (market influence)                            │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Power/Interest Matrix

```
┌─────────────────────────────────────────────────────────────────┐
│              STAKEHOLDER POWER/INTEREST MATRIX                  │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  HIGH    │  KEEP SATISFIED      │  MANAGE CLOSELY              │
│  POWER   │                      │                              │
│          │  • Executive sponsor │  • Product Owner             │
│          │  • Regulators        │  • Key customers             │
│          │  • Legal             │  • Project sponsor           │
│          │                      │                              │
│          │  Strategy: Regular   │  Strategy: Engage fully,     │
│          │  updates, involve    │  involve in decisions,       │
│          │  in major decisions  │  frequent communication      │
│          │                      │                              │
│  ────────┼──────────────────────┼──────────────────────────────│
│          │                      │                              │
│  LOW     │  MONITOR             │  KEEP INFORMED               │
│  POWER   │  (Minimal effort)    │                              │
│          │                      │  • End users                 │
│          │  • General public    │  • Dev team                  │
│          │  • Competitors       │  • Support team              │
│          │                      │                              │
│          │  Strategy: Monitor,  │  Strategy: Regular updates,  │
│          │  no active engagement│  address concerns, feedback  │
│          │                      │  channels                    │
│          │                      │                              │
│          └──────────────────────┴──────────────────────────────│
│                    LOW INTEREST          HIGH INTEREST          │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Stakeholder Analysis Template

```
┌─────────────────────────────────────────────────────────────────┐
│              STAKEHOLDER ANALYSIS TEMPLATE                      │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  Stakeholder: [Name/Role]                                       │
│  Category: Internal/External                                    │
│                                                                 │
│  ┌────────────────────────────────────────────────────────────┐│
│  │ INTERESTS & NEEDS                                          ││
│  │ • What do they want from the project?                      ││
│  │ • What problems does this solve for them?                  ││
│  │ • What are their success criteria?                         ││
│  └────────────────────────────────────────────────────────────┘│
│                                                                 │
│  ┌────────────────────────────────────────────────────────────┐│
│  │ INFLUENCE & POWER                                          ││
│  │ • Can they approve/reject deliverables?                    ││
│  │ • Do they control resources/budget?                        ││
│  │ • Can they block progress?                                 ││
│  └────────────────────────────────────────────────────────────┘│
│                                                                 │
│  ┌────────────────────────────────────────────────────────────┐│
│  │ ENGAGEMENT STRATEGY                                        ││
│  │ • Communication frequency: Daily/Weekly/Monthly            ││
│  │ • Communication method: Email/Meeting/Report               ││
│  │ • Key messages to deliver                                  ││
│  │ • Potential concerns to address                            ││
│  └────────────────────────────────────────────────────────────┘│
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### RACI Matrix

```
RACI = Responsible, Accountable, Consulted, Informed

┌──────────────────┬────────┬────────┬────────┬────────┬────────┐
│ Activity         │ PO     │ Dev    │ QA     │ Sponsor│ User   │
├──────────────────┼────────┼────────┼────────┼────────┼────────┤
│ Define Backlog   │ A/R    │ C      │ C      │ I      │ C      │
│ Sprint Planning  │ A      │ R      │ C      │ I      │ I      │
│ Development      │ C      │ A/R    │ C      │ I      │ I      │
│ Testing          │ I      │ C      │ A/R    │ I      │ C      │
│ Deployment       │ A      │ R      │ C      │ I      │ I      │
│ Budget Approval  │ C      │ I      │ I      │ A/R    │ I      │
│ User Acceptance  │ A      │ I      │ C      │ I      │ R      │
└──────────────────┴────────┴────────┴────────┴────────┴────────┘

R = Responsible (does the work)
A = Accountable (final decision maker, only ONE per activity)
C = Consulted (provides input, two-way communication)
I = Informed (kept up-to-date, one-way communication)
```

### Communication Plan

```
┌─────────────────────────────────────────────────────────────────┐
│              STAKEHOLDER COMMUNICATION PLAN                     │
├────────────┬───────────┬────────────┬───────────┬──────────────┤
│ Stakeholder│ Frequency │ Method     │ Content   │ Owner        │
├────────────┼───────────┼────────────┼───────────┼──────────────┤
│ Sponsor    │ Weekly    │ Email      │ Progress  │ PM           │
│            │ Monthly   │ Meeting    │ Metrics   │              │
├────────────┼───────────┼────────────┼───────────┼──────────────┤
│ PO         │ Daily     │ Standup    │ Blockers  │ Scrum Master │
│            │ Sprint    │ Review     │ Demo      │              │
├────────────┼───────────┼────────────┼───────────┼──────────────┤
│ Users      │ Sprint    │ Demo       │ Features  │ PO           │
│            │ Monthly   │ Newsletter │ Updates   │              │
├────────────┼───────────┼────────────┼───────────┼──────────────┤
│ Dev Team   │ Daily     │ Standup    │ Tasks     │ Self         │
│            │ Sprint    │ Retro      │ Improve   │              │
└────────────┴───────────┴────────────┴───────────┴──────────────┘
```

## 📌 Key Points (Optional)
- Effective stakeholder communication is as important as technical development
- A technically perfect project can fail if it doesn't meet stakeholder expectations
- Map stakeholders early, understand their motivations, and maintain open communication
- Transparency and expectation management prevent future conflicts
- Different stakeholders need different communication approaches

## 🖼️ Recommended Image
- Power/Interest matrix with stakeholder icons placed in quadrants

## 🔗 Connections
- [[210-Product Owner]]
- [[214-Business Requirements]]
- [[218-User Story]]
- [[226-Sprint Planning]]
- [[207-Scrum Framework]]
- [[231-Sprint Review]]