---
Theme: Heap Data Structure
Date created: 2026-01-31 12:00
tags: [ProgramLevel, DataStructures, Heap, PriorityQueue]
---

## 📚 Idea/Concept

A heap is a specialized complete binary tree that satisfies the heap property: in a max-heap, every parent is greater than or equal to its children; in a min-heap, every parent is less than or equal to its children. Heaps efficiently support priority queue operations and form the basis of HeapSort.

### Heap Properties

**Max-Heap**: parent ≥ children (root is maximum)
```
         [90]
        /    \
     [80]    [70]
     /  \    /
   [40][50][60]
```

**Min-Heap**: parent ≤ children (root is minimum)
```
         [10]
        /    \
     [20]    [30]
     /  \    /
   [40][50][60]
```

**Complete Binary Tree Property**:
- All levels filled except possibly last
- Last level filled left to right
- Enables efficient array representation

### Array Representation
```
Heap:        [90]
            /    \
         [80]    [70]
         /  \    /
       [40][50][60]

Array: [90, 80, 70, 40, 50, 60]
Index:   0   1   2   3   4   5

// Index relationships (0-based):
Parent(i) = (i - 1) / 2
LeftChild(i) = 2 * i + 1
RightChild(i) = 2 * i + 2

// Example: node at index 1 (value 80)
Parent = (1-1)/2 = 0 (value 90) ✓
LeftChild = 2*1+1 = 3 (value 40) ✓
RightChild = 2*1+2 = 4 (value 50) ✓
```

### Core Operations

**Insert (Push)**: O(log n)
```c
void insert(Heap* h, int key) {
    // Add at end
    h->arr[h->size] = key;
    int i = h->size;
    h->size++;
    
    // Bubble up (heapify up)
    while (i > 0 && h->arr[parent(i)] < h->arr[i]) {
        swap(&h->arr[i], &h->arr[parent(i)]);
        i = parent(i);
    }
}

// Inserting 95 into max-heap:
// [90,80,70,40,50,60] → [90,80,70,40,50,60,95]
//                     → [90,80,95,40,50,60,70] (bubble up)
//                     → [95,80,90,40,50,60,70] (bubble up)
```

**Extract Max/Min (Pop)**: O(log n)
```c
int extractMax(Heap* h) {
    if (h->size == 0) return -1;
    
    int max = h->arr[0];
    
    // Replace root with last element
    h->arr[0] = h->arr[h->size - 1];
    h->size--;
    
    // Bubble down (heapify down)
    heapifyDown(h, 0);
    
    return max;
}

void heapifyDown(Heap* h, int i) {
    int largest = i;
    int left = leftChild(i);
    int right = rightChild(i);
    
    if (left < h->size && h->arr[left] > h->arr[largest])
        largest = left;
    if (right < h->size && h->arr[right] > h->arr[largest])
        largest = right;
    
    if (largest != i) {
        swap(&h->arr[i], &h->arr[largest]);
        heapifyDown(h, largest);
    }
}
```

**Peek**: O(1)
```c
int getMax(Heap* h) {
    return h->arr[0];  // Root is always max/min
}
```

### Build Heap: O(n)

Convert arbitrary array to heap in linear time:
```c
void buildHeap(Heap* h) {
    // Start from last internal node
    for (int i = (h->size / 2) - 1; i >= 0; i--) {
        heapifyDown(h, i);
    }
}

// Why O(n) not O(n log n)?
// Most nodes are near the bottom and require few swaps
// Detailed analysis: Σ (height at level) × (nodes at level) = O(n)
```

### HeapSort: O(n log n)
```c
void heapSort(int arr[], int n) {
    // Build max-heap
    for (int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    
    // Extract elements one by one
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        swap(&arr[0], &arr[i]);
        
        // Heapify reduced heap
        heapify(arr, i, 0);
    }
}

// Steps for [4, 10, 3, 5, 1]:
// Build heap: [10, 5, 3, 4, 1]
// Extract 10: [5, 4, 3, 1 | 10]
// Extract 5:  [4, 1, 3 | 5, 10]
// Extract 4:  [3, 1 | 4, 5, 10]
// Extract 3:  [1 | 3, 4, 5, 10]
// Result:     [1, 3, 4, 5, 10]
```

### Priority Queue

Heap is the standard implementation for priority queues:
```cpp
#include <queue>

// Max priority queue (default)
priority_queue<int> maxPQ;
maxPQ.push(30);
maxPQ.push(10);
maxPQ.push(50);
cout << maxPQ.top();  // 50
maxPQ.pop();

// Min priority queue
priority_queue<int, vector<int>, greater<int>> minPQ;
minPQ.push(30);
minPQ.push(10);
minPQ.push(50);
cout << minPQ.top();  // 10
```

**Applications**:
- Dijkstra's shortest path
- Prim's minimum spanning tree
- Task scheduling
- Huffman encoding
- K largest/smallest elements

### Complexity Summary

| Operation | Time |
|-----------|------|
| Insert | O(log n) |
| Extract Max/Min | O(log n) |
| Peek | O(1) |
| Build Heap | O(n) |
| HeapSort | O(n log n) |

## 📌 Key Points (Optional)
- Heap is a complete binary tree satisfying heap property
- Array representation enables efficient index calculations
- Insert and extract are O(log n) via bubbling
- Build heap is O(n) using bottom-up heapify
- HeapSort is O(n log n) in-place but not stable

## 🖼️ Recommended Image
- Step-by-step diagram of insert and extract operations

## 🔗 Connections
- [[172-Binary Trees and Traversals]]
- [[171-Data Structures Overview]]
- [[175-Sorting Algorithms Comparison]]
- [[177-Dijkstra Shortest Path Algorithm]]