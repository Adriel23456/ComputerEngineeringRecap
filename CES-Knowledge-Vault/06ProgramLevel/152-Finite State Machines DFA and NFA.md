---
Theme: Finite State Machines DFA and NFA
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, Automata, FSM]
---

## 📚 Idea/Concept

Finite State Machines (FSM) are mathematical models of computation that provide the theoretical foundation for lexical analysis. They define how a system transitions between states based on input, enabling recognition of patterns defined by regular expressions.

### Formal Definition of Finite Automaton

A Finite Automaton is defined as a 5-tuple: M = (Q, Σ, δ, q₀, F)

| Component | Description |
|-----------|-------------|
| Q | Finite set of states |
| Σ | Input alphabet |
| δ | Transition function |
| q₀ | Initial state (q₀ ∈ Q) |
| F | Set of accepting states (F ⊆ Q) |

### Deterministic Finite Automaton (DFA)

In a DFA, for each state and input symbol, there is exactly one transition:
- δ: Q × Σ → Q
- No ambiguity in state transitions
- No ε-transitions allowed

**DFA Example: Binary strings ending in "01"**
```
States: Q = {q0, q1, q2}
Alphabet: Σ = {0, 1}
Initial: q0
Accepting: F = {q2}

Transition Table:
        │  0   │  1
   ─────┼──────┼──────
    q0  │  q1  │  q0
    q1  │  q1  │  q2
    q2  │  q1  │  q0

State Diagram:
              1
         ┌────────┐
         ▼        │
    ──► (q0) ─0─► (q1) ─1─► ((q2))
         ▲         │         │
         └────1────┘         │
         ▲                   │
         └─────────0─────────┘
```

### Non-deterministic Finite Automaton (NFA)

In an NFA, multiple transitions are possible for a state-symbol pair:
- δ: Q × (Σ ∪ {ε}) → P(Q)
- Can have ε-transitions (move without consuming input)
- Multiple possible next states

**NFA Example: Strings containing "ab"**
```
         a,b           a,b
          │             │
          ▼             ▼
    ──► (q0) ──a──► (q1) ──b──► ((q2))
          │                       ▲
          └───────────────────────┘
                    ε
```

### DFA vs NFA Comparison

| Aspect | DFA | NFA |
|--------|-----|-----|
| Transitions per symbol | Exactly one | Zero, one, or many |
| ε-transitions | Not allowed | Allowed |
| Implementation | Direct, fast | Requires simulation |
| Number of states | Often more | Often fewer |
| Construction | More complex | Easier from regex |
| Execution speed | O(n) | O(n × |Q|²) |

### NFA to DFA Conversion (Subset Construction)

Every NFA can be converted to an equivalent DFA:

**Algorithm**:
1. Compute ε-closure of start state → initial DFA state
2. For each DFA state D and input symbol a:
   - Compute move(D, a) → set of NFA states
   - Compute ε-closure of result → new DFA state
3. Mark as accepting if any NFA accepting state is included
4. Repeat until no new states

**Example Conversion**:
```
NFA: States {0, 1, 2}, accepts on "a*b"

ε-closure({0}) = {0}     → DFA state A
move(A, a) = {0}         → ε-closure = {0} = A
move(A, b) = {1}         → ε-closure = {1,2} = B (accepting)

DFA Result:
    ──► (A) ──a──► (A)
         │
         b
         ▼
       ((B))
```

### DFA Minimization

Reduce DFA states while preserving language:

**Hopcroft's Algorithm**:
1. Partition states into accepting/non-accepting
2. Iteratively refine partitions
3. States in same partition are equivalent
4. Merge equivalent states

### Applications in Lexers

The lexer construction process:
```
Regular Expression → NFA → DFA → Minimized DFA → Lexer Code
      (Easy)       Thompson   Subset   Hopcroft    Table-driven
                  Construction Construction        or Direct
```

## 📌 Key Points (Optional)
- DFAs guarantee deterministic execution with O(n) complexity
- NFAs are easier to construct but may require conversion
- Subset construction converts NFA to equivalent DFA
- Minimization produces the most efficient recognizer

## 🖼️ Recommended Image
- Side-by-side comparison of NFA and equivalent DFA for the same language

## 🔗 Connections
- [[151-Front End Lexical Analysis]]
- [[150-Compiler Fundamentals and Structure]]