---
Theme: Microkernel Architecture
Date created: 2026-01-31 12:00
tags: [KernelLevel, OS, Microkernel]
---

## 📚 Idea/Concept

A microkernel keeps only the absolute minimum in kernel mode to increase reliability. Everything else (drivers, file systems, network stack) runs as isolated user-space processes.

### Core Principle

"Mechanism, not policy":
- Kernel provides the HOW (IPC, memory control, scheduling mechanisms)
- Policies (specific strategies) reside in user-space servers/daemons

### What Stays in the Microkernel

Decision criteria:
1. Is there only one "universal" way to implement this function?
   - If NO (multiple policies exist) → Move out of kernel
2. Is it critical for security/isolation/response times?
   - If YES → Keep in kernel

Typical microkernel functions:
- Interrupt handling
- Process synchronization primitives
- Basic CPU scheduling
- Inter-process communication (IPC)
- Low-level memory management

### Microkernel Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    User Space                            │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────────┐  │
│  │  File   │ │ Network │ │ Device  │ │    User     │  │
│  │ System  │ │  Stack  │ │ Drivers │ │ Applications│  │
│  │ Server  │ │ Server  │ │ Servers │ │             │  │
│  └────┬────┘ └────┬────┘ └────┬────┘ └──────┬──────┘  │
│       │           │           │              │          │
│       └───────────┴─────┬─────┴──────────────┘          │
│                         │ IPC Messages                   │
├─────────────────────────┼───────────────────────────────┤
│  ┌──────────────────────┴──────────────────────────┐   │
│  │              MICROKERNEL                         │   │
│  │  - IPC mechanisms                                │   │
│  │  - Basic scheduling                              │   │
│  │  - Interrupt handling                            │   │
│  │  - Low-level memory management                   │   │
│  └──────────────────────────────────────────────────┘   │
│                      Kernel Mode                         │
├─────────────────────────────────────────────────────────┤
│                       Hardware                           │
└─────────────────────────────────────────────────────────┘
```

### Tradeoffs

Advantages:
- High reliability (isolated failures)
- Modularity (replace components independently)
- Maintainability (smaller kernel = fewer bugs)
- Security (smaller attack surface in kernel)

Disadvantages:
- Performance overhead (more IPC, context switches)
- Complexity in message passing
- Latency for system services

### MINIX and the Torvalds vs Tanenbaum Debate

Historical context:
- MINIX: Classic microkernel + user servers
- Linux: Monolithic kernel
- Debate: Practicality (Linux) vs. Design purity (MINIX)

Tanenbaum's position: Microkernel is superior design
Torvalds' position: Monolithic is more practical for performance

### The Kernel Remains the Only Hardware Path

Even in microkernels, user-space servers cannot touch hardware directly. They must request services through the microkernel. The microkernel sacrifices some performance but gains sustainability, modularity, and fault robustness.

## 📌 Key Points (Optional)
- Microkernels trade performance for reliability and modularity.
- The debate between monolithic and microkernel continues today.

## 🖼️ Recommended Image
- Microkernel architecture showing servers in user space communicating via IPC.

## 🔗 Connections
- [[118-Monolithic Systems and Layered Systems]]
- [[120-Client Server Model on Microkernel]]
- [[131-Inter Process Communication IPC]]