---
Theme: Boot Process and Daemons
Date created: 2026-01-31 12:00
tags: [KernelLevel, OS, Boot, Daemons]
---

## 📚 Idea/Concept

The boot process is the sequence of steps that brings a computer from power-on to a running operating system. Daemons are background processes that provide essential services without user interaction.

### BIOS/Firmware Role

BIOS (Basic Input/Output System) contains:
- Low-level I/O software
- Hardware initialization routines
- Boot device detection

Power-on sequence:
```
1. Power applied to motherboard
2. CPU executes from fixed address (BIOS ROM)
3. BIOS performs POST (Power-On Self-Test)
4. Detects RAM, slots, cards
5. Determines boot device
```

### Boot Sequence Steps

```
┌─────────────────────────────────────────────────────────────┐
│  1. BIOS/UEFI Initialization                                │
│     - POST (Power-On Self-Test)                             │
│     - Hardware detection                                     │
│     - Select boot device                                     │
├─────────────────────────────────────────────────────────────┤
│  2. Read MBR/GPT (Sector Zero)                              │
│     - First sector loaded to memory                         │
│     - Contains partition table                              │
│     - Identifies active partition                           │
├─────────────────────────────────────────────────────────────┤
│  3. Secondary Bootloader                                     │
│     - Loaded from active partition                          │
│     - Understands filesystem                                │
│     - Examples: GRUB, Windows Boot Manager                  │
├─────────────────────────────────────────────────────────────┤
│  4. Kernel Loading                                          │
│     - Bootloader loads kernel image                         │
│     - Kernel decompresses itself                            │
│     - Queries BIOS for configuration                        │
├─────────────────────────────────────────────────────────────┤
│  5. Kernel Initialization                                    │
│     - Detect hardware devices                               │
│     - Load device drivers                                   │
│     - Initialize internal tables                            │
│     - Mount root filesystem                                 │
├─────────────────────────────────────────────────────────────┤
│  6. Init Process / Systemd                                  │
│     - First user-space process (PID 1)                      │
│     - Creates background processes (daemons)                │
│     - Launches login/GUI                                    │
└─────────────────────────────────────────────────────────────┘
```

### Sector Zero

The first sector of a storage device:
- Always reserved for boot information
- Contains MBR (Master Boot Record) or GPT header
- Size: 512 bytes (legacy) or 4096 bytes (modern)

MBR Structure:
```
┌────────────────────────────────────┐
│ Bootstrap Code (446 bytes)        │
├────────────────────────────────────┤
│ Partition Table (64 bytes)        │
│ - Entry 1 (16 bytes)              │
│ - Entry 2 (16 bytes)              │
│ - Entry 3 (16 bytes)              │
│ - Entry 4 (16 bytes)              │
├────────────────────────────────────┤
│ Boot Signature (2 bytes: 0x55AA)  │
└────────────────────────────────────┘
```

### Daemons

Definition:
- Background programs that don't interact with operator
- Provide services (logging, networking, scheduling)
- Start during boot, run continuously

Common daemons:
| Daemon | Purpose |
|--------|---------|
| syslogd | System logging |
| crond | Scheduled tasks |
| sshd | Secure shell server |
| httpd | Web server |
| mysqld | Database server |
| networkd | Network management |

Daemon characteristics:
- No controlling terminal
- Run as specific user (often root or dedicated)
- Write to log files
- Listen on ports or watch files

### Modern Init Systems

Traditional SysV init:
- Sequential startup
- Shell scripts in /etc/init.d/

Systemd (modern):
- Parallel startup
- Unit files for services
- Dependency management
- Socket activation

## 📌 Key Points (Optional)
- Sector zero is always reserved for boot information.
- Daemons provide essential background services invisibly.

## 🖼️ Recommended Image
- Boot sequence flowchart from power-on to login prompt.

## 🔗 Connections
- [[115-Operating System Fundamentals]]
- [[128-Process Creation and Termination]]