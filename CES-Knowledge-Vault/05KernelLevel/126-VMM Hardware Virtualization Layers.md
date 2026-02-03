---
Theme: VMM Hardware Virtualization Layers
Date created: 2026-01-31 12:00
tags: [KernelLevel, Virtualization, VMM]
---

## 📚 Idea/Concept

Virtual Machine Monitors (VMMs) virtualize hardware resources to allow multiple operating systems to run simultaneously on one physical machine. Understanding the virtualization layers is essential for modern cloud and server infrastructure.

### VMM Types

Type 1 (Bare-Metal):
- Runs directly on hardware
- No host OS underneath
- Examples: VMware ESXi, Xen, Microsoft Hyper-V

Type 2 (Hosted):
- Runs on top of host OS
- Uses host OS for hardware access
- Examples: VirtualBox, VMware Workstation, Parallels

```
Type 1:                         Type 2:
┌─────────────────────┐        ┌─────────────────────┐
│ Guest │ Guest│Guest │        │ Guest │ Guest│Guest │
│  OS   │  OS  │ OS   │        │  OS   │  OS  │ OS   │
├───────┴──────┴──────┤        ├───────┴──────┴──────┤
│    Hypervisor       │        │       VMM           │
├─────────────────────┤        ├─────────────────────┤
│     Hardware        │        │      Host OS        │
└─────────────────────┘        ├─────────────────────┤
                               │     Hardware        │
                               └─────────────────────┘
```

### Virtualization Techniques

Full Virtualization:
- Complete hardware simulation
- Guest OS unmodified
- Binary translation for privileged instructions

Paravirtualization:
- Modified guest OS
- Hypercalls instead of privileged instructions
- Better performance, less compatibility

Hardware-Assisted (VT-x/AMD-V):
- CPU extensions for virtualization
- New execution mode for guests
- Traps on privileged operations

### Hardware Layers Virtualized

| Layer | Virtualization Method |
|-------|----------------------|
| CPU | Time-sharing, trap-and-emulate, VT-x |
| Memory | Shadow page tables, EPT/NPT |
| I/O | Device emulation, passthrough, virtio |
| Storage | Virtual disks (VMDK, QCOW2) |
| Network | Virtual switches, TAP devices |

### Division by Zero in VMM Architecture

When a user program divides by zero in a VM:
```
1. ALU detects division by zero in guest
2. Hardware raises exception
3. VMM receives the trap (hardware mode bit = 1)
4. VMM calls virtual hardware to enter kernel mode
5. Virtual hardware signals kernel mode to guest OS
6. Guest OS requests exception handler code
7. Division by zero = fatal error → Guest OS terminates process
8. Guest continues with next process
9. Mode bits return to user level (real and virtual)
```

### Exokernel Alternative

Instead of virtualizing hardware:
- Partition resources directly
- Separate protection from management
- Assign disk blocks, memory pages to applications
- Maximum control and performance

```
Traditional VMM:               Exokernel:
┌─────────────────────┐       ┌─────────────────────┐
│   Virtual HW Copy   │       │  App A  │  App B   │
├─────────────────────┤       │  (owns  │  (owns   │
│   Virtual HW Copy   │       │  blocks │  blocks  │
├─────────────────────┤       │  1-100) │  101-200)│
│       VMM           │       ├─────────┴──────────┤
├─────────────────────┤       │     Exokernel      │
│     Hardware        │       ├─────────────────────┤
└─────────────────────┘       │     Hardware        │
                              └─────────────────────┘
```

### Modern Virtualization Platforms

| Platform | Type | Features |
|----------|------|----------|
| VMware ESXi | Type 1 | Enterprise, vSphere |
| KVM | Type 1* | Linux kernel module |
| Xen | Type 1 | Paravirt + HVM |
| VirtualBox | Type 2 | Cross-platform |
| Proxmox VE | Platform | KVM + LXC |

*KVM is technically type 1 as it's a kernel module.

## 📌 Key Points (Optional)
- VMMs enable hardware consolidation and isolation.
- Hardware assistance (VT-x/AMD-V) dramatically improves virtualization performance.

## 🖼️ Recommended Image
- Layer diagram comparing Type 1 vs Type 2 hypervisors.

## 🔗 Connections
- [[121-Types of Operating System1]]
- [[117-CPU Architecture with OS Integration]]