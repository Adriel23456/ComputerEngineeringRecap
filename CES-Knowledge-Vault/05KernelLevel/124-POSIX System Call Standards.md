---
Theme: POSIX System Call Standards
Date created: 2026-01-31 12:00
tags: [KernelLevel, OS, POSIX, Standards]
---

## 📚 Idea/Concept

POSIX (Portable Operating System Interface) defines a standard set of system calls and interfaces that enable software portability across UNIX-like operating systems.

### Process Management Calls

| Call | Description |
|------|-------------|
| `pid = fork()` | Create child process (clone of parent) |
| `pid = waitpid(pid, &stat, opts)` | Wait for child to terminate |
| `execve(name, argv, env)` | Replace process image with new program |
| `exit(status)` | Terminate process and return status |

Fork-Exec pattern:
```c
pid_t pid = fork();
if (pid == 0) {
    // Child process
    execve("/bin/ls", args, envp);
    exit(1);  // Only reached if exec fails
} else {
    // Parent process
    waitpid(pid, &status, 0);
}
```

### File Management Calls

| Call | Description |
|------|-------------|
| `fd = open(file, mode)` | Open file, return file descriptor |
| `close(fd)` | Close file descriptor |
| `n = read(fd, buf, count)` | Read bytes from file |
| `n = write(fd, buf, count)` | Write bytes to file |
| `lseek(fd, offset, whence)` | Reposition file pointer |
| `stat(path, &buf)` | Get file metadata |

File descriptor flow:
```
open() → returns fd (e.g., 3)
              ↓
        ┌─────────────┐
        │ fd table    │
        │ 0: stdin    │
        │ 1: stdout   │
        │ 2: stderr   │
        │ 3: myfile ──┼──→ file structure → inode → disk blocks
        └─────────────┘
```

### Directory and Filesystem Calls

| Call | Description |
|------|-------------|
| `mkdir(name, mode)` | Create directory |
| `rmdir(name)` | Remove empty directory |
| `link(name1, name2)` | Create hard link |
| `unlink(name)` | Remove directory entry |
| `mount(device, point, flags)` | Mount filesystem |
| `umount(device)` | Unmount filesystem |

Note: `link` and `unlink` do not exist in Windows; they serve to allow modifications to a copy that also modify the original (hard links).

### Protection and Ownership Calls

| Call | Description |
|------|-------------|
| `chmod(path, mode)` | Change file permissions |
| `chown(path, owner, group)` | Change ownership |
| `getuid()` | Get user ID |
| `setuid(uid)` | Set user ID (privileged) |

### Time and Signal Calls

| Call | Description |
|------|-------------|
| `time(&t)` | Get current time |
| `kill(pid, sig)` | Send signal to process |
| `signal(sig, handler)` | Set signal handler |
| `alarm(seconds)` | Schedule SIGALRM |

### POSIX Categories Summary

```
┌────────────────────────────────────────────────────────────┐
│                    POSIX System Calls                       │
├────────────────┬───────────────────────────────────────────┤
│   Process      │  fork, exec, wait, exit, kill             │
├────────────────┼───────────────────────────────────────────┤
│   Files        │  open, close, read, write, lseek, stat    │
├────────────────┼───────────────────────────────────────────┤
│   Directories  │  mkdir, rmdir, link, unlink, mount        │
├────────────────┼───────────────────────────────────────────┤
│   Protection   │  chmod, chown, getuid, setuid             │
├────────────────┼───────────────────────────────────────────┤
│   Time/Signals │  time, alarm, signal, kill                │
└────────────────┴───────────────────────────────────────────┘
```

### Portability Benefits

POSIX compliance means:
- Same source code compiles on different UNIX systems
- Predictable behavior across platforms
- Standard error codes and semantics
- Reduced vendor lock-in

## 📌 Key Points (Optional)
- POSIX standardizes the programmer interface to UNIX-like systems.
- Fork-exec is the fundamental pattern for process creation in UNIX.

## 🖼️ Recommended Image
- Table showing POSIX call categories with key functions.

## 🔗 Connections
- [[123-System Call Macro Steps and Micro Steps]]
- [[128-Process Creation and Termination]]