---
Theme: User Story
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Agile, Requirements, Scrum]
---

## 📚 Idea/Concept

A User Story is a simple, brief description of a feature told from the perspective of the end user. It follows the format: "As a [type of user], I want [action/functionality] so that [benefit/value]". User Stories are the fundamental unit of work in Agile, designed to foster conversation between the team and stakeholders. They are not detailed specifications but promises of conversation. They include acceptance criteria that define expected behavior and serve as the basis for tests. They must meet INVEST criteria: Independent, Negotiable, Valuable, Estimable, Small, Testable.

### User Story Format

```
┌─────────────────────────────────────────────────────────────────┐
│                      USER STORY TEMPLATE                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│   As a    [USER ROLE]                                           │
│   I want  [GOAL/DESIRE]                                         │
│   So that [BENEFIT/VALUE]                                       │
│                                                                 │
├─────────────────────────────────────────────────────────────────┤
│   ACCEPTANCE CRITERIA (Given-When-Then):                        │
│                                                                 │
│   Given [PRECONDITION]                                          │
│   When  [ACTION]                                                │
│   Then  [EXPECTED RESULT]                                       │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### The 3 C's of User Stories

| Component | Description |
|-----------|-------------|
| Card | Physical token representing the story (brief description) |
| Conversation | Discussion between team and stakeholders to clarify details |
| Confirmation | Acceptance criteria that verify the story is complete |

### INVEST Criteria

```
I - Independent    : Can be developed in any order
N - Negotiable     : Details can be negotiated with PO
V - Valuable       : Delivers value to user/business
E - Estimable      : Team can estimate size
S - Small          : Fits within one Sprint
T - Testable       : Clear criteria for verification
```

### User Story Example

```
STORY: Product Purchase

As a registered customer
I want to add products to my shopping cart
So that I can purchase multiple items at once

ACCEPTANCE CRITERIA:

Given I am logged in as a customer
  And I am viewing a product page
When I click the "Add to Cart" button
Then the product should appear in my cart
  And the cart count should increase by 1
  And I should see a confirmation message

Given my cart contains items
When I navigate away from the product page
Then my cart items should be preserved
```

### Story Splitting Patterns

When stories are too large, split by:
- Workflow steps
- Business rule variations
- Data variations
- Interface variations
- Operations (CRUD)
- Performance constraints

### Story Hierarchy

```
Epic (Large Feature)
  │
  ├── Feature
  │     │
  │     ├── User Story
  │     │     └── Tasks
  │     └── User Story
  │           └── Tasks
  │
  └── Feature
        └── User Story
              └── Tasks
```

## 📌 Key Points (Optional)
- Stories are deceptively simple; their power lies in forcing conversation
- The value must be explicit; if you can't explain the "why," it's probably not a good story
- Remember: the story is the excuse for conversation, not the end itself
- Good stories generate questions, they don't avoid them

## 🖼️ Recommended Image
- Visual card showing user story format with acceptance criteria example

## 🔗 Connections
- [[220-Product Backlog]]
- [[224-Story Points]]
- [[227-Sprint Backlog]]
- [[225-Planning Poker]]
- [[210-Product Owner]]
- [[215-Functional Requirements]]
- [[230-Definition of Done]]
- [[226-Sprint Planning]]
- [[223-User Story Mapping]]