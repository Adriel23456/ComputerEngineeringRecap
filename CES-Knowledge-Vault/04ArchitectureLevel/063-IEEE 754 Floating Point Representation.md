---
Theme: IEEE 754 Floating Point Representation
Date created: 2026-01-27 12:00
tags: [ArchitectureLayer, DataRepresentation, Fundamentals]
---

## 📚 Idea/Concept

IEEE 754 is the standard for floating-point arithmetic, defining how real numbers are represented in binary, enabling consistent computation across different hardware platforms.

### Single Precision Format (32 bits)

| Bit 31 | Bits 30-23 | Bits 22-0 |
|--------|------------|-----------|
| Sign (1 bit) | Exponent (8 bits) | Fraction/Mantissa (23 bits) |

### Components

1. Sign (1 bit):
   - 0 = positive number
   - 1 = negative number

2. Exponent (8 bits):
   - Biased representation with bias = 127
   - Actual exponent = stored value - 127
   - Range: -126 to +127 (normalized)

3. Fraction (23 bits):
   - Represents mantissa in normalized form
   - Implicit leading 1 (1.fraction)
   - Provides ~7 decimal digits precision

### Value Calculation

For normalized numbers:
$$Value = (-1)^{sign} \times 1.fraction \times 2^{exponent-127}$$

### Conversion Procedure

To convert decimal N to IEEE 754:
1. Determine sign bit (0 if N ≥ 0, 1 if N < 0)
2. Convert |N| to binary normalized form: 1.mantissa × 2^E
3. Calculate stored exponent: E + 127
4. Extract 23 bits after the binary point
5. Assemble: [sign][exponent][mantissa]

### Example: Converting 12.625

1. Binary: 12.625₁₀ = 1100.101₂ = 1.100101 × 2³
2. Sign = 0 (positive)
3. Exponent = 3 + 127 = 130 = 10000010₂
4. Mantissa = 10010100000000000000000

Result: 0 10000010 10010100000000000000000

### Special Values

| Exponent | Fraction | Meaning |
|----------|----------|---------|
| 0 | 0 | Zero (±0) |
| 0 | ≠0 | Denormalized |
| 255 | 0 | Infinity (±∞) |
| 255 | ≠0 | NaN (Not a Number) |

### Double Precision (64 bits)

- Sign: 1 bit
- Exponent: 11 bits (bias = 1023)
- Mantissa: 52 bits
- Precision: ~15-16 decimal digits

## 📌 Key Points (Optional)
- Floating-point operations may introduce rounding errors.
- Comparison of floating-point values requires tolerance (epsilon).

## 🖼️ Recommended Image
- Bit field diagram showing IEEE 754 single and double precision layouts.

## 🔗 Connections
- [[054-ISA Instruction Set Architecture Fundamentals]]
- [[062-Memory Alignment and Data Structure Optimization]]
- [[045-ALU Architecture Operations Flags and Control Signals]]