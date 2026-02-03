---
Theme: Page Replacement Algorithms
Date created: 2026-01-31 12:00
tags: [KernelLevel, Memory, Paging, Algorithms]
---

## 📚 Idea/Concept

When physical memory is full and a new page is needed, the OS must choose a victim page to evict. Page replacement algorithms determine which page to remove, balancing the goal of minimizing future page faults.

### The Page Fault Scenario

```
Page fault occurs:
1. Requested page not in RAM (Present = 0)
2. RAM is full (no free frames)
3. Must evict a page to make room
4. Which page to evict? → Replacement algorithm decides

If victim is dirty (Modified = 1):
   Write to disk first (expensive!)
If victim is clean (Modified = 0):
   Just discard (cheap)
```

### Algorithm Overview

| Algorithm | Description | Complexity | Quality |
|-----------|-------------|------------|---------|
| NRU | Not Recently Used (R, M bits) | Simple | Fair |
| FIFO | First In, First Out | Simple | Poor |
| Second Chance | FIFO + Reference bit | Simple | Good |
| Clock | Circular Second Chance | Simple | Good |
| LRU | Least Recently Used | Complex | Excellent |
| Working Set | Pages in recent window | Complex | Excellent |

### NRU (Not Recently Used)

Uses R (Referenced) and M (Modified) bits to classify pages:

```
Class 0: R=0, M=0  ← Best victim (not used, not modified)
Class 1: R=0, M=1  ← Good victim (not used, but modified)
Class 2: R=1, M=0  ← Avoid (recently used)
Class 3: R=1, M=1  ← Worst victim (recently used AND modified)

Algorithm:
1. Select random page from lowest non-empty class
2. Periodically clear all R bits (timer interrupt)
```

Simple and cheap, reasonable performance.

### FIFO (First In, First Out)

```
Evict the page that has been in memory longest.

Page reference: 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
Frames = 3

   Ref: 1  2  3  4  1  2  5  1  2  3  4  5
       ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
    F1 │1 │1 │1 │4 │4 │4 │5 │5 │5 │5 │4 │4 │
       ├──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┤
    F2 │- │2 │2 │2 │1 │1 │1 │1 │1 │3 │3 │3 │
       ├──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┤
    F3 │- │- │3 │3 │3 │2 │2 │2 │2 │2 │2 │5 │
       └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
Fault:  *  *  *  *  *  *  *        *  *  *

Problem: Belady's Anomaly - More frames can cause MORE faults!
```

### Second Chance (Enhanced FIFO)

FIFO + check Reference bit before evicting:

```
Algorithm:
1. Check oldest page (front of FIFO queue)
2. If R = 0: Evict it
3. If R = 1: Set R = 0, move to back of queue, repeat

┌───────────────────────────────────────────────────────────┐
│                                                           │
│   Front ← [Page A, R=1] [Page B, R=0] [Page C, R=1] ← Back│
│                                                           │
│   Check A: R=1 → Clear R, move to back                   │
│   Front ← [Page B, R=0] [Page C, R=1] [Page A, R=0] ← Back│
│                                                           │
│   Check B: R=0 → EVICT B                                 │
└───────────────────────────────────────────────────────────┘
```

If all pages have R=1, algorithm clears all to R=0, then evicts the original oldest (degenerates to FIFO).

### Clock Algorithm

Second Chance with circular list and single pointer:

```
        ┌───────────────────────────┐
        │                           │
        ▼                           │
    ┌───────┐     ┌───────┐     ┌───────┐
    │ Page A│────→│ Page B│────→│ Page C│────┐
    │  R=1  │     │  R=0  │     │  R=1  │    │
    └───────┘     └───────┘     └───────┘    │
        ↑                                     │
        │         "Clock hand"                │
        │              ↓                      │
    ┌───────┐     ┌───────┐     ┌───────┐    │
    │ Page F│←────│ Page E│←────│ Page D│←───┘
    │  R=0  │     │  R=1  │     │  R=0  │
    └───────┘     └───────┘     └───────┘

Algorithm:
1. Check page at hand
2. If R=0: Evict, advance hand
3. If R=1: Clear R, advance hand, repeat
```

Advantage: Single pointer, no queue manipulation.

### LRU (Least Recently Used)

Evict page that hasn't been used for longest time:

```
Optimal in theory (approximates future from past)
But expensive to implement exactly:
- Need timestamp for every access, or
- Need ordered list updated on every access

Hardware approximations:
1. Counter: Each page has counter, incremented on access
2. Matrix: N×N bit matrix for N pages
   - On reference to page i: Set row i to 1, column i to 0
   - Page with smallest row value is LRU
```

Matrix example:
```
Reference page 2:           Reference page 0:
    0 1 2 3                     0 1 2 3
  ┌─────────┐                 ┌─────────┐
0 │ 0 0 0 0 │               0 │ 0 1 1 1 │  ← Row 0 all 1s
1 │ 0 0 0 0 │               1 │ 0 0 0 0 │
2 │ 1 1 0 1 │  ← Row 2      2 │ 0 1 0 1 │
3 │ 0 0 0 0 │               3 │ 0 0 0 0 │
  └─────────┘                 └─────────┘

LRU = row with smallest binary value
```

### Working Set Model

Define working set as pages referenced in last τ time units:

```
W(t, τ) = {pages referenced in interval (t-τ, t)}

Goal: Keep working set in memory
If page not in working set → Candidate for eviction

Example (τ = 10):
Time:     1  2  3  4  5  6  7  8  9  10 11 12
Refs:     a  b  c  a  b  d  a  c  a  b  e  a

At t=12, τ=5:
Working set = {a, b, c, e} (pages in last 5 refs)
```

### WSClock Algorithm

Combines Clock with Working Set:
- Use circular list with clock hand
- Each entry has: R bit, M bit, last-use time
- On page fault, scan for page outside working set and clean

## 📌 Key Points (Optional)
- R and M bits enable decisions without tracking every access.
- Second Chance/Clock provide good performance with simple implementation.
- LRU is theoretically best but expensive; approximations are used in practice.

## 🖼️ Recommended Image
- Clock algorithm diagram showing circular page list with hand pointer.

## 🔗 Connections
- [[140-Virtual Memory and Paging]]
- [[138-Memory Management Fundamentals]]
- [[130-Context Switch and Process Control Block]]
