---
Theme: C Language Fundamentals and Data Types
Date created: 2026-01-31 12:00
tags: [ProgramLevel, C, DataTypes, Fundamentals]
---

## 📚 Idea/Concept

C is a general-purpose, procedural programming language that provides low-level access to memory while maintaining portability. Understanding C's type system and memory model is fundamental to systems programming and forms the basis for understanding how high-level abstractions map to hardware.

### Primitive Data Types

| Type | Size (typical) | Range | Format Specifier |
|------|----------------|-------|------------------|
| `char` | 1 byte | -128 to 127 | `%c`, `%d` |
| `unsigned char` | 1 byte | 0 to 255 | `%u` |
| `short` | 2 bytes | -32,768 to 32,767 | `%hd` |
| `int` | 4 bytes | -2³¹ to 2³¹-1 | `%d` |
| `unsigned int` | 4 bytes | 0 to 2³²-1 | `%u` |
| `long` | 4/8 bytes | Platform-dependent | `%ld` |
| `long long` | 8 bytes | -2⁶³ to 2⁶³-1 | `%lld` |
| `float` | 4 bytes | ±3.4×10³⁸ | `%f` |
| `double` | 8 bytes | ±1.7×10³⁰⁸ | `%lf` |

### Memory Representation

**Integer Storage** (Two's Complement):
```
Positive 5 (int):  00000000 00000000 00000000 00000101
Negative -5 (int): 11111111 11111111 11111111 11111011

To get -5: Invert bits of 5, then add 1
  5:       00000000 00000000 00000000 00000101
  Invert:  11111111 11111111 11111111 11111010
  Add 1:   11111111 11111111 11111111 11111011 = -5
```

**IEEE 754 Floating Point** (32-bit float):
```
┌───┬──────────┬───────────────────────┐
│ S │ Exponent │       Mantissa        │
│ 1 │  8 bits  │       23 bits         │
└───┴──────────┴───────────────────────┘

Value = (-1)^S × 1.Mantissa × 2^(Exponent-127)

Example: 6.5 = 1.625 × 2²
  S = 0
  Exponent = 2 + 127 = 129 = 10000001
  Mantissa = .625 = .101 (binary)
  
  6.5 = 0 10000001 10100000000000000000000
```

### Variable Declaration and Initialization
```c
// Declaration
int count;              // Uninitialized (contains garbage)
int total = 0;          // Initialized

// Multiple declarations
int a, b, c;
int x = 1, y = 2, z = 3;

// Constants
const int MAX_SIZE = 100;
#define BUFFER_SIZE 1024

// Type qualifiers
volatile int sensor_value;  // May change unexpectedly
static int counter = 0;     // Persists between function calls
extern int global_var;      // Defined elsewhere
```

### Type Casting
```c
// Implicit casting (automatic)
int i = 10;
double d = i;           // int → double (safe)

// Explicit casting
double pi = 3.14159;
int truncated = (int)pi;  // 3 (truncation)

// Pointer casting
int *ip = (int *)malloc(sizeof(int));
void *vp = ip;          // Any pointer to void*
int *ip2 = (int *)vp;   // void* back to specific type

// Dangerous casts
float f = 3.14;
int bits = *(int *)&f;  // Reinterpret float bits as int
```

### Arrays
```c
// Static arrays
int arr[5] = {1, 2, 3, 4, 5};
int zeros[100] = {0};           // All zeros
int partial[5] = {1, 2};        // Rest are 0

// Multi-dimensional arrays
int matrix[3][4];               // 3 rows, 4 columns
int cube[2][3][4];              // 3D array

// Array memory layout (row-major)
int m[2][3] = {{1,2,3}, {4,5,6}};
// Memory: 1, 2, 3, 4, 5, 6 (contiguous)

// Array decay to pointer
void print_array(int *arr, int size);  // Array becomes pointer
print_array(arr, 5);                    // arr decays to &arr[0]
```

### Strings in C
```c
// String literals (null-terminated)
char str1[] = "Hello";          // Array of 6 chars (includes '\0')
char *str2 = "World";           // Pointer to string literal

// String operations (string.h)
#include <string.h>

strlen(str);           // Length (excluding '\0')
strcpy(dest, src);     // Copy string
strncpy(dest, src, n); // Copy at most n chars
strcat(dest, src);     // Concatenate
strcmp(s1, s2);        // Compare (0 if equal)
strchr(str, c);        // Find character
strstr(str, sub);      // Find substring
```

### Structs and Unions
```c
// Structure definition
struct Point {
    int x;
    int y;
};

struct Point p1 = {10, 20};
struct Point p2 = {.y = 30, .x = 40};  // Designated initializers

// Typedef for cleaner syntax
typedef struct {
    char name[50];
    int age;
    float salary;
} Employee;

Employee emp = {"John", 30, 50000.0};

// Union (shared memory)
union Data {
    int i;
    float f;
    char str[4];
};  // Size = max member size

union Data d;
d.i = 42;       // Now d.f and d.str contain garbage
```

### Enumerations
```c
// Basic enum
enum Color { RED, GREEN, BLUE };  // 0, 1, 2

// Enum with explicit values
enum Status {
    SUCCESS = 0,
    ERROR = -1,
    PENDING = 100
};

enum Color c = GREEN;
```

## 📌 Key Points (Optional)
- C provides direct mapping between types and machine representation
- Two's complement for signed integers, IEEE 754 for floats
- Arrays decay to pointers when passed to functions
- Strings are null-terminated character arrays
- Structs group related data; unions share memory

## 🖼️ Recommended Image
- Memory layout diagram showing how different types are stored

## 🔗 Connections
- [[163-Pointers and Memory Management in C]]
- [[164-Call Stack and Heap Memory]]