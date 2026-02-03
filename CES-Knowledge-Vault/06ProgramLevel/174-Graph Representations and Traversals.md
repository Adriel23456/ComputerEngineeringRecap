---
Theme: Graph Representations and Traversals
Date created: 2026-01-31 12:00
tags: [ProgramLevel, DataStructures, Graphs, Algorithms]
---

## 📚 Idea/Concept

A graph is a non-linear data structure consisting of vertices (nodes) connected by edges. Graphs model relationships and connections, making them fundamental for network analysis, social networks, routing, and countless optimization problems. Understanding graph representations and traversals is essential for graph algorithms.

### Graph Terminology
```
Graph G = (V, E)
V = {A, B, C, D, E}  // Vertices
E = {(A,B), (A,C), (B,D), (C,D), (D,E)}  // Edges

     A ─── B
     │     │
     C ─── D ─── E

- Vertex (Node): Entity in graph
- Edge: Connection between vertices
- Adjacent: Directly connected vertices
- Degree: Number of edges connected to vertex
- Path: Sequence of vertices connected by edges
- Cycle: Path that starts and ends at same vertex
- Connected: Path exists between every pair of vertices
```

### Graph Types

**Directed vs Undirected**:
```
Undirected:          Directed (Digraph):
  A ─── B              A ──→ B
  │     │              ↓     │
  C ─── D              C ──→ D
(A-B means A↔B)      (A→B, not B→A)
```

**Weighted vs Unweighted**:
```
Unweighted:          Weighted:
  A ─── B              A ─5─ B
  │     │              │     │
  C ─── D              3     2
                       │     │
                       C ─4─ D
```

**Special Types**:
- **Tree**: Connected acyclic graph
- **DAG**: Directed Acyclic Graph
- **Complete**: Every pair connected
- **Bipartite**: Vertices divisible into two disjoint sets

### Representations

#### Adjacency Matrix
```c
// For graph with n vertices
int adj[n][n];

// adj[i][j] = 1 if edge exists, 0 otherwise
// For weighted: adj[i][j] = weight

// Example:
//   A B C D
// A 0 1 1 0
// B 1 0 0 1
// C 1 0 0 1
// D 0 1 1 0

Space: O(V²)
Edge lookup: O(1)
All edges of vertex: O(V)
Add edge: O(1)
```

#### Adjacency List
```c
// Array of linked lists
struct Node {
    int vertex;
    int weight;  // For weighted graphs
    struct Node* next;
};
Node* adj[n];

// Example:
// A → B → C → NULL
// B → A → D → NULL
// C → A → D → NULL
// D → B → C → NULL

Space: O(V + E)
Edge lookup: O(degree)
All edges of vertex: O(degree)
Add edge: O(1)
```

**Comparison**:
| Aspect | Matrix | List |
|--------|--------|------|
| Space | O(V²) | O(V+E) |
| Edge lookup | O(1) | O(V) |
| Iterate neighbors | O(V) | O(degree) |
| Best for | Dense graphs | Sparse graphs |

### Breadth-First Search (BFS)

Explores vertices level by level using a queue:
```c
void BFS(int start) {
    bool visited[V] = {false};
    Queue q;
    
    visited[start] = true;
    enqueue(q, start);
    
    while (!isEmpty(q)) {
        int v = dequeue(q);
        printf("%d ", v);
        
        for (each neighbor u of v) {
            if (!visited[u]) {
                visited[u] = true;
                enqueue(q, u);
            }
        }
    }
}

// Example traversal from A:
//     A ─── B           Level 0: A
//     │     │           Level 1: B, C
//     C ─── D ─── E     Level 2: D
//                       Level 3: E
// BFS order: A, B, C, D, E
```

**Applications**:
- Shortest path (unweighted)
- Level-order traversal
- Finding connected components
- Testing bipartiteness

**Complexity**: O(V + E)

### Depth-First Search (DFS)

Explores as deep as possible before backtracking:
```c
void DFS(int v, bool visited[]) {
    visited[v] = true;
    printf("%d ", v);
    
    for (each neighbor u of v) {
        if (!visited[u]) {
            DFS(u, visited);
        }
    }
}

// Iterative version using stack
void DFSIterative(int start) {
    bool visited[V] = {false};
    Stack s;
    
    push(s, start);
    
    while (!isEmpty(s)) {
        int v = pop(s);
        
        if (!visited[v]) {
            visited[v] = true;
            printf("%d ", v);
            
            for (each neighbor u of v) {
                push(s, u);
            }
        }
    }
}

// DFS order (from A): A, B, D, C, E (depends on neighbor order)
```

**Applications**:
- Topological sorting
- Finding strongly connected components
- Cycle detection
- Path finding
- Maze solving

**Complexity**: O(V + E)

### BFS vs DFS

| Aspect | BFS | DFS |
|--------|-----|-----|
| Data structure | Queue | Stack/Recursion |
| Order | Level by level | Deep then backtrack |
| Shortest path | Yes (unweighted) | No |
| Memory | O(V) at worst | O(V) recursion depth |
| Best for | Shortest path, levels | Cycles, topological sort |

### Topological Sort (DAG)

Linear ordering where for every edge u→v, u comes before v:
```c
// Using DFS
void topologicalSort() {
    Stack result;
    bool visited[V] = {false};
    
    for (each vertex v) {
        if (!visited[v])
            topoDFS(v, visited, result);
    }
    
    // Pop stack for order
    while (!isEmpty(result))
        printf("%d ", pop(result));
}

void topoDFS(int v, bool visited[], Stack result) {
    visited[v] = true;
    
    for (each neighbor u of v) {
        if (!visited[u])
            topoDFS(u, visited, result);
    }
    
    push(result, v);  // Add after all descendants
}

// Example DAG:
// 5 → 2 → 3 → 1
// ↓       ↑
// 0 ──────┘
// One valid order: 5, 0, 2, 3, 1
```

## 📌 Key Points (Optional)
- Graphs model relationships with vertices and edges
- Adjacency matrix: O(1) lookup, O(V²) space—good for dense graphs
- Adjacency list: O(V+E) space—good for sparse graphs
- BFS: level-order, finds shortest path in unweighted graphs
- DFS: depth-first, used for topological sort, cycle detection

## 🖼️ Recommended Image
- Side-by-side comparison of BFS and DFS traversal on the same graph

## 🔗 Connections
- [[171-Data Structures Overview]]
- [[177-Dijkstra Shortest Path Algorithm]]
- [[176-Algorithm Complexity Analysis]]