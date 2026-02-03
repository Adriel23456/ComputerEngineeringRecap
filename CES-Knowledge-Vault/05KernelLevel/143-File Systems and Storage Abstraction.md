---
Theme: File Systems and Storage Abstraction
Date created: 2026-01-31 12:00
tags: [KernelLevel, FileSystem, Storage, Abstraction]
---

## 📚 Idea/Concept

File systems provide the abstraction of persistent named data containers (files) over raw block storage. This abstraction hides disk geometry and enables organized, protected, shareable storage.

### Why File Systems Exist

Three requirements for persistent storage:
1. Store large amounts of information
2. Data survives process termination
3. Multiple processes can access data concurrently

Files are the ONLY practical way to achieve persistence in modern systems.

### Files: The Core Abstraction

```
Process View:                   Reality:
┌──────────────────────┐       ┌──────────────────────┐
│  file.txt            │       │ Disk sectors         │
│  "Hello World"       │       │ scattered across     │
│                      │   →   │ platters, tracks,    │
│  read(fd, buf, 11)   │  OS   │ cylinders            │
└──────────────────────┘       └──────────────────────┘

The OS translates file operations to block I/O
```

### File Descriptor (fd)

When a process opens a file:
```
fd = open("myfile.txt", O_RDWR);

┌─────────────────────────────────────────────────────────────┐
│  Process                                                     │
│  ┌─────────────────┐                                        │
│  │ fd table        │                                        │
│  │ 0: stdin        │                                        │
│  │ 1: stdout       │                                        │
│  │ 2: stderr       │                                        │
│  │ 3: myfile.txt ──┼──→ System-wide open file table         │
│  └─────────────────┘              │                         │
│                                   ↓                         │
│                         ┌─────────────────┐                 │
│                         │ File structure  │                 │
│                         │ - position ptr  │                 │
│                         │ - mode (R/W)    │                 │
│                         │ - inode ptr ────┼──→ Disk blocks  │
│                         └─────────────────┘                 │
└─────────────────────────────────────────────────────────────┘
```

### File Extensions

Extensions indicate file type:
- `.txt` - text file
- `.jpg` - image
- `.exe` - executable

OS uses extension to determine:
- Default application
- How to interpret contents
- Security policies

Note: In UNIX, extensions are conventions, not requirements.

### Three File Structure Models

```
1. BYTE SEQUENCE (Most common - UNIX, Windows)
   ┌─────────────────────────────────────────┐
   │ b y t e b y t e b y t e b y t e . . . │
   └─────────────────────────────────────────┘
   - OS doesn't know/care about internal structure
   - Maximum flexibility for applications
   - May be inefficient for structured queries

2. RECORD SEQUENCE (Historical - Mainframes)
   ┌─────────┬─────────┬─────────┬─────────┐
   │Record 1 │Record 2 │Record 3 │Record 4 │
   └─────────┴─────────┴─────────┴─────────┘
   - Fixed-size records
   - Read/write by record
   - Legacy systems only

3. TREE (Indexed - Databases)
   ┌─────────────────────────────────────────┐
   │           Root Node                      │
   │        /     |     \                    │
   │     Key1   Key2   Key3                  │
   │      |       |       |                  │
   │   Record  Record  Record                │
   └─────────────────────────────────────────┘
   - Key-value structure
   - Efficient search by key
   - Used in database systems
```

### Directory Systems

**Single-Level Directory:**
```
┌──────────────────────────────────────────┐
│  Root Directory                          │
│  ┌────┬────┬────┬────┬────┬────┬────┐  │
│  │f1  │f2  │f3  │f4  │f5  │f6  │... │  │
│  └────┴────┴────┴────┴────┴────┴────┘  │
└──────────────────────────────────────────┘

Problem: Names cannot repeat!
fd would fail with duplicate names
```

**Hierarchical Directories:**
```
                    /
         ┌──────────┼──────────┐
        home       etc       var
         │          │          │
    ┌────┴────┐    passwd    log
   user1   user2              │
    │                       syslog
  file.txt

- Paths: /home/user1/file.txt
- Names can repeat in different directories
- Absolute path (from /) vs Relative path (from cwd)
```

Creating directories consumes resources (negligible but real).

### Directory Operations

| Operation | Description |
|-----------|-------------|
| create | Create empty directory |
| delete | Remove empty directory |
| opendir | Open for reading entries |
| closedir | Close directory |
| readdir | Read next entry |
| rename | Change name |
| link | Create hard link (UNIX) |
| unlink | Remove link/file (UNIX) |

Note: `link` and `unlink` don't exist in Windows. They allow one file to have multiple names (hard links).

### Block Allocation Strategies

**Contiguous Allocation:**
```
File A: Blocks 4, 5, 6, 7 (contiguous)

Advantage: Fast sequential read
Disadvantage: External fragmentation, file growth difficult
```

**Linked List Allocation:**
```
File A: Block 4 → Block 9 → Block 2 → Block 15 → null

Each block contains pointer to next block
Advantage: No external fragmentation
Disadvantage: Slow random access, pointer overhead
```

**FAT (File Allocation Table):**
```
Move pointers to table in memory:

FAT Table:        File starts at block 4
┌───┬───┐
│ 0 │ - │         Block 4 → FAT[4] = 9
├───┼───┤         Block 9 → FAT[9] = 2
│ 1 │ - │         Block 2 → FAT[2] = 15
├───┼───┤         Block 15 → FAT[15] = EOF
│ 2 │ 15│
├───┼───┤         Advantage: No overhead in data blocks
│ 3 │ - │         Disadvantage: Table must fit in RAM
├───┼───┤
│ 4 │ 9 │
├───┼───┤
...
```

**I-Nodes (Index Nodes):**
```
┌─────────────────────────────────────────────────────────────┐
│  I-Node for file                                            │
├─────────────────────────────────────────────────────────────┤
│  Attributes (size, permissions, timestamps)                 │
├─────────────────────────────────────────────────────────────┤
│  Direct block pointers (12 typically)                       │
│  [ptr0][ptr1][ptr2]...[ptr11] → Data blocks                │
├─────────────────────────────────────────────────────────────┤
│  Single indirect pointer → Block of pointers → Data        │
├─────────────────────────────────────────────────────────────┤
│  Double indirect pointer → Block → Block → Data            │
├─────────────────────────────────────────────────────────────┤
│  Triple indirect pointer → Block → Block → Block → Data    │
└─────────────────────────────────────────────────────────────┘

Combines benefits of linked list and FAT
Only i-node needs to be in memory when file open
Used in UNIX/Linux file systems (ext2, ext3, ext4)
```

Key insight: I-nodes combine linked list and FAT benefits—they're the foundation of Linux file systems.

### Sector Zero

The first sector of any storage device is ALWAYS reserved for boot information (MBR/GPT).

## 📌 Key Points (Optional)
- Files are the only way to achieve data persistence.
- fd maps to system tables, which map to disk blocks via i-nodes or FAT.
- I-nodes are the key data structure in UNIX file systems.

## 🖼️ Recommended Image
- I-node structure showing direct and indirect block pointers.

## 🔗 Connections
- [[142-IO Management and Device Interaction]]
- [[125-Boot Process and Daemons]]
- [[138-Memory Management Fundamentals]]