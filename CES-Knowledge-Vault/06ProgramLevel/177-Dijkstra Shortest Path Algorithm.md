---
Theme: Dijkstra Shortest Path Algorithm
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Algorithms, Graphs, ShortestPath]
---

## 📚 Idea/Concept

Dijkstra's algorithm finds the shortest path from a source vertex to all other vertices in a weighted graph with non-negative edge weights. It uses a greedy approach, always selecting the vertex with the smallest known distance, and is fundamental to network routing, GPS navigation, and many optimization problems.

### Algorithm Overview

**Key Idea**: Greedily expand from source, always processing the nearest unvisited vertex

**Requirements**:
- Non-negative edge weights (use Bellman-Ford for negative weights)
- Works on directed or undirected graphs

### Algorithm Steps
```
1. Initialize:
   - Set distance[source] = 0
   - Set distance[all others] = ∞
   - Mark all vertices as unvisited
   - Add all vertices to priority queue

2. While unvisited vertices remain:
   - Select vertex u with minimum distance (greedy choice)
   - Mark u as visited
   - For each neighbor v of u:
     - If distance[u] + weight(u,v) < distance[v]:
       - Update distance[v] = distance[u] + weight(u,v)
       - Set predecessor[v] = u

3. Result: distance[] contains shortest distances from source
```

### Implementation
```c
#define INF INT_MAX

typedef struct {
    int vertex;
    int distance;
} HeapNode;

void dijkstra(int graph[][V], int src, int dist[], int pred[]) {
    bool visited[V] = {false};
    
    // Initialize distances
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        pred[i] = -1;
    }
    dist[src] = 0;
    
    // Priority queue (min-heap)
    MinHeap* pq = createMinHeap(V);
    insertHeap(pq, src, 0);
    
    while (!isEmpty(pq)) {
        int u = extractMin(pq);
        
        if (visited[u]) continue;
        visited[u] = true;
        
        // Relax all edges from u
        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !visited[v]) {
                int newDist = dist[u] + graph[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    pred[v] = u;
                    insertHeap(pq, v, newDist);
                }
            }
        }
    }
}
```

### Step-by-Step Example
```
Graph:
       2
    A ───── B
    │ \     │
   1│  \4   │3
    │   \   │
    C ───── D
        5

Source: A

Step 1: Initialize
  dist[A]=0, dist[B]=∞, dist[C]=∞, dist[D]=∞

Step 2: Process A (distance 0)
  Neighbors: B(2), C(1), D(4)
  Update: dist[B]=2, dist[C]=1, dist[D]=4
  
  Current: A=0, B=2, C=1, D=4

Step 3: Process C (distance 1, smallest unvisited)
  Neighbors: D(5)
  dist[C] + 5 = 6 > dist[D]=4, no update
  
Step 4: Process B (distance 2)
  Neighbors: D(3)
  dist[B] + 3 = 5 > dist[D]=4, no update

Step 5: Process D (distance 4)
  No unvisited neighbors

Result:
  A→A: 0
  A→B: 2 (path: A→B)
  A→C: 1 (path: A→C)
  A→D: 4 (path: A→D)
```

### Path Reconstruction
```c
void printPath(int pred[], int dest) {
    if (pred[dest] == -1) {
        printf("%c ", 'A' + dest);
        return;
    }
    printPath(pred, pred[dest]);
    printf("→ %c ", 'A' + dest);
}

// Example: Path to D
// pred[D] = A (direct edge)
// Output: A → D
```

### Complexity Analysis

| Implementation | Time | Space |
|----------------|------|-------|
| Array (linear search) | O(V²) | O(V) |
| Binary Heap | O((V+E) log V) | O(V) |
| Fibonacci Heap | O(E + V log V) | O(V) |

**For sparse graphs (E ≈ V)**: Binary heap is O(V log V)
**For dense graphs (E ≈ V²)**: Array might be faster

### Why It Works

**Greedy Property**: Once we process a vertex u with distance d, we've found the shortest path to u.

**Proof Sketch**: If there were a shorter path, it would go through some unvisited vertex v. But since all edges are non-negative and d ≤ dist[v], this path can't be shorter.

**Why Non-Negative Weights Required**:
```
    A ───1──→ B
    │         │
    2        -5
    │         │
    └→ C ←────┘

With negative edge B→C = -5:
Direct A→C = 2
But A→B→C = 1 + (-5) = -4 (shorter!)

Dijkstra would process C before discovering this path.
```

### Comparison with Other Algorithms

| Algorithm | Use Case | Time | Handles Negative |
|-----------|----------|------|------------------|
| Dijkstra | Single source, non-negative | O((V+E) log V) | No |
| Bellman-Ford | Single source, any weights | O(VE) | Yes |
| Floyd-Warshall | All pairs | O(V³) | Yes (no neg cycles) |
| A* | Single target with heuristic | O(E) best case | No |

## 📌 Key Points (Optional)
- Dijkstra's finds shortest paths from single source in O((V+E) log V)
- Uses greedy approach: always process nearest unvisited vertex
- Requires non-negative edge weights
- Priority queue (min-heap) is crucial for efficiency
- Store predecessors to reconstruct actual paths

## 🖼️ Recommended Image
- Step-by-step diagram showing algorithm progression on example graph

## 🔗 Connections
- [[174-Graph Representations and Traversals]]
- [[173-Heap Data Structure]]
- [[176-Algorithm Complexity Analysis]]
- [[178-A Star Pathfinding Algorithm]]