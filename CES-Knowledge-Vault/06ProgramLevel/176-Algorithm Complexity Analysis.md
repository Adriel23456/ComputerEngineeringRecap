---
Theme: Algorithm Complexity Analysis
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Algorithms, Complexity, BigO]
---

## 📚 Idea/Concept

Algorithm complexity analysis measures how an algorithm's resource usage (time or space) grows with input size. Big O notation provides a standardized way to describe upper bounds on growth rates, enabling comparison of algorithms independent of hardware. Understanding complexity is essential for choosing efficient algorithms and predicting performance at scale.

### Why Analyze Complexity?
```
Input Size (n)    O(n)        O(n²)         O(2ⁿ)
10               10 ops       100 ops       1,024 ops
100              100 ops      10,000 ops    1.27 × 10³⁰ ops
1,000            1,000 ops    1,000,000 ops Impossible
1,000,000        1M ops       1 trillion    Impossible

At n = 1,000,000:
- O(n): ~1 millisecond
- O(n²): ~12 days
- O(2ⁿ): Longer than universe lifetime
```

### Big O Notation

Describes the upper bound (worst case) of growth rate:

**Formal Definition**: f(n) = O(g(n)) if there exist constants c > 0 and n₀ such that f(n) ≤ c·g(n) for all n ≥ n₀

**Key Rules**:
1. Drop constants: O(2n) = O(n)
2. Drop lower-order terms: O(n² + n) = O(n²)
3. Different inputs = different variables: O(n + m), not O(n)

### Common Complexities
```
O(1)        Constant      Hash table lookup
O(log n)    Logarithmic   Binary search
O(n)        Linear        Array traversal
O(n log n)  Linearithmic  Merge sort
O(n²)       Quadratic     Nested loops
O(n³)       Cubic         Matrix multiplication (naive)
O(2ⁿ)       Exponential   All subsets
O(n!)       Factorial     All permutations
```

**Visual Growth**:
```
     │
     │                                    n!
     │                                 2ⁿ
Time │                              n²
     │                        n log n
     │                    n
     │            log n
     │    1
     └────────────────────────────────────
                    Input Size (n)
```

### Analyzing Code

**Constant - O(1)**:
```c
int getFirst(int arr[]) {
    return arr[0];  // Always one operation
}
```

**Linear - O(n)**:
```c
int sum(int arr[], int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {  // n iterations
        total += arr[i];            // O(1) per iteration
    }
    return total;
}
// Total: O(n)
```

**Quadratic - O(n²)**:
```c
void printPairs(int arr[], int n) {
    for (int i = 0; i < n; i++) {       // n iterations
        for (int j = 0; j < n; j++) {   // n iterations each
            printf("%d,%d ", arr[i], arr[j]);
        }
    }
}
// Total: O(n × n) = O(n²)
```

**Logarithmic - O(log n)**:
```c
int binarySearch(int arr[], int n, int target) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}
// Each iteration halves the search space: n → n/2 → n/4 → ... → 1
// log₂(n) iterations
```

**Linearithmic - O(n log n)**:
```c
void mergeSort(int arr[], int n) {
    if (n <= 1) return;
    // Split: O(1)
    mergeSort(left, n/2);   // T(n/2)
    mergeSort(right, n/2);  // T(n/2)
    merge(arr, n);          // O(n)
}
// Recurrence: T(n) = 2T(n/2) + O(n)
// Solution: O(n log n)
```

### Space Complexity

Measures additional memory used:
```c
// O(1) space - in-place
void reverseInPlace(int arr[], int n) {
    for (int i = 0; i < n/2; i++) {
        swap(&arr[i], &arr[n-1-i]);
    }
}

// O(n) space - creates copy
int* reverseCopy(int arr[], int n) {
    int* result = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        result[i] = arr[n-1-i];
    }
    return result;
}

// Recursion uses stack space
int factorial(int n) {  // O(n) space for call stack
    if (n <= 1) return 1;
    return n * factorial(n-1);
}
```

### Best, Average, Worst Case
```c
// Linear Search
int linearSearch(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

// Best case: O(1) - target at first position
// Average case: O(n/2) = O(n) - target in middle
// Worst case: O(n) - target not present or at end
```

**Quick Sort Example**:
- Best: O(n log n) - balanced partitions
- Average: O(n log n) - random data
- Worst: O(n²) - already sorted, poor pivot choice

### Amortized Analysis

Average time per operation over worst-case sequence:
```c
// Dynamic array append
// Single append: usually O(1), but O(n) when resize needed
// Over n appends: total O(n) work
// Amortized per operation: O(1)

void append(DynamicArray* da, int value) {
    if (da->size == da->capacity) {
        // Resize: double capacity - O(n)
        da->capacity *= 2;
        da->arr = realloc(da->arr, da->capacity * sizeof(int));
    }
    da->arr[da->size++] = value;  // O(1)
}
```

### Common Recurrences

| Recurrence | Solution | Example |
|------------|----------|---------|
| T(n) = T(n/2) + O(1) | O(log n) | Binary search |
| T(n) = T(n-1) + O(1) | O(n) | Linear recursion |
| T(n) = 2T(n/2) + O(1) | O(n) | Tree traversal |
| T(n) = 2T(n/2) + O(n) | O(n log n) | Merge sort |
| T(n) = T(n-1) + O(n) | O(n²) | Selection sort |
| T(n) = 2T(n-1) + O(1) | O(2ⁿ) | Naive Fibonacci |

## 📌 Key Points (Optional)
- Big O describes upper bound growth rate, ignoring constants
- Common classes: O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(2ⁿ)
- Nested loops often multiply: O(n) inside O(n) = O(n²)
- Recursion splits typically yield O(log n) factor
- Space complexity counts additional memory, including call stack

## 🖼️ Recommended Image
- Graph comparing growth rates of common complexity classes

## 🔗 Connections
- [[175-Sorting Algorithms Comparison]]
- [[171-Data Structures Overview]]
- [[167-Recursion Fundamentals]]
- [[172-Binary Trees and Traversals]]