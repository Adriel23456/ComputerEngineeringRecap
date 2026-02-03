---
Theme: EARS Requirements Syntax
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Requirements, Documentation, Methodology]
---

## 📚 Idea/Concept

EARS (Easy Approach to Requirements Syntax) is a structured method for writing high-quality natural language requirements. Developed by Alistair Mavin and colleagues, it provides templates that reduce ambiguity, improve completeness, and make requirements easier to verify. EARS defines five patterns for different requirement types, each with specific keywords that signal the requirement's nature and trigger conditions.

### The Five EARS Patterns

```
┌─────────────────────────────────────────────────────────────────┐
│                    EARS REQUIREMENT PATTERNS                    │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  1. UBIQUITOUS (Always Active)                                 │
│     Keyword: None / "The system shall..."                       │
│     Template: The <system> shall <action>                       │
│                                                                 │
│  2. EVENT-DRIVEN                                                │
│     Keyword: "When"                                             │
│     Template: When <trigger>, the <system> shall <action>       │
│                                                                 │
│  3. STATE-DRIVEN                                                │
│     Keyword: "While"                                            │
│     Template: While <state>, the <system> shall <action>        │
│                                                                 │
│  4. UNWANTED BEHAVIOR                                           │
│     Keywords: "If...then"                                       │
│     Template: If <condition>, then the <system> shall <action>  │
│                                                                 │
│  5. OPTIONAL FEATURE                                            │
│     Keyword: "Where"                                            │
│     Template: Where <feature>, the <system> shall <action>      │
│                                                                 │
│  6. COMPLEX (Combinations)                                      │
│     Uses multiple keywords together                             │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Pattern Details and Examples

**1. Ubiquitous Requirements**
```
Always active, no preconditions needed.

Template: The <system> shall <action>

Examples:
• The system shall use HTTPS for all communications.
• The system shall display prices in local currency.
• The system shall log all user authentication attempts.
```

**2. Event-Driven Requirements**
```
Triggered by a specific event.

Template: WHEN <trigger>, the <system> shall <action>

Examples:
• When a user submits the login form, the system shall 
  validate credentials within 2 seconds.
• When payment is confirmed, the system shall send a 
  confirmation email to the customer.
• When the session expires, the system shall redirect 
  the user to the login page.
```

**3. State-Driven Requirements**
```
Active while system is in a specific state.

Template: WHILE <state>, the <system> shall <action>

Examples:
• While the user is logged in, the system shall display 
  their profile information in the header.
• While in maintenance mode, the system shall display 
  a maintenance notification to all users.
• While processing a transaction, the system shall 
  prevent duplicate submissions.
```

**4. Unwanted Behavior Requirements**
```
Handling errors, exceptions, or edge cases.

Template: IF <condition>, THEN the <system> shall <action>

Examples:
• If the payment gateway is unavailable, then the system 
  shall queue the transaction for retry.
• If invalid credentials are entered 5 times, then the 
  system shall lock the account for 30 minutes.
• If the file upload exceeds 10MB, then the system shall 
  reject the upload with an error message.
```

**5. Optional Feature Requirements**
```
Applies when specific feature/configuration is enabled.

Template: WHERE <feature>, the <system> shall <action>

Examples:
• Where two-factor authentication is enabled, the system 
  shall require a verification code after password entry.
• Where the premium subscription is active, the system 
  shall provide unlimited storage.
• Where geolocation is available, the system shall 
  display nearby store locations.
```

**6. Complex Requirements**
```
Combining multiple patterns.

Template: WHILE <state>, WHEN <trigger>, the <system> shall <action>

Examples:
• While the user is on the checkout page, when the 
  shipping address changes, the system shall recalculate 
  delivery costs.
• While in administrator mode, if a user deletion is 
  requested, then the system shall require confirmation 
  before proceeding.
```

### EARS Benefits

| Benefit | Description |
|---------|-------------|
| Reduced Ambiguity | Keywords clarify intent |
| Improved Testability | Clear triggers and actions |
| Better Coverage | Patterns prompt edge cases |
| Consistent Format | Easier review and maintenance |
| Traceable | Clear mapping to test cases |

## 📌 Key Points (Optional)
- EARS transforms vague requirements into precise, testable statements
- The keyword-based approach helps identify missing requirements
- Each pattern maps directly to test scenarios
- EARS is language-agnostic and tool-independent

## 🖼️ Recommended Image
- Flowchart showing decision tree for selecting EARS pattern

## 🔗 Connections
- [[215-Functional Requirements]]
- [[216-Non Functional Requirements]]
- [[218-User Story]]
- [[219-Software Requirements Specification]]
- [[214-Business Requirements]]