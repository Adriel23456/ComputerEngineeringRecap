---
Theme: System Calls and CPU Protection
Date created: 2026-01-31 12:00
tags: [KernelLevel, OS, SystemCalls, Security]
---

## 📚 Idea/Concept

System calls are extended operations that the OS offers to user programs, providing the interface between applications and the operating system. CPU protection mechanisms ensure only the kernel can access hardware directly.

### The Protection Problem

Scenario: What if a programmer writes an infinite loop?
- Without protection: CPU monopolized forever
- Solution: Timer interrupts + scheduling
- Timer connected directly to CPU, cannot be disabled by user code

### CPU Protection Mechanisms

1. Mode Bit Protection:
```
Mode Bit = 0 → User Mode (restricted)
Mode Bit = 1 → Kernel Mode (privileged)
```

2. Hardware Timer:
- Generates periodic interrupts
- Forces return to OS periodically
- Enables preemptive scheduling

3. Boundary Registers:
- Define valid memory range per process
- MMU checks all accesses
- Violations trigger exceptions

### System Call Interface

What are system calls:
- Extended operations OS offers to user programs
- Interface between applications and kernel
- When user needs OS service, yield control via trap

Example: `read(fd, buffer, nbytes)`
- fd: file descriptor
- buffer: destination for data
- nbytes: how many bytes to read

### Interrupt Vector Table

The interrupt/system call vector:
- Table identifying operations the OS performs
- Stored in OS memory (RAM)
- Each entry points to handler routine

```
Vector Table:
┌─────────┬───────────────────────────┐
│ Index   │ Handler Address           │
├─────────┼───────────────────────────┤
│    0    │ Division by zero handler  │
│    1    │ Debug exception           │
│   ...   │ ...                       │
│   128   │ System call dispatcher    │
│   ...   │ ...                       │
└─────────┴───────────────────────────┘
```

### Interrupt Classification

By dimension:
- Temporal: When they occur (synchronous/asynchronous)
- By detection: How detected (hardware/software)
- By origin: Where from (internal/external)

### Mode Transition Flow

```
User Mode                    Kernel Mode
    │                            │
    │  1. syscall instruction    │
    ├───────────────────────────→│
    │                            │  2. Save user context
    │                            │  3. Switch to kernel stack
    │                            │  4. Execute handler
    │                            │  5. Restore context
    │  6. return from trap       │
    │←───────────────────────────┤
    │                            │
```

### Security Guarantees

Violation handling:
- User attempting privileged instruction → Exception
- At process level: "Segmentation Fault"
- Preserves system isolation
- No user process can directly access hardware

## 📌 Key Points (Optional)
- Hardware mode bit is the fundamental security boundary.
- Timer prevents infinite loops from monopolizing CPU.
- All hardware interaction must go through system calls.

## 🖼️ Recommended Image
- Flow diagram showing user-to-kernel mode transition during system call.

## 🔗 Connections
- [[123-System Call Macro Steps and Micro Steps]]
- [[117-CPU Architecture with OS Integration]]
- [[148-Interrupt Handling Mechanism]]