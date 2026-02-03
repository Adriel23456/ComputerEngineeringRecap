---
Theme: Abstract Syntax Tree Construction
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, AST, DataStructures]
---

## 📚 Idea/Concept

The Abstract Syntax Tree (AST) is a tree representation of the syntactic structure of source code that abstracts away non-essential details like parentheses and punctuation. It serves as the primary data structure for semantic analysis and code generation.

### Parse Tree vs AST

**Parse Tree (Concrete Syntax Tree)**:
- Includes all grammar symbols
- Reflects grammar structure exactly
- Contains redundant nodes

**Abstract Syntax Tree**:
- Only semantically meaningful nodes
- Operators become internal nodes
- Operands become leaves
- Compact and efficient
```
Expression: (a + b) * c

Parse Tree:            AST:
      E                  *
     /|\                / \
    T * F              +   c
   /|\   |            / \
  ( E )  c           a   b
   /|\
  T + T
  |   |
  F   F
  |   |
  a   b
```

### AST Node Types

**Expression Nodes**:
```c
// Binary operation node
struct BinaryExpr {
    enum { ADD, SUB, MUL, DIV, ... } op;
    Expr *left;
    Expr *right;
};

// Unary operation node
struct UnaryExpr {
    enum { NEG, NOT, ... } op;
    Expr *operand;
};

// Literal node
struct Literal {
    enum { INT, FLOAT, STRING, ... } type;
    union { int i; float f; char *s; } value;
};

// Identifier node
struct Identifier {
    char *name;
    Symbol *symbol;  // Link to symbol table
};
```

**Statement Nodes**:
```c
// If statement
struct IfStmt {
    Expr *condition;
    Stmt *thenBranch;
    Stmt *elseBranch;  // May be NULL
};

// While loop
struct WhileStmt {
    Expr *condition;
    Stmt *body;
};

// Assignment
struct AssignStmt {
    Identifier *target;
    Expr *value;
};

// Block of statements
struct Block {
    Stmt **statements;
    int count;
};
```

### Building AST During Parsing

**Semantic Actions in Bison**:
```bison
expr: expr '+' expr    { $$ = make_binary(ADD, $1, $3); }
    | expr '*' expr    { $$ = make_binary(MUL, $1, $3); }
    | '(' expr ')'     { $$ = $2; }
    | NUMBER           { $$ = make_literal($1); }
    | ID               { $$ = make_identifier($1); }
    ;
```

**Helper Functions**:
```c
Expr* make_binary(Operator op, Expr* left, Expr* right) {
    BinaryExpr *node = malloc(sizeof(BinaryExpr));
    node->op = op;
    node->left = left;
    node->right = right;
    return (Expr*)node;
}

Expr* make_literal(int value) {
    Literal *node = malloc(sizeof(Literal));
    node->type = INT;
    node->value.i = value;
    return (Expr*)node;
}
```

### AST Traversal

**Visitor Pattern**:
```c
// Visitor interface
struct ExprVisitor {
    void (*visitBinary)(BinaryExpr*);
    void (*visitUnary)(UnaryExpr*);
    void (*visitLiteral)(Literal*);
    void (*visitIdentifier)(Identifier*);
};

// Traverse AST
void accept(Expr *expr, ExprVisitor *v) {
    switch (expr->type) {
        case BINARY: v->visitBinary((BinaryExpr*)expr); break;
        case UNARY:  v->visitUnary((UnaryExpr*)expr); break;
        case LITERAL: v->visitLiteral((Literal*)expr); break;
        case IDENT:  v->visitIdentifier((Identifier*)expr); break;
    }
}
```

### AST Example

**Source Code**:
```c
if (x > 0) {
    y = x * 2;
} else {
    y = -x;
}
```

**AST Structure**:
```
IfStmt
├── condition: BinaryExpr(GT)
│   ├── left: Identifier("x")
│   └── right: Literal(0)
├── thenBranch: Block
│   └── AssignStmt
│       ├── target: Identifier("y")
│       └── value: BinaryExpr(MUL)
│           ├── left: Identifier("x")
│           └── right: Literal(2)
└── elseBranch: Block
    └── AssignStmt
        ├── target: Identifier("y")
        └── value: UnaryExpr(NEG)
            └── operand: Identifier("x")
```

### AST Properties

**Type Annotations**:
After semantic analysis, AST nodes are annotated with type information.

**Symbol References**:
Identifiers link to symbol table entries.

**Source Location**:
Nodes store line/column for error reporting.

## 📌 Key Points (Optional)
- ASTs abstract away syntactic details, keeping only semantic structure
- Parser semantic actions construct AST nodes during parsing
- Visitor pattern enables clean AST traversal
- ASTs are annotated with types during semantic analysis

## 🖼️ Recommended Image
- Side-by-side comparison of parse tree and AST for a complex expression

## 🔗 Connections
- [[153-Front End Syntactic Analysis]]
- [[157-Front End Semantic Analysis]]
- [[158-Middle End Optimization]]