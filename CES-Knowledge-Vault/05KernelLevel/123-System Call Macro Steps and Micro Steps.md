---
Theme: System Call Macro Steps and Micro Steps
Date created: 2026-01-31 12:00
tags: [KernelLevel, OS, SystemCalls]
---

## 📚 Idea/Concept

System calls involve a carefully orchestrated sequence of steps, both at the high level (macro-steps) and the detailed implementation level (micro-steps), ensuring secure transition between user and kernel modes.

### Macro-Steps of a System Call

High-level sequence (using `read()` as example):

```
┌─────────────────────────────────────────────────────────────┐
│  USER MODE                                                   │
│                                                              │
│  1. Push parameters onto stack (fd, buffer, nbytes)         │
│  2. Call library function (read)                            │
│  3. Library places syscall number in register               │
│  4. Execute TRAP instruction (switch to kernel)             │
│                                                              │
├─────────────────────────────────────────────────────────────┤
│  KERNEL MODE                                                 │
│                                                              │
│  5. Dispatch to correct handler via syscall table           │
│  6. Execute the system call handler                         │
│  7. Return status/result in register                        │
│                                                              │
├─────────────────────────────────────────────────────────────┤
│  USER MODE                                                   │
│                                                              │
│  8. Return to caller procedure                              │
│  9. Clean up stack (increment SP)                           │
│  10. Continue program execution                             │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Micro-Steps Detail

Step-by-step breakdown:

1. Parameter Preparation (User Mode):
```c
// User code calls: result = read(fd, buffer, n);
// Compiler generates:
push n          // 3rd parameter
push buffer     // 2nd parameter  
push fd         // 1st parameter
call _read      // Call library wrapper
```

2. Library Wrapper (User Mode):
```assembly
_read:
    mov eax, SYS_READ    // Syscall number
    mov ebx, [esp+4]     // fd
    mov ecx, [esp+8]     // buffer
    mov edx, [esp+12]    // n
    int 0x80             // TRAP to kernel
    ret
```

3. Kernel Entry (Transition):
```
- CPU saves user state (CS, EIP, EFLAGS, SS, ESP)
- Mode bit switches to 1 (Kernel)
- CPU loads kernel stack pointer
- Jump to interrupt handler (vector 0x80)
```

4. Syscall Dispatch (Kernel Mode):
```
- Save remaining registers
- Validate syscall number
- Index into syscall table
- Call appropriate handler: sys_read(fd, buffer, n)
```

5. Handler Execution (Kernel Mode):
```
- Validate parameters (fd valid? buffer in user space?)
- Perform actual I/O operation
- Copy data to user buffer
- Set return value in eax
```

6. Return Path (Kernel → User):
```
- Restore saved registers
- Execute IRET instruction
- Mode bit switches to 0 (User)
- CPU restores user state
- Continue at instruction after TRAP
```

### Visual Timeline

```
Time →
User Mode ─────────┬───────────────────┬─────────────────────
                   │                   │
             1.Push params      8.Return to caller
             2.Call library     9.Increment SP
             3.Set syscall#
             4.TRAP
                   │                   │
                   ↓                   ↑
Kernel Mode ───────┴───────────────────┴─────────────────────
                   
             5.Dispatch
             6.Execute handler
             7.Set result
```

### Key Implementation Details

Stack manipulation:
- User stack used for parameters
- Kernel stack used during syscall execution
- Stack pointer saved/restored during transition

Register conventions:
- eax: syscall number, return value
- ebx, ecx, edx: parameters
- Varies by architecture (x86_64 uses different registers)

## 📌 Key Points (Optional)
- System calls involve careful state preservation across mode transitions.
- The library wrapper hides architecture-specific details from user code.

## 🖼️ Recommended Image
- Detailed flow diagram showing stack contents at each step of syscall.

## 🔗 Connections
- [[122-System Calls and CPU Protection]]
- [[124-POSIX System Call Standards]]
- [[148-Interrupt Handling Mechanism]]