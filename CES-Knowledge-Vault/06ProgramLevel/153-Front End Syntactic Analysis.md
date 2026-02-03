---
Theme: Front End Syntactic Analysis
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, FrontEnd, Parsing]
---

## 📚 Idea/Concept

Syntactic Analysis (Parsing) is the second phase of compilation that takes the token stream from the lexer and constructs a hierarchical structure (parse tree or AST) that represents the grammatical structure of the program according to the language's grammar rules.

### Context-Free Grammars (CFG)

A CFG is defined as a 4-tuple: G = (V, T, P, S)

| Component | Description |
|-----------|-------------|
| V | Set of non-terminals (variables) |
| T | Set of terminals (tokens) |
| P | Set of production rules |
| S | Start symbol (S ∈ V) |

**Example Grammar for Arithmetic Expressions**:
```
E → E + T | E - T | T
T → T * F | T / F | F
F → ( E ) | id | num
```

### Derivations

A derivation shows how a string is generated from the grammar:

**Leftmost Derivation** (expand leftmost non-terminal first):
```
E ⇒ E + T
  ⇒ T + T
  ⇒ F + T
  ⇒ id + T
  ⇒ id + F
  ⇒ id + num
```

**Rightmost Derivation** (expand rightmost non-terminal first):
```
E ⇒ E + T
  ⇒ E + F
  ⇒ E + num
  ⇒ T + num
  ⇒ F + num
  ⇒ id + num
```

### Parse Tree vs Abstract Syntax Tree

**Parse Tree** (Concrete Syntax Tree):
- Includes all grammar symbols
- Shows complete derivation
- Contains redundant information

**Abstract Syntax Tree (AST)**:
- Compressed representation
- Only essential structure
- Used for further compilation phases
```
Expression: id + num * id

Parse Tree:              AST:
     E                     +
    /|\                   / \
   E + T                 id   *
   |  /|\                    / \
   T T * F                 num  id
   |  |   |
   F  F   id
   |  |
  id num
```

### Parsing Strategies

**Top-Down Parsing**:
- Starts from start symbol
- Builds tree from root to leaves
- Examples: Recursive Descent, LL parsers

**Bottom-Up Parsing**:
- Starts from input tokens
- Builds tree from leaves to root
- Examples: LR, SLR, LALR parsers

### Ambiguity in Grammars

A grammar is ambiguous if a string has multiple parse trees:

**Ambiguous Grammar**:
```
E → E + E | E * E | id
```

For "id + id * id":
```
Tree 1:      Tree 2:
    E            E
   /|\          /|\
  E + E        E * E
  |  /|\      /|\   |
 id E * E    E + E  id
    |   |    |   |
   id  id   id  id
```

**Solution**: Rewrite grammar with precedence:
```
E → E + T | T
T → T * F | F
F → id
```

### Parser Tools

| Tool | Type | Target Languages |
|------|------|------------------|
| Yacc/Bison | LALR(1) | C/C++ |
| ANTLR | LL(*) | Java, Python, C#, etc. |
| PLY | LALR(1) | Python |
| JavaCC | LL(k) | Java |

## 📌 Key Points (Optional)
- Parsers verify syntactic correctness using CFG rules
- Parse trees show complete derivation; ASTs show essential structure
- Ambiguity must be eliminated through grammar design
- Top-down and bottom-up are the two main parsing strategies

## 🖼️ Recommended Image
- Diagram comparing parse tree and AST for the same expression

## 🔗 Connections
- [[151-Front End Lexical Analysis]]
- [[154-LL Parsing and Predictive Analysis]]
- [[155-LR Parsing and Bottom Up Analysis]]
- [[156-Abstract Syntax Tree Construction]]
- [[157-Front End Semantic Analysis]]