---
Theme: Project Wiki Structure
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Documentation, Collaboration, Knowledge]
---

## 📚 Idea/Concept

A Project Wiki is a centralized knowledge base that contains all essential documentation for a software project. It serves as the single source of truth for team members, stakeholders, and new contributors. A well-structured wiki typically includes sections for Business Requirements, User Requirements, Non-Functional Requirements, and Working Group Process documentation. The wiki evolves with the project and provides context that code alone cannot convey.

### Wiki Structure Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                   PROJECT WIKI STRUCTURE                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  📁 PROJECT_NAME Wiki                                          │
│  │                                                              │
│  ├── 📄 Home (Project Overview)                                │
│  │                                                              │
│  ├── 📁 1. Business Requirements                               │
│  │   ├── 📄 Background                                         │
│  │   ├── 📄 Business Opportunity                               │
│  │   ├── 📄 Business Objectives                                │
│  │   └── 📄 Risk Matrix                                        │
│  │                                                              │
│  ├── 📁 2. User Requirements                                   │
│  │   ├── 📄 User Personas                                      │
│  │   ├── 📄 Feature Tree                                       │
│  │   ├── 📄 User Stories                                       │
│  │   └── 📄 Use Case Diagrams                                  │
│  │                                                              │
│  ├── 📁 3. Non-Functional Requirements                         │
│  │   ├── 📄 FURPS+ Analysis                                    │
│  │   ├── 📄 Performance Requirements                           │
│  │   ├── 📄 Security Requirements                              │
│  │   └── 📄 Compliance Requirements                            │
│  │                                                              │
│  ├── 📁 4. Working Group Process                               │
│  │   ├── 📄 Definition of Done                                 │
│  │   ├── 📄 Technology Stack                                   │
│  │   ├── 📄 Coding Standards                                   │
│  │   ├── 📄 Git Workflow                                       │
│  │   ├── 📄 Team Roles                                         │
│  │   └── 📄 Risk Management                                    │
│  │                                                              │
│  ├── 📁 5. Architecture                                        │
│  │   ├── 📄 C1 - Context Diagram                               │
│  │   ├── 📄 C2 - Container Diagram                             │
│  │   ├── 📄 C3 - Component Diagrams                            │
│  │   └── 📄 Decision Records (ADRs)                            │
│  │                                                              │
│  └── 📁 6. Reference                                           │
│      ├── 📄 Glossary                                           │
│      ├── 📄 API Documentation                                  │
│      └── 📄 Meeting Notes                                      │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### 1. Business Requirements Section

```
┌─────────────────────────────────────────────────────────────────┐
│              BUSINESS REQUIREMENTS WIKI                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  BACKGROUND                                                     │
│  ══════════                                                     │
│  • Current situation and context                               │
│  • Problem statement                                            │
│  • Market analysis summary                                      │
│  • Stakeholder overview                                         │
│                                                                 │
│  BUSINESS OPPORTUNITY                                           │
│  ════════════════════                                           │
│  • Market gap identification                                    │
│  • Competitive advantage                                        │
│  • Value proposition                                            │
│  • Target market                                                │
│                                                                 │
│  BUSINESS OBJECTIVES                                            │
│  ═══════════════════                                            │
│  • SMART goals                                                  │
│  • Key Performance Indicators (KPIs)                           │
│  • Success metrics                                              │
│  • Timeline milestones                                          │
│                                                                 │
│  RISK MATRIX                                                    │
│  ═══════════                                                    │
│  ┌────────────────────┬───────────┬────────────┬─────────────┐ │
│  │ Risk               │Probability│ Impact     │ Mitigation  │ │
│  ├────────────────────┼───────────┼────────────┼─────────────┤ │
│  │ Scope creep        │ High      │ High       │ Change ctrl │ │
│  │ Key person leaves  │ Medium    │ High       │ Doc + train │ │
│  │ Tech issues        │ Medium    │ Medium     │ Prototypes  │ │
│  │ Budget overrun     │ Low       │ High       │ Monitoring  │ │
│  └────────────────────┴───────────┴────────────┴─────────────┘ │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### 2. User Requirements Section

```
┌─────────────────────────────────────────────────────────────────┐
│                USER REQUIREMENTS WIKI                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  FEATURE TREE                                                   │
│  ════════════                                                   │
│                                                                 │
│  MyPetsCR System                                                │
│  ├── User Management                                            │
│  │   ├── Registration                                          │
│  │   ├── Authentication                                        │
│  │   └── Profile Management                                    │
│  ├── Appointment System                                         │
│  │   ├── Schedule Appointment                                  │
│  │   ├── Cancel Appointment                                    │
│  │   └── View History                                          │
│  ├── Medical Records                                            │
│  │   ├── Pet Profiles                                          │
│  │   ├── Treatment History                                     │
│  │   └── Prescription Management                               │
│  └── E-Commerce                                                 │
│      ├── Product Catalog                                        │
│      ├── Shopping Cart                                          │
│      └── Order Processing                                       │
│                                                                 │
│  FUNCTIONALITY DIAGRAM                                          │
│  ══════════════════════                                         │
│  • Use Case Diagrams for each module                           │
│  • User journey maps                                            │
│  • Interaction flows                                            │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### 3. Non-Functional Requirements Section

```
┌─────────────────────────────────────────────────────────────────┐
│             NON-FUNCTIONAL REQUIREMENTS WIKI                    │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  FURPS+ ANALYSIS                                                │
│  ═══════════════                                                │
│                                                                 │
│  ┌────────────────────────────────────────────────────────────┐│
│  │ F - FUNCTIONALITY (See Functional Requirements)            ││
│  ├────────────────────────────────────────────────────────────┤│
│  │ U - USABILITY                                              ││
│  │   • Interface must be intuitive for non-technical users    ││
│  │   • Support for users with limited reading ability         ││
│  │   • Maximum 3 clicks to complete any task                  ││
│  ├────────────────────────────────────────────────────────────┤│
│  │ R - RELIABILITY                                            ││
│  │   • 99.9% uptime during business hours                     ││
│  │   • Automatic failover in case of server failure           ││
│  │   • Daily automated backups                                ││
│  ├────────────────────────────────────────────────────────────┤│
│  │ P - PERFORMANCE                                            ││
│  │   • Page load time < 3 seconds                             ││
│  │   • API response time < 500ms                              ││
│  │   • Support 100 concurrent users                           ││
│  ├────────────────────────────────────────────────────────────┤│
│  │ S - SUPPORTABILITY                                         ││
│  │   • Comprehensive logging for troubleshooting              ││
│  │   • Modular architecture for easy updates                  ││
│  │   • Documentation for all APIs                             ││
│  ├────────────────────────────────────────────────────────────┤│
│  │ + - ADDITIONAL CONSTRAINTS                                 ││
│  │   • Must run on Windows 7+                                 ││
│  │   • Internet connection 5 MB/s required                    ││
│  │   • Data protection compliance                             ││
│  └────────────────────────────────────────────────────────────┘│
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### 4. Working Group Process Section

```
┌─────────────────────────────────────────────────────────────────┐
│              WORKING GROUP PROCESS WIKI                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  DEFINITION OF DONE                                             │
│  ══════════════════                                             │
│  ☐ Code follows team coding standards                          │
│  ☐ Code reviewed and approved                                  │
│  ☐ Unit tests written (>80% coverage)                          │
│  ☐ Integration tests passing                                   │
│  ☐ Documentation updated                                       │
│  ☐ Deployed to staging environment                             │
│  ☐ Product Owner approval                                      │
│                                                                 │
│  TECHNOLOGY STACK                                               │
│  ════════════════                                               │
│  │ Layer      │ Technology            │ Version │              │
│  ├────────────┼───────────────────────┼─────────┤              │
│  │ Frontend   │ Angular               │ 17.x    │              │
│  │ Backend    │ ASP.NET Core          │ 8.0     │              │
│  │ Database   │ JSON File Storage     │ -       │              │
│  │ API        │ REST with JSON        │ -       │              │
│                                                                 │
│  CODING STANDARDS                                               │
│  ════════════════                                               │
│  • Link to detailed standards document                         │
│  • Naming conventions                                           │
│  • Formatting rules                                             │
│  • Review checklist                                             │
│                                                                 │
│  GIT WORKFLOW                                                   │
│  ════════════                                                   │
│  • Branch naming: feature/*, bugfix/*, hotfix/*                │
│  • Commit message format                                        │
│  • PR requirements                                              │
│  • Branch protection rules                                      │
│                                                                 │
│  TEAM ROLES                                                     │
│  ══════════                                                     │
│  │ Name           │ Role            │ Responsibilities │       │
│  ├────────────────┼─────────────────┼──────────────────┤       │
│  │ Adriel Chaves  │ Product Owner   │ Backlog, Vision  │       │
│  │ Jose Vindas    │ Architect       │ Design, Tech     │       │
│  │ Sergio Salazar │ Developer       │ Implementation   │       │
│  │ Luis Gonzales  │ Designer        │ UI/UX, NFRs      │       │
│                                                                 │
│  COORDINATION PLATFORMS                                         │
│  ══════════════════════                                         │
│  • Project Management: Azure DevOps / Jira                     │
│  • Communication: Slack / Microsoft Teams                      │
│  • Documentation: Confluence / Wiki                            │
│  • Code: GitHub / Azure Repos                                  │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Wiki Best Practices

```
┌─────────────────────────────────────────────────────────────────┐
│                   WIKI BEST PRACTICES                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ORGANIZATION                                                   │
│  ☐ Consistent naming conventions for pages                     │
│  ☐ Clear hierarchy and navigation                              │
│  ☐ Table of contents on long pages                             │
│  ☐ Cross-linking between related pages                         │
│                                                                 │
│  CONTENT                                                        │
│  ☐ Keep information up-to-date                                 │
│  ☐ Include diagrams and visuals                                │
│  ☐ Add examples and code snippets                              │
│  ☐ Mark outdated content clearly                               │
│                                                                 │
│  MAINTENANCE                                                    │
│  ☐ Regular review schedule (monthly)                           │
│  ☐ Assign page owners                                          │
│  ☐ Track page history/versions                                 │
│  ☐ Archive deprecated content                                  │
│                                                                 │
│  ONBOARDING                                                     │
│  ☐ "Start Here" guide for new members                          │
│  ☐ Setup instructions                                          │
│  ☐ Key contacts and escalation paths                           │
│  ☐ Frequently asked questions                                  │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

## 📌 Key Points (Optional)
- A wiki is only valuable if it's kept current
- Structure should be intuitive for new team members
- Include both technical and business documentation
- Link wiki pages to related issues and code
- Assign ownership to ensure maintenance
- Make it the first place people look for answers

## 🖼️ Recommended Image
- Screenshot of well-organized project wiki navigation

## 🔗 Connections
- [[214-Business Requirements]]
- [[215-Functional Requirements]]
- [[216-Non Functional Requirements]]
- [[230-Definition of Done]]
- [[237-Coding Standards]]
- [[238-Git Workflow and Version Control]]
- [[236-C4 Model Architecture]]
- [[219-Software Requirements Specification]]