---
Theme: Call Stack and Heap Memory
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Memory, Stack, Heap, Runtime]
---

## 📚 Idea/Concept

The call stack and heap are two fundamental memory regions used during program execution. The stack manages function calls and local variables with automatic allocation, while the heap provides dynamic memory allocation under programmer control. Understanding their behavior is essential for writing efficient, bug-free code.

### The Call Stack

**Purpose**:
- Store return addresses for function calls
- Allocate local variables
- Pass function arguments
- Maintain execution context

**Stack Frame (Activation Record)**:
```
┌─────────────────────────┐  High Address
│   Arguments passed      │
├─────────────────────────┤
│   Return address        │  ← Where to return after call
├─────────────────────────┤
│   Saved frame pointer   │  ← Previous stack frame
├─────────────────────────┤  ← Current frame pointer (EBP/RBP)
│   Local variables       │
│                         │
├─────────────────────────┤  ← Stack pointer (ESP/RSP)
│   (Next frame here)     │
└─────────────────────────┘  Low Address
```

### Stack in Action
```c
int multiply(int a, int b) {
    int result = a * b;
    return result;
}

int add(int x, int y) {
    int sum = x + y;
    return sum;
}

int main() {
    int a = 5, b = 3;
    int product = multiply(a, b);
    int total = add(product, 10);
    return 0;
}
```

**Stack Evolution**:
```
Step 1: main() called
┌─────────────────┐
│ main's frame    │
│  a = 5          │
│  b = 3          │
│  product = ?    │
│  total = ?      │
└─────────────────┘

Step 2: multiply(5, 3) called
┌─────────────────┐
│ multiply's frame│
│  a = 5          │
│  b = 3          │
│  result = 15    │
├─────────────────┤
│ main's frame    │
│  ...            │
└─────────────────┘

Step 3: multiply returns, add(15, 10) called
┌─────────────────┐
│ add's frame     │
│  x = 15         │
│  y = 10         │
│  sum = 25       │
├─────────────────┤
│ main's frame    │
│  product = 15   │
│  ...            │
└─────────────────┘
```

### Recursion and the Stack
```c
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

factorial(4);
```

**Stack During Recursion**:
```
┌──────────────────┐
│ factorial(1)     │  ← Returns 1
│   n = 1          │
├──────────────────┤
│ factorial(2)     │  ← Returns 2 * 1 = 2
│   n = 2          │
├──────────────────┤
│ factorial(3)     │  ← Returns 3 * 2 = 6
│   n = 3          │
├──────────────────┤
│ factorial(4)     │  ← Returns 4 * 6 = 24
│   n = 4          │
├──────────────────┤
│ main()           │
└──────────────────┘
```

**Stack Overflow**:
```c
void infinite_recursion() {
    infinite_recursion();  // Stack grows until exhausted
}
// Results in: Stack Overflow / Segmentation Fault
```

### The Heap

**Characteristics**:
- Manually managed memory
- Allocate with malloc/calloc/realloc
- Deallocate with free
- Persists until explicitly freed
- Larger than stack (limited by system memory)
- Slower allocation than stack
```c
// Heap allocation
int *arr = (int *)malloc(1000 * sizeof(int));

// Use the memory
for (int i = 0; i < 1000; i++) {
    arr[i] = i;
}

// Must manually free
free(arr);
```

### Stack vs Heap Comparison

| Aspect | Stack | Heap |
|--------|-------|------|
| Allocation | Automatic | Manual (malloc/free) |
| Speed | Very fast | Slower |
| Size | Limited (1-8 MB typical) | Large (system memory) |
| Scope | Function lifetime | Until freed |
| Fragmentation | None | Can fragment |
| Access | LIFO only | Random access |
| Thread safety | Each thread has own | Shared, needs sync |
| Overflow | Stack overflow | Out of memory |

### When to Use Each

**Use Stack**:
- Small, fixed-size data
- Short-lived variables
- Function-local data
- Performance-critical allocation

**Use Heap**:
- Large data structures
- Data that outlives function
- Size unknown at compile time
- Data shared between functions
```c
// Stack: Small, known size
void process() {
    int buffer[100];      // Stack allocation
    // ...
}  // Automatically deallocated

// Heap: Large or dynamic size
int* create_large_array(int size) {
    int *arr = malloc(size * sizeof(int));  // Heap
    return arr;  // Caller must free!
}
```

### Memory Allocation Internals

**Heap Allocator Responsibilities**:
1. Track free and allocated blocks
2. Find suitable block for request
3. Split blocks if too large
4. Coalesce free blocks to reduce fragmentation

**Common Allocator Strategies**:
- First Fit: Use first block that fits
- Best Fit: Use smallest block that fits
- Buddy System: Power-of-2 block sizes

**Fragmentation**:
```
External Fragmentation:
┌────┬────────┬────┬────────┬────┐
│Used│  Free  │Used│  Free  │Used│
└────┴────────┴────┴────────┴────┘
      ← Can't allocate large block despite free space

Internal Fragmentation:
┌──────────────────────┐
│ Requested │ Padding  │  ← Wasted space in block
└──────────────────────┘
```

### Stack-Based vs Heap-Based Languages

**Stack-Oriented** (C, C++):
- Programmer manages heap
- Maximum control
- Maximum responsibility

**Heap-Oriented with GC** (Java, Python, Go):
- Automatic garbage collection
- Less control
- Fewer memory bugs

## 📌 Key Points (Optional)
- Stack: automatic, fast, LIFO, limited size, function-scoped
- Heap: manual, slower, random access, large, programmer-managed
- Each function call creates a stack frame with locals and return address
- Deep recursion can cause stack overflow
- Heap fragmentation can reduce effective memory

## 🖼️ Recommended Image
- Side-by-side diagram of stack frames during function calls and heap allocations

## 🔗 Connections
- [[163-Pointers and Memory Management in C]]
- [[167-Recursion Fundamentals]]
- [[171-Data Structures Overview]]