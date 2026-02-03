---
Theme: A Star Pathfinding Algorithm
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Algorithms, Graphs, Pathfinding, AI]
---

## 📚 Idea/Concept

A* (A-Star) is an informed search algorithm that finds the shortest path between nodes using heuristics to guide its search. By combining the actual cost from the start (like Dijkstra) with an estimated cost to the goal, A* efficiently explores the most promising paths first, making it the standard algorithm for game AI, robotics, and navigation systems.

### A* vs Dijkstra vs BFS

| Algorithm | Strategy | Use Case | Completeness |
|-----------|----------|----------|--------------|
| BFS | Explores level by level | Unweighted graphs | Yes |
| Dijkstra | Explores by lowest cost from start | Weighted, no target | Yes |
| A* | Explores by lowest f(n) = g(n) + h(n) | Weighted, specific target | Yes* |

*With admissible heuristic

### The A* Formula
```
f(n) = g(n) + h(n)

Where:
- f(n): Total estimated cost through node n
- g(n): Actual cost from start to n (known)
- h(n): Heuristic estimate from n to goal (estimated)
```

**Visual Intuition**:
```
Start ════════► Current Node n ─ ─ ─ ─ ─► Goal
      g(n)                        h(n)
      (known)                     (estimated)
      
      └──────────── f(n) ────────────────┘
                (total estimate)
```

### Heuristic Functions

The heuristic h(n) estimates remaining cost. Must be **admissible** (never overestimates) to guarantee optimal path.

**Common Heuristics for Grid Maps**:
```
Manhattan Distance (4-directional movement):
h(n) = |n.x - goal.x| + |n.y - goal.y|

    Goal
      ┌───┐
      │   │
  ┌───┼───┘  Manhattan: 3 + 2 = 5
  │   │
  └───┘
 Start

Euclidean Distance (any direction):
h(n) = √((n.x - goal.x)² + (n.y - goal.y)²)

Diagonal/Chebyshev Distance (8-directional):
h(n) = max(|n.x - goal.x|, |n.y - goal.y|)
```

**Heuristic Properties**:
- **Admissible**: h(n) ≤ actual cost (guarantees optimal)
- **Consistent**: h(n) ≤ cost(n,m) + h(m) (more efficient)
- **h(n) = 0**: A* becomes Dijkstra
- **h(n) = ∞**: A* becomes greedy best-first

### Algorithm Implementation
```python
import heapq

def astar(grid, start, goal):
    # Priority queue: (f_score, node)
    open_set = [(0, start)]
    came_from = {}
    
    g_score = {start: 0}
    f_score = {start: heuristic(start, goal)}
    
    while open_set:
        current = heapq.heappop(open_set)[1]
        
        if current == goal:
            return reconstruct_path(came_from, current)
        
        for neighbor in get_neighbors(grid, current):
            tentative_g = g_score[current] + cost(current, neighbor)
            
            if tentative_g < g_score.get(neighbor, float('inf')):
                # Better path found
                came_from[neighbor] = current
                g_score[neighbor] = tentative_g
                f_score[neighbor] = tentative_g + heuristic(neighbor, goal)
                
                if neighbor not in [n for _, n in open_set]:
                    heapq.heappush(open_set, (f_score[neighbor], neighbor))
    
    return None  # No path exists

def heuristic(a, b):
    # Manhattan distance
    return abs(a[0] - b[0]) + abs(a[1] - b[1])

def reconstruct_path(came_from, current):
    path = [current]
    while current in came_from:
        current = came_from[current]
        path.append(current)
    return path[::-1]
```

### Step-by-Step Example
```
Grid (S=Start, G=Goal, #=Wall):
┌───┬───┬───┬───┬───┐
│ S │   │   │ # │ G │
├───┼───┼───┼───┼───┤
│   │ # │   │ # │   │
├───┼───┼───┼───┼───┤
│   │   │   │   │   │
└───┴───┴───┴───┴───┘
  0   1   2   3   4

Step 1: Start at S(0,0)
  g=0, h=4+0=4, f=4
  Open: [(4, S)]

Step 2: Expand S, add neighbors
  (0,1): g=1, h=3, f=4
  (1,0): g=1, h=4, f=5
  Open: [(4,(0,1)), (5,(1,0))]

Step 3: Expand (0,1) - lowest f
  (0,2): g=2, h=2, f=4
  Open: [(4,(0,2)), (5,(1,0))]

... continues until reaching G(0,4)

Final path: S → (0,1) → (0,2) → (1,2) → (2,2) → (2,3) → (2,4) → (1,4) → G
```

### Optimizations

**Tie-Breaking**:
When f-scores are equal, prefer nodes closer to goal:
```python
# Add small bias toward goal
f_score[n] = g + h * (1 + epsilon)
```

**Bidirectional A***:
Search from both start and goal, meet in middle.

**Jump Point Search (JPS)**:
Skips intermediate nodes in uniform grids.

**Hierarchical A***:
Pre-compute paths between regions for large maps.

### Comparison: When to Use What
```
Need shortest path in unweighted graph?
  → Use BFS

Need shortest path, no specific target?
  → Use Dijkstra

Need shortest path to specific target?
  → Use A* (with good heuristic)

Need any path quickly (not necessarily shortest)?
  → Use Greedy Best-First (h only)
```

### Complexity

| Aspect | Complexity |
|--------|------------|
| Time (worst) | O(b^d) where b=branching, d=depth |
| Time (with good h) | Much better in practice |
| Space | O(b^d) - stores all generated nodes |

## 📌 Key Points (Optional)
- A* combines actual cost g(n) with heuristic estimate h(n)
- Admissible heuristic (never overestimates) guarantees optimal path
- Manhattan distance for grids, Euclidean for continuous space
- A* = Dijkstra when h=0; A* = Greedy when g=0
- Standard algorithm for games, robotics, and GPS navigation

## 🖼️ Recommended Image
- Grid showing A* expansion with f, g, h values at each cell

## 🔗 Connections
- [[177-Dijkstra Shortest Path Algorithm]]
- [[174-Graph Representations and Traversals]]
- [[173-Heap Data Structure]]
- [[179-Heuristic and Metaheuristic Algorithms]]