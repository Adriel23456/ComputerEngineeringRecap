---
Theme: Front End Semantic Analysis
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Compilers, FrontEnd, Semantics]
---

## 📚 Idea/Concept

Semantic Analysis is the third phase of compilation that checks the meaning of the program beyond syntactic correctness. It verifies type consistency, scope rules, and other context-sensitive properties, decorating the AST with semantic information.

### Semantic Analysis Tasks

1. **Type Checking**: Verify type compatibility
2. **Scope Resolution**: Bind identifiers to declarations
3. **Declaration Verification**: Check all variables are declared
4. **Flow Analysis**: Verify return statements, breaks, etc.
5. **Symbol Table Management**: Build and query symbol information

### Type Checking

**Type Compatibility Rules**:
```
int + int     → int
int + float   → float (promotion)
float + float → float
int == int    → bool
pointer + int → pointer
array[int]    → element type
```

**Type Checking Algorithm**:
```c
Type* typeCheck(Expr* expr) {
    switch (expr->kind) {
        case BINARY:
            Type* left = typeCheck(expr->left);
            Type* right = typeCheck(expr->right);
            return checkBinaryOp(expr->op, left, right);
        
        case IDENTIFIER:
            Symbol* sym = lookup(expr->name);
            if (!sym) error("Undeclared variable");
            return sym->type;
        
        case LITERAL:
            return inferLiteralType(expr);
        
        case CALL:
            return checkFunctionCall(expr);
    }
}
```

### Scope Management

**Scope Types**:
- Global scope
- Function scope
- Block scope (in C: within { })
- Class scope (in OOP languages)

**Symbol Table Stack**:
```c
struct Scope {
    HashMap *symbols;
    Scope *parent;
};

// Enter new scope
void enterScope() {
    Scope *newScope = malloc(sizeof(Scope));
    newScope->symbols = createHashMap();
    newScope->parent = currentScope;
    currentScope = newScope;
}

// Exit scope
void exitScope() {
    currentScope = currentScope->parent;
}

// Lookup with scope chain
Symbol* lookup(char *name) {
    Scope *scope = currentScope;
    while (scope != NULL) {
        Symbol *sym = hashMapGet(scope->symbols, name);
        if (sym) return sym;
        scope = scope->parent;
    }
    return NULL;  // Not found
}
```

### Type Coercion

**Implicit Conversions**:
```
char → int → long → float → double
     (widening conversions)
```

**Coercion Rules**:
```c
Type* coerce(Type* from, Type* to, Expr** expr) {
    if (typeEquals(from, to)) return to;
    
    if (isNumeric(from) && isNumeric(to)) {
        if (rank(to) > rank(from)) {
            // Insert cast node
            *expr = makeCastExpr(to, *expr);
            return to;
        }
    }
    
    error("Incompatible types");
    return NULL;
}
```

### Function Overloading Resolution

In languages that support overloading:
```c
Symbol* resolveOverload(char *name, Type** argTypes, int numArgs) {
    Symbol *candidates = lookupAll(name);
    
    for (each candidate) {
        if (numArgs == candidate->numParams) {
            bool match = true;
            for (int i = 0; i < numArgs; i++) {
                if (!canConvert(argTypes[i], candidate->paramTypes[i])) {
                    match = false;
                    break;
                }
            }
            if (match) return candidate;
        }
    }
    
    error("No matching overload");
    return NULL;
}
```

### Common Semantic Errors

| Error Type | Example |
|------------|---------|
| Undeclared variable | Using `x` before declaration |
| Type mismatch | `int x = "hello";` |
| Redeclaration | Declaring same variable twice in scope |
| Argument mismatch | Wrong number/types in function call |
| Invalid operation | `int[] + int[]` |
| Missing return | Non-void function without return |
| Break outside loop | `break;` not in loop/switch |

### Attribute Grammars

Formal specification of semantic rules:

**Synthesized Attributes** (computed bottom-up):
```
E → E₁ + T    { E.type = max(E₁.type, T.type) }
T → num       { T.type = INT }
T → num.num   { T.type = FLOAT }
```

**Inherited Attributes** (passed top-down):
```
D → T L       { L.type = T.type }
T → int       { T.type = INT }
L → L₁, id    { L₁.type = L.type
                addType(id, L.type) }
```

## 📌 Key Points (Optional)
- Semantic analysis ensures program meaning is valid beyond syntax
- Type checking enforces type compatibility rules
- Scope management tracks variable visibility
- The symbol table is the central semantic data structure

## 🖼️ Recommended Image
- Diagram showing AST annotation with type information during semantic analysis

## 🔗 Connections
- [[153-Front End Syntactic Analysis]]
- [[156-Abstract Syntax Tree Construction]]
- [[158-Middle End Optimization]]