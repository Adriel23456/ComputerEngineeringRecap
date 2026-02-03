---
Theme: User Story Mapping
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Agile, Requirements, Planning]
---

## 📚 Idea/Concept

User Story Mapping is a visual technique for organizing user stories into a useful model that helps understand system functionality, identify gaps, and plan releases effectively. Created by Jeff Patton, it arranges user activities and stories in a two-dimensional grid where the horizontal axis represents the user journey (narrative flow) and the vertical axis represents priority (from essential to nice-to-have). This technique bridges the gap between flat backlogs and meaningful product planning.

### User Story Map Structure

```
┌─────────────────────────────────────────────────────────────────┐
│                     USER STORY MAP                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  BACKBONE (User Activities - Left to Right Journey)            │
│  ┌─────────┐  ┌─────────┐  ┌─────────┐  ┌─────────┐           │
│  │ Browse  │─▶│  Select │─▶│ Purchase│─▶│ Receive │           │
│  │Products │  │ Product │  │         │  │  Order  │           │
│  └────┬────┘  └────┬────┘  └────┬────┘  └────┬────┘           │
│       │            │            │            │                  │
│  WALKING SKELETON (User Tasks)                                  │
│  ┌────┴────┐  ┌────┴────┐  ┌────┴────┐  ┌────┴────┐           │
│  │ Search  │  │  View   │  │Add to   │  │  Track  │           │
│  │         │  │ Details │  │  Cart   │  │ Delivery│           │
│  └────┬────┘  └────┬────┘  └────┬────┘  └────┬────┘           │
│       │            │            │            │                  │
├───────┼────────────┼────────────┼────────────┼──────────────────┤
│ R     │            │            │            │    MVP           │
│ E  ┌──┴──┐     ┌───┴───┐   ┌───┴───┐   ┌───┴───┐              │
│ L  │Basic│     │Product│   │Simple │   │ Email │              │
│ E  │Search│    │ Page  │   │Checkout│   │Confirm│              │
│ A  └─────┘     └───────┘   └───────┘   └───────┘              │
│ S                                                               │
│ E  ┌─────┐     ┌───────┐   ┌───────┐   ┌───────┐  Release 2   │
│    │Filter│    │Reviews│   │ Save  │   │  SMS  │              │
│ 1  │     │     │       │   │ Cart  │   │Notify │              │
│    └─────┘     └───────┘   └───────┘   └───────┘              │
│                                                                 │
│    ┌─────┐     ┌───────┐   ┌───────┐   ┌───────┐  Release 3   │
│ 2  │Recomm│    │Compare│   │ Gift  │   │  Live │              │
│    │endati│    │       │   │Wrapping│   │ Track │              │
│    └─────┘     └───────┘   └───────┘   └───────┘              │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Key Components

```
┌─────────────────────────────────────────────────────────────────┐
│                    MAP COMPONENTS                               │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  BACKBONE (Activities)                                          │
│  • High-level user goals                                        │
│  • Arranged left-to-right in sequence                          │
│  • Represents the user's journey                               │
│  • Example: "Purchase a product"                               │
│                                                                 │
│  WALKING SKELETON (Tasks)                                       │
│  • Steps to accomplish each activity                           │
│  • More detailed than activities                               │
│  • Example: "Add to cart", "Enter shipping"                    │
│                                                                 │
│  USER STORIES (Details)                                         │
│  • Specific implementations of tasks                           │
│  • Arranged vertically by priority                             │
│  • Higher = more essential                                     │
│  • Example: "Basic search", "Advanced filters"                 │
│                                                                 │
│  RELEASES (Horizontal Slices)                                   │
│  • Group stories into deliverable increments                   │
│  • Each slice = working software                               │
│  • MVP = minimum viable product (top slice)                    │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Creating a User Story Map

```
STEP-BY-STEP PROCESS:

1. FRAME THE PROBLEM
   └── Who are the users? What problem are we solving?

2. MAP THE BIG PICTURE
   └── Identify main activities (backbone)
   
3. EXPLORE EACH ACTIVITY
   └── Break into tasks (walking skeleton)
   
4. FILL IN DETAILS
   └── Add user stories under each task
   
5. SLICE FOR RELEASES
   └── Draw horizontal lines to define releases
   
6. IDENTIFY MVP
   └── Top slice = minimum viable product
```

### Benefits of Story Mapping

| Benefit | Description |
|---------|-------------|
| Big Picture | See entire product at once |
| Context | Stories have context in user journey |
| Gaps Identified | Missing functionality becomes visible |
| Release Planning | Natural way to slice releases |
| Shared Understanding | Team aligns on product vision |
| Prioritization | Vertical arrangement shows importance |

### Story Map vs Flat Backlog

```
FLAT BACKLOG:                    STORY MAP:
┌──────────────┐                 ┌───┬───┬───┬───┐
│ Story 1      │                 │ A │ B │ C │ D │ ← Activities
│ Story 2      │                 ├───┼───┼───┼───┤
│ Story 3      │                 │ 1 │ 4 │ 7 │10 │ ← Release 1
│ Story 4      │        →        ├───┼───┼───┼───┤
│ Story 5      │                 │ 2 │ 5 │ 8 │11 │ ← Release 2
│ Story 6      │                 ├───┼───┼───┼───┤
│ ...          │                 │ 3 │ 6 │ 9 │12 │ ← Release 3
└──────────────┘                 └───┴───┴───┴───┘

Problems with flat:              Benefits of map:
• No context                     • Visual context
• Hard to see gaps              • Gaps obvious
• Difficult to plan releases    • Natural release slices
• No user journey               • User journey visible
```

## 📌 Key Points (Optional)
- Story mapping keeps the user journey visible at all times
- The backbone represents what users are trying to accomplish
- Slice horizontally to create releasable increments
- The MVP is the thinnest possible slice that delivers value
- Update the map as you learn more about the product

## 🖼️ Recommended Image
- Physical story map with sticky notes on a wall

## 🔗 Connections
- [[218-User Story]]
- [[220-Product Backlog]]
- [[221-MoSCoW Prioritization]]
- [[222-Impact Mapping]]
- [[226-Sprint Planning]]
- [[210-Product Owner]]