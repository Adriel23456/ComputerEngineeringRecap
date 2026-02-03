---
Theme: Heuristic and Metaheuristic Algorithms
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Algorithms, Optimization, AI, Heuristics]
---

## 📚 Idea/Concept

When exact algorithms are too slow or problems are NP-hard, we turn to heuristic and metaheuristic algorithms. These methods trade guaranteed optimality for practical solutions in reasonable time. Understanding the spectrum from deterministic heuristics to probabilistic and nature-inspired algorithms is essential for tackling real-world optimization problems.

### Algorithm Classification
```
Optimization Algorithms
├── Exact Algorithms
│   └── Guaranteed optimal (but may be slow)
│       Examples: Brute force, Dynamic Programming, Branch & Bound
│
└── Approximate Algorithms
    ├── Heuristics
    │   └── Problem-specific rules of thumb
    │       Examples: Greedy, Nearest Neighbor
    │
    └── Metaheuristics
        ├── Single-Solution Based
        │   └── Simulated Annealing, Tabu Search
        │
        └── Population-Based
            ├── Evolutionary: Genetic Algorithms
            └── Swarm: Particle Swarm, Ant Colony
```

### Heuristic Algorithms

**Definition**: Problem-specific strategies that find good (not necessarily optimal) solutions quickly.

**Greedy Heuristic Example - Traveling Salesman**:
```python
def nearest_neighbor_tsp(cities, start):
    """Always visit the nearest unvisited city"""
    path = [start]
    unvisited = set(cities) - {start}
    current = start
    
    while unvisited:
        nearest = min(unvisited, key=lambda c: distance(current, c))
        path.append(nearest)
        unvisited.remove(nearest)
        current = nearest
    
    return path

# Fast O(n²) but typically 20-25% worse than optimal
```

**Characteristics**:
- Fast execution
- No guarantee of optimality
- Problem-specific design
- Good for initial solutions

---

### Probabilistic (Randomized) Algorithms

**Definition**: Algorithms that use random numbers to make decisions, providing expected good performance.

**Types**:
1. **Las Vegas**: Always correct, random running time
2. **Monte Carlo**: Fixed time, may be incorrect with known probability

**Monte Carlo Example - Estimating π**:
```python
import random

def estimate_pi(num_samples):
    """
    Throw random darts at unit square.
    Ratio in circle ≈ π/4
    """
    inside_circle = 0
    
    for _ in range(num_samples):
        x = random.random()
        y = random.random()
        if x*x + y*y <= 1:
            inside_circle += 1
    
    return 4 * inside_circle / num_samples

# More samples → better estimate
# 1,000 samples: ~3.1
# 1,000,000 samples: ~3.14159
```

**Randomized QuickSort**:
```python
def randomized_partition(arr, low, high):
    # Random pivot avoids worst-case O(n²)
    pivot_idx = random.randint(low, high)
    arr[pivot_idx], arr[high] = arr[high], arr[pivot_idx]
    return partition(arr, low, high)

# Expected O(n log n) regardless of input
```

---

### Genetic Algorithms (GA)

**Definition**: Evolutionary algorithm inspired by natural selection. Solutions evolve through selection, crossover, and mutation.

**Core Concepts**:
```
Individual (Chromosome): One candidate solution
Population: Set of individuals
Gene: Single element of solution encoding
Fitness: Quality measure of solution
Generation: One iteration of the algorithm
```

**Algorithm Structure**:
```python
def genetic_algorithm(population_size, generations):
    # 1. Initialize random population
    population = [random_solution() for _ in range(population_size)]
    
    for gen in range(generations):
        # 2. Evaluate fitness
        fitness_scores = [evaluate(ind) for ind in population]
        
        # 3. Selection (fittest survive)
        parents = select_parents(population, fitness_scores)
        
        # 4. Crossover (combine parents)
        offspring = []
        for i in range(0, len(parents), 2):
            child1, child2 = crossover(parents[i], parents[i+1])
            offspring.extend([child1, child2])
        
        # 5. Mutation (random changes)
        for individual in offspring:
            if random.random() < mutation_rate:
                mutate(individual)
        
        # 6. Replace population
        population = offspring
    
    return best_individual(population)
```

**Example - Binary String Optimization**:
```python
# Goal: Maximize number of 1s in binary string

# Encoding: [1, 0, 1, 1, 0, 1, 0, 0]
# Fitness: count of 1s = 4

# Crossover (single-point):
Parent1: [1, 0, 1, 1 | 0, 1, 0, 0]
Parent2: [0, 1, 0, 0 | 1, 1, 1, 1]
Child1:  [1, 0, 1, 1 | 1, 1, 1, 1]  # Better!
Child2:  [0, 1, 0, 0 | 0, 1, 0, 0]

# Mutation (bit flip):
Before: [1, 0, 1, 1, 1, 1, 1, 1]
After:  [1, 0, 1, 1, 1, 1, 1, 0]  # Random bit flipped
```

**GA Operators**:

| Operator | Purpose | Methods |
|----------|---------|---------|
| Selection | Choose parents | Tournament, Roulette, Rank |
| Crossover | Combine solutions | Single-point, Two-point, Uniform |
| Mutation | Introduce variation | Bit flip, Swap, Insert |

---

### Simulated Annealing

**Definition**: Probabilistic technique inspired by metallurgical annealing. Accepts worse solutions with decreasing probability to escape local optima.
```python
import math
import random

def simulated_annealing(initial, temp, cooling_rate, iterations):
    current = initial
    current_cost = cost(current)
    best = current
    best_cost = current_cost
    
    for i in range(iterations):
        # Generate neighbor
        neighbor = get_neighbor(current)
        neighbor_cost = cost(neighbor)
        
        # Calculate acceptance probability
        delta = neighbor_cost - current_cost
        
        if delta < 0:  # Better solution
            accept = True
        else:  # Worse solution - maybe accept
            probability = math.exp(-delta / temp)
            accept = random.random() < probability
        
        if accept:
            current = neighbor
            current_cost = neighbor_cost
            
            if current_cost < best_cost:
                best = current
                best_cost = best_cost
        
        # Cool down
        temp *= cooling_rate
    
    return best

# High temp → accepts worse solutions (exploration)
# Low temp → only accepts improvements (exploitation)
```

---

### Algorithm Comparison

| Algorithm | Type | Strengths | Weaknesses |
|-----------|------|-----------|------------|
| Greedy | Heuristic | Fast, simple | Often suboptimal |
| Monte Carlo | Probabilistic | Handles uncertainty | Approximate |
| Genetic | Metaheuristic | Global search, parallel | Many parameters |
| Simulated Annealing | Metaheuristic | Escapes local optima | Slow cooling needed |

### When to Use What
```
Problem has clear greedy choice?
  → Try Greedy first (fast baseline)

Need to estimate/sample from distribution?
  → Monte Carlo methods

Large search space, complex constraints?
  → Genetic Algorithm

Continuous optimization, single solution refinement?
  → Simulated Annealing

Combination approach:
  → Use greedy for initial solution
  → Refine with GA or SA
```

## 📌 Key Points (Optional)
- Heuristics: Fast, problem-specific, no optimality guarantee
- Probabilistic: Use randomness for expected good performance
- Genetic Algorithms: Evolve population through selection, crossover, mutation
- Simulated Annealing: Accept worse solutions early to escape local optima
- Metaheuristics are problem-independent frameworks

## 🖼️ Recommended Image
- Flowchart of genetic algorithm with visual chromosome representation

## 🔗 Connections
- [[176-Algorithm Complexity Analysis]]
- [[178-A Star Pathfinding Algorithm]]
- [[175-Sorting Algorithms Comparison]]