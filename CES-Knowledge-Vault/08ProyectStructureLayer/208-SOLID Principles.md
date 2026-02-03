---
Theme: SOLID Principles
Date created: 2026-02-02 12:00
tags: [ProjectStructureLevel, CodingStandards, Design, BestPractices]
---

## 📚 Idea/Concept

SOLID is an acronym for five design principles intended to make software designs more understandable, flexible, and maintainable. Introduced by Robert C. Martin (Uncle Bob), these principles are fundamental to object-oriented programming and design. They guide developers in creating systems that are easy to maintain and extend over time, reducing the cost of changes and the likelihood of bugs.

### The Five SOLID Principles

```
┌─────────────────────────────────────────────────────────────────┐
│                      SOLID PRINCIPLES                           │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  S - Single Responsibility Principle                           │
│      A class should have only one reason to change             │
│                                                                 │
│  O - Open/Closed Principle                                     │
│      Open for extension, closed for modification               │
│                                                                 │
│  L - Liskov Substitution Principle                             │
│      Subtypes must be substitutable for their base types       │
│                                                                 │
│  I - Interface Segregation Principle                           │
│      Many specific interfaces are better than one general      │
│                                                                 │
│  D - Dependency Inversion Principle                            │
│      Depend on abstractions, not concretions                   │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### S - Single Responsibility Principle (SRP)

```
❌ VIOLATION:
class User {
    saveToDatabase() { }
    sendEmail() { }
    generateReport() { }
    validateInput() { }
}
// This class has 4 reasons to change!

✓ CORRECT:
class User { }
class UserRepository { saveToDatabase() { } }
class EmailService { sendEmail() { } }
class ReportGenerator { generateReport() { } }
class UserValidator { validateInput() { } }
// Each class has ONE responsibility
```

### O - Open/Closed Principle (OCP)

```
❌ VIOLATION:
class AreaCalculator {
    calculate(shape) {
        if (shape.type === 'circle') {
            return Math.PI * shape.radius ** 2;
        }
        if (shape.type === 'rectangle') {
            return shape.width * shape.height;
        }
        // Must modify this class for each new shape!
    }
}

✓ CORRECT:
interface Shape {
    getArea(): number;
}

class Circle implements Shape {
    getArea() { return Math.PI * this.radius ** 2; }
}

class Rectangle implements Shape {
    getArea() { return this.width * this.height; }
}

class AreaCalculator {
    calculate(shape: Shape) {
        return shape.getArea(); // Works with any new shape!
    }
}
```

### L - Liskov Substitution Principle (LSP)

```
❌ VIOLATION:
class Rectangle {
    setWidth(w) { this.width = w; }
    setHeight(h) { this.height = h; }
}

class Square extends Rectangle {
    setWidth(w) { 
        this.width = w; 
        this.height = w; // Unexpected behavior!
    }
}

// rect.setWidth(5); rect.setHeight(10);
// Area expected: 50, Square gives: 100

✓ CORRECT:
interface Shape {
    getArea(): number;
}

class Rectangle implements Shape { }
class Square implements Shape { }
// No inheritance, both implement interface
```

### I - Interface Segregation Principle (ISP)

```
❌ VIOLATION:
interface Worker {
    work();
    eat();
    sleep();
}

class Robot implements Worker {
    work() { /* OK */ }
    eat() { /* Robots don't eat! */ }
    sleep() { /* Robots don't sleep! */ }
}

✓ CORRECT:
interface Workable { work(); }
interface Eatable { eat(); }
interface Sleepable { sleep(); }

class Human implements Workable, Eatable, Sleepable { }
class Robot implements Workable { }
```

### D - Dependency Inversion Principle (DIP)

```
❌ VIOLATION:
class OrderService {
    private MySQLDatabase db = new MySQLDatabase();
    // Tightly coupled to specific database!
}

✓ CORRECT:
interface Database {
    save(data);
    find(id);
}

class OrderService {
    constructor(private db: Database) { }
    // Can work with any database implementation!
}

// Inject dependency
const service = new OrderService(new PostgreSQLDatabase());
const testService = new OrderService(new MockDatabase());
```

### SOLID Benefits

| Principle | Primary Benefit |
|-----------|-----------------|
| SRP | Easier maintenance, smaller classes |
| OCP | Easier extension without breaking existing code |
| LSP | Reliable inheritance hierarchies |
| ISP | Focused interfaces, no unnecessary dependencies |
| DIP | Flexible, testable, loosely coupled code |

### Common Violations

```
RED FLAGS that suggest SOLID violations:

• "God class" with too many responsibilities (SRP)
• Switch statements on type (OCP)
• Subclass that overrides base methods unexpectedly (LSP)
• Implementing interfaces with empty methods (ISP)
• Using 'new' keyword inside methods (DIP)
```

## 📌 Key Points (Optional)
- SOLID principles are guidelines, not rigid rules
- They often trade off against simplicity for small projects
- The goal is maintainable, flexible code that accommodates change
- Apply pragmatically: don't over-engineer simple solutions
- These principles underpin most design patterns

## 🖼️ Recommended Image
- Diagram showing each SOLID principle with before/after code comparison

## 🔗 Connections
- [[237-Coding Standards]]
- [[230-Definition of Done]]
- [[235-UML Class Diagram]]