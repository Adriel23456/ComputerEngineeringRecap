---
Theme: Front End Lexical Analysis
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, FrontEnd, Lexical]
---

## 📚 Idea/Concept

Lexical Analysis (Scanning) is the first phase of compilation that reads the source code character by character and groups them into meaningful sequences called tokens. The component that performs this task is called a lexer or scanner.

### Core Terminology

**Alphabet (Σ)**:
A finite set of symbols from which strings are formed.
- Example: Binary alphabet Σ = {0, 1}
- Example: C alphabet includes letters, digits, operators

**String (Chain)**:
A finite sequence of symbols from an alphabet.
- ε (epsilon) represents the empty string
- |s| denotes the length of string s

**Language**:
A set of strings over an alphabet.
- Can be finite or infinite
- Defined by rules or patterns

**Token**:
A categorized unit of the source program:
```
Token = <token_type, attribute_value>
Example: <IDENTIFIER, "count">
         <NUMBER, 42>
         <OPERATOR, "+">
```

**Lexeme**:
The actual character sequence that matches a token pattern:
```c
int count = 42;
```
Lexemes: "int", "count", "=", "42", ";"

**Pattern**:
A rule describing the form of lexemes for a token type.

### Token Categories

| Category | Examples | Pattern |
|----------|----------|---------|
| Keywords | if, while, return | Reserved words |
| Identifiers | count, myVar | letter(letter\|digit)* |
| Numbers | 42, 3.14, 0xFF | digit+ |
| Operators | +, -, *, <=, == | Specific symbols |
| Punctuation | ;, {, }, ( | Single characters |
| Strings | "hello" | "character*" |
| Comments | // text, /* */ | Special delimiters |

### Regular Expressions

Patterns are typically specified using regular expressions:

**Basic Operations**:
- **Concatenation**: ab (a followed by b)
- **Union (|)**: a|b (a or b)
- **Kleene Star (*)**: a* (zero or more a's)
- **Kleene Plus (+)**: a+ (one or more a's)
- **Optional (?)**: a? (zero or one a)

**Examples**:
```
Identifier: [a-zA-Z_][a-zA-Z0-9_]*
Integer:    [0-9]+
Float:      [0-9]+\.[0-9]+
Keyword:    if|else|while|for|return
```

### Finite State Machines (FSM)

Lexers are implemented using Finite Automata:

**Deterministic Finite Automaton (DFA)**:
- One transition per input symbol per state
- Faster execution
- May require more states

**Non-deterministic Finite Automaton (NFA)**:
- Multiple transitions possible per symbol
- ε-transitions allowed
- Easier to construct from regex

**DFA for Integer Recognition**:
```
        digit
    ┌─────────┐
    ▼         │
──► (S) ──digit──► ((F))
         └────────┘
           digit
```

### Lexer Tools

Common lexer generators:
- **Lex/Flex**: Traditional Unix lexer generator
- **ANTLR**: Generates lexers for multiple languages
- **JFlex**: Java-based lexer generator

**Flex Example**:
```lex
%%
[0-9]+          { return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*  { return IDENTIFIER; }
"if"            { return IF; }
"while"         { return WHILE; }
"+"             { return PLUS; }
[ \t\n]         { /* skip whitespace */ }
%%
```

### Error Handling in Lexical Analysis

**Panic Mode Recovery**:
- Skip characters until a valid token is found
- Report error location

**Common Lexical Errors**:
- Invalid characters
- Malformed numbers (e.g., 12.34.56)
- Unterminated strings
- Unrecognized symbols

## 📌 Key Points (Optional)
- Lexical analysis converts character streams into token streams
- Regular expressions define token patterns
- DFAs provide efficient token recognition
- Tools like Flex automate lexer generation

## 🖼️ Recommended Image
- Diagram showing transformation from source code to token stream with FSM

## 🔗 Connections
- [[150-Compiler Fundamentals and Structure]]
- [[153-Front End Syntactic Analysis]]
- [[152-Finite State Machines DFA and NFA]]