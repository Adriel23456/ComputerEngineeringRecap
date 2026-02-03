---
Theme: Containerization Fundamentals
Date created: 2026-02-02 12:00
tags: [CloudLevel, Containers, Docker, Virtualization, CloudNative]
---

## 📚 Idea/Concept

Containerization is an OS-level virtualization method that packages application code with its dependencies into isolated, portable units called containers. Unlike VMs, containers share the host OS kernel, providing lightweight, fast, and consistent execution environments across any infrastructure.

### Concept Overview

VMs vs Containers:
```
┌─────────────────────────────────────────────────────────────────────────┐
│           Virtual Machines                    Containers                │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                          │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐    ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐│
│  │  App A  │ │  App B  │ │  App C  │    │App A│ │App B│ │App C│ │App D││
│  ├─────────┤ ├─────────┤ ├─────────┤    ├─────┤ ├─────┤ ├─────┤ ├─────┤│
│  │  Bins/  │ │  Bins/  │ │  Bins/  │    │Bins/│ │Bins/│ │Bins/│ │Bins/││
│  │  Libs   │ │  Libs   │ │  Libs   │    │Libs │ │Libs │ │Libs │ │Libs ││
│  ├─────────┤ ├─────────┤ ├─────────┤    └──┬──┘ └──┬──┘ └──┬──┘ └──┬──┘│
│  │ Guest   │ │ Guest   │ │ Guest   │       │       │       │       │   │
│  │   OS    │ │   OS    │ │   OS    │       └───────┴───────┴───────┘   │
│  └────┬────┘ └────┬────┘ └────┬────┘                   │               │
│       │          │          │              ┌───────────▼───────────┐   │
│       └──────────┴──────────┘              │   Container Runtime   │   │
│                  │                         │   (Docker, containerd)│   │
│       ┌──────────▼──────────┐              └───────────┬───────────┘   │
│       │      Hypervisor     │                         │               │
│       │  (VMware, KVM, etc) │              ┌──────────▼──────────┐    │
│       └──────────┬──────────┘              │      Host OS        │    │
│                  │                         │      Kernel         │    │
│       ┌──────────▼──────────┐              └──────────┬──────────┘    │
│       │      Host OS        │                         │               │
│       └──────────┬──────────┘                         │               │
│                  │                                    │               │
│       ┌──────────▼──────────┐              ┌──────────▼──────────┐    │
│       │     Hardware        │              │     Hardware        │    │
│       └─────────────────────┘              └─────────────────────┘    │
│                                                                          │
│  Resource overhead: HIGH                   Resource overhead: LOW       │
│  Startup time: Minutes                     Startup time: Seconds        │
│  Isolation: Strong (separate OS)           Isolation: Process-level     │
│  Density: ~10s per host                    Density: ~100s per host      │
└─────────────────────────────────────────────────────────────────────────┘
```

### Container Architecture

Linux kernel features enabling containers:
```
┌─────────────────────────────────────────────────────────────────┐
│                    Container Isolation                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  NAMESPACES (What a container can SEE)                          │
│  ├── PID namespace    → Own process tree (PID 1 inside)        │
│  ├── Network namespace → Own network stack, IPs, ports         │
│  ├── Mount namespace  → Own filesystem view                    │
│  ├── UTS namespace    → Own hostname                           │
│  ├── IPC namespace    → Own inter-process communication        │
│  └── User namespace   → Own user/group IDs                     │
│                                                                  │
│  CGROUPS (What a container can USE)                             │
│  ├── CPU             → Limit CPU usage (cores, shares)         │
│  ├── Memory          → Limit RAM usage                         │
│  ├── I/O             → Limit disk bandwidth                    │
│  └── Network         → Limit network bandwidth                 │
│                                                                  │
│  UNION FILESYSTEM (How images are layered)                      │
│  └── OverlayFS, AUFS → Copy-on-write layers                    │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Container Image Structure

Layered filesystem:
```
┌─────────────────────────────────────────────────────────────────┐
│                    Container Image Layers                        │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  ┌───────────────────────────────────────┐  ← Writable Layer    │
│  │      Container Layer (runtime)        │    (ephemeral)       │
│  ├───────────────────────────────────────┤                      │
│  │  Layer 4: COPY app.py /app            │  ← Application code  │
│  ├───────────────────────────────────────┤                      │
│  │  Layer 3: RUN pip install flask       │  ← Dependencies      │
│  ├───────────────────────────────────────┤                      │
│  │  Layer 2: RUN apt-get install python  │  ← Runtime           │
│  ├───────────────────────────────────────┤                      │
│  │  Layer 1: Base Image (Ubuntu 22.04)   │  ← Base OS           │
│  └───────────────────────────────────────┘                      │
│                                                                  │
│  Each layer is:                                                 │
│  - Immutable (read-only after creation)                        │
│  - Cached and shared across images                             │
│  - Identified by content hash (SHA256)                         │
│                                                                  │
│  Benefits:                                                      │
│  - Efficient storage (layers shared)                           │
│  - Fast builds (cache unchanged layers)                        │
│  - Fast distribution (pull only changed layers)                │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Dockerfile Fundamentals

Building container images:
```dockerfile
# Base image - start from existing image
FROM python:3.11-slim

# Metadata
LABEL maintainer="team@example.com"
LABEL version="1.0"

# Environment variables
ENV APP_HOME=/app
ENV PYTHONUNBUFFERED=1

# Set working directory
WORKDIR $APP_HOME

# Copy dependency file first (for caching)
COPY requirements.txt .

# Install dependencies (cached if requirements unchanged)
RUN pip install --no-cache-dir -r requirements.txt

# Copy application code
COPY . .

# Create non-root user for security
RUN useradd -m appuser && chown -R appuser:appuser $APP_HOME
USER appuser

# Expose port (documentation)
EXPOSE 8080

# Health check
HEALTHCHECK --interval=30s --timeout=3s \
  CMD curl -f http://localhost:8080/health || exit 1

# Default command
CMD ["python", "app.py"]
```

Dockerfile best practices:
```
┌─────────────────────────────────────────────────────────────────┐
│                  Dockerfile Best Practices                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  1. Use specific base image tags (not :latest)                  │
│     ✗ FROM python                                               │
│     ✓ FROM python:3.11-slim-bookworm                           │
│                                                                  │
│  2. Order instructions by change frequency                      │
│     - Least changing first (base, deps)                        │
│     - Most changing last (app code)                            │
│                                                                  │
│  3. Minimize layers, combine RUN commands                       │
│     ✗ RUN apt-get update                                       │
│       RUN apt-get install -y curl                              │
│     ✓ RUN apt-get update && apt-get install -y curl \         │
│         && rm -rf /var/lib/apt/lists/*                         │
│                                                                  │
│  4. Use multi-stage builds                                      │
│     - Build stage: compile/build with full tools               │
│     - Runtime stage: minimal image with only artifacts         │
│                                                                  │
│  5. Run as non-root user                                        │
│     - Security best practice                                    │
│                                                                  │
│  6. Use .dockerignore                                           │
│     - Exclude unnecessary files from context                   │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Multi-Stage Build Example

Efficient production images:
```dockerfile
# ========== BUILD STAGE ==========
FROM golang:1.21 AS builder

WORKDIR /build
COPY go.mod go.sum ./
RUN go mod download

COPY . .
RUN CGO_ENABLED=0 GOOS=linux go build -o app .

# ========== RUNTIME STAGE ==========
FROM alpine:3.18

# Security: non-root user
RUN adduser -D -g '' appuser

WORKDIR /app
COPY --from=builder /build/app .

USER appuser
EXPOSE 8080
CMD ["./app"]

# Result:
# Build image: ~1GB (Go SDK, tools)
# Final image: ~15MB (just binary + minimal OS)
```

### Container Networking

Network modes:
```
┌─────────────────────────────────────────────────────────────────┐
│                    Container Networking                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  BRIDGE (default)                                               │
│  ┌─────────────────────────────────────────────────┐           │
│  │           docker0 bridge (172.17.0.1)           │           │
│  │    ┌──────────┐  ┌──────────┐  ┌──────────┐    │           │
│  │    │Container │  │Container │  │Container │    │           │
│  │    │172.17.0.2│  │172.17.0.3│  │172.17.0.4│    │           │
│  │    └──────────┘  └──────────┘  └──────────┘    │           │
│  └─────────────────────────────────────────────────┘           │
│  - Containers get private IP, NAT to host                      │
│  - Port mapping required for external access                   │
│                                                                  │
│  HOST                                                           │
│  - Container shares host network namespace                      │
│  - No network isolation, best performance                      │
│                                                                  │
│  NONE                                                           │
│  - No networking (isolated)                                    │
│                                                                  │
│  CUSTOM NETWORKS                                                │
│  - User-defined bridges with DNS resolution                    │
│  - Containers can reach each other by name                     │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Container Orchestration Need

Why containers alone aren't enough:
```
Single Container Challenges:

┌─────────┐
│Container│  ← Single point of failure
│   App   │  ← Manual scaling
└─────────┘  ← No service discovery
             ← No health management
             ← Manual updates

Production Requirements:
┌─────────────────────────────────────────────────────────────────┐
│ ✓ High availability (multiple instances)                        │
│ ✓ Auto-scaling based on load                                    │
│ ✓ Self-healing (restart failed containers)                      │
│ ✓ Rolling updates (zero-downtime deployments)                   │
│ ✓ Service discovery (find other services)                       │
│ ✓ Load balancing (distribute traffic)                           │
│ ✓ Configuration and secrets management                          │
│ ✓ Resource management (CPU/memory limits)                       │
└─────────────────────────────────────────────────────────────────┘

Solution: Container Orchestration (Kubernetes, Docker Swarm, etc.)
```

### Container Registry

Storing and distributing images:
```
┌─────────────────────────────────────────────────────────────────┐
│                    Container Registry                            │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  Developer                Registry              Production      │
│  ┌──────────┐            ┌────────────┐        ┌──────────┐    │
│  │  Build   │   push     │            │  pull  │  Deploy  │    │
│  │  Image   │ ─────────► │  Registry  │ ◄───── │  Image   │    │
│  └──────────┘            │            │        └──────────┘    │
│                          └────────────┘                         │
│                                                                  │
│  Registry Options:                                              │
│  - Docker Hub (public/private)                                 │
│  - AWS ECR (Elastic Container Registry)                        │
│  - Azure ACR (Container Registry)                              │
│  - GCP Artifact Registry                                       │
│  - GitHub Container Registry                                   │
│  - Self-hosted (Harbor, Nexus)                                │
│                                                                  │
│  Image Naming: registry/namespace/image:tag                    │
│  Example: docker.io/library/nginx:1.25                        │
│  Example: 123456789.dkr.ecr.us-east-1.amazonaws.com/myapp:v1  │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

## 📌 Key Points (Optional)
- Containers provide OS-level virtualization, sharing the host kernel for efficiency.
- Images are built in layers, enabling caching and efficient distribution.
- Dockerfile best practices significantly impact image size and security.
- Containers solve packaging; orchestration solves production deployment.

## 🖼️ Recommended Image
- Side-by-side VM vs container stack diagram showing shared kernel architecture.

## 🔗 Connections
- [[201-Kubernetes Container Orchestration]]
- [[190-Horizontal and Vertical Scaling in Cloud]]
- [[183-IaaS vs PaaS vs SaaS]]
- [[192-Hybrid Cloud Architecture]]
- [[203-Microservices Architecture]]