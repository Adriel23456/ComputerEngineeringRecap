---
Theme: LL Parsing and Predictive Analysis
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, Parsing, TopDown]
---

## 📚 Idea/Concept

LL Parsing is a top-down parsing technique that reads input Left-to-right and produces a Leftmost derivation. LL(k) parsers look ahead k tokens to decide which production rule to apply, with LL(1) being the most common variant.

### LL(1) Parser Requirements

For a grammar to be LL(1):
1. No left recursion
2. No common prefixes (left-factored)
3. For each non-terminal A with productions A → α | β:
   - FIRST(α) ∩ FIRST(β) = ∅
   - If ε ∈ FIRST(α), then FIRST(β) ∩ FOLLOW(A) = ∅

### FIRST Sets

FIRST(α) = set of terminals that begin strings derived from α

**Computing FIRST**:
```
FIRST(terminal) = {terminal}
FIRST(ε) = {ε}
FIRST(A → α) = FIRST(α)

If A → Y₁Y₂...Yₖ:
  Add FIRST(Y₁) - {ε} to FIRST(A)
  If ε ∈ FIRST(Y₁), add FIRST(Y₂) - {ε}
  Continue until Yᵢ doesn't derive ε
  If all Yᵢ derive ε, add ε to FIRST(A)
```

**Example**:
```
Grammar:
E  → T E'
E' → + T E' | ε
T  → F T'
T' → * F T' | ε
F  → ( E ) | id

FIRST Sets:
FIRST(F)  = { (, id }
FIRST(T') = { *, ε }
FIRST(T)  = { (, id }
FIRST(E') = { +, ε }
FIRST(E)  = { (, id }
```

### FOLLOW Sets

FOLLOW(A) = set of terminals that can appear immediately after A

**Computing FOLLOW**:
```
1. Add $ to FOLLOW(Start Symbol)
2. If A → αBβ:
   Add FIRST(β) - {ε} to FOLLOW(B)
3. If A → αB or (A → αBβ and ε ∈ FIRST(β)):
   Add FOLLOW(A) to FOLLOW(B)
```

**Example** (continuing from above):
```
FOLLOW Sets:
FOLLOW(E)  = { ), $ }
FOLLOW(E') = { ), $ }
FOLLOW(T)  = { +, ), $ }
FOLLOW(T') = { +, ), $ }
FOLLOW(F)  = { *, +, ), $ }
```

### LL(1) Parsing Table

Construct table M[A, a] for non-terminal A and terminal a:
```
For each production A → α:
  For each terminal a in FIRST(α):
    Add A → α to M[A, a]
  If ε ∈ FIRST(α):
    For each terminal b in FOLLOW(A):
      Add A → α to M[A, b]
```

**Parsing Table Example**:
```
        │  id   │   +   │   *   │   (   │   )   │   $
   ─────┼───────┼───────┼───────┼───────┼───────┼───────
    E   │ E→TE' │       │       │ E→TE' │       │
    E'  │       │E'→+TE'│       │       │ E'→ε  │ E'→ε
    T   │ T→FT' │       │       │ T→FT' │       │
    T'  │       │ T'→ε  │T'→*FT'│       │ T'→ε  │ T'→ε
    F   │ F→id  │       │       │ F→(E) │       │
```

### Predictive Parsing Algorithm
```
stack = [$, Start_Symbol]
input = token_stream + $
X = top of stack
a = current input token

while X ≠ $:
    if X == a:
        pop stack
        advance input
    else if X is terminal:
        error()
    else if M[X, a] is empty:
        error()
    else if M[X, a] = X → Y₁Y₂...Yₖ:
        pop X from stack
        push Yₖ...Y₂Y₁ onto stack (reverse order)
        output production
    X = top of stack
```

### Eliminating Left Recursion

Left recursion prevents top-down parsing:

**Direct Left Recursion**:
```
A → Aα | β
Converts to:
A  → βA'
A' → αA' | ε
```

**Example**:
```
E → E + T | T
Becomes:
E  → T E'
E' → + T E' | ε
```

### Left Factoring

Remove common prefixes for LL(1) compatibility:
```
A → αβ₁ | αβ₂
Becomes:
A  → αA'
A' → β₁ | β₂
```

**Example**:
```
Stmt → if Expr then Stmt
     | if Expr then Stmt else Stmt
Becomes:
Stmt     → if Expr then Stmt ElsePart
ElsePart → else Stmt | ε
```

## 📌 Key Points (Optional)
- LL(1) parsers use one lookahead token to select productions
- FIRST and FOLLOW sets determine parsing table entries
- Left recursion must be eliminated for top-down parsing
- Left factoring resolves common prefix conflicts

## 🖼️ Recommended Image
- Flowchart showing predictive parsing with stack and input visualization

## 🔗 Connections
- [[153-Front End Syntactic Analysis]]
- [[155-LR Parsing and Bottom Up Analysis]]
- [[156-Abstract Syntax Tree Construction]]
- [[150-Compiler Fundamentals and Structure]]