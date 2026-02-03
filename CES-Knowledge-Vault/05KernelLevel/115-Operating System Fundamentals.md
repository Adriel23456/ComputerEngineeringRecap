---
Theme: Operating System Fundamentals
Date created: 2026-01-31 12:00
tags: [KernelLevel, OS, Fundamentals]
---

## 📚 Idea/Concept

An Operating System (OS) is a layer of software that acts as an intermediary between hardware and user applications. It serves two primary functions: providing an extended machine abstraction and managing system resources.

### Two Main Functions

1. Extended Machine:
   - Provides simple abstractions of complex hardware
   - Hides details like disk sectors, memory addresses
   - Users work with files, processes, not raw hardware

2. Resource Manager:
   - Manages and allocates system resources in an orderly manner
   - Keeps track of which program uses which resource
   - Resolves conflicts between competing processes

### Operating Modes

The CPU operates in two privilege levels:

| Mode | Access Level | Purpose |
|------|-------------|---------|
| User Mode | Limited | Applications run here, no direct hardware access |
| Kernel Mode | Full | OS operates here with complete hardware access |

Mode transitions occur through interrupts and system calls:
```
User Mode → [Interrupt/Syscall] → Kernel Mode → [Return] → User Mode
```

### Time and Space Multiplexing

Resource sharing mechanisms:

Time Multiplexing:
- Programs take turns using the CPU
- Each process gets a time quantum
- Enables concurrent execution illusion

Space Multiplexing:
- Memory is divided among programs
- Each program occupies a portion of RAM
- Isolation through boundary registers

### Interrupts - The Core Mechanism

Basic interrupt flow:
```
1. Interrupt signal generated
2. CPU saves current state (PC, registers)
3. Switch to kernel mode
4. Execute interrupt handler
5. Restore state and return
```

Types of interrupts:
- Hardware interrupts (timer, I/O devices)
- Software interrupts (system calls, traps)
- Exceptions (division by zero, page faults)

### The OS as True Hardware Client

Important insight: The OS is the real "customer" of the hardware, not the end user. Applications never touch hardware directly; all interaction goes through the kernel.

## 📌 Key Points (Optional)
- The OS abstracts hardware complexity, making development possible without hardware expertise.
- Mode switching is the security boundary between user programs and system resources.

## 🖼️ Recommended Image
- Layered diagram showing Applications → OS Interface → Kernel → Hardware.

## 🔗 Connections
- [[116-Operating System History]]
- [[117-CPU Architecture with OS Integration]]
- [[162-C Language Fundamentals and Data Types]]