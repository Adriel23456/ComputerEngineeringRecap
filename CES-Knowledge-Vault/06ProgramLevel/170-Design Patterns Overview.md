---
Theme: Design Patterns Overview
Date created: 2026-01-31 12:00
tags: [ProgramLevel, OOP, DesignPatterns, Architecture]
---

## 📚 Idea/Concept

Design patterns are reusable solutions to commonly occurring problems in software design. Cataloged by the "Gang of Four" (GoF), they represent best practices evolved over time, providing a shared vocabulary for developers and proven approaches to software architecture challenges.

### Pattern Categories

| Category | Purpose | Examples |
|----------|---------|----------|
| **Creational** | Object creation mechanisms | Singleton, Factory, Builder |
| **Structural** | Class/object composition | Adapter, Facade, Decorator |
| **Behavioral** | Object interaction/responsibility | Observer, Strategy, Command |

---

### Creational Patterns

#### Singleton

Ensures a class has only one instance with global access:
```cpp
class DatabaseConnection {
private:
    static DatabaseConnection* instance;
    
    // Private constructor prevents direct instantiation
    DatabaseConnection() {}
    
public:
    // Deleted copy constructor and assignment
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
    
    static DatabaseConnection* getInstance() {
        if (instance == nullptr) {
            instance = new DatabaseConnection();
        }
        return instance;
    }
    
    void query(string sql) {
        // Execute query
    }
};

DatabaseConnection* DatabaseConnection::instance = nullptr;

// Usage
auto db = DatabaseConnection::getInstance();
db->query("SELECT * FROM users");
```

**Use when**: Exactly one instance needed (config, logging, connection pool)

#### Factory Method

Creates objects without specifying exact class:
```cpp
// Product interface
class Document {
public:
    virtual void open() = 0;
    virtual void save() = 0;
};

// Concrete products
class PDFDocument : public Document {
public:
    void open() override { cout << "Opening PDF" << endl; }
    void save() override { cout << "Saving PDF" << endl; }
};

class WordDocument : public Document {
public:
    void open() override { cout << "Opening Word" << endl; }
    void save() override { cout << "Saving Word" << endl; }
};

// Factory
class DocumentFactory {
public:
    static Document* createDocument(string type) {
        if (type == "pdf") return new PDFDocument();
        if (type == "word") return new WordDocument();
        return nullptr;
    }
};

// Usage
Document* doc = DocumentFactory::createDocument("pdf");
doc->open();
```

**Use when**: Object creation logic should be separate from usage

---

### Structural Patterns

#### Adapter

Converts one interface to another that clients expect:
```cpp
// Existing interface (incompatible)
class LegacyPrinter {
public:
    void printOldFormat(string text) {
        cout << "*** " << text << " ***" << endl;
    }
};

// Target interface (what client expects)
class ModernPrinter {
public:
    virtual void print(string text) = 0;
};

// Adapter bridges the gap
class PrinterAdapter : public ModernPrinter {
private:
    LegacyPrinter* legacyPrinter;
    
public:
    PrinterAdapter(LegacyPrinter* lp) : legacyPrinter(lp) {}
    
    void print(string text) override {
        legacyPrinter->printOldFormat(text);
    }
};

// Usage
LegacyPrinter* old = new LegacyPrinter();
ModernPrinter* printer = new PrinterAdapter(old);
printer->print("Hello");  // Uses legacy printer transparently
```

**Use when**: Integrating incompatible interfaces

#### Facade

Provides simplified interface to complex subsystem:
```cpp
// Complex subsystem classes
class CPU { public: void freeze() {} void execute() {} };
class Memory { public: void load(long addr, char* data) {} };
class HardDrive { public: char* read(long sector, int size) { return nullptr; } };

// Facade provides simple interface
class ComputerFacade {
private:
    CPU cpu;
    Memory memory;
    HardDrive hd;
    
public:
    void start() {
        cpu.freeze();
        memory.load(0, hd.read(0, 1024));
        cpu.execute();
        cout << "Computer started!" << endl;
    }
};

// Usage - client doesn't need to know subsystem details
ComputerFacade computer;
computer.start();
```

**Use when**: Need simple interface to complex system

---

### Behavioral Patterns

#### Observer

Defines one-to-many dependency; when one object changes, all dependents are notified:
```cpp
#include <vector>
#include <algorithm>

// Observer interface
class Observer {
public:
    virtual void update(float temp, float humidity) = 0;
};

// Subject (Observable)
class WeatherStation {
private:
    vector<Observer*> observers;
    float temperature;
    float humidity;
    
public:
    void attach(Observer* obs) {
        observers.push_back(obs);
    }
    
    void detach(Observer* obs) {
        observers.erase(
            remove(observers.begin(), observers.end(), obs),
            observers.end()
        );
    }
    
    void notify() {
        for (Observer* obs : observers) {
            obs->update(temperature, humidity);
        }
    }
    
    void setMeasurements(float temp, float hum) {
        temperature = temp;
        humidity = hum;
        notify();  // Notify all observers
    }
};

// Concrete observers
class PhoneDisplay : public Observer {
public:
    void update(float temp, float humidity) override {
        cout << "Phone: " << temp << "°C, " << humidity << "%" << endl;
    }
};

class WebDisplay : public Observer {
public:
    void update(float temp, float humidity) override {
        cout << "Web: " << temp << "°C, " << humidity << "%" << endl;
    }
};

// Usage
WeatherStation station;
PhoneDisplay phone;
WebDisplay web;

station.attach(&phone);
station.attach(&web);
station.setMeasurements(25.0, 65.0);
// Both displays update automatically
```

**Use when**: Multiple objects need to react to state changes

#### Strategy

Defines family of algorithms, encapsulates each, makes them interchangeable:
```cpp
// Strategy interface
class SortStrategy {
public:
    virtual void sort(vector<int>& data) = 0;
};

// Concrete strategies
class QuickSort : public SortStrategy {
public:
    void sort(vector<int>& data) override {
        cout << "Quick sorting..." << endl;
        // Quick sort implementation
    }
};

class MergeSort : public SortStrategy {
public:
    void sort(vector<int>& data) override {
        cout << "Merge sorting..." << endl;
        // Merge sort implementation
    }
};

// Context
class Sorter {
private:
    SortStrategy* strategy;
    
public:
    void setStrategy(SortStrategy* s) {
        strategy = s;
    }
    
    void performSort(vector<int>& data) {
        strategy->sort(data);
    }
};

// Usage - algorithm can be switched at runtime
Sorter sorter;
vector<int> data = {5, 2, 8, 1, 9};

sorter.setStrategy(new QuickSort());
sorter.performSort(data);

sorter.setStrategy(new MergeSort());
sorter.performSort(data);
```

**Use when**: Need to switch algorithms at runtime

---

### MVC Architecture Pattern

Separates application into Model, View, and Controller:
```
┌─────────────────────────────────────────────────┐
│                    User                          │
│                      │                           │
│                      ▼                           │
│  ┌──────────────────────────────────────────┐   │
│  │              Controller                   │   │
│  │  - Handles user input                     │   │
│  │  - Updates Model                          │   │
│  │  - Selects View                           │   │
│  └─────────────┬────────────────┬───────────┘   │
│                │                │                │
│        Updates │                │ Selects        │
│                ▼                ▼                │
│  ┌─────────────────┐    ┌─────────────────┐     │
│  │     Model       │    │      View       │     │
│  │  - Data/Logic   │───▶│  - Presentation │     │
│  │  - Business     │    │  - User         │     │
│  │    rules        │    │    Interface    │     │
│  └─────────────────┘    └─────────────────┘     │
└─────────────────────────────────────────────────┘
```

## 📌 Key Points (Optional)
- Singleton: One instance with global access
- Factory: Decouple object creation from usage
- Adapter: Bridge incompatible interfaces
- Facade: Simplify complex subsystems
- Observer: Publish-subscribe for state changes
- Strategy: Interchangeable algorithms
- MVC: Separate data, presentation, and control

## 🖼️ Recommended Image
- UML diagrams showing structure of each pattern

## 🔗 Connections
- [[168-Object Oriented Programming Fundamentals]]