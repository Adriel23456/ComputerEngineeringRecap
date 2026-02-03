---
Theme: Interviews for Requirements
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, Requirements, Elicitation, Communication]
---

## 📚 Idea/Concept

Interviews are a fundamental requirements elicitation technique where structured or semi-structured conversations are conducted with stakeholders to discover needs, expectations, constraints, and system objectives. It is a direct, personal method that allows deepening into details, clarifying ambiguities, and capturing tacit knowledge that other methods might miss. Effective interviews require careful preparation, communication skills, active listening, and the ability to ask the right questions.

### Interview Types

```
┌─────────────────────────────────────────────────────────────────┐
│                      INTERVIEW TYPES                            │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  STRUCTURED                                                     │
│  ══════════                                                     │
│  • Fixed set of questions                                       │
│  • Same questions for all interviewees                         │
│  • Easier to analyze and compare                               │
│  • Less flexibility for exploration                            │
│  • Best for: Validating specific requirements                  │
│                                                                 │
│  SEMI-STRUCTURED                                                │
│  ════════════════                                               │
│  • Prepared guide with key topics                              │
│  • Flexibility to explore interesting paths                    │
│  • Balance of consistency and discovery                        │
│  • Most commonly used                                          │
│  • Best for: Initial requirements gathering                    │
│                                                                 │
│  UNSTRUCTURED                                                   │
│  ══════════════                                                 │
│  • Open conversation                                            │
│  • Follows interviewee's lead                                  │
│  • Maximum flexibility                                          │
│  • Harder to analyze                                            │
│  • Best for: Exploring new domains                             │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Interview Process

```
┌─────────────────────────────────────────────────────────────────┐
│                    INTERVIEW WORKFLOW                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  1. PREPARATION (Before)                                       │
│     ├── Research the stakeholder and their domain              │
│     ├── Define interview objectives                            │
│     ├── Prepare questions (open and closed)                    │
│     ├── Schedule appropriate time (45-60 min)                  │
│     ├── Choose comfortable location                            │
│     └── Prepare recording method (with permission)             │
│                                                                 │
│  2. OPENING (5-10 min)                                         │
│     ├── Introduce yourself and purpose                         │
│     ├── Explain how information will be used                   │
│     ├── Ask permission to record                               │
│     ├── Set expectations for time                              │
│     └── Build rapport with small talk                          │
│                                                                 │
│  3. BODY (30-45 min)                                           │
│     ├── Start with broad questions                             │
│     ├── Move to specific details                               │
│     ├── Use active listening                                   │
│     ├── Probe deeper with follow-ups                           │
│     ├── Take notes on key points                               │
│     └── Watch for non-verbal cues                              │
│                                                                 │
│  4. CLOSING (5-10 min)                                         │
│     ├── Summarize key points                                   │
│     ├── Ask if anything was missed                             │
│     ├── Explain next steps                                     │
│     ├── Thank the participant                                  │
│     └── Leave door open for follow-up                          │
│                                                                 │
│  5. FOLLOW-UP (After)                                          │
│     ├── Transcribe/organize notes within 24 hours              │
│     ├── Send summary for validation                            │
│     ├── Clarify any ambiguities                                │
│     ├── Share relevant findings with team                      │
│     └── Update requirements documentation                      │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Question Techniques

```
┌─────────────────────────────────────────────────────────────────┐
│                  QUESTIONING TECHNIQUES                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  OPEN QUESTIONS (Exploration)                                  │
│  "Tell me about your typical day using the current system"     │
│  "What challenges do you face with..."                         │
│  "How would you describe..."                                   │
│                                                                 │
│  CLOSED QUESTIONS (Confirmation)                               │
│  "Do you use this feature daily?"                              │
│  "Is the response time acceptable?"                            │
│  "Would you prefer option A or B?"                             │
│                                                                 │
│  PROBING QUESTIONS (Deeper understanding)                      │
│  "Can you give me an example?"                                 │
│  "Why is that important to you?"                               │
│  "What happens when that occurs?"                              │
│                                                                 │
│  5 WHYS TECHNIQUE (Root cause)                                 │
│  Problem: "Reports are always late"                            │
│  Why? "Data extraction takes too long"                         │
│  Why? "Multiple systems must be queried"                       │
│  Why? "Data isn't consolidated"                                │
│  Why? "Systems were built separately"                          │
│  Why? "No integration strategy existed"                        │
│  → Root cause: Lack of data integration strategy               │
│                                                                 │
│  HYPOTHETICAL QUESTIONS (Future needs)                         │
│  "If you could change one thing, what would it be?"           │
│  "Imagine the perfect solution - what would it do?"           │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Active Listening Techniques

```
┌─────────────────────────────────────────────────────────────────┐
│                   ACTIVE LISTENING                              │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  PARAPHRASING                                                   │
│  "So what you're saying is..."                                 │
│  "If I understand correctly..."                                │
│                                                                 │
│  SUMMARIZING                                                    │
│  "Let me recap the main points..."                             │
│  "The key issues seem to be..."                                │
│                                                                 │
│  CLARIFYING                                                     │
│  "What do you mean by...?"                                     │
│  "Could you elaborate on...?"                                  │
│                                                                 │
│  REFLECTING FEELINGS                                            │
│  "It sounds like that's frustrating..."                        │
│  "You seem excited about..."                                   │
│                                                                 │
│  ENCOURAGING                                                    │
│  "Tell me more about that..."                                  │
│  "That's interesting, please continue..."                      │
│                                                                 │
│  NON-VERBAL                                                     │
│  • Eye contact                                                 │
│  • Nodding                                                     │
│  • Open body posture                                           │
│  • Taking notes                                                │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Common Pitfalls

```
❌ INTERVIEWER PITFALLS:

• Leading questions: "Don't you think X is better?"
• Interrupting: Let them finish their thoughts
• Solutioning: Focus on problems, not solutions
• Assuming: Verify understanding, don't assume
• Not recording: Memory is unreliable
• Time pressure: Rushing leads to missed insights

❌ INTERVIEWEE CHALLENGES:

• Speaking in solutions: "I need a button that..."
  → Dig deeper: "What problem does that solve?"
  
• Contradictory information: Different stakeholders disagree
  → Document both perspectives, facilitate resolution
  
• Hidden agenda: Personal preferences disguised as needs
  → Ask "why" to uncover true motivations
  
• Unavailable stakeholders: Key people too busy
  → Escalate importance, schedule shorter sessions
```

## 📌 Key Points (Optional)
- Interviews are an underestimated art
- The difference between a mediocre and excellent interview is in preparation and the ability to go beyond superficial answers
- The secret is creating a trusting environment where stakeholders feel comfortable sharing their real pain points
- Favorite technique: Start by asking about "a typical day" - it reveals more than 20 direct questions
- Beware of "solutionism": stakeholders often ask for specific solutions when they should describe problems

## 🖼️ Recommended Image
- Two people in conversation with thought bubbles showing requirements emerging

## 🔗 Connections
- [[209-Stakeholder Management]]
- [[214-Business Requirements]]
- [[218-User Story]]
- [[210-Product Owner]]
- [[215-Functional Requirements]]
- [[216-Non Functional Requirements]]
- [[220-Product Backlog]]