---
Theme: Python Fundamentals
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Python, Scripting, HighLevel]
---

## 📚 Idea/Concept

Python is a high-level, interpreted, dynamically-typed programming language emphasizing code readability and simplicity. Its design philosophy prioritizes developer productivity with automatic memory management, rich standard library, and support for multiple programming paradigms.

### Python Characteristics

| Feature | Description |
|---------|-------------|
| Interpreted | No compilation step; executed line by line |
| Dynamically typed | Types checked at runtime |
| Garbage collected | Automatic memory management |
| Multi-paradigm | OOP, functional, procedural |
| Indentation-based | Whitespace defines blocks |

### Data Types

**Numeric Types**:
```python
# Integers (arbitrary precision)
x = 42
big = 10**100  # No overflow!

# Floating point
pi = 3.14159
scientific = 6.022e23

# Complex numbers
z = 3 + 4j
z.real  # 3.0
z.imag  # 4.0

# Boolean (subclass of int)
flag = True   # True == 1
done = False  # False == 0
```

**Sequences**:
```python
# String (immutable)
s = "Hello, World!"
s[0]        # 'H'
s[7:12]     # 'World'
s[::-1]     # '!dlroW ,olleH' (reverse)

# List (mutable)
lst = [1, 2, 3, "mixed", True]
lst.append(4)
lst.extend([5, 6])
lst[0] = 100

# Tuple (immutable)
point = (3, 4)
x, y = point  # Unpacking

# Range
for i in range(5):      # 0, 1, 2, 3, 4
    print(i)
```

**Collections**:
```python
# Dictionary (hash map)
person = {"name": "Alice", "age": 30}
person["email"] = "alice@example.com"
person.get("phone", "N/A")  # Default value

# Set (unique elements)
s = {1, 2, 3, 3, 3}  # {1, 2, 3}
s.add(4)
a = {1, 2, 3}
b = {2, 3, 4}
a | b  # Union: {1, 2, 3, 4}
a & b  # Intersection: {2, 3}
a - b  # Difference: {1}
```

### Control Flow
```python
# Conditionals
if x > 0:
    print("positive")
elif x < 0:
    print("negative")
else:
    print("zero")

# Ternary operator
result = "even" if x % 2 == 0 else "odd"

# Loops
for item in collection:
    process(item)

while condition:
    do_something()

# Loop control
for i in range(10):
    if i == 3:
        continue  # Skip to next iteration
    if i == 7:
        break     # Exit loop
else:
    print("Loop completed without break")
```

### Functions
```python
# Basic function
def greet(name):
    """Docstring: describes the function."""
    return f"Hello, {name}!"

# Default arguments
def power(base, exp=2):
    return base ** exp

power(3)      # 9
power(3, 3)   # 27

# *args and **kwargs
def flexible(*args, **kwargs):
    print(f"Positional: {args}")
    print(f"Keyword: {kwargs}")

flexible(1, 2, 3, name="Alice", age=30)
# Positional: (1, 2, 3)
# Keyword: {'name': 'Alice', 'age': 30}

# Lambda functions
square = lambda x: x ** 2
numbers = [1, 2, 3, 4, 5]
squared = list(map(lambda x: x**2, numbers))
```

### List Comprehensions
```python
# Basic comprehension
squares = [x**2 for x in range(10)]

# With condition
evens = [x for x in range(20) if x % 2 == 0]

# Nested comprehension
matrix = [[i*j for j in range(3)] for i in range(3)]
# [[0, 0, 0], [0, 1, 2], [0, 2, 4]]

# Dictionary comprehension
word_lengths = {word: len(word) for word in ["hello", "world"]}
# {'hello': 5, 'world': 5}

# Set comprehension
unique_lengths = {len(word) for word in ["hi", "hello", "hey"]}
# {2, 3, 5}

# Generator expression (lazy evaluation)
gen = (x**2 for x in range(1000000))  # Memory efficient
```

### Object-Oriented Python
```python
class Animal:
    # Class variable
    kingdom = "Animalia"
    
    # Constructor
    def __init__(self, name, age):
        self.name = name      # Instance variable
        self.age = age
        self._protected = 1   # Convention: protected
        self.__private = 2    # Name mangling: _Animal__private
    
    # Instance method
    def speak(self):
        pass
    
    # Class method
    @classmethod
    def get_kingdom(cls):
        return cls.kingdom
    
    # Static method
    @staticmethod
    def is_animal(obj):
        return isinstance(obj, Animal)
    
    # Property
    @property
    def info(self):
        return f"{self.name}, {self.age} years old"


class Dog(Animal):
    def __init__(self, name, age, breed):
        super().__init__(name, age)
        self.breed = breed
    
    def speak(self):
        return "Woof!"
```

### Exception Handling
```python
try:
    result = 10 / 0
except ZeroDivisionError as e:
    print(f"Error: {e}")
except (TypeError, ValueError) as e:
    print(f"Type or Value error: {e}")
else:
    print("No exception occurred")
finally:
    print("Always executes")

# Raising exceptions
def validate_age(age):
    if age < 0:
        raise ValueError("Age cannot be negative")
    return age

# Custom exceptions
class CustomError(Exception):
    pass
```

### Python Memory Model
```python
# Everything is an object
x = 42
id(x)        # Memory address
type(x)      # <class 'int'>

# Mutable vs Immutable
# Immutable: int, float, str, tuple, frozenset
# Mutable: list, dict, set

# Reference semantics
a = [1, 2, 3]
b = a          # b references same list
b.append(4)
print(a)       # [1, 2, 3, 4]

# Shallow vs deep copy
import copy
c = a[:]              # Shallow copy
d = copy.deepcopy(a)  # Deep copy (nested structures)
```

## 📌 Key Points (Optional)
- Python is dynamically typed and garbage collected
- Everything is an object with identity, type, and value
- List comprehensions provide concise iteration patterns
- Mutable objects (list, dict) vs immutable (str, tuple)
- OOP with classes, inheritance, and special methods

## 🖼️ Recommended Image
- Diagram showing Python's object model and reference semantics

## 🔗 Connections
- [[168-Object Oriented Programming Fundamentals]]
- [[169-Programming Paradigms Overview]]
- [[167-Recursion Fundamentals]]
- [[171-Data Structures Overview]]