---
Theme: Object Oriented Programming Fundamentals
Date created: 2026-01-31 12:00
tags: [ProgramLevel, OOP, Paradigms, Design]
---

## 📚 Idea/Concept

Object-Oriented Programming (OOP) is a paradigm that organizes software design around data, or objects, rather than functions and logic. OOP models real-world entities as objects with state (attributes) and behavior (methods), enabling code reuse, modularity, and easier maintenance through four core principles: encapsulation, abstraction, inheritance, and polymorphism.

### Core Concepts

**Object**: An instance of a class containing state and behavior
**Class**: A blueprint defining structure and behavior of objects
**Instance**: A specific object created from a class
```cpp
// Class definition (blueprint)
class Dog {
private:
    string name;    // State (attributes)
    int age;
    
public:
    // Constructor
    Dog(string n, int a) : name(n), age(a) {}
    
    // Behavior (methods)
    void bark() {
        cout << name << " says Woof!" << endl;
    }
    
    int getAge() { return age; }
};

// Object instantiation
Dog myDog("Buddy", 3);  // Instance of Dog
myDog.bark();           // Behavior
```

### The Four Pillars of OOP

#### 1. Encapsulation

Bundling data and methods that operate on that data, restricting direct access:
```cpp
class BankAccount {
private:
    double balance;  // Hidden from outside
    
public:
    BankAccount(double initial) : balance(initial) {}
    
    // Controlled access through methods
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
        }
    }
    
    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }
    
    double getBalance() const {
        return balance;
    }
};
```

**Access Modifiers**:
| Modifier | Same Class | Subclass | Outside |
|----------|------------|----------|---------|
| private | ✓ | ✗ | ✗ |
| protected | ✓ | ✓ | ✗ |
| public | ✓ | ✓ | ✓ |

#### 2. Abstraction

Hiding complex implementation details, exposing only essential features:
```cpp
// Abstract class
class Shape {
public:
    virtual double area() = 0;      // Pure virtual (abstract)
    virtual double perimeter() = 0;
    virtual void draw() = 0;
    
    // Concrete method
    void printArea() {
        cout << "Area: " << area() << endl;
    }
};

// Concrete implementation
class Circle : public Shape {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
    double area() override {
        return 3.14159 * radius * radius;
    }
    
    double perimeter() override {
        return 2 * 3.14159 * radius;
    }
    
    void draw() override {
        cout << "Drawing circle with radius " << radius << endl;
    }
};
```

#### 3. Inheritance

Creating new classes based on existing classes, inheriting attributes and methods:
```cpp
// Base class (parent)
class Animal {
protected:
    string name;
    
public:
    Animal(string n) : name(n) {}
    
    virtual void speak() {
        cout << "Some sound" << endl;
    }
    
    void eat() {
        cout << name << " is eating" << endl;
    }
};

// Derived class (child)
class Dog : public Animal {
private:
    string breed;
    
public:
    Dog(string n, string b) : Animal(n), breed(b) {}
    
    void speak() override {
        cout << name << " barks: Woof!" << endl;
    }
    
    void fetch() {
        cout << name << " is fetching" << endl;
    }
};

// Multiple inheritance (C++)
class FlyingFish : public Fish, public Bird {
    // Inherits from both
};
```

**Inheritance Types**:
```
Single:    A → B
Multiple:  A, B → C
Multilevel: A → B → C
Hierarchical: A → B, A → C
Hybrid: Combination of above
```

#### 4. Polymorphism

Objects of different types responding to the same interface:

**Compile-time (Static) Polymorphism**:
```cpp
// Method overloading
class Calculator {
public:
    int add(int a, int b) { return a + b; }
    double add(double a, double b) { return a + b; }
    int add(int a, int b, int c) { return a + b + c; }
};

// Operator overloading
class Vector {
public:
    double x, y;
    
    Vector operator+(const Vector& v) {
        return {x + v.x, y + v.y};
    }
};
```

**Runtime (Dynamic) Polymorphism**:
```cpp
class Animal {
public:
    virtual void speak() = 0;  // Pure virtual
};

class Dog : public Animal {
public:
    void speak() override { cout << "Woof!" << endl; }
};

class Cat : public Animal {
public:
    void speak() override { cout << "Meow!" << endl; }
};

// Polymorphic behavior
void makeAnimalSpeak(Animal* animal) {
    animal->speak();  // Calls appropriate version
}

Dog dog;
Cat cat;
makeAnimalSpeak(&dog);  // "Woof!"
makeAnimalSpeak(&cat);  // "Meow!"
```

### Class Relationships

**Association**: Objects are related but independent
```cpp
class Teacher {
    // Can teach many courses
};
class Course {
    Teacher* instructor;  // Association
};
```

**Aggregation**: "Has-a" relationship (weak ownership)
```cpp
class Department {
    vector<Professor*> professors;  // Professors can exist independently
};
```

**Composition**: "Has-a" relationship (strong ownership)
```cpp
class House {
    Room rooms[10];  // Rooms can't exist without House
};
```

### SOLID Principles

| Principle | Description |
|-----------|-------------|
| **S**ingle Responsibility | A class should have one reason to change |
| **O**pen/Closed | Open for extension, closed for modification |
| **L**iskov Substitution | Subtypes must be substitutable for base types |
| **I**nterface Segregation | Many specific interfaces over one general |
| **D**ependency Inversion | Depend on abstractions, not concretions |

## 📌 Key Points (Optional)
- Encapsulation protects data and provides controlled access
- Abstraction hides complexity behind simple interfaces
- Inheritance enables code reuse and hierarchical relationships
- Polymorphism allows uniform treatment of different object types
- SOLID principles guide good OOP design

## 🖼️ Recommended Image
- UML class diagram showing inheritance hierarchy and relationships

## 🔗 Connections
- [[170-Design Patterns Overview]]
- [[169-Programming Paradigms Overview]]
- [[165-Python Fundamentals]]