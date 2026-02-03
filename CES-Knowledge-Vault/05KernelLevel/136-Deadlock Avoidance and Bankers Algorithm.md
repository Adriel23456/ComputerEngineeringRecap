---
Theme: Deadlock Avoidance and Bankers Algorithm
Date created: 2026-01-31 12:00
tags: [KernelLevel, Deadlock, BankersAlgorithm, Avoidance]
---

## 📚 Idea/Concept

Deadlock avoidance dynamically examines resource allocation state before granting requests. The Banker's Algorithm (Dijkstra, 1965) grants resources only if the system remains in a "safe state."

### Safe State Definition

A state is **safe** if there exists some sequence in which ALL processes can complete, even if they request their maximum resources.

```
Safe State:
  ∃ sequence <P1, P2, ..., Pn> where:
  - For each Pi, resources Pi needs can be satisfied
    by currently available + resources held by Pj (j < i)
  - When Pi finishes, it releases resources for Pi+1

Unsafe State:
  No such sequence exists
  (Does NOT mean deadlock, but deadlock is possible)

┌─────────────────────────────────────────────────────────────┐
│                                                              │
│  Safe States ←────────────────→ Unsafe States               │
│  (No deadlock      can lead to    (Deadlock                │
│   possible)                        possible)                │
│                                                              │
│  ┌─────────────────────────────────────────────────────┐   │
│  │ UNSAFE                                              │   │
│  │    ┌──────────────────────────────────────────┐    │   │
│  │    │           DEADLOCK                        │    │   │
│  │    │        (actually stuck)                   │    │   │
│  │    └──────────────────────────────────────────┘    │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Banker's Algorithm Overview

Named after banker deciding loan approvals:
- Each process declares maximum needs upfront
- OS only grants request if state remains safe
- Check safety by simulating completion

### Algorithm Data Structures

```
n = number of processes
m = number of resource types

Available[m]    - Currently available resources
Max[n][m]       - Maximum demand of each process
Allocation[n][m] - Currently allocated to each process
Need[n][m]      - Remaining need (Max - Allocation)
```

### Safety Algorithm

```
1. Initialize:
   Work[m] = Available[m]
   Finish[n] = {false, false, ..., false}

2. Find process Pi where:
   Finish[i] == false AND Need[i] ≤ Work
   
   If no such Pi exists, go to step 4

3. Simulate Pi completing:
   Work = Work + Allocation[i]
   Finish[i] = true
   Go to step 2

4. If all Finish[i] == true:
   State is SAFE
   Else:
   State is UNSAFE
```

### Resource Request Algorithm

When process Pi requests resources Request[i]:

```
1. If Request[i] > Need[i]:
   ERROR - process exceeded maximum claim
   
2. If Request[i] > Available:
   Pi must wait (resources not available)
   
3. Pretend to allocate:
   Available = Available - Request[i]
   Allocation[i] = Allocation[i] + Request[i]
   Need[i] = Need[i] - Request[i]
   
4. Run Safety Algorithm:
   If safe: Grant the request
   If unsafe: Deny, restore old state, Pi waits
```

### Worked Example

Initial state:
```
          Allocation    Max       Need      Available
          A  B  C      A  B  C   A  B  C    A  B  C
P0        0  1  0      7  5  3   7  4  3    3  3  2
P1        2  0  0      3  2  2   1  2  2
P2        3  0  2      9  0  2   6  0  0
P3        2  1  1      2  2  2   0  1  1
P4        0  0  2      4  3  3   4  3  1
```

Can P1 request (1, 0, 2)?

Step 1: Request ≤ Need? (1,0,2) ≤ (1,2,2)? YES
Step 2: Request ≤ Available? (1,0,2) ≤ (3,3,2)? YES
Step 3: Pretend allocation:
```
Available = (3,3,2) - (1,0,2) = (2,3,0)
Allocation[P1] = (2,0,0) + (1,0,2) = (3,0,2)
Need[P1] = (1,2,2) - (1,0,2) = (0,2,0)
```

Step 4: Safety check - find safe sequence:
- P1 can finish: Need(0,2,0) ≤ Work(2,3,0)
- After P1: Work = (2,3,0) + (3,0,2) = (5,3,2)
- P3 can finish: Need(0,1,1) ≤ Work(5,3,2)
- ...continue until all finish

If sequence exists → GRANT request
If no sequence → DENY request (restore old state)

### Practical Limitations

Why Banker's isn't used in general-purpose OS:
- Processes rarely know maximum needs in advance
- Checking every request is expensive
- Dynamic process creation complicates tracking
- Overly conservative (may deny safe requests)

Best suited for:
- Real-time systems with known workloads
- Embedded systems with fixed processes
- Database transaction systems

## 📌 Key Points (Optional)
- Safe state guarantees no deadlock; unsafe means deadlock is possible.
- Banker's algorithm simulates allocation before committing.
- Practical use is limited to systems with known, fixed resource needs.

## 🖼️ Recommended Image
- State diagram showing safe vs unsafe states with example matrices.

## 🔗 Connections
- [[135-Deadlock Fundamentals and Coffman Conditions]]
- [[137-Deadlock Prevention Strategies]]
- [[147-CPU Scheduling Algorithms]]