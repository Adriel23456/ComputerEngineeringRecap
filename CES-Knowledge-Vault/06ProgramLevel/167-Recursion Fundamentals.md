---
Theme: Recursion Fundamentals
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Recursion, Algorithms, Fundamentals]
---

## 📚 Idea/Concept

Recursion is a problem-solving technique where a function calls itself to solve smaller instances of the same problem. Every recursive solution requires a base case to stop recursion and a recursive case that reduces the problem toward the base case. Understanding recursion is essential for tree traversals, divide-and-conquer algorithms, and many elegant solutions.

### Anatomy of Recursion

**Essential Components**:
1. **Base Case**: Condition that stops recursion
2. **Recursive Case**: Function calls itself with smaller input
3. **Progress**: Each call must move toward base case
```c
// Classic example: Factorial
int factorial(int n) {
    // Base case
    if (n <= 1) return 1;
    
    // Recursive case (progresses toward base)
    return n * factorial(n - 1);
}

// factorial(4) = 4 * factorial(3)
//              = 4 * 3 * factorial(2)
//              = 4 * 3 * 2 * factorial(1)
//              = 4 * 3 * 2 * 1
//              = 24
```

### How Recursion Uses the Stack
```
Call: factorial(4)

Stack grows:           Stack unwinds:
┌─────────────┐        ┌─────────────┐
│factorial(1) │   →    │  returns 1  │
├─────────────┤        ├─────────────┤
│factorial(2) │   →    │ returns 2*1 │
├─────────────┤        ├─────────────┤
│factorial(3) │   →    │ returns 3*2 │
├─────────────┤        ├─────────────┤
│factorial(4) │   →    │ returns 4*6 │
└─────────────┘        └─────────────┘
```

### Common Recursive Patterns

**Linear Recursion** (one recursive call):
```c
int sum(int n) {
    if (n == 0) return 0;
    return n + sum(n - 1);
}
```

**Binary Recursion** (two recursive calls):
```c
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
```

**Multiple Recursion** (multiple calls):
```c
// Tower of Hanoi
void hanoi(int n, char from, char to, char aux) {
    if (n == 1) {
        printf("Move disk 1 from %c to %c\n", from, to);
        return;
    }
    hanoi(n - 1, from, aux, to);
    printf("Move disk %d from %c to %c\n", n, from, to);
    hanoi(n - 1, aux, to, from);
}
```

### Recursion vs Iteration

| Aspect | Recursion | Iteration |
|--------|-----------|-----------|
| Syntax | Often cleaner | More verbose |
| Memory | Uses call stack | Constant space |
| Speed | Overhead per call | Generally faster |
| Risk | Stack overflow | Infinite loop |
| Best for | Trees, divide-conquer | Simple loops |

**Same problem, both approaches**:
```c
// Recursive
int sumRecursive(int n) {
    if (n == 0) return 0;
    return n + sumRecursive(n - 1);
}

// Iterative
int sumIterative(int n) {
    int total = 0;
    for (int i = 1; i <= n; i++) {
        total += i;
    }
    return total;
}
```

### Tail Recursion

The recursive call is the last operation—can be optimized to iteration:
```c
// NOT tail recursive (must multiply after return)
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);  // Must wait for result
}

// Tail recursive (call is last operation)
int factorialTail(int n, int accumulator) {
    if (n <= 1) return accumulator;
    return factorialTail(n - 1, n * accumulator);  // Nothing after call
}

int factorial(int n) {
    return factorialTail(n, 1);
}
```

**Tail Call Optimization (TCO)**:
Compiler can reuse stack frame, converting to loop:
```c
// Compiler transforms to:
int factorialOptimized(int n) {
    int acc = 1;
    while (n > 1) {
        acc = n * acc;
        n = n - 1;
    }
    return acc;
}
```

### Classic Recursive Algorithms

**Binary Search**:
```c
int binarySearch(int arr[], int low, int high, int target) {
    if (low > high) return -1;  // Base: not found
    
    int mid = low + (high - low) / 2;
    
    if (arr[mid] == target) return mid;  // Base: found
    
    if (arr[mid] > target)
        return binarySearch(arr, low, mid - 1, target);
    else
        return binarySearch(arr, mid + 1, high, target);
}
```

**Merge Sort**:
```c
void mergeSort(int arr[], int left, int right) {
    if (left >= right) return;  // Base: single element
    
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);       // Sort left half
    mergeSort(arr, mid + 1, right);  // Sort right half
    merge(arr, left, mid, right);    // Combine
}
```

**Tree Traversal**:
```c
void inorder(Node *root) {
    if (root == NULL) return;  // Base case
    
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}
```

### Memoization: Optimizing Recursion

Cache results to avoid redundant computation:
```c
// Naive Fibonacci: O(2^n)
int fib(int n) {
    if (n <= 1) return n;
    return fib(n-1) + fib(n-2);  // Recomputes same values
}

// Memoized Fibonacci: O(n)
int memo[100] = {0};
int fibMemo(int n) {
    if (n <= 1) return n;
    if (memo[n] != 0) return memo[n];  // Return cached
    memo[n] = fibMemo(n-1) + fibMemo(n-2);
    return memo[n];
}
```

**Call tree comparison**:
```
Naive fib(5):                    Memoized fib(5):
       fib(5)                          fib(5)
      /      \                        /      \
   fib(4)    fib(3)                fib(4)   [cached]
   /    \    /    \                /    \
 fib(3) fib(2) fib(2) fib(1)    fib(3) [cached]
  ...    ...   ...               /    \
                              fib(2) [cached]
15 calls total                5 unique calls
```

## 📌 Key Points (Optional)
- Every recursion needs a base case and progress toward it
- Recursion uses the call stack—deep recursion risks overflow
- Tail recursion can be optimized to iteration by compilers
- Memoization caches results to avoid redundant recursive calls
- Choose recursion when it's cleaner; iteration when performance matters

## 🖼️ Recommended Image
- Call tree diagram showing recursive calls and stack frames

## 🔗 Connections
- [[164-Call Stack and Heap Memory]]
- [[172-Binary Trees and Traversals]]
- [[175-Sorting Algorithms Comparison]]
- [[176-Algorithm Complexity Analysis]]