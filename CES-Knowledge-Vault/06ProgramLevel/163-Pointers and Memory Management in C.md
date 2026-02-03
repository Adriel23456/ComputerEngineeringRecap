---
Theme: Pointers and Memory Management in C
Date created: 2026-01-31 12:00
tags: [ProgramLevel, C, Pointers, Memory]
---

## 📚 Idea/Concept

Pointers are variables that store memory addresses. They provide direct memory access, enable dynamic memory allocation, and are essential for implementing data structures. Understanding pointers is crucial for systems programming and debugging memory-related issues.

### Pointer Basics
```c
int x = 42;
int *ptr = &x;      // ptr holds address of x

// Operators
&x                  // Address-of: get address of x
*ptr                // Dereference: get value at address

printf("Address: %p\n", (void *)ptr);  // e.g., 0x7ffd5e8c3a4c
printf("Value: %d\n", *ptr);           // 42

*ptr = 100;         // Modify x through pointer
printf("x = %d\n", x);  // 100
```

### Pointer Arithmetic
```c
int arr[] = {10, 20, 30, 40, 50};
int *p = arr;       // Points to arr[0]

// Arithmetic moves by sizeof(type)
p++;                // Now points to arr[1]
p += 2;             // Now points to arr[3]

// Pointer difference
int *start = arr;
int *end = &arr[4];
ptrdiff_t diff = end - start;  // 4 (elements, not bytes)

// Array indexing is pointer arithmetic
arr[i]  ≡  *(arr + i)
&arr[i] ≡  arr + i
```

**Memory Visualization**:
```
Address:    0x100  0x104  0x108  0x10C  0x110
            ┌──────┬──────┬──────┬──────┬──────┐
arr:        │  10  │  20  │  30  │  40  │  50  │
            └──────┴──────┴──────┴──────┴──────┘
              ↑             ↑
              p            p+2
```

### Pointers and Arrays
```c
// Array name is pointer to first element
int arr[5] = {1, 2, 3, 4, 5};
int *p = arr;       // Equivalent to: int *p = &arr[0];

// But arrays and pointers are NOT identical
sizeof(arr)         // 20 (5 * 4 bytes)
sizeof(p)           // 8 (pointer size on 64-bit)

// Passing arrays to functions
void process(int *arr, int size);   // Array decays to pointer
void process(int arr[], int size);  // Same as above
void process(int arr[10], int size); // Size ignored, still pointer
```

### Pointer to Pointer
```c
int x = 5;
int *p = &x;
int **pp = &p;      // Pointer to pointer

printf("%d\n", **pp);  // 5

// Useful for:
// 1. Modifying pointer in function
void allocate(int **ptr) {
    *ptr = malloc(sizeof(int));
}
int *p = NULL;
allocate(&p);  // Now p points to allocated memory

// 2. Array of strings
char *names[] = {"Alice", "Bob", "Charlie"};
char **name_ptr = names;
```

### Dynamic Memory Allocation
```c
#include <stdlib.h>

// malloc: allocate uninitialized memory
int *p = (int *)malloc(10 * sizeof(int));
if (p == NULL) {
    // Allocation failed
}

// calloc: allocate and zero-initialize
int *arr = (int *)calloc(10, sizeof(int));

// realloc: resize allocation
arr = (int *)realloc(arr, 20 * sizeof(int));

// free: deallocate memory
free(p);
p = NULL;  // Good practice: avoid dangling pointer
```

### Memory Layout
```
High Address
┌─────────────────────┐
│   Command-line      │
│   args, environment │
├─────────────────────┤
│       Stack         │  ← Local variables, return addresses
│         ↓           │    Grows downward
│                     │
│         ↑           │
│        Heap         │  ← Dynamic allocation (malloc)
├─────────────────────┤    Grows upward
│        BSS          │  ← Uninitialized global/static
├─────────────────────┤
│       Data          │  ← Initialized global/static
├─────────────────────┤
│       Text          │  ← Program code (read-only)
└─────────────────────┘
Low Address
```

### Common Pointer Errors

**Dangling Pointer**:
```c
int *p = malloc(sizeof(int));
free(p);
*p = 10;    // ERROR: accessing freed memory
```

**Memory Leak**:
```c
void leak() {
    int *p = malloc(100);
    return;  // Memory never freed!
}
```

**Buffer Overflow**:
```c
int arr[5];
arr[10] = 42;  // Writing beyond array bounds
```

**Null Pointer Dereference**:
```c
int *p = NULL;
*p = 10;    // CRASH: segmentation fault
```

**Double Free**:
```c
int *p = malloc(sizeof(int));
free(p);
free(p);    // ERROR: undefined behavior
```

### Function Pointers
```c
// Declaration
int (*func_ptr)(int, int);

// Assignment
int add(int a, int b) { return a + b; }
func_ptr = add;  // or: func_ptr = &add;

// Call
int result = func_ptr(3, 4);  // 7

// Use case: callback
void sort(int *arr, int n, int (*compare)(int, int));

// Array of function pointers
int (*operations[4])(int, int) = {add, subtract, multiply, divide};
result = operations[0](5, 3);  // add(5, 3)
```

### Const and Pointers
```c
int x = 10, y = 20;

// Pointer to constant (can't modify value through pointer)
const int *p1 = &x;
*p1 = 30;           // ERROR
p1 = &y;            // OK

// Constant pointer (can't change what it points to)
int *const p2 = &x;
*p2 = 30;           // OK
p2 = &y;            // ERROR

// Constant pointer to constant
const int *const p3 = &x;
*p3 = 30;           // ERROR
p3 = &y;            // ERROR

// Read right-to-left: "p3 is a const pointer to const int"
```

## 📌 Key Points (Optional)
- Pointers store memory addresses; use & to get address, * to dereference
- Pointer arithmetic scales by the size of the pointed-to type
- Dynamic memory (malloc/free) lives on the heap
- Always check for NULL after allocation and free memory when done
- Common bugs: dangling pointers, memory leaks, buffer overflows

## 🖼️ Recommended Image
- Memory diagram showing stack, heap, and pointer relationships

## 🔗 Connections
- [[162-C Language Fundamentals and Data Types]]
- [[164-Call Stack and Heap Memory]]
- [[171-Data Structures Overview]]