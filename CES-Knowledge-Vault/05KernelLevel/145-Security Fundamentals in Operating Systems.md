---
Theme: Security Fundamentals in Operating Systems
Date created: 2026-01-31 12:00
tags: [KernelLevel, Security, Protection]
---

## 📚 Idea/Concept

Security is a transversal concern in operating systems. The OS must protect data and resources from unauthorized access while enabling legitimate use. Security involves confidentiality, integrity, and availability.

### Four Primary Security Objectives

```
┌─────────────────────────────────────────────────────────────┐
│  1. CONFIDENTIALITY                                         │
│     - Secret data stays secret                              │
│     - Only authorized users can read                        │
│     - Threat: Data exposure                                 │
├─────────────────────────────────────────────────────────────┤
│  2. INTEGRITY                                               │
│     - Data cannot be modified without authorization         │
│     - Includes: creation, modification, deletion            │
│     - Threat: Data tampering                                │
├─────────────────────────────────────────────────────────────┤
│  3. AVAILABILITY                                            │
│     - System remains usable                                 │
│     - No user can render system unusable for others         │
│     - Threat: Denial of Service (DoS)                       │
├─────────────────────────────────────────────────────────────┤
│  4. CONTROL (Exclusion of External Threats)                 │
│     - Keep out viruses, malware, unauthorized users         │
│     - Prevent system from becoming "zombie"                 │
│     - Threat: Malware infection                             │
└─────────────────────────────────────────────────────────────┘
```

### Types of Intruders

| Type | Description |
|------|-------------|
| **Curious users** | Non-technical, explore due to poor configuration |
| **Technical snoopers** | Students/programmers treating security as challenge |
| **Financial criminals** | Bank employees, seeking monetary gain |
| **Spies** | Commercial or military espionage, state-backed |

### Accidental Data Loss

Not all threats are malicious:
- Natural disasters (fire, flood, earthquake)
- Hardware failures (disk crash, memory corruption)
- Human error (accidental deletion, wrong command)

Requires: Backups, redundancy (RAID), disaster recovery plans.

### Cryptography Fundamentals

Basic model:
```
Plaintext (P) + Key (K) → Encryption (E) → Ciphertext (C)
C = E(P, K)

Ciphertext (C) + Key (K) → Decryption (D) → Plaintext (P)
P = D(C, K)
```

Kerckhoffs' principle: Security depends on key secrecy, not algorithm secrecy.

**Symmetric (Secret Key):**
- Same key for encryption and decryption
- Fast, efficient
- Problem: Key distribution
- Examples: AES, DES

**Asymmetric (Public Key):**
- Key pair: public (shareable) + private (secret)
- Public encrypts, private decrypts (or vice versa for signatures)
- Slower but solves key distribution
- Examples: RSA, ECC

### Digital Signatures

Provides authentication, integrity, and non-repudiation:

```
Signing:
1. Compute hash of document: H = hash(Document)
2. Encrypt hash with private key: Signature = E(H, PrivateKey)
3. Send: Document + Signature

Verification:
1. Compute hash of received document: H' = hash(Document)
2. Decrypt signature with public key: H = D(Signature, PublicKey)
3. Compare: H == H'? Valid : Invalid
```

RSA is commonly used because its mathematical structure allows both encryption and signing.

### Protection Mechanisms

**Protection Domains:**
A domain is a set of (object, permissions) pairs:
```
Domain 1 (User A):
  - (File1, read)
  - (File2, read/write)
  - (Printer, execute)

Domain 2 (User B):
  - (File2, read)
  - (File3, read/write)
```

**Access Control Matrix:**
```
           │ File1  │ File2  │ File3  │ Printer │
───────────┼────────┼────────┼────────┼─────────┤
 Domain 1  │  R     │  RW    │   -    │    X    │
───────────┼────────┼────────┼────────┼─────────┤
 Domain 2  │  -     │  R     │  RW    │    -    │
───────────┼────────┼────────┼────────┼─────────┤

Problem: Matrix is mostly empty (wastes memory)
```

**ACL (Access Control List):**
```
Store non-empty entries per OBJECT:

File1: [(Domain1, R)]
File2: [(Domain1, RW), (Domain2, R)]
File3: [(Domain2, RW)]
Printer: [(Domain1, X)]

"Who can access this object and how?"
```

**Capability Lists (C-Lists):**
```
Store permissions per DOMAIN/PROCESS:

Domain1: [(File1, R), (File2, RW), (Printer, X)]
Domain2: [(File2, R), (File3, RW)]

"What can this process access?"
```

### Authentication Methods

How OS verifies identity before granting access:

**1. Something you know:**
- Passwords, PINs
- Store as hashes, not plaintext
- Vulnerable to guessing, phishing

**2. Something you have:**
- Smart cards, tokens, keys
- Physical device required
- Can be stolen but harder

**3. Something you are (Biometrics):**
- Fingerprints, iris, face
- Very hard to forge
- Privacy concerns, can't "change password"

Multi-factor: Combine two or more methods.

### Buffer Overflow Vulnerabilities

Classic security problem in C/C++:

```c
void vulnerable(char *input) {
    char buffer[64];
    strcpy(buffer, input);  // No bounds checking!
}
```

If `input` is longer than 64 bytes:
1. Overflow overwrites stack
2. Can overwrite return address
3. Redirect execution to attacker's code (shellcode)

Prevention:
- Use safe functions: `strncpy`, `snprintf`
- Compiler protections: Stack canaries, ASLR
- Never trust input lengths

### Security Mindset

Key principles:
1. **Never trust input** - Validate everything
2. **Think like an attacker** - What could go wrong?
3. **Defense in depth** - Multiple layers of protection
4. **Least privilege** - Give minimum necessary permissions
5. **Fail securely** - Errors should deny access, not grant it

## 📌 Key Points (Optional)
- Security has four objectives: confidentiality, integrity, availability, control.
- ACLs focus on objects; Capability lists focus on subjects.
- Buffer overflows remain a major vulnerability class in low-level code.

## 🖼️ Recommended Image
- Diagram showing access control matrix, ACL, and capability list relationships.

## 🔗 Connections
- [[138-Memory Management Fundamentals]]
- [[122-System Calls and CPU Protection]]
- [[127-Process Concept and Program vs Process]]