---
Theme: Programming Paradigms Overview
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Paradigms, Languages, Fundamentals]
---

## 📚 Idea/Concept

Programming paradigms are fundamental approaches to organizing and structuring code. Each paradigm provides different mental models for problem-solving, affecting how we decompose problems, manage state, and structure programs. Understanding multiple paradigms makes you a more versatile programmer and helps you choose the right tool for each problem.

### Major Paradigms
```
Programming Paradigms
├── Imperative (How to do)
│   ├── Procedural
│   └── Object-Oriented
└── Declarative (What to do)
    ├── Functional
    └── Logic
```

---

### Imperative Paradigm

Specifies *how* to achieve results through sequences of statements that change state.

#### Procedural Programming

Organizes code into procedures (functions) that operate on data:
```c
// C - Procedural
struct Account {
    int id;
    double balance;
};

void deposit(struct Account* acc, double amount) {
    if (amount > 0) {
        acc->balance += amount;
    }
}

void withdraw(struct Account* acc, double amount) {
    if (amount > 0 && acc->balance >= amount) {
        acc->balance -= amount;
    }
}

// Usage
struct Account myAccount = {1, 1000.0};
deposit(&myAccount, 500.0);
withdraw(&myAccount, 200.0);
```

**Characteristics**:
- Sequential execution
- Functions operate on data
- Explicit state changes
- Control structures (if, for, while)

#### Object-Oriented Programming

Organizes code around objects containing both data and behavior:
```cpp
// C++ - Object-Oriented
class Account {
private:
    int id;
    double balance;
    
public:
    Account(int id, double initial) : id(id), balance(initial) {}
    
    void deposit(double amount) {
        if (amount > 0) balance += amount;
    }
    
    bool withdraw(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }
    
    double getBalance() const { return balance; }
};

// Usage
Account myAccount(1, 1000.0);
myAccount.deposit(500.0);
myAccount.withdraw(200.0);
```

**Characteristics**:
- Encapsulation, inheritance, polymorphism
- Objects = data + methods
- Message passing between objects
- Code reuse through inheritance

---

### Declarative Paradigm

Specifies *what* result is wanted, not how to compute it.

#### Functional Programming

Treats computation as evaluation of mathematical functions:
```haskell
-- Haskell - Pure Functional
-- No mutable state, functions are first-class

-- Function definition
double x = x * 2

-- Higher-order functions
map double [1, 2, 3]  -- [2, 4, 6]

-- Function composition
sumOfSquares = sum . map (^2)
sumOfSquares [1, 2, 3]  -- 14

-- Recursion instead of loops
factorial 0 = 1
factorial n = n * factorial (n - 1)

-- List comprehension
evens = [x | x <- [1..100], even x]
```
```python
# Python - Functional style
from functools import reduce

# Lambda functions
double = lambda x: x * 2

# Map, filter, reduce
numbers = [1, 2, 3, 4, 5]
doubled = list(map(lambda x: x * 2, numbers))      # [2, 4, 6, 8, 10]
evens = list(filter(lambda x: x % 2 == 0, numbers)) # [2, 4]
total = reduce(lambda acc, x: acc + x, numbers, 0)  # 15

# List comprehension
squares = [x**2 for x in numbers]  # [1, 4, 9, 16, 25]
```

**Characteristics**:
- Pure functions (no side effects)
- Immutable data
- First-class functions
- Recursion over iteration
- Function composition

#### Logic Programming

Expresses logic and lets the system derive conclusions:
```prolog
% Prolog - Logic Programming

% Facts
parent(tom, bob).
parent(tom, liz).
parent(bob, ann).
parent(bob, pat).

% Rules
grandparent(X, Y) :- parent(X, Z), parent(Z, Y).
sibling(X, Y) :- parent(Z, X), parent(Z, Y), X \= Y.

% Queries
?- grandparent(tom, ann).    % true
?- grandparent(tom, Who).    % Who = ann; Who = pat
?- sibling(ann, pat).        % true
```

**Characteristics**:
- Facts and rules define relationships
- Pattern matching and unification
- Backtracking search
- Declarative goal specification

---

### Paradigm Comparison

| Aspect | Procedural | OOP | Functional | Logic |
|--------|------------|-----|------------|-------|
| Focus | Procedures | Objects | Functions | Relations |
| State | Mutable | Encapsulated | Immutable | Unification |
| Control | Explicit | Message passing | Composition | Backtracking |
| Reuse | Functions | Inheritance | Higher-order | Rules |

### Same Problem, Different Paradigms

**Problem**: Sum of squares of even numbers in a list
```c
// Procedural (C)
int sumOfSquaresOfEvens(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] % 2 == 0) {
            sum += arr[i] * arr[i];
        }
    }
    return sum;
}
```
```python
# Functional (Python)
def sum_of_squares_of_evens(numbers):
    return sum(x**2 for x in numbers if x % 2 == 0)

# Or with explicit functions:
from functools import reduce
sum(map(lambda x: x**2, filter(lambda x: x % 2 == 0, numbers)))
```
```sql
-- Declarative (SQL)
SELECT SUM(value * value)
FROM numbers
WHERE value % 2 = 0;
```

### Multi-Paradigm Languages

Most modern languages support multiple paradigms:

| Language | Primary | Also Supports |
|----------|---------|---------------|
| Python | OOP | Functional, Procedural |
| JavaScript | Multi | OOP, Functional, Procedural |
| Scala | Functional | OOP |
| C++ | OOP | Procedural, Generic |
| Rust | Multi | Functional, Procedural |

## 📌 Key Points (Optional)
- Imperative: *how* to compute (procedural, OOP)
- Declarative: *what* to compute (functional, logic)
- Functional: pure functions, immutability, composition
- OOP: encapsulation, inheritance, polymorphism
- Choose paradigm based on problem domain and team expertise

## 🖼️ Recommended Image
- Diagram showing paradigm hierarchy and language examples

## 🔗 Connections
- [[168-Object Oriented Programming Fundamentals]]
- [[165-Python Fundamentals]]
- [[162-C Language Fundamentals and Data Types]]
- [[167-Recursion Fundamentals]]