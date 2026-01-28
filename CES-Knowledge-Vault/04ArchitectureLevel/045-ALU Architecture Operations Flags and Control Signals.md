---
Theme: ALU Architecture Operations Flags and Control Signals
Date created: 2026-01-28 12:00
tags: [ArchitectureLayer, Microarchitecture, Datapath]
---

## 📚 Idea/Concept

The Arithmetic Logic Unit (ALU) is the computational core of the processor, performing arithmetic and logical operations on data, producing results and status flags that influence subsequent execution.

### ALU Basic Structure

```
         ┌───────────────────────────────────┐
   A ───▶│                                   │
         │            ALU                    │───▶ Result
   B ───▶│                                   │
         │                                   │───▶ Flags
         └───────────────────────────────────┘
                        ▲
                        │
                   ALU Control
                   (Operation Select)
```

### ALU Operations

Arithmetic Operations:
| Operation | Function | Example |
|-----------|----------|---------|
| ADD | A + B | Addition |
| SUB | A - B | Subtraction |
| ADDC | A + B + Carry | Multi-word add |
| NEG | -A (two's complement) | Negation |
| INC | A + 1 | Increment |
| DEC | A - 1 | Decrement |

Logical Operations:
| Operation | Function | Example |
|-----------|----------|---------|
| AND | A ∧ B | Bit masking |
| OR | A ∨ B | Bit setting |
| XOR | A ⊕ B | Bit toggling |
| NOT | ¬A | Complement |
| NOR | ¬(A ∨ B) | Combined op |

Shift Operations:
| Operation | Function | Example |
|-----------|----------|---------|
| SLL/SHL | Shift left logical | Multiply by 2 |
| SRL | Shift right logical | Unsigned divide |
| SRA | Shift right arithmetic | Signed divide |
| ROL | Rotate left | Circular shift |
| ROR | Rotate right | Circular shift |

### Status Flags

Common flags in condition code register:

| Flag | Name | Set When |
|------|------|----------|
| Z | Zero | Result = 0 |
| N/S | Negative/Sign | Result < 0 (MSB = 1) |
| C | Carry | Unsigned overflow |
| V/O | Overflow | Signed overflow |
| P | Parity | Even number of 1s |

### Flag Generation Logic

Zero Flag:
$$Z = \overline{R_{n-1} + R_{n-2} + ... + R_1 + R_0}$$
(NOR of all result bits)

Negative Flag:
$$N = R_{n-1}$$
(Most significant bit of result)

Carry Flag (for addition):
$$C = C_{out}$$
(Carry out of MSB position)

Overflow Flag (for signed addition):
$$V = C_{n-1} \oplus C_n$$
(XOR of carry into and out of MSB)

Alternative:
$$V = (A_{n-1} \cdot B_{n-1} \cdot \overline{R_{n-1}}) + (\overline{A_{n-1}} \cdot \overline{B_{n-1}} \cdot R_{n-1})$$

### ALU Control Signals

Typical 4-bit ALU control:
| ALUOp | Operation |
|-------|-----------|
| 0000 | AND |
| 0001 | OR |
| 0010 | ADD |
| 0110 | SUB |
| 0111 | SLT (Set Less Than) |
| 1100 | NOR |

Control signal generation from instruction:
```
Instruction Opcode + Funct → ALU Control → ALUOp
```

### Simple 1-bit ALU Slice

```
       ┌───────────────────────────────────┐
  Ainvert ──▶│    ┌───┐                    │
       A ────┼───▶│MUX├──┐                 │
             │    └───┘  │                 │
  Binvert ──▶│    ┌───┐  │    ┌─────┐     │
       B ────┼───▶│MUX├──┼───▶│     │     │
             │    └───┘  │    │ AND ├──┐   │
             │           ├───▶│     │  │   │
             │           │    └─────┘  │   │
             │           │    ┌─────┐  │   │──▶ Result
             │           ├───▶│ OR  ├──┼──▶│
             │           │    └─────┘  │   │
             │           │    ┌─────┐  │   │
  CarryIn ───┼───────────┼───▶│Adder├──┤   │
             │           │    └──┬──┘  │   │
             │           │       │     │   │
             │           │    CarryOut │   │
       └───────────────────────────────────┘
                   Operation Select (2 bits)
```

### Multi-bit ALU Construction

Ripple-carry connection:
```
┌─────────┐   ┌─────────┐   ┌─────────┐   ┌─────────┐
│ALU Bit 0│──▶│ALU Bit 1│──▶│ALU Bit 2│──▶│ALU Bit 3│
│  C_in   │   │         │   │         │   │  C_out  │
└─────────┘   └─────────┘   └─────────┘   └─────────┘
```

### Carry Lookahead Optimization

Generate (G) and Propagate (P):
$$G_i = A_i \cdot B_i$$
$$P_i = A_i \oplus B_i$$

Carry calculation:
$$C_1 = G_0 + P_0 \cdot C_0$$
$$C_2 = G_1 + P_1 \cdot C_1 = G_1 + P_1 \cdot G_0 + P_1 \cdot P_0 \cdot C_0$$

Reduces carry chain from O(n) to O(log n).

### ALU in MIPS Datapath

```
     Read       Read
     Reg 1      Reg 2
        │          │
        ▼          ▼
    ┌──────────────────┐
    │  Register File   │
    └────┬────────┬────┘
         │        │
         ▼        ▼
       ┌────┐  ┌────┐
       │ A  │  │MUX │◀── Immediate
       └──┬─┘  └─┬──┘
          │      │
          ▼      ▼
        ┌──────────┐
        │   ALU    │───▶ Zero flag (for branch)
        └────┬─────┘
             │
             ▼
          Result
```

### Special ALU Functions

Set Less Than (SLT):
```
if (A < B) Result = 1; else Result = 0;
```
Implementation: Subtract and check sign bit.

Comparison:
- Uses subtraction
- Only flags matter, result discarded
- CMP instruction = SUB without storing result

### Multi-function ALU Example

32-bit ALU supporting:
- All logical operations (AND, OR, XOR, NOR)
- Addition/Subtraction
- Shift operations
- Comparison (SLT)

May use separate shifter unit for complex shifts.

## 📌 Key Points (Optional)
- The ALU is the heart of computation; its speed affects critical path.
- Status flags enable conditional execution and branch decisions.

## 🖼️ Recommended Image
- Schematic of multi-bit ALU with operation select and flag outputs.

## 🔗 Connections
- [[048-Control Unit Architecture]]
- [[065-Pipeline MIPS Five Stage Design]]
- [[047-Datapath Components and Organization]]
- [[054-ISA Instruction Set Architecture Fundamentals]]