---
Theme: CPU Architecture with OS Integration
Date created: 2026-01-31 12:00
tags: [KernelLevel, CPU, Hardware]
---

## 📚 Idea/Concept

The CPU must include specific hardware features to support operating system functions, particularly for protection, interrupt handling, and multiprogramming guarantees.

### Essential CPU Features for OS Support

1. Mode Bit (Privilege Bit):
   - Single bit indicating User (0) or Kernel (1) mode
   - Switched by software during syscall/trap
   - Prevents user code from executing privileged instructions

2. Timer:
   - Hardware timer connected directly to CPU
   - Generates periodic interrupts
   - Prevents infinite loops from monopolizing CPU
   - Enables preemptive scheduling

3. Boundary Registers:
   - Mark start and end of each program's memory region
   - MMU checks all memory accesses
   - Violations trigger protection faults

### CPU Diagram with OS Components

```
┌─────────────────────────────────────────────────────────────┐
│                         CPU                                  │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐ │
│  │   ALU       │  │  Registers  │  │   Control Unit      │ │
│  │             │  │  - PC       │  │   - Mode Bit        │ │
│  │             │  │  - SP       │  │   - Instruction     │ │
│  │             │  │  - GPRs     │  │     Decoder         │ │
│  └─────────────┘  └─────────────┘  └─────────────────────┘ │
│                                                              │
│  ┌─────────────────────────────────────────────────────────┐│
│  │              Memory Management Unit (MMU)                ││
│  │  ┌──────────────┐  ┌──────────────┐  ┌───────────────┐ ││
│  │  │Base Register │  │Limit Register│  │ TLB Cache     │ ││
│  │  └──────────────┘  └──────────────┘  └───────────────┘ ││
│  └─────────────────────────────────────────────────────────┘│
│                                                              │
│  ┌─────────────┐  ┌─────────────────────────────────────┐  │
│  │   Timer     │  │     Interrupt Controller            │  │
│  │  (Hardware) │  │  - Vector Table Pointer             │  │
│  └─────────────┘  └─────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                              ↓
              ┌───────────────────────────────┐
              │      System Bus               │
              └───────────────────────────────┘
                    ↓         ↓         ↓
                Memory      I/O      Devices
```

### Multiprogramming Hardware Guarantee

The first guarantee of multiprogramming:
- Make each program believe it is alone on the computer
- Initial instructions (managed by OS) set memory limits
- Boundary registers map start and end of each program

### Division by Zero Example

When a user program divides by zero:
```
1. ALU performs division → detects divide-by-zero
2. Hardware raises exception (not timer interrupt)
3. Mode bit switches to Kernel (1)
4. CPU jumps to exception handler address
5. OS identifies exception type
6. OS terminates offending process (fatal error)
7. Scheduler selects next process
8. Mode bit returns to User (0)
```

### Protection Violations

When user code attempts privileged instruction:
```
1. Instruction decoder detects privileged opcode
2. Mode bit is 0 (User mode)
3. Hardware triggers protection fault
4. Similar handling to exceptions
5. Result: "Segmentation Fault" termination
```

## 📌 Key Points (Optional)
- Hardware support is essential; the OS cannot protect without CPU cooperation.
- The timer prevents any single process from monopolizing the CPU.

## 🖼️ Recommended Image
- Detailed CPU block diagram showing mode bit, timer, MMU, and interrupt paths.

## 🔗 Connections
- [[115-Operating System Fundamentals]]
- [[130-Context Switch and Process Control Block]]
- [[103-Cache Coherence Problem and Solutions]]