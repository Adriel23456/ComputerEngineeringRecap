---
Theme: Sorting Algorithms Comparison
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Algorithms, Sorting, Fundamentals]
---

## 📚 Idea/Concept

Sorting arranges elements in a specific order (ascending/descending) and is one of the most fundamental algorithmic problems. Different algorithms trade off time complexity, space complexity, and stability. Understanding when to use each algorithm is essential for efficient software development.

### Complexity Summary

| Algorithm | Best | Average | Worst | Space | Stable |
|-----------|------|---------|-------|-------|--------|
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) | Yes |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) | No |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) | Yes |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Yes |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) | No |
| Heap Sort | O(n log n) | O(n log n) | O(n log n) | O(1) | No |
| Radix Sort | O(nk) | O(nk) | O(nk) | O(n+k) | Yes |

*Stable: preserves relative order of equal elements*

---

### O(n²) Algorithms

#### Bubble Sort

Repeatedly swap adjacent elements if out of order:
```c
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        bool swapped = false;
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(&arr[j], &arr[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;  // Already sorted
    }
}

// [5,3,8,4,2] → [3,5,4,2,8] → [3,4,2,5,8] → [3,2,4,5,8] → [2,3,4,5,8]
```

#### Selection Sort

Find minimum, place at beginning:
```c
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        int minIdx = i;
        for (int j = i+1; j < n; j++) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        swap(&arr[i], &arr[minIdx]);
    }
}

// [5,3,8,4,2] → [2,3,8,4,5] → [2,3,8,4,5] → [2,3,4,8,5] → [2,3,4,5,8]
```

#### Insertion Sort

Build sorted portion by inserting each element:
```c
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

// [5,3,8,4,2] → [3,5,8,4,2] → [3,5,8,4,2] → [3,4,5,8,2] → [2,3,4,5,8]
```

**Best for**: Small arrays, nearly sorted data

---

### O(n log n) Algorithms

#### Merge Sort (Divide and Conquer)

Divide into halves, sort recursively, merge:
```c
void mergeSort(int arr[], int l, int r) {
    if (l >= r) return;
    
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);      // Sort left half
    mergeSort(arr, m+1, r);    // Sort right half
    merge(arr, l, m, r);       // Merge sorted halves
}

void merge(int arr[], int l, int m, int r) {
    // Create temp arrays
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    
    // Copy data
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    
    // Merge back
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    
    // Copy remaining
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}
```

**Guaranteed O(n log n), stable, but O(n) extra space**

#### Quick Sort (Divide and Conquer)

Partition around pivot, sort partitions:
```c
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];  // Choose last as pivot
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Partition example with pivot=4:
// [5,3,8,4,2] → [3,2,8,4,5] → [3,2,4,8,5]
//              i moves when element < pivot
```

**Average O(n log n), in-place, but O(n²) worst case (sorted array)**

#### Heap Sort

Build max-heap, extract maximum repeatedly:
```c
void heapSort(int arr[], int n) {
    // Build max-heap
    for (int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    
    // Extract elements
    for (int i = n-1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}
```

**Guaranteed O(n log n), in-place, but not stable**

---

### When to Use Which

| Scenario | Best Algorithm |
|----------|----------------|
| Small array (n < 50) | Insertion Sort |
| Nearly sorted | Insertion Sort |
| Guaranteed O(n log n) | Merge Sort |
| Average performance, in-place | Quick Sort |
| Memory constrained | Heap Sort |
| Stability required | Merge Sort |
| Integer keys, limited range | Radix/Counting Sort |
| Standard library | Usually Quick Sort variant |

## 📌 Key Points (Optional)
- O(n²) algorithms are simple but slow for large data
- Merge Sort: stable, guaranteed O(n log n), but uses O(n) space
- Quick Sort: fastest in practice, but O(n²) worst case
- Heap Sort: in-place O(n log n), but not stable
- Choose based on size, nearly-sorted status, stability needs, and space constraints

## 🖼️ Recommended Image
- Visual comparison of how each algorithm progresses on the same input

## 🔗 Connections
- [[171-Data Structures Overview]]
- [[173-Heap Data Structure]]
- [[176-Algorithm Complexity Analysis]]
- [[167-Recursion Fundamentals]]