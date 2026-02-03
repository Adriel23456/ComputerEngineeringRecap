---
Theme: Process Creation and Termination
Date created: 2026-01-31 12:00
tags: [KernelLevel, Process, Lifecycle]
---

## 📚 Idea/Concept

Processes are created and destroyed throughout system operation. Understanding when and how this happens, along with platform-specific differences, is essential for OS comprehension.

### When Processes Are Created

Four primary triggers:
1. **System startup** - Init/systemd creates initial processes
2. **System call from existing process** - Parent spawns child
3. **User request** - Double-click, command line execution
4. **Batch job initiation** - Scheduled or queued work

### How Processes Are Created

General mechanism:
1. Existing process executes system call
2. OS creates new process
3. Associates it with a program
4. Allocates resources (memory, file handles)

### UNIX Process Creation: Fork + Exec

Two-step approach:
```c
pid_t pid = fork();  // Step 1: Clone current process

if (pid == 0) {
    // Child process - identical copy of parent
    execve("/bin/ls", argv, envp);  // Step 2: Replace image
    // execve only returns on error
    exit(1);
} else if (pid > 0) {
    // Parent process
    waitpid(pid, &status, 0);  // Wait for child
} else {
    // fork failed
    perror("fork");
}
```

Fork behavior:
```
Before fork():           After fork():
┌──────────────┐        ┌──────────────┐  ┌──────────────┐
│   Parent     │   →    │   Parent     │  │    Child     │
│   PID: 100   │        │   PID: 100   │  │   PID: 101   │
│              │        │   fork()=101 │  │   fork()=0   │
└──────────────┘        └──────────────┘  └──────────────┘
                              ↓                  ↓
                         continues          execve() replaces
                                            memory image
```

### Windows Process Creation: CreateProcess

Single-step approach:
```c
STARTUPINFO si = { sizeof(si) };
PROCESS_INFORMATION pi;

CreateProcess(
    "C:\\Windows\\notepad.exe",  // Application
    NULL,                         // Command line
    NULL,                         // Process security
    NULL,                         // Thread security
    FALSE,                        // Inherit handles
    0,                            // Creation flags
    NULL,                         // Environment
    NULL,                         // Current directory
    &si,                          // Startup info
    &pi                           // Process info (output)
);
```

### Comparison: UNIX vs Windows

| Aspect | UNIX (fork+exec) | Windows (CreateProcess) |
|--------|------------------|------------------------|
| Steps | Two (fork, exec) | One |
| Initial state | Clone of parent | Fresh process |
| Memory sharing | Briefly shared (COW) | Separate from start |
| Parameters | ~3 (exec) | ~10 |
| Flexibility | High (can run code between fork/exec) | Moderate |
| Process hierarchy | Yes (parent-child tree) | No meaningful hierarchy |

Note: Windows has no process hierarchy concept like UNIX. Parent-child relationships are less significant.

### Process Termination

Four ways a process terminates:

1. **Normal exit (voluntary)**
   - Work completed successfully
   - UNIX: `exit(0)`, Windows: `ExitProcess(0)`
   - Example: Compiler finishes compiling

2. **Error exit (voluntary)**
   - User error detected
   - Example: `gcc test.c` when test.c doesn't exist
   - Process chooses to terminate

3. **Fatal error (involuntary)**
   - Illegal instruction, memory violation, divide by zero
   - OS terminates the process
   - Not the process's choice

4. **Killed by another process (involuntary)**
   - UNIX: `kill(pid, SIGKILL)`
   - Windows: `TerminateProcess(handle, exitCode)`
   - Requires appropriate permissions

### Daemons: Background Processes

Processes that run in background:
- No user interaction
- Started at boot
- Provide services (email, print, antivirus, web)
- Dozens running on typical system

```
$ ps aux | grep -E "d$"   # Many daemon names end in 'd'
sshd, httpd, mysqld, crond, systemd...
```

## 📌 Key Points (Optional)
- UNIX uses fork+exec; Windows uses CreateProcess.
- Process termination can be voluntary or involuntary.
- Windows lacks the hierarchical process structure of UNIX.

## 🖼️ Recommended Image
- Flow diagram showing fork-exec pattern vs CreateProcess.

## 🔗 Connections
- [[127-Process Concept and Program vs Process]]
- [[129-Process States FSM Simplified and Complete]]
- [[124-POSIX System Call Standards]]