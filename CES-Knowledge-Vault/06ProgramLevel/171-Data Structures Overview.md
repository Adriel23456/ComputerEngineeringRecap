---
Theme: Data Structures Overview
Date created: 2026-01-31 12:00
tags: [ProgramLevel, DataStructures, Fundamentals, Algorithms]
---

## 📚 Idea/Concept

Data structures are specialized formats for organizing, storing, and managing data to enable efficient access and modification. Choosing the right data structure is crucial for algorithm efficiency—it can mean the difference between an O(n) and O(log n) solution. This overview covers the fundamental structures that form the building blocks of computer science.

### Classification of Data Structures
```
Data Structures
├── Linear
│   ├── Arrays
│   ├── Linked Lists
│   ├── Stacks
│   ├── Queues
│   └── Hash Tables
└── Non-Linear
    ├── Trees
    │   ├── Binary Trees
    │   ├── BST, AVL, Red-Black
    │   ├── Heaps
    │   └── B-Trees
    └── Graphs
        ├── Directed/Undirected
        └── Weighted/Unweighted
```

### Complexity Comparison

| Structure | Access | Search | Insert | Delete | Space |
|-----------|--------|--------|--------|--------|-------|
| Array | O(1) | O(n) | O(n) | O(n) | O(n) |
| Linked List | O(n) | O(n) | O(1)* | O(1)* | O(n) |
| Stack | O(n) | O(n) | O(1) | O(1) | O(n) |
| Queue | O(n) | O(n) | O(1) | O(1) | O(n) |
| Hash Table | N/A | O(1)† | O(1)† | O(1)† | O(n) |
| BST | O(log n)† | O(log n)† | O(log n)† | O(log n)† | O(n) |
| AVL Tree | O(log n) | O(log n) | O(log n) | O(log n) | O(n) |
| Heap | O(1)‡ | O(n) | O(log n) | O(log n) | O(n) |

*With reference to node; †Average case; ‡For min/max only

### Arrays

Contiguous memory storing elements of same type:
```c
int arr[5] = {10, 20, 30, 40, 50};

// Access: O(1)
int x = arr[2];  // Direct memory calculation: base + index * sizeof(type)

// Insertion at position i: O(n)
// Must shift elements right
for (int j = n; j > i; j--) {
    arr[j] = arr[j-1];
}
arr[i] = newValue;
```

**Dynamic Arrays** (vector in C++, ArrayList in Java):
- Automatically resize when full
- Amortized O(1) insertion at end
- Typically doubles capacity when full

### Linked Lists

Nodes connected by pointers:
```c
struct Node {
    int data;
    struct Node* next;
};

// Singly Linked List
Head → [10|→] → [20|→] → [30|→] → NULL

// Doubly Linked List
NULL ← [←|10|→] ⟷ [←|20|→] ⟷ [←|30|→] → NULL

// Insertion at front: O(1)
newNode->next = head;
head = newNode;

// Deletion (with reference): O(1)
prev->next = current->next;
free(current);
```

### Stack (LIFO)

Last-In-First-Out structure:
```c
// Operations
push(x)   // Add to top: O(1)
pop()     // Remove from top: O(1)
peek()    // View top: O(1)
isEmpty() // Check empty: O(1)

// Applications
- Function call stack
- Expression evaluation
- Undo operations
- Parentheses matching
- DFS traversal

// Parentheses matching example
bool isBalanced(string s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else {
            if (st.empty()) return false;
            char top = st.top(); st.pop();
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{'))
                return false;
        }
    }
    return st.empty();
}
```

### Queue (FIFO)

First-In-First-Out structure:
```c
// Operations
enqueue(x) // Add to rear: O(1)
dequeue()  // Remove from front: O(1)
front()    // View front: O(1)
isEmpty()  // Check empty: O(1)

// Circular queue (array implementation)
front = (front + 1) % capacity;
rear = (rear + 1) % capacity;

// Applications
- BFS traversal
- Task scheduling
- Print queue
- Buffer management

// Priority Queue
- Elements have priorities
- Highest priority dequeued first
- Often implemented with heap
```

### Hash Table

Key-value storage with O(1) average operations:
```
Hash Function: h(key) → index

Key "apple" → h("apple") = 3
Key "banana" → h("banana") = 7

Array:
[0] → NULL
[1] → NULL
[2] → NULL
[3] → ("apple", 5)
[4] → NULL
...
[7] → ("banana", 12)
```

**Collision Resolution**:
- **Chaining**: Each slot holds a linked list
- **Open Addressing**: Probe for next empty slot
  - Linear probing: h(k) + i
  - Quadratic probing: h(k) + i²
  - Double hashing: h₁(k) + i·h₂(k)

### Choosing the Right Structure

| Need | Best Structure |
|------|----------------|
| Fast random access | Array |
| Fast insert/delete at ends | Linked List, Deque |
| LIFO operations | Stack |
| FIFO operations | Queue |
| Fast lookup by key | Hash Table |
| Ordered data + fast search | BST, AVL |
| Fast min/max | Heap |
| Relationships/connections | Graph |

## 📌 Key Points (Optional)
- Arrays: O(1) access, O(n) insert/delete
- Linked Lists: O(1) insert/delete at known position, O(n) access
- Stacks/Queues: O(1) for primary operations
- Hash Tables: O(1) average for all operations
- Choose based on most frequent operations

## 🖼️ Recommended Image
- Visual comparison of all data structures with operation complexities

## 🔗 Connections
- [[172-Binary Trees and Traversals]]
- [[173-Heap Data Structure]]
- [[174-Graph Representations and Traversals]]
- [[176-Algorithm Complexity Analysis]]
- [[175-Sorting Algorithms Comparison]]