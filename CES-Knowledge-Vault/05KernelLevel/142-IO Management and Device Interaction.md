---
Theme: IO Management and Device Interaction
Date created: 2026-01-31 12:00
tags: [KernelLevel, IO, Devices, Drivers]
---

## 📚 Idea/Concept

I/O management is one of the most complex OS subsystems, comprising approximately 58% of OS code. The OS must provide uniform interfaces to diverse hardware while maximizing performance.

### Device Classification

**Block Devices:**
- Addressable in fixed-size blocks
- Support random access (seek)
- Examples: Hard drives, SSDs, USB drives

**Character Devices (Stream):**
- Not addressable, no seek operation
- Emit or accept streams of data
- Examples: Keyboard, mouse, printer, serial ports

### Driver: Hardware vs Software

```
┌─────────────────────────────────────────────────────────────┐
│  HARDWARE DRIVER (Controller)                               │
│  - Electronic circuit connecting device to bus              │
│  - Implements physical protocol (PCI, SATA, USB)            │
│  - Synchronizes signals (H-Sync, V-Sync for video)          │
│  - Has registers: status, control, data                     │
└─────────────────────────────────────────────────────────────┘
                              ↕
┌─────────────────────────────────────────────────────────────┐
│  SOFTWARE DRIVER                                            │
│  - Kernel module that talks to hardware controller          │
│  - Exposes standard interface (read/write) to OS            │
│  - OS-specific (Windows driver ≠ Linux driver)              │
│  - Typically runs in kernel mode                            │
└─────────────────────────────────────────────────────────────┘
```

### Three Ways to Interact with Devices

#### 1. Memory-Mapped I/O

Device registers appear as memory addresses:
```
┌─────────────────────────────────────────────────────────────┐
│                    Address Space                             │
├─────────────────────────────────────────────────────────────┤
│  0x00000000 - 0xBFFFFFFF : Regular RAM                      │
├─────────────────────────────────────────────────────────────┤
│  0xC0000000 - 0xC0000FFF : Device A registers               │
│  0xC0001000 - 0xC0001FFF : Device B registers               │
│  ...                                                         │
└─────────────────────────────────────────────────────────────┘

// Read device status (like reading memory)
int status = *((int*)0xC0000000);

// Write command (like writing memory)
*((int*)0xC0000004) = COMMAND_START;
```

Advantages: No special instructions, protection via MMU
Disadvantages: Cache coherence issues, bus contention

#### 2. Programmed I/O (Polling)

CPU handles everything, continuously checks device status:

```c
// Print string using polling
void print_string(char *str) {
    while (*str != '\0') {
        while (printer_status() != READY) {
            // Busy wait - CPU doing nothing useful!
        }
        send_char_to_printer(*str);
        str++;
    }
}
```

Problem: CPU wastes cycles waiting for slow devices.

#### 3. Interrupt-Driven I/O

Device signals CPU when ready:

```
┌─────────────────────────────────────────────────────────────┐
│  1. CPU sends command to device                             │
│  2. CPU continues other work                                │
│  3. Device completes operation                              │
│  4. Device raises interrupt (IRQ)                           │
│  5. CPU stops current work, runs interrupt handler          │
│  6. Handler processes completion, returns                   │
│  7. CPU resumes previous work                               │
└─────────────────────────────────────────────────────────────┘
```

Better CPU utilization, but many interrupts = overhead.

#### 4. DMA (Direct Memory Access)

For bulk transfers, DMA controller moves data without CPU:

```
┌─────────────────────────────────────────────────────────────┐
│                                                              │
│   CPU ←───────────────────────────────────────→ Memory      │
│    │                                                ↑        │
│    │ Setup DMA                                     │        │
│    ↓                                               │        │
│  ┌───────────┐                              ┌──────┴─────┐  │
│  │    DMA    │══════════════════════════════│   Device   │  │
│  │Controller │  Direct data path!           │(Disk, NIC) │  │
│  └───────────┘                              └────────────┘  │
│                                                              │
│  CPU only involved at setup and completion interrupt        │
└─────────────────────────────────────────────────────────────┘
```

DMA steps:
1. CPU programs DMA: source, destination, count
2. DMA transfers data block by block
3. DMA raises interrupt when complete
4. CPU processes result

### When to Use Each Method

| Method | Best For | Example |
|--------|----------|---------|
| Polling | Very fast devices (freq ≈ CPU) | High-speed SSD |
| Interrupts | Slow devices | Keyboard, mouse |
| DMA | Bulk data transfers | Disk I/O, network packets |

Rule of thumb:
- Device as fast as CPU → Polling
- Device much slower → Interrupts
- Large data volumes → DMA

### I/O Software Layers

```
┌─────────────────────────────────────────────────────────────┐
│            User-Level I/O Software                          │
│            (printf, scanf, file operations)                 │
├─────────────────────────────────────────────────────────────┤
│            Device-Independent OS Software                   │
│            (Buffering, caching, error handling)            │
├─────────────────────────────────────────────────────────────┤
│            Device Drivers                                   │
│            (Specific to each device type)                  │
├─────────────────────────────────────────────────────────────┤
│            Interrupt Handlers                               │
│            (PIC management, vector dispatch)               │
├─────────────────────────────────────────────────────────────┤
│            Hardware                                         │
│            (Controllers, devices, buses)                   │
└─────────────────────────────────────────────────────────────┘
```

### Buffering

Critical for performance:
```
Without buffering:           With buffering:
Byte arrives → Interrupt     Fill buffer → One interrupt
Byte arrives → Interrupt     for many bytes
Byte arrives → Interrupt     
(1000 interrupts!)           (1 interrupt for 1000 bytes)
```

### Driver Functions

A device driver must:
- Accept abstract read/write requests
- Initialize device (default register values)
- Format/align data for device requirements
- Check device availability before operations
- Handle errors at lowest level (retries for transient failures)

## 📌 Key Points (Optional)
- Polling wastes CPU; interrupts provide better utilization for slow devices.
- DMA handles bulk transfers without CPU involvement per byte.
- Drivers abstract hardware differences behind uniform interfaces.

## 🖼️ Recommended Image
- Layered I/O software stack diagram showing user → kernel → hardware flow.

## 🔗 Connections
- [[115-Operating System Fundamentals]]
- [[122-System Calls and CPU Protection]]
- [[148-Interrupt Handling Mechanism]]