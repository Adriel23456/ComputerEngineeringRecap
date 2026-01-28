---
Theme: Memory Alignment and Data Structure Optimization
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, MemoryAccess, Optimization]
---

## 📚 Idea/Concept

Memory alignment ensures data types start at addresses that are multiples of their size, enabling efficient single-cycle memory access and avoiding performance penalties.

### Alignment Principle
Processors work optimally when each data item begins at an address that is a multiple of its size.
- int (4 bytes): address ≡ 0 (mod 4)
- double (8 bytes): address ≡ 0 (mod 8)

### Unaligned Access Penalties
When data is not properly aligned:
- Multiple memory cycles required
- Additional data manipulation needed
- Significant performance degradation
- Some architectures raise exceptions

### Typical Alignment Requirements (Intel/Linux)

| Data Type | 32-bit | 64-bit |
|-----------|--------|--------|
| char | 1 B | 1 B |
| short | 2 B | 2 B |
| int | 4 B | 4 B |
| long | 8 B | 8 B |
| float | 4 B | 4 B |
| double | 8 B | 8 B |
| long long | 8 B | 8 B |
| long double | 4 B | 16 B |
| pointers | 4 B | 8 B |

### Structure Optimization Example

Original structure (inefficient):
```c
struct {
    char a;     // 1 B
    short a1;   // 2 B
    char b1;    // 1 B
    float b;    // 4 B
    int c;      // 4 B
    char e;     // 1 B
    double f;   // 8 B
};
// Total: 32 bytes (due to padding)
```

Optimized structure (reordered by decreasing size):
```c
struct {
    double f;   // 8 B, aligned to 8
    float b;    // 4 B, aligned to 4
    int c;      // 4 B, aligned to 4
    short a1;   // 2 B, aligned to 2
    char a;     // 1 B
    char b1;    // 1 B
    char e;     // 1 B
};
// Total: 21 bytes (~35% reduction)
```

### Practical Case: RGB Pixel Structure

Original (10 bytes per pixel due to padding):
```c
typedef struct {
    unsigned char red;   // 1 B
    float alpha;         // 4 B (needs alignment)
    unsigned char green; // 1 B
    unsigned char blue;  // 1 B
} rgb_pixel_t;
// 320×240 image = 768,000 bytes
```

Optimized (7 bytes per pixel):
```c
typedef struct {
    float alpha;         // 4 B, aligned to 4
    unsigned char red;   // 1 B
    unsigned char green; // 1 B
    unsigned char blue;  // 1 B
} rgb_pixel_t;
// 320×240 image = 537,600 bytes
```

## 📌 Key Points (Optional)
- Always order structure members by decreasing alignment requirement.
- Cache line utilization improves with properly aligned and packed structures.

## 🖼️ Recommended Image
- Memory layout diagrams showing padding in original vs optimized structures.

## 🔗 Connections
- [[059-Endianness Little Endian vs Big Endian]]
- [[087-Cache Memory Organization and Parameters]]
- [[086-Memory Hierarchy Principles and Locality]]
- [[054-ISA Instruction Set Architecture Fundamentals]]