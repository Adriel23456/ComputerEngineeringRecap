---
Theme: Stack and Heap Memory Fundamentals
Date created: 2026-01-31 12:00
tags: [KernelLevel, Memory, Stack, Heap]
---

## 📚 Idea/Concept

Understanding stack and heap memory is fundamental to comprehending how programs execute and how the OS manages process memory. These two regions serve different purposes and have different allocation characteristics.

### Process Memory Layout

```
High Address ─────────────────────────────────
              │                              │
              │          STACK               │ ↓ Grows downward
              │    (Local variables,         │
              │     function calls)          │
              │            ↓                 │
              ├──────────────────────────────┤
              │                              │
              │       (Unused space)         │
              │                              │
              ├──────────────────────────────┤
              │            ↑                 │
              │          HEAP                │ ↑ Grows upward
              │    (Dynamic allocation)      │
              │                              │
              ├──────────────────────────────┤
              │          BSS                 │ Uninitialized globals
              ├──────────────────────────────┤
              │          DATA                │ Initialized globals
              ├──────────────────────────────┤
              │          TEXT                │ Program code
Low Address  ─────────────────────────────────
```

### Stack Memory

Characteristics:
- **LIFO** (Last In, First Out) structure
- Automatic allocation/deallocation
- Fast (pointer adjustment only)
- Limited size (typically 1-8 MB)
- Grows downward (toward lower addresses)

What goes on the stack:
```c
void function(int param) {      // param on stack
    int local_var = 10;         // local_var on stack
    char buffer[100];           // buffer on stack
    // Return address also on stack
}
```

Stack frame structure:
```
┌─────────────────────────────────────────────────────────────┐
│                     Stack Frame                              │
├─────────────────────────────────────────────────────────────┤
│  Function parameters (pushed by caller)                     │
├─────────────────────────────────────────────────────────────┤
│  Return address (where to go after function)                │
├─────────────────────────────────────────────────────────────┤
│  Saved frame pointer (previous BP)                          │
├─────────────────────────────────────────────────────────────┤
│  Local variables                                            │
├─────────────────────────────────────────────────────────────┤
│  Saved registers (if needed)                                │
└─────────────────────────────────────────────────────────────┘
         ↑
    Stack Pointer (SP) points here
```

### Heap Memory

Characteristics:
- Dynamic allocation (programmer controlled)
- Slower (allocator algorithms, fragmentation)
- Limited by available memory
- Grows upward (toward higher addresses)
- Must be explicitly freed (or garbage collected)

What goes on the heap:
```c
int *array = malloc(1000 * sizeof(int));  // Heap
char *string = strdup("Hello");            // Heap
struct Node *node = new Node();            // Heap (C++)

// MUST free when done
free(array);
free(string);
delete node;
```

### Stack vs Heap Comparison

| Aspect | Stack | Heap |
|--------|-------|------|
| Allocation | Automatic | Manual (malloc/free) |
| Speed | Very fast | Slower |
| Size | Limited (MB) | Large (GB) |
| Lifetime | Function scope | Until freed |
| Fragmentation | None | Can fragment |
| Thread safety | Each thread has own | Shared, needs sync |
| Overflow | Stack overflow | Out of memory |

### Practical Example: List vs Vector

Why data structure choice affects memory:

```cpp
// VECTOR (Array-based) - Contiguous heap allocation
std::vector<int> vec;
vec.push_back(1);  // Allocates array on heap
vec.push_back(2);  // May reallocate (double size)
vec.push_back(3);  // Contiguous: [1][2][3]

Memory: One heap block, may resize
Access: O(1) random access
Cache: Excellent (contiguous)

// LINKED LIST - Many small heap allocations
std::list<int> lst;
lst.push_back(1);  // Allocate node on heap
lst.push_back(2);  // Allocate another node
lst.push_back(3);  // Another allocation

Memory: Many scattered heap blocks
Access: O(n) to find element
Cache: Poor (nodes scattered)
```

### Connection to OS Memory Management

How stack/heap relate to OS concepts:

```
Process requests memory:
┌─────────────────────────────────────────────────────────────┐
│  Application                                                 │
│    malloc(1000) → C library → brk()/mmap() syscall          │
│                                     ↓                        │
│  Kernel                                                      │
│    Allocate virtual pages → Update page table               │
│                                     ↓                        │
│  Hardware (on access)                                        │
│    Page fault (if not in RAM) → Load from disk/allocate     │
└─────────────────────────────────────────────────────────────┘
```

Stack overflow:
```
void recursive() {
    char big_array[1000000];  // 1MB on stack
    recursive();               // Infinite recursion
}
// Eventually: Stack overflows into heap or guard page
// OS catches it: SIGSEGV (Segmentation Fault)
```

### Buffer Overflow on Stack (Security)

Why stack layout matters for security:
```
Stack layout for vulnerable function:
┌─────────────────────────────────────────────────────────────┐
│  Return Address    │ ← Attacker's target                    │
├────────────────────┤                                        │
│  Saved BP          │                                        │
├────────────────────┤                                        │
│  buffer[64]        │ ← Overflow goes UP toward return addr  │
│    ...             │                                        │
│  buffer[0]         │ ← Input starts here                    │
└─────────────────────────────────────────────────────────────┘

If input > 64 bytes: Overwrites return address!
Attacker can redirect execution to malicious code.
```

### Memory Leaks

Heap memory that is never freed:
```c
void leak() {
    int *ptr = malloc(1000);
    // Oops, forgot to free(ptr)
    return;  // ptr lost, memory leaked
}

// Called millions of times = system runs out of memory
```

OS reclaims ALL process memory when process terminates, but leaks during execution cause problems.

## 📌 Key Points (Optional)
- Stack is automatic and fast; heap is manual and flexible.
- Stack overflow and buffer overflow are different but related vulnerabilities.
- Understanding memory layout is crucial for security and performance.

## 🖼️ Recommended Image
- Process memory layout showing stack growing down, heap growing up.

## 🔗 Connections
- [[140-Virtual Memory and Paging]]
- [[145-Security Fundamentals in Operating Systems]]
- [[127-Process Concept and Program vs Process]]