---
Theme: Interrupt Handling Mechanism
Date created: 2026-01-31 12:00
tags: [KernelLevel, Interrupts, Hardware, OS]
---

## 📚 Idea/Concept

Interrupts are the fundamental mechanism that enables operating systems to function. They allow the CPU to respond to events, switch between user and kernel mode, handle I/O, enforce time-sharing, and manage exceptions. Without interrupts, modern operating systems would be impossible.

### What Is an Interrupt?

An interrupt is a signal that causes the CPU to stop its current execution and transfer control to a special handler routine. It's the mechanism that breaks the sequential execution model.

```
Normal execution:          With interrupt:
┌──────────────────┐      ┌──────────────────┐
│ Instruction 1    │      │ Instruction 1    │
│ Instruction 2    │      │ Instruction 2    │
│ Instruction 3    │      │ ← INTERRUPT      │
│ Instruction 4    │      │    │             │
│ Instruction 5    │      │    ↓             │
│ ...              │      │ [Handler code]   │
└──────────────────┘      │    │             │
                          │    ↓             │
                          │ Instruction 3    │ ← Resume
                          │ Instruction 4    │
                          └──────────────────┘
```

### Classification of Interrupts

```
┌─────────────────────────────────────────────────────────────┐
│                    INTERRUPTS                                │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  BY SOURCE:                                                  │
│  ┌─────────────────────┐  ┌─────────────────────────────┐  │
│  │  EXTERNAL (Hardware)│  │  INTERNAL (Software)        │  │
│  │  - Timer            │  │  - System calls (TRAP/INT)  │  │
│  │  - Keyboard         │  │  - Exceptions               │  │
│  │  - Disk completion  │  │  - Breakpoints              │  │
│  │  - Network packet   │  │                             │  │
│  └─────────────────────┘  └─────────────────────────────┘  │
│                                                              │
│  BY TIMING:                                                  │
│  ┌─────────────────────┐  ┌─────────────────────────────┐  │
│  │  ASYNCHRONOUS       │  │  SYNCHRONOUS                │  │
│  │  - Can occur anytime│  │  - Result of instruction    │  │
│  │  - External events  │  │  - Predictable timing       │  │
│  │  - Timer, I/O       │  │  - Div by zero, syscall     │  │
│  └─────────────────────┘  └─────────────────────────────┘  │
│                                                              │
│  BY MASKABILITY:                                             │
│  ┌─────────────────────┐  ┌─────────────────────────────┐  │
│  │  MASKABLE           │  │  NON-MASKABLE (NMI)         │  │
│  │  - Can be disabled  │  │  - Cannot be ignored        │  │
│  │  - Most I/O devices │  │  - Critical hardware errors │  │
│  │                     │  │  - Memory parity, watchdog  │  │
│  └─────────────────────┘  └─────────────────────────────┘  │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Exceptions vs Interrupts

| Type | Cause | Examples |
|------|-------|----------|
| **Interrupt** | External event, asynchronous | Timer tick, keyboard press, disk ready |
| **Trap** | Intentional, synchronous | System call (INT 0x80, SYSCALL) |
| **Fault** | Recoverable error | Page fault, segment not present |
| **Abort** | Unrecoverable error | Hardware failure, double fault |

### The Interrupt Vector Table (IVT)

Central data structure mapping interrupt numbers to handler addresses:

```
┌─────────────────────────────────────────────────────────────┐
│              Interrupt Vector Table (IVT)                   │
│              (Also called IDT in x86)                       │
├─────────┬───────────────────────────────────────────────────┤
│ Vector  │  Handler Address / Description                    │
├─────────┼───────────────────────────────────────────────────┤
│    0    │  0x00001000 → Divide by Zero Handler              │
│    1    │  0x00001100 → Debug Exception                     │
│    2    │  0x00001200 → NMI (Non-Maskable Interrupt)        │
│    3    │  0x00001300 → Breakpoint                          │
│    4    │  0x00001400 → Overflow                            │
│   ...   │  ...                                              │
│   13    │  0x00002000 → General Protection Fault            │
│   14    │  0x00002100 → Page Fault                          │
│   ...   │  ...                                              │
│   32    │  0x00003000 → Timer Interrupt (IRQ 0)             │
│   33    │  0x00003100 → Keyboard Interrupt (IRQ 1)          │
│   ...   │  ...                                              │
│  128    │  0x00005000 → System Call Handler (Linux)         │
│   ...   │  ...                                              │
└─────────┴───────────────────────────────────────────────────┘

Location: Set up by OS during boot, stored in memory
Access: CPU has special register pointing to IVT (IDTR in x86)
```

### Programmable Interrupt Controller (PIC)

Hardware that manages multiple interrupt sources:

```
┌─────────────────────────────────────────────────────────────┐
│                                                              │
│    Devices              PIC                     CPU         │
│                                                              │
│  ┌─────────┐        ┌─────────┐                            │
│  │  Timer  │───IRQ0─│         │                            │
│  └─────────┘        │         │                            │
│  ┌─────────┐        │         │                            │
│  │Keyboard │───IRQ1─│   PIC   │────INT────→  CPU           │
│  └─────────┘        │         │                            │
│  ┌─────────┐        │         │             │              │
│  │  Disk   │───IRQ14│         │←───INTA─────┘              │
│  └─────────┘        │         │  (Acknowledge)             │
│  ┌─────────┐        └─────────┘                            │
│  │   NIC   │───IRQ11─────┘                                 │
│  └─────────┘                                                │
│                                                              │
│  PIC Functions:                                             │
│  - Prioritize multiple simultaneous interrupts              │
│  - Mask (disable) specific interrupt lines                  │
│  - Signal CPU with single INT line                          │
│  - Provide vector number when acknowledged                  │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

Modern systems use APIC (Advanced PIC) for multi-core support.

### Complete Interrupt Handling Sequence

```
┌─────────────────────────────────────────────────────────────┐
│  HARDWARE PHASE (Automatic by CPU)                          │
├─────────────────────────────────────────────────────────────┤
│  1. Interrupt signal arrives (or exception occurs)          │
│  2. CPU finishes current instruction                        │
│  3. CPU acknowledges interrupt (gets vector number)         │
│  4. CPU saves critical state to stack:                      │
│     - Flags register (including interrupt enable flag)      │
│     - Code Segment (CS)                                     │
│     - Instruction Pointer (IP/EIP/RIP)                      │
│     - (If privilege change: also SS and SP)                 │
│  5. CPU clears interrupt flag (disables further interrupts) │
│  6. CPU switches to kernel mode (if not already)            │
│  7. CPU loads handler address from IVT[vector]              │
│  8. CPU jumps to handler                                    │
├─────────────────────────────────────────────────────────────┤
│  SOFTWARE PHASE (Interrupt Handler Code)                    │
├─────────────────────────────────────────────────────────────┤
│  9. Handler saves additional registers (PUSHA or selective) │
│  10. Handler identifies interrupt source (if shared IRQ)    │
│  11. Handler performs actual work:                          │
│      - Timer: update time, check quantum, maybe reschedule  │
│      - Keyboard: read scancode, buffer keystroke            │
│      - Disk: mark I/O complete, wake waiting process        │
│      - Page fault: load page, update page table             │
│      - Syscall: dispatch to appropriate kernel function     │
│  12. Handler sends EOI (End Of Interrupt) to PIC            │
│  13. Handler restores saved registers (POPA)                │
│  14. Handler executes IRET (Interrupt Return)               │
├─────────────────────────────────────────────────────────────┤
│  RETURN PHASE (IRET instruction)                            │
├─────────────────────────────────────────────────────────────┤
│  15. CPU pops IP, CS, Flags from stack                      │
│  16. CPU restores interrupt flag (re-enables interrupts)    │
│  17. CPU switches back to user mode (if returning to user)  │
│  18. Execution continues from interrupted point             │
└─────────────────────────────────────────────────────────────┘
```

### Timer Interrupt: The Heartbeat of the OS

The timer interrupt is special—it enables preemptive multitasking:

```
┌─────────────────────────────────────────────────────────────┐
│                    Timer Interrupt Flow                      │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  1. Hardware timer generates IRQ0 every N milliseconds      │
│     (typically 1-10ms, configurable)                        │
│                                                              │
│  2. Timer handler executes:                                 │
│     - Increment system time                                 │
│     - Decrement current process quantum                     │
│     - Check if quantum expired                              │
│                                                              │
│  3. If quantum expired:                                     │
│     - Save current process state to its PCB                 │
│     - Call scheduler to select next process                 │
│     - Load next process state from its PCB                  │
│     - Return to NEW process (not original!)                 │
│                                                              │
│  4. If quantum NOT expired:                                 │
│     - Return to same process                                │
│                                                              │
└─────────────────────────────────────────────────────────────┘

This is HOW preemption works!
Without timer interrupt, no process could be forced off CPU.
```

### Interrupt Priority and Nesting

Some interrupts are more urgent than others:

```
Priority levels (example):
┌─────────────────────────────────────────────────────────────┐
│  HIGHEST:  NMI (Non-Maskable) - Hardware failure            │
│            Machine Check      - CPU error                   │
│  HIGH:     Timer              - Scheduling                  │
│  MEDIUM:   Disk               - I/O completion              │
│            Network            - Packet arrival              │
│  LOW:      Keyboard           - User input                  │
│            Mouse              - User input                  │
│  LOWEST:   Software interrupt - System calls                │
└─────────────────────────────────────────────────────────────┘

Nesting: Higher priority interrupt can interrupt lower priority handler
         (if interrupts re-enabled in handler)
```

### Interrupt Latency

Time from interrupt signal to handler execution:

```
Interrupt Latency = 
    Instruction completion time +
    State saving time +
    Mode switch time +
    Handler dispatch time

Typical: 1-10 microseconds (varies by system)

Critical for real-time systems!
```

### Why Interrupts Are Fundamental

Interrupts enable EVERYTHING in modern OS:

| OS Feature | Depends on Interrupt |
|------------|---------------------|
| Preemptive scheduling | Timer interrupt |
| I/O completion | Device interrupts |
| System calls | Software trap/interrupt |
| Exception handling | Fault interrupts |
| Virtual memory | Page fault interrupt |
| User/Kernel boundary | Mode switch via interrupt |

Without interrupts:
- No preemption (processes run forever)
- No I/O notification (must poll everything)
- No protection (can't trap illegal operations)
- No system calls (can't enter kernel safely)

### Division by Zero: Complete Flow

Detailed example combining all concepts:

```
User program executes: int x = 10 / 0;

1. ALU detects division by zero
2. CPU generates exception (vector 0)
3. Hardware saves: FLAGS, CS, EIP to kernel stack
4. CPU enters kernel mode (mode bit = 1)
5. CPU jumps to IVT[0] → divide_by_zero_handler

6. Handler examines exception:
   - Identifies faulting process
   - Determines exception is fatal (cannot recover)
   
7. Handler calls process termination:
   - Sends SIGFPE signal to process
   - Default handler: terminate with core dump
   - Clean up process resources
   - Remove from process table
   
8. Scheduler called to select next process

9. Context switch to next process

10. IRET to next process (not the one that faulted!)
```

## 📌 Key Points (Optional)
- Interrupts are THE mechanism enabling OS functionality—scheduling, I/O, protection all depend on them.
- The IVT/IDT maps interrupt numbers to handler addresses.
- Timer interrupt is the heartbeat that enables preemptive multitasking.
- Hardware saves minimal state; handler saves the rest.

## 🖼️ Recommended Image
- Complete interrupt handling flow diagram showing hardware and software phases.

## 🔗 Connections
- [[122-System Calls and CPU Protection]]
- [[130-Context Switch and Process Control Block]]
- [[147-CPU Scheduling Algorithms]]
- [[142-IO Management and Device Interaction]]
- [[117-CPU Architecture with OS Integration]]