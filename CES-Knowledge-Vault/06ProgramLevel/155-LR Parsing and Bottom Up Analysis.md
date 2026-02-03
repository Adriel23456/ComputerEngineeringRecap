---
Theme: LR Parsing and Bottom Up Analysis
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, Parsing, BottomUp]
---

## 📚 Idea/Concept

LR Parsing is a bottom-up parsing technique that reads input Left-to-right and produces a Rightmost derivation in reverse. LR parsers are more powerful than LL parsers, able to handle a larger class of grammars, and are the basis for most parser generators.

### LR Parser Types

| Type | Description | Power |
|------|-------------|-------|
| LR(0) | No lookahead | Weakest |
| SLR(1) | Simple LR with FOLLOW sets | Medium |
| LALR(1) | Lookahead LR (Yacc/Bison) | Strong |
| LR(1) | Canonical LR | Strongest |

### Shift-Reduce Parsing

Two main actions:
- **Shift**: Push input token onto stack
- **Reduce**: Replace handle on stack with non-terminal

**Additional actions**:
- **Accept**: Parsing complete successfully
- **Error**: Syntax error detected

**Example**: Parsing "id * id + id" with E → E+T | T, T → T*F | F, F → id
```
Stack          Input           Action
$              id*id+id$       Shift
$id            *id+id$         Reduce F→id
$F             *id+id$         Reduce T→F
$T             *id+id$         Shift
$T*            id+id$          Shift
$T*id          +id$            Reduce F→id
$T*F           +id$            Reduce T→T*F
$T             +id$            Reduce E→T
$E             +id$            Shift
$E+            id$             Shift
$E+id          $               Reduce F→id
$E+F           $               Reduce T→F
$E+T           $               Reduce E→E+T
$E             $               Accept
```

### LR(0) Items

An LR(0) item is a production with a dot (•) indicating parsing progress:

For production A → XYZ:
- A → •XYZ (nothing parsed yet)
- A → X•YZ (X has been seen)
- A → XY•Z (XY has been seen)
- A → XYZ• (complete, ready to reduce)

### Viable Prefixes

A viable prefix is any prefix of a right sentential form that does not extend past the handle. The set of viable prefixes forms a regular language recognizable by a DFA.

### Building LR(0) Automaton

**Closure Operation**:
```
closure(I):
  repeat
    for each item A → α•Bβ in I:
      for each production B → γ:
        add B → •γ to I
  until no more items added
  return I
```

**Goto Operation**:
```
goto(I, X):
  J = empty set
  for each item A → α•Xβ in I:
    add A → αX•β to J
  return closure(J)
```

### SLR Parsing Table Construction

1. Construct LR(0) item sets (canonical collection)
2. For each state I with item A → α•aβ (a is terminal):
   - ACTION[I, a] = shift to goto(I, a)
3. For each state I with item A → α• (A ≠ S'):
   - ACTION[I, b] = reduce A → α, for all b in FOLLOW(A)
4. For state I with S' → S•:
   - ACTION[I, $] = accept
5. For non-terminal transitions:
   - GOTO[I, A] = goto(I, A)

### LALR vs SLR

**SLR Limitations**:
- Uses FOLLOW sets for reduce decisions
- May have conflicts that LALR avoids

**LALR Improvements**:
- Computes specific lookahead for each item
- Merges LR(1) states with same core
- Same number of states as SLR
- More powerful than SLR

### Conflict Resolution

**Shift-Reduce Conflict**:
- Both shift and reduce are valid
- Resolution: Precedence/associativity rules

**Reduce-Reduce Conflict**:
- Multiple reductions possible
- Resolution: Choose based on grammar order

**Example Conflict Resolution** (dangling else):
```
Grammar creates shift-reduce conflict:
  Stmt → if Expr then Stmt
       | if Expr then Stmt else Stmt

Resolution: Prefer shift (match else with nearest if)
```

### Parser Generator: Bison
```bison
%token ID NUMBER
%left '+' '-'
%left '*' '/'

%%
expr: expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | '(' expr ')'
    | ID
    | NUMBER
    ;
%%
```

## 📌 Key Points (Optional)
- LR parsers are more powerful than LL parsers
- Shift-reduce parsing builds the parse tree bottom-up
- SLR uses FOLLOW sets; LALR uses specific lookaheads
- Conflicts are resolved by precedence and associativity

## 🖼️ Recommended Image
- State diagram of LR automaton with shift/reduce actions labeled

## 🔗 Connections
- [[153-Front End Syntactic Analysis]]
- [[154-LL Parsing and Predictive Analysis]]
- [[156-Abstract Syntax Tree Construction]]
- [[160-GCC Compiler Architecture]]