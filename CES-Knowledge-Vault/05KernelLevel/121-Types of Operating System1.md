---
Theme: Types of Operating Systems
Date created: 2026-01-31 12:00
tags: [KernelLevel, OS, Classification]
---

## 📚 Idea/Concept

Operating systems can be classified by their purpose and capabilities, from simple batch systems to complex distributed environments. Understanding these types helps select appropriate OS for specific use cases.

### Time-Sharing and Multiprogramming Systems

Multiprogramming:
- Multiple programs available in memory simultaneously
- CPU shared through time quanta and clock interrupts
- Goal: maximize CPU utilization

Time-sharing:
- Extension of multiprogramming
- Focus on interactivity with many users/processes
- Fast context switches for responsiveness

```
Multiprogramming Example:

Memory Layout:
┌───────────────┐
│  Program A    │ ← Currently executing
├───────────────┤
│  Program B    │ ← Blocked on I/O
├───────────────┤
│  Program C    │ ← Ready to run
├───────────────┤
│     OS        │
└───────────────┘

Time Sharing:
┌────┬────┬────┬────┬────┬────┬────┬────┐
│ A  │ B  │ C  │ A  │ B  │ C  │ A  │ B  │ ...
└────┴────┴────┴────┴────┴────┴────┴────┘
       Each slot = quantum (e.g., 10ms)
```

### Virtual Machines and VMM

Key concepts:
- "Transparent" = exists but not seen
- "Virtual" = seen but doesn't exist

Virtual Machine Monitor (VMM):
- Software that divides one physical machine into multiple virtual machines
- Each VM can run different OS from the host
- VMs appear as "exact copies" of hardware

Types of VMM:
- Type 1 (Bare-metal): Runs directly on hardware (ESXi, Xen)
- Type 2 (Hosted): Runs on host OS (VirtualBox, VMware Workstation)

### Containers vs Virtual Machines

| Aspect | Virtual Machine | Container |
|--------|-----------------|-----------|
| Isolation | Complete (own kernel) | Process-level (shared kernel) |
| Overhead | High (full OS per VM) | Low (shared OS) |
| Boot Time | Minutes | Seconds |
| Resource Use | Heavy | Light |
| Use Case | Different OS needed | Same OS, app isolation |

```
VM Architecture:              Container Architecture:
┌─────────────────┐           ┌─────────────────┐
│ App │ App │ App │           │ App │ App │ App │
├─────┼─────┼─────┤           ├─────┴─────┴─────┤
│ OS  │ OS  │ OS  │           │  Container Eng  │
├─────┴─────┴─────┤           ├─────────────────┤
│      VMM        │           │    Host OS      │
├─────────────────┤           ├─────────────────┤
│    Hardware     │           │    Hardware     │
└─────────────────┘           └─────────────────┘
```

### Exokernel

Alternative to traditional virtualization:
- Instead of cloning machines, partitions resources
- Separates protection from administration
- Assigns resources (disk blocks, memory pages) directly to applications
- Maximum control and performance

### Virtualization Examples

Common platforms:
- VMware (ESXi, Workstation)
- Red Hat (KVM, libvirt, virt-manager)
- Proxmox VE (KVM + LXC containers)
- Microsoft Hyper-V

## 📌 Key Points (Optional)
- Time-sharing enabled interactive computing.
- Virtualization provides isolation and consolidation.
- Containers offer lightweight alternative to full VMs.

## 🖼️ Recommended Image
- Comparison diagram of VM stack vs. Container stack.

## 🔗 Connections
- [[115-Operating System Fundamentals]]
- [[126-VMM Hardware Virtualization Layers]]